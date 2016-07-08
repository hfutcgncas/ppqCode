#pragma once

#include "GxIAPI.h"
#include "DxImageProc.h"
#include "highgui.h"
#include<ctype.h>

#include <iostream>  
#include <opencv2/opencv.hpp>  
//using namespace cv;
//
//using namespace std;

static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

class MERCamera
{
public:
	GX_DEV_HANDLE hDevice = NULL;

	
	bool m_bIsSnaping = false;
	int64_t m_nPixelFomat;
	int m_nImageWidth;
	int m_nImageHeight;
	int m_nPayLoadSize;
	cv::Mat img;
	GX_STATUS status;

	uint64_t timeStart = 0;

	char CameraName[30];

	MERCamera();
	~MERCamera();

	//void InitCamera();
	void InitCamera(char* IP, char* cfgFile);
	void StartContinuesMode();
	void EndContinuesMode();

	void StartTRIGGER_Mode();
	void EndTRIGGERMode();
	//void ProcessImage(BYTE *pImageBuf);

	static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	bool processImg(BYTE* pImgBuf);
};

