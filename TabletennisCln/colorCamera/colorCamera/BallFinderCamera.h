#pragma once
#include"GlobleSrouce.h"
#include "MERCamera.h"
#include <vector>
#include <fstream>

#define BUFSIZE (3)

using namespace cv;

using namespace std;
struct PointInf
{
	Point pnt;
	uint64_t TimeStep;
};

class ScreenBuf
{
public:
	
	Mat screenBuf[BUFSIZE];
	int index_now;
	int index_last;
	int imgwidth;
	int imgheight;

	int bufsize = BUFSIZE;

	void Init(int width, int height)
	{
		index_now = 0;
		index_last = BUFSIZE;

		imgwidth = width;
		imgheight = height;

		for (int i = 0; i < BUFSIZE; i++)
		{
			screenBuf[i] = Mat(height, width, CV_8UC1);
		}
	}

	void updateData(BYTE* pImgBuf)
	{
		index_now++;
		if (index_now >= BUFSIZE)index_now = 0;
		index_last = index_now - 1;
		if (index_last < 0)index_last += BUFSIZE;

		cv::Mat imgSrc(imgheight, imgwidth, CV_8UC3);
		cv::Mat RawImg(imgheight, imgwidth, CV_8U, pImgBuf);
		cvtColor(RawImg, imgSrc, COLOR_BayerRG2RGB);

		//imshow("src", imgSrc);

		cv::Mat ycrcb = Mat(imgheight, imgwidth, CV_8UC3);
		cv::Mat cb = Mat(imgheight, imgwidth, CV_8UC1);
		cvtColor(imgSrc, ycrcb, CV_RGB2YCrCb);


		IplImage *ycrcb1 = &IplImage(ycrcb);
		IplImage *cb1 = &IplImage(screenBuf[index_now]);
		cvSplit(ycrcb1, 0, 0, cb1, 0);

		threshold(screenBuf[index_now], screenBuf[index_now], 160, 255, CV_THRESH_BINARY);
		
		//// 膨胀  
		//cv::dilate(screenBuf[index_now], screenBuf[index_now], cv::Mat(), Point(-1, -1), 1);
		//// 腐蚀  
		//cv::erode(screenBuf[index_now], screenBuf[index_now], cv::Mat(), Point(-1, -1), 5);

	}
};

class BallFinderCamera :
	public MERCamera
{
public:

	BallFinderCamera();
	
	~BallFinderCamera();

	bool saveData = false;

	int ImgNum;
	ScreenBuf screenBuf;
	CvPoint ballCentor;

	Mat img_bkg;

	//void InitCamera();
	void InitCamera(char* IP,char* cfgFile);
	void InitCamera(char* IP, char* cfgFile, GXCaptureCallBack CallBack);

	static void GX_STDC BallFinderCamera::BallFinderCallBack(GX_FRAME_CALLBACK_PARAM* pFrame);

	CvPoint GetCenterPoint(Mat srcMat);
	
	bool processImg();
	//----------------------------------------------------------
	void AdjustCentor(Mat srcMat, CvPoint & centor);
	void convert2YCbCr(Mat imgSrc, Mat & dist);
	bool processImgV2( CvPoint &centor);
	//----------------------------------------------------------
	
	bool winflag = false;
	int WinSize;
	Rect rect;

	double timeNow;
	double timeLast; 
	double dT = 0;

	CvPoint BallLocation;
	bool NewLocation = false;
	bool Translate = false;

	int frameID;
	
	uint64_t imgTim;


	double CapedFrameTime;
	PointInf PntInf;
	vector<PointInf> pntQueue; //用于存储采集到的轨迹点
	ofstream file;
	string fileName;

	string CameraID;
};


void GX_STDC BallFinderCallBackA(GX_FRAME_CALLBACK_PARAM* pFrame);
void GX_STDC BallFinderCallBackB(GX_FRAME_CALLBACK_PARAM* pFrame);