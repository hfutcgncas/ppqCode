#pragma once
#include "GlobleSrouce.h"
#include "BallFinderCamera.h"
#pragma comment(lib, "ws2_32.lib")


struct BallState
{
	double x;
	double y;
	double z;
	double t;
};

class StereoCamera
{
public:
	StereoCamera();
	~StereoCamera();

	BallFinderCamera CamA, CamB;
	
	void Init(char* IP_A, char* cfgFile_A, char* IP_B, char* cfgFile_B);

	void StartCapture();

	bool CaptureOnce(BallPoint *out);
	bool CaptureOnceV2(BallPoint *out);

	void EndCapture();

	Mat MA,MB;

	Mat ReconstructBall(Point pntA, Point pntB);
	
	void FindMarch(vector<PointInf> A, vector<PointInf> B);

	

	vector<BallState> ballVector;
	BallPoint polyfit(vector<BallState> vb);
	BallPoint StereoCamera::RANSAC(vector<BallState> vb);
};

