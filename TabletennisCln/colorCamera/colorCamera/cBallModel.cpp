

#include "cBallModel.h"
#include "GlobleSrouce.h"
#include <iostream>
using namespace std;


cBallModel::cBallModel()
{
	st[0] = 0.75;
	st[1] = 0.80;
	st[2] = 0.87;
	st[3] = 1.0;
	st[4] = 1.1;

	hitplane.clear();
	hitplane.push_back(1.03); //这里必须从小到大排列
	hitplane.push_back(1.07);
	hitplane.push_back(1.17);
}


inline bool cBallModel::predict_OneStep(BallPoint In, BallPoint* pOut, double dt)
{
	//本函数中单位为 m ，m/s ，s
	double V;
	double a[3];
	double C1 = 0.1500, C2 = 0.0060, g = 9.802;

	V = sqrt(In.Vx * In.Vx + In.Vy * In.Vy + In.Vz * In.Vz);
	a[0] = -C1*V*In.Vx + C2*In.Wy*In.Vz - C2*In.Wz*In.Vy;
	a[1] = -C1*V*In.Vy + C2*In.Wz*In.Vx - C2*In.Wx*In.Vz;
	a[2] = -C1*V*In.Vz + C2*In.Wx*In.Vy - C2*In.Wy*In.Vx - g;


	pOut->x = In.x + In.Vx * dt;
	pOut->y = In.y + In.Vy * dt;
	pOut->z = In.z + In.Vz * dt;

	pOut->Vx = In.Vx + a[0] * dt;
	pOut->Vy = In.Vy + a[1] * dt;
	pOut->Vz = In.Vz + a[2] * dt;

	pOut->Wx = In.Wx;
	pOut->Wy = In.Wy;
	pOut->Wz = In.Wz;

	pOut->t = In.t + dt;
	return true;
}

bool cBallModel::predict_Rebound(BallPoint In, BallPoint* pOut)
{
	double r = 0.02;
	double u = 0.25, et = 0.93;  // 需要找到合适的摩擦系数0.25、反弹系数
	//double u = 0.25, et = 0.8;  // 需要找到合适的摩擦系数0.25、反弹系数
	double vbt1 = In.x - r*In.Wy;
	double vbt2 = In.y + r*In.Wx;
	double vs = 1 - 2.5*u*(1 + et)*signFun(In.Vz) / sqrt(vbt1*vbt1 + vbt2*vbt2);
	double aa = u*(1 + et)*signFun(In.Vz) / sqrt(vbt1*vbt1 + vbt2*vbt2);
	if (vs <= 0)
	{
		aa = 2 / 5.0;
	}

	pOut->Vx = (1 - aa)*(In.Vx) + aa*r*In.Wy;
	pOut->Vy = (1 - aa)*(In.Vy) - aa*r*In.Wx;
	pOut->Vz = -et*(In.Vz);


	pOut->Wx = -3.0*aa / 2.0 / r*(In.Vy) + (1 - 3 * aa / 2)*In.Wx;
	pOut->Wy = 3.0*aa / 2.0 / r*(In.Vx) + (1 - 3 * aa / 2)*In.Wy;
	pOut->Wz = In.Wz;

	pOut->x = In.x;
	pOut->y = In.y;
	pOut->z = In.z;

	pOut->t = In.t;

	return true;
}
int cBallModel::predict_dt(BallPoint startPoint, BallPoint* pEndPoint, double* confidence, double T)
{
	int count = 0;
	double dt = 0.0005;
	BallPoint temp1;
	BallPoint temp2;
	temp1.Copy(startPoint);
	if (startPoint.Vx < 0)
	{
		*confidence = 0;
		return PREDICT_BACKFORWARD;//反向不预测
	}

	//-----------------------------------------------------------
	//反弹前
	while (temp1.z > 0 || temp1.Vz>0)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;



		if (count*dt > T)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = count;
			return PREDICT_NORMAL;  //未反弹
		}
		if (count*dt > 0.5)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}
	}

	//-------
}


int cBallModel::predict(BallPoint startPoint, BallPoint* pEndPoint, double* confidence, double stopPlane)
//return 1 : 正常
//return -1 : 反向
//return 2 : 未反弹
//return 3 : 超时
//return 4 : 反弹后还是够不着
{
	int count = 0;
	double dt = 0.0005;
	BallPoint temp1;
	BallPoint temp2;


	temp1.Copy(startPoint);
	if (startPoint.Vx < 0)
	{
		*confidence = 0;
		return PREDICT_BACKFORWARD;//反向不预测
	}

	//-----------------------------------------------------------
	//反弹前
	while (temp1.z > 0 || temp1.Vz>0)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;

		if (temp1.x > stopPlane)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = exp(-count);
			return PREDICT_NOREBOND;  //未反弹
		}
		if (count > 0.5 / dt)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}
	}
	//-----------------------------------------------------------
	//反弹
	predict_Rebound(temp1, &temp2);
	temp1.Copy(temp2);
	//-----------------------------------------------------------
	//反弹后
	while (temp1.x < stopPlane)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;

		if (count > 0.5 / dt)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}

		if (temp1.z < 0 && temp1.Vz <= 0)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = exp(-count);
			return PREDICT_TOOSHORT; //一次反弹后没能达到目标位置
		}

	}
	(*pEndPoint).Copy(temp1);
	*confidence = exp(-count); //迭代次数越多，置信度越低
	return PREDICT_NORMAL; //正常
}

// 选择接球点
// 返回值 -1 表示没有合适的接球点
//		 否则表示选择 st[i] 作为击球点 
int cBallModel::predict_Mut(BallPoint startPoint, BallPoint* pEndPoint, double* confidence)
{
	BallPoint temp1;
	BallPoint temp2;
	
	BallPoint out[5];

	vector<BallPoint>hitPar;

	double cf = 0;
	int rst;
	bool hasRebounded = false;

	temp1.Copy(startPoint);
	int i;
	for (i = 0; i < hitplane.size(); i++)
	{
	
		rst = predict(temp1, &temp2, &cf, hitplane[i]);
		if (rst == PREDICT_NORMAL && !hasRebounded) //经过且只经过一次反弹
		{
			out[i].Copy(temp2);
			temp1.Copy(temp2);
			hitPar.push_back(temp2);
			hasRebounded = true;
		}
		else if (rst == PREDICT_NOREBOND )
		{
			out[i].Copy(temp2);
			temp1.Copy(temp2);
			hitPar.push_back(temp2);
		}
		else
		{
			break;
		}
	}

	int hitParSize = hitPar.size();

	int Priority[3] = { 0, 1, 2 }; //优先级
	for (int index = 0; index <3; index++)
	{
		if (Priority[index] < hitParSize && isInScope(hitPar[Priority[index]]))
		{
			pEndPoint->Copy(hitPar[Priority[index]]);
			return Priority[index];
		}
			
	}
	/*i--;
	for (; i >= 0; i--) 
	{
		if (isInScope(out[i]))
		{
			pEndPoint->Copy(out[i]);
			return i;
		}	
	}*/
	return -1;	
}


//=================================================================================================
//=================================================================================================
//=================================================================================================
cPrediectOutput::cPrediectOutput()
{
	Output = Output * 0;
	HisGroup.Ball_Start = HisGroup.Ball_Start * 0;
	HisGroup.Ball_Stop = HisGroup.Ball_Stop * 0;
	HisGroup.Confidence = 0;
}


void cPrediectOutput::RefreashOutput(PrediectGroup CurrentGroup)
{
	double fac = 0;

	if (HisGroup.Confidence == 0)
	{
		HisGroup.Ball_Start = CurrentGroup.Ball_Start;
		HisGroup.Ball_Stop = CurrentGroup.Ball_Stop;
		HisGroup.Confidence = CurrentGroup.Confidence;
		return;
	}
	cBallModel model;

	BallPoint PredictCurrent;

	double a = CurrentGroup.Confidence;
	double b = HisGroup.Confidence;
	double Dt = CurrentGroup.Ball_Start.t - HisGroup.Ball_Start.t;

	model.predict_dt(HisGroup.Ball_Start, &PredictCurrent, &a, Dt);


	fac = a / sqrt(a*a + b*b);

	PrediectGroup NewGroup;
	NewGroup.Ball_Start = PredictCurrent + (CurrentGroup.Ball_Start - PredictCurrent)*fac;

	HisGroup.Ball_Start = NewGroup.Ball_Start;
	HisGroup.Confidence = a*b / (a*a + b*b);

	Output.Copy(NewGroup.Ball_Start);

	//EstimatedBallStatue = EstimatedBallStatue + (InPoint - EstimatedBallStatue)*fac;
	//Output = Output*(1 - fac) + InPoint*fac;
}

