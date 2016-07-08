#include "BallFinderCamera.h"
#include<vector>

extern cClock SystemClock;

BallFinderCamera::BallFinderCamera()
{
}


BallFinderCamera::~BallFinderCamera()
{
}



void BallFinderCamera::InitCamera(char* IP, char* cfgFile)
{
	MERCamera::InitCamera(IP, cfgFile);
	ImgNum = 0;

	img_bkg = Mat(m_nImageHeight, m_nImageWidth, CV_8UC1);
	if (status == GX_STATUS_SUCCESS)
	{
		//ע��ͼ����ص�����
		status = GXRegisterCaptureCallback(hDevice, this, BallFinderCallBack);
	}

	if (status != GX_STATUS_SUCCESS)cout << "�ص���������" << endl;

	screenBuf.Init(m_nImageWidth, m_nImageHeight);

	WinSize = 80;
	rect = Rect(0, 0, 2 * WinSize, 2 * WinSize);

	//===============================================
	//�洢�ļ�
	CameraID = IP;
	fileName = "./temporaryData/" + CameraID + ".txt";
}

void BallFinderCamera::InitCamera(char* IP, char* cfgFile, GXCaptureCallBack CallBack)
{
	MERCamera::InitCamera(IP, cfgFile);
	ImgNum = 0;

	img_bkg = Mat(m_nImageHeight, m_nImageWidth, CV_8UC1);
	if (status == GX_STATUS_SUCCESS)
	{
		//ע��ͼ����ص�����
		status = GXRegisterCaptureCallback(hDevice, this, CallBack);
	}

	if (status != GX_STATUS_SUCCESS)cout << "�ص���������" << endl;

	screenBuf.Init(m_nImageWidth, m_nImageHeight);

	WinSize = 80;
	rect = Rect(0, 0, 2 * WinSize, 2 * WinSize);

	//===============================================
	//�洢�ļ�
	CameraID = IP;
	fileName = "./temporaryData/" + (string)(CameraID) + ".txt";
	cout << fileName << endl;
}

CvPoint BallFinderCamera::GetCenterPoint(Mat srcMat)
{
	int i, j;
	double x0 = 0, y0 = 0, sum = 0;

	Mat srcMat1;
	// ����  
	cv::dilate(srcMat, srcMat1, cv::Mat(), Point(-1, -1), 5);

	IplImage *src = &IplImage(srcMat1);
	CvPoint center(0,0);
	CvScalar pixel;
	for (i = 0; i<src->width; i++)
		for (j = 0; j<src->height; j++)
		{
			pixel = cvGet2D(src, j, i);
			if (pixel.val[0] == 255)
			{
				x0 = x0 + i;
				y0 = y0 + j;
				sum = sum + 1.0;
			}
		}
	if (sum == 0)
	{
		return center;
	}
	
	center.x = ((double)x0) / sum;
	center.y = ((double)y0) / sum;
	return center;
}

bool BallFinderCamera::processImg()
{
	timeNow = static_cast<double>(getTickCount());
	double dt = (timeNow - timeLast) / getTickFrequency();
	dT = 0.9*dT + 0.1*dt;
	timeLast = timeNow;

	Mat dframe1(m_nImageHeight, m_nImageWidth, CV_8UC1);
	CvPoint centor;
	IplImage *src;
	
	Mat img1 = screenBuf.screenBuf[screenBuf.index_now];
	Mat img_bkgWind;
	Mat img2Src = img1;

	img_bkg = img_bkg*0.95 + 0.05 * img1; //���±���
	
	if (winflag == false) //��ʱ���±���
	{	
		
		img_bkgWind = img_bkg;
	}
	else  // ѡȡС����
	{	
		img1 = img1(rect);
		img_bkgWind = img_bkg(rect);
	}

	// ��֡&Ԥ����
	absdiff(img_bkgWind, img1, dframe1);

	threshold(dframe1, dframe1, 160, 255, CV_THRESH_BINARY);
	// ��ʴ  
	cv::erode(dframe1, dframe1, cv::Mat(), Point(-1, -1), 1);
	


	centor = GetCenterPoint(dframe1);

	if (centor.x == 0 || centor.y == 0) //û�ҵ��㣬������
	{
		rect.x = 0;
		rect.y = 0;
		winflag = false;
	}
	else //�ҵ���󣬸��´���
	{
		#pragma region С����������
		if (winflag)centor = GetCenterPoint(img1); //  ����ǰΪС��ģʽ��������ԭͼ��С������������

		//�õ�ƹ�����ͼ������
		centor.x += rect.x; //��ʱ��rect������һʱ�̵Ĵ���
		centor.y += rect.y;

		//���´���λ�ã�Ϊ�´�������׼��
		rect.x = centor.x - WinSize;
		rect.y = centor.y - WinSize;

		rect.x = (rect.x > 0 ? rect.x : 0);
		rect.y = (rect.y > 0 ? rect.y : 0);
		rect.x = (rect.x < (m_nImageWidth - 2 * WinSize) ? rect.x : (m_nImageWidth - 2 * WinSize));
		rect.y = (rect.y < (m_nImageHeight - 2 * WinSize) ? rect.y : (m_nImageHeight - 2 * WinSize));

		#pragma endregion С����������

		winflag = true;
	}
	
	if (centor.x != 0 && centor.y != 0)
	{	
		NewLocation = true;
		BallLocation = centor;
		PntInf.pnt = centor;
		PntInf.TimeStep = imgTim;

		if (saveData)
		{
			pntQueue.push_back(PntInf);
		}
		
		//while (pntQueue.size()>100)
		//{
		//	pntQueue.erase(pntQueue.begin());
		//}
	/*	cout << CameraName << ":  " << " frameRate: " << (1.0 / dT) <<endl
			<< " winflag : " << winflag << " x: " << centor.x << endl << " y: " << centor.y << endl;
		    circle(img2Src, centor, 3, CV_RGB(0, 255, 0), 1, 8, 3);
		cout <<CameraName<<" tp: " <<pntQueue.back().TimeStep << endl;*/
	}
	else
	{
		NewLocation = false;
	}

	//static int index = 0;
	//index++;
	//if (status == 0 && index % 5 == 0)
	//{
	//	cv::imshow("dframe1", dframe1);
	//	imshow(CameraName, img2Src);
	//	cv::waitKey(3);
	//}
	return false;
}

void BallFinderCamera::AdjustCentor(Mat srcMat, CvPoint & centor)
{
	int smallWnd = 20;
	int i, j;
	double x0 = 0, y0 = 0, sum = 0;
	// ����  
	cv::dilate(srcMat, srcMat, cv::Mat(), Point(-1, -1), 5);

	IplImage *src = &IplImage(srcMat);
	int startX = centor.x;
	int startY = centor.y;
	int IndexPX = startX, IndexNX = startX;
	int IndexPY = startY, IndexNY = startY;
	CvScalar pixel;

	pixel = cvGet2D(src, startY, startX);
	while (pixel.val[0] == 255 && IndexPX >0) // ��������
	{
		pixel = cvGet2D(src, startY, IndexPX);
		IndexPX--;
	}
	pixel = cvGet2D(src, startY, startX);
	while (pixel.val[0] == 255 && IndexNX <src->width) // ��������
	{
		pixel = cvGet2D(src, startY, IndexNX);
		IndexNX++;
	}
	pixel = cvGet2D(src, startY, startX);
	while (pixel.val[0] == 255 && IndexPY >0) // ��������
	{
		pixel = cvGet2D(src, IndexPY, startX);
		IndexPY--;
	}
	pixel = cvGet2D(src, startY, startX);
	while (pixel.val[0] == 255 && IndexNY <src->height) // ��������
	{
		pixel = cvGet2D(src, IndexNY, startX);
		IndexNY++;
	}

	centor.x = (IndexPX + IndexNX) / 2;
	centor.y = (IndexPY + IndexNY) / 2;
}

void BallFinderCamera::convert2YCbCr(Mat imgSrc, Mat & dist)
{
	int imgheight = imgSrc.rows;
	int imgwidth = imgSrc.cols;

	Mat ycrcb = Mat(imgheight, imgwidth, CV_8UC3);
	Mat cb = Mat(imgheight, imgwidth, CV_8UC1);
	cvtColor(imgSrc, ycrcb, CV_RGB2YCrCb);

	IplImage *ycrcb1 = &IplImage(ycrcb);
	IplImage *cb1 = &IplImage(dist);
	cvSplit(ycrcb1, 0, 0, cb1, 0);

	threshold(dist, dist, 155, 255, CV_THRESH_BINARY);
}

bool BallFinderCamera::processImgV2( CvPoint &centor)
{
	bool NewLocation = false;

	//-------------------------------------------
	//תΪYCbcr
	Mat img1 = screenBuf.screenBuf[screenBuf.index_now];
	int m_nImageWidth = img1.rows;
	int m_nImageHeight = img1.cols;
	//-------------------------------------------
	//static bool winflag = false;
	//static Rect rect = Rect(0, 0, 2 * WinSize, 2 * WinSize);
	//static Mat img_bkg = Mat(m_nImageWidth, m_nImageHeight, CV_8UC1);

	//	cout << "Ԥ����" << endl;

	Mat dframe1(m_nImageWidth, m_nImageHeight, CV_8UC1);
	Mat img_bkgWind;
	Mat img2Src = img1;

	img_bkg = img_bkg*0.95 + 0.05 * img1; //���±���
	//	cout << "���±���" << endl;


	if (winflag == false) //��ʱ���±���
	{
		img_bkgWind = img_bkg;
	}
	else  // ѡȡС����
	{
		img1 = img1(rect);
		img_bkgWind = img_bkg(rect);
	}

	//	cout << "ѡȡС����" << endl;

	// ��֡&Ԥ����
	absdiff(img_bkgWind, img1, dframe1);

	threshold(dframe1, dframe1, 160, 255, CV_THRESH_BINARY);
	// ��ʴ  
	cv::erode(dframe1, dframe1, cv::Mat(), Point(-1, -1), 1);

	//	cout << "��֡��ʴ" << endl;

	centor = GetCenterPoint(dframe1);

	//	cout << "������" << endl;

	if (centor.x == 0 || centor.y == 0) //û�ҵ��㣬������
	{
		rect.x = 0;
		rect.y = 0;
		winflag = false;
		//		cout << "û�е�" << endl;
	}
	else //�ҵ���󣬸��´���
	{
#pragma region С����������
		//	if (winflag)centor = GetCenterPoint(img1);
		if (winflag)AdjustCentor(img1, centor); //  ����ǰΪС��ģʽ��������ԭͼ��С����������
		//		cout << "�µ�" << endl;
		//�õ�ƹ�����ͼ������
		centor.x += rect.x; //��ʱ��rect������һʱ�̵Ĵ���
		centor.y += rect.y;

		//���´���λ�ã�Ϊ�´�������׼��
		rect.x = centor.x - WinSize;
		rect.y = centor.y - WinSize;

		rect.x = (rect.x > 0 ? rect.x : 0);
		rect.y = (rect.y > 0 ? rect.y : 0);
		//rect.x = (rect.x < (m_nImageWidth - 2 * WinSize) ? rect.x : (m_nImageWidth - 2 * WinSize  ));
		//rect.y = (rect.y < (m_nImageHeight - 2 * WinSize) ? rect.y : (m_nImageHeight - 2 * WinSize ));
		rect.x = (rect.x < (m_nImageHeight - 2 * WinSize) ? rect.x : (m_nImageHeight - 2 * WinSize));
		rect.y = (rect.y < (m_nImageWidth - 2 * WinSize) ? rect.y : (m_nImageWidth - 2 * WinSize));


#pragma endregion С����������
		//
		winflag = true;
	}

	//	cout << "����------------" << endl;
	//
	if (centor.x != 0 && centor.y != 0)
	{
		NewLocation = true;
		/*BallLocation = centor;
		PntInf.pnt = centor;
		PntInf.TimeStep = imgTim;
		*/

		//while (pntQueue.size()>100)
		//{
		//	pntQueue.erase(pntQueue.begin());
		//}
		/*	cout << CameraName << ":  " << " frameRate: " << (1.0 / dT) <<endl
		<< " winflag : " << winflag << " x: " << centor.x << endl << " y: " << centor.y << endl;
		circle(img2Src, centor, 3, CV_RGB(0, 255, 0), 1, 8, 3);
		cout <<CameraName<<" tp: " <<pntQueue.back().TimeStep << endl;*/
	}
	else
	{
		NewLocation = false;
	}
	//
	//
	//	cv::imshow("dframe1", dframe1);

	return NewLocation;
}
//=====================================================================================

void GX_STDC BallFinderCamera::BallFinderCallBack(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	int height = pFrame->nHeight;
	int width = pFrame->nWidth;
	int WinSize = 80;
	
	#pragma region ��ʼ��ScreenBuf
	static int indexInit = 0;
	BallFinderCamera * pBallFinderCamera = (BallFinderCamera *)(pFrame->pUserParam);
	ScreenBuf* ps = &(pBallFinderCamera->screenBuf);

	
	pBallFinderCamera->imgTim = pFrame->nTimestamp;
	if (indexInit <ps->bufsize)
	{
		pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);
		ps->screenBuf[ps->index_now].copyTo(pBallFinderCamera->img_bkg);

		indexInit++;

		pBallFinderCamera->Translate = true;
		return;
	}
	#pragma endregion ��ʼ��ScreenBuf

	////------------------------------------------------------

	pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);

	pBallFinderCamera->processImg();	

	pBallFinderCamera->Translate = true;

	pBallFinderCamera->frameID = pFrame->nFrameID ;

	return;
}

void GX_STDC BallFinderCallBackA(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	int height = pFrame->nHeight;
	int width = pFrame->nWidth;
	int WinSize = 80;

#pragma region ��ʼ��ScreenBuf
	static int indexInit = 0;
	BallFinderCamera * pBallFinderCamera = (BallFinderCamera *)(pFrame->pUserParam);
	ScreenBuf* ps = &(pBallFinderCamera->screenBuf);


	pBallFinderCamera->imgTim = pFrame->nTimestamp;
	if (indexInit <ps->bufsize)
	{
		pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);
		ps->screenBuf[ps->index_now].copyTo(pBallFinderCamera->img_bkg);

		indexInit++;

		pBallFinderCamera->Translate = true;
		return;
	}
#pragma endregion ��ʼ��ScreenBuf

	////------------------------------------------------------
	////------------------------------------------------------

	pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);

	//bool caped = false;
	//caped = pBallFinderCamera->processImgV2(pBallFinderCamera->BallLocation);

	//if (caped)pBallFinderCamera->NewLocation = true;

	pBallFinderCamera->NewLocation = pBallFinderCamera->processImgV2(pBallFinderCamera->BallLocation);
	if (pBallFinderCamera->NewLocation)
	{
		pBallFinderCamera->CapedFrameTime = SystemClock.now();
		pBallFinderCamera->PntInf.pnt = pBallFinderCamera->BallLocation;
		pBallFinderCamera->PntInf.TimeStep = pBallFinderCamera->imgTim;
		if (pBallFinderCamera->saveData)
		{
			pBallFinderCamera->pntQueue.push_back(pBallFinderCamera->PntInf);
		}

	}

	pBallFinderCamera->Translate = true;

	pBallFinderCamera->frameID = pFrame->nFrameID;

	return;
}

void GX_STDC BallFinderCallBackB(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	int height = pFrame->nHeight;
	int width = pFrame->nWidth;
	int WinSize = 80;

#pragma region ��ʼ��ScreenBuf
	static int indexInit = 0;
	BallFinderCamera * pBallFinderCamera = (BallFinderCamera *)(pFrame->pUserParam);
	ScreenBuf* ps = &(pBallFinderCamera->screenBuf);

	
	pBallFinderCamera->imgTim = pFrame->nTimestamp;
	if (indexInit <ps->bufsize)
	{
		pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);
		ps->screenBuf[ps->index_now].copyTo(pBallFinderCamera->img_bkg);

		indexInit++;

		pBallFinderCamera->Translate = true;
		return;
	}
#pragma endregion ��ʼ��ScreenBuf

	////------------------------------------------------------
	////------------------------------------------------------

	pBallFinderCamera->screenBuf.updateData((BYTE*)pFrame->pImgBuf);

	//bool caped = false;
	pBallFinderCamera->NewLocation = pBallFinderCamera->processImgV2(pBallFinderCamera->BallLocation);

//	if (caped)pBallFinderCamera->NewLocation = true;
	if (pBallFinderCamera->NewLocation)
	{
		pBallFinderCamera->CapedFrameTime = SystemClock.now();
		pBallFinderCamera->PntInf.pnt = pBallFinderCamera->BallLocation;
		pBallFinderCamera->PntInf.TimeStep = pBallFinderCamera->imgTim;
		if (pBallFinderCamera->saveData)
		{
			pBallFinderCamera->pntQueue.push_back(pBallFinderCamera->PntInf);
		}

	}

	pBallFinderCamera->Translate = true;

	pBallFinderCamera->frameID = pFrame->nFrameID;

	return;
}