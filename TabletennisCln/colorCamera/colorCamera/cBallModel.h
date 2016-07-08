#pragma once

#define PREDICT_BACKFORWARD -1
#define PREDICT_NORMAL 1
#define PREDICT_NOREBOND 2
#define PREDICT_TIMEOUT 3
#define PREDICT_TOOSHORT 4

#include"dataStruct.h"


class cBallModel
{
	inline double signFun(double a)
	{
		if (a == 0)
		{
			return 0.0;
		}
		if (a> 0)
		{
			return a;
		}
		else
			return -a;
	}
public:
	double st[5];
	vector<double>hitplane;

	cBallModel();
	bool predict_OneStep(BallPoint In, BallPoint* pOut, double dt);
	bool predict_Rebound(BallPoint In, BallPoint* pOut);

	int predict(BallPoint startPoint, BallPoint* pendPoint, double* confidence, double stopPlane);
	int predict_Mut(BallPoint startPoint, BallPoint* pendPoint, double* confidence);
	
	int predict_dt(BallPoint startPoint, BallPoint* pEndPoint, double* confidence, double dt);

	bool Flysolve(BallPoint hit, BallPoint land, BallPoint &vout);

};


class PrediectGroup
{
public:
	BallPoint Ball_Start;
	BallPoint Ball_Stop;
	double Confidence;
};

class cPrediectOutput
{
public:
	BallPoint Output;
	BallPoint EstimatedBallStatue;
	cBallPosTemp BallTemp;
	cPrediectOutput();

	PrediectGroup HisGroup;

	void cPrediectOutput::RefreashOutput(PrediectGroup CurrentGroup);


};