#include"MERCamera.h"

using namespace cv;

using namespace std;

MERCamera::MERCamera()
{
	img = Mat(Size(656, 492), CV_8UC3);

	uint32_t      nDeviceNum = 0;
	// 初始化库 
	status = GXInitLib();
	if (status != GX_STATUS_SUCCESS)
	{
		return;
	}
	// 枚举设备列表
	status = GXUpdateDeviceList(&nDeviceNum, 1000);
	if ((status != GX_STATUS_SUCCESS) || (nDeviceNum <= 0))
	{
		return;
	}

	//打开设备
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


	//打开设备
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
		cout << "读入配置文件失败" << endl;
	}



	int64_t nValue = 0;

	m_bIsSnaping = false;

	if (status == GX_STATUS_SUCCESS)
	{
		//注册图像处理回调函数
		status = GXRegisterCaptureCallback(hDevice, this, OnFrameCallbackFun);
	}
	if (status != GX_STATUS_SUCCESS)
	{
		cout << "注册图像处理回调函数失败" << endl;
	}
	cout << "注册回调函数成功" << endl;
	//	GXSetInt(hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_RGB8_PLANAR);
	//if (status != GX_STATUS_SUCCESS)
	//{
	//	cout << "set format fail" << endl;
	//}

	status = GXGetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, &m_nPixelFomat);

	//获取宽度(需在停止采集状态下设置)  
	status = GXGetInt(hDevice, GX_INT_WIDTH, &nValue);
	m_nImageWidth = (int)nValue;

	//获取高度(需在停止采集状态下设置)  
	status = GXGetInt(hDevice, GX_INT_HEIGHT, &nValue);
	m_nImageHeight = (int)nValue;

	//获取图象数据大小  
	status = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &nValue);
	m_nPayLoadSize = (int)nValue;




}

void MERCamera::StartContinuesMode()
{
	 
	//关闭触发模式
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	//设置采集模式。一般相机的默认采集模式为连续模式。 
	status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);

	if (status == GX_STATUS_SUCCESS)
	{

		//开采之后发送软触发信号（或者外触发）
		status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
	}

}

void MERCamera::EndContinuesMode()
{
	//发送停采命令
	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
}

void MERCamera::StartTRIGGER_Mode()
{
	//开启触发模式
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON); 

	//设置传输控制模式为用户控制模式
	status = GXSetEnum(hDevice, GX_ENUM_TRANSFER_CONTROL_MODE, GX_ENUM_TRANSFER_CONTROL_MODE_BASIC);

	//设置传输操作模式为指定发送帧数
	status = GXSetEnum(hDevice, GX_ENUM_TRANSFER_OPERATION_MODE,GX_ENUM_TRANSFER_OPERATION_MODE_MULTIBLOCK);
	//设置每次命令输出帧数帧
	status = GXSetInt(hDevice, GX_INT_TRANSFER_BLOCK_COUNT, 1);

	//设置触发激活方式为上升沿
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_ACTIVATION, GX_TRIGGER_ACTIVATION_RISINGEDGE);

	//line0触发
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE0);

	//设置采集模式。一般相机的默认采集模式为连续模式。 
	status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);

	if (status == GX_STATUS_SUCCESS)
	{
		//开采之后发送软触发信号（或者外触发）
		status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
	}

}

void MERCamera::EndTRIGGERMode()
{
	//关闭触发模式
	status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);

	//发送停采命令

	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	//发送停采命令

	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	//注销采集回调

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