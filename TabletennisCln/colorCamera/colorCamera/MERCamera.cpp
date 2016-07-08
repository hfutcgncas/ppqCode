#include"MERCamera.h"

using namespace cv;

using namespace std;

MERCamera::MERCamera()
{
	img = Mat(Size(656, 492), CV_8UC3);

	uint32_t      nDeviceNum = 0;
	// ��ʼ���� 
	status = GXInitLib();
	if (status != GX_STATUS_SUCCESS)
	{
		return;
	}
	// ö���豸�б�
	status = GXUpdateDeviceList(&nDeviceNum, 1000);
	if ((status != GX_STATUS_SUCCESS) || (nDeviceNum <= 0))
	{
		return;
	}

	//���豸
	//GX_OPEN_PARAM stOpenParam;
	//stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	/*stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";*/

	//stOpenParam.openMode = GX_OPEN_IP;
	//stOpenParam.pszContent = "192.168.64.100";
	//
	//status = GXOpenDevice(&stOpenParam, &hDevice);

}



MERCamera::~MERCamera()
{
	status = GXUnregisterCaptureCallback(hDevice);
	status = GXCloseDevice(hDevice);
	status = GXCloseLib();
}

void MERCamera::InitCamera(char* IP,char* cfgFile)
{


	//���豸
	GX_OPEN_PARAM stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	//stOpenParam.openMode = GX_OPEN_INDEX;
	//stOpenParam.pszContent = "1";

	stOpenParam.openMode = GX_OPEN_IP;
	stOpenParam.pszContent = IP;

	status = GXOpenDevice(&stOpenParam, &hDevice);
	strcpy_s(CameraName, IP);

	
	status = GXImportConfigFile(hDevice, cfgFile);
	if (status != GX_STATUS_SUCCESS)
	{
		cout << "���������ļ�ʧ��" << endl;
	}



	int64_t nValue = 0;

	m_bIsSnaping = false;

	if (status == GX_STATUS_SUCCESS)
	{
		//ע��ͼ����ص�����
		status = GXRegisterCaptureCallback(hDevice, this, OnFrameCallbackFun);
	}
	if (status != GX_STATUS_SUCCESS)
	{
		cout << "ע��ͼ����ص�����ʧ��" << endl;
	}
	cout << "ע��ص������ɹ�" << endl;
	//	GXSetInt(hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_RGB8_PLANAR);
	//if (status != GX_STATUS_SUCCESS)
	//{
	//	cout << "set format fail" << endl;
	//}

	status = GXGetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, &m_nPixelFomat);

	//��ȡ���(����ֹͣ�ɼ�״̬������)  
	status = GXGetInt(hDevice, GX_INT_WIDTH, &nValue);
	m_nImageWidth = (int)nValue;

	//��ȡ�߶�(����ֹͣ�ɼ�״̬������)  
	status = GXGetInt(hDevice, GX_INT_HEIGHT, &nValue);
	m_nImageHeight = (int)nValue;

	//��ȡͼ�����ݴ�С  
	status = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &nValue);
	m_nPayLoadSize = (int)nValue;




}

void MERCamera::StartContinuesMode()
{
	 
	//�رմ���ģʽ
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	//���òɼ�ģʽ��һ�������Ĭ�ϲɼ�ģʽΪ����ģʽ�� 
	status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);

	if (status == GX_STATUS_SUCCESS)
	{

		//����֮���������źţ������ⴥ����
		status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
	}

}

void MERCamera::EndContinuesMode()
{
	//����ͣ������
	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
}

void MERCamera::StartTRIGGER_Mode()
{
	//��������ģʽ
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON); 

	//���ô������ģʽΪ�û�����ģʽ
	status = GXSetEnum(hDevice, GX_ENUM_TRANSFER_CONTROL_MODE, GX_ENUM_TRANSFER_CONTROL_MODE_BASIC);

	//���ô������ģʽΪָ������֡��
	status = GXSetEnum(hDevice, GX_ENUM_TRANSFER_OPERATION_MODE,GX_ENUM_TRANSFER_OPERATION_MODE_MULTIBLOCK);
	//����ÿ���������֡��֡
	status = GXSetInt(hDevice, GX_INT_TRANSFER_BLOCK_COUNT, 1);

	//���ô������ʽΪ������
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_ACTIVATION, GX_TRIGGER_ACTIVATION_RISINGEDGE);

	//line0����
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE0);

	//���òɼ�ģʽ��һ�������Ĭ�ϲɼ�ģʽΪ����ģʽ�� 
	status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);

	if (status == GX_STATUS_SUCCESS)
	{
		//����֮���������źţ������ⴥ����
		status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
	}

}

void MERCamera::EndTRIGGERMode()
{
	//�رմ���ģʽ
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);

	//����ͣ������

	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	//����ͣ������

	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	//ע���ɼ��ص�

	status = GXUnregisterCaptureCallback(hDevice);

}




bool MERCamera::processImg(BYTE* pImgBuf)
{
	int nWidth = m_nImageWidth;
	int nHeight = m_nImageHeight;

	static int index = 0;


	if (status == 0)
	{
		//memcpy(m_pBufferRaw, (BYTE*)pFrame->pImgBuf, pFrame->nImgSize);

		cv::Mat RawImg(nHeight, nWidth, CV_8U, (BYTE*)pImgBuf);
		cv::Mat img(nHeight, nWidth, CV_8UC3);
		cvtColor(RawImg, img, COLOR_BayerRG2RGB);


		cv::Mat hsv = Mat(nHeight, nWidth, CV_8UC3);
		cv::Mat hue = Mat(nHeight, nWidth, CV_8UC1);
		cvtColor(img, hsv, CV_RGB2YCrCb);


		IplImage *hsv1 = &IplImage(hsv);
		IplImage *hue1 = &IplImage(hue);
		cvSplit(hsv1, 0, 0, hue1, 0);

		//cvtColor(hue, hue, CV_BGR2GRAY);
		threshold(hue, hue, 150, 255, CV_THRESH_BINARY);


		cv::imshow(CameraName, img);
		//cv::imshow("hsv", hsv);
		//cv::imshow("hue", hue);
		cv::waitKey(3);


	}

	return true;
}

//BYTE m_pBufferRaw[656 * 492];
void GX_STDC MERCamera::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	MERCamera * m_MERCamera = (MERCamera *)(pFrame->pUserParam);
	m_MERCamera->processImg((BYTE*)pFrame->pImgBuf);

	return;
}