#include "StereoCamera.h"

using namespace std;

extern cFile  dataFile;

StereoCamera::StereoCamera()
{
	//��ʼ�����������
	MA = Mat(3, 4, CV_64F);
	MB = Mat(3, 4, CV_64F);

	
	MA = (Mat_<double>(3, 4) << 326.617144110931,	1110.45359653824,	138.132864689727,	679005.460410298,
		- 342.177857611385,	42.0462330449680,	1092.11745836789,	864608.234777657,
		0.863935163836458, - 0.0122251298662745,	0.503454644319252,	3063.42911441561);
	MB = (Mat_<double>(3, 4) << -294.209543796488 ,- 1110.02131021776,	162.887289338160,	1318509.81737721,
		406.107231808457, - 31.5231098806836,	1067.03880714072,	831746.069591339,
		- 0.826808448025553, - 0.00253956058411331	,0.562477858146982	,2936.35906211506);

	dataFile.FileInit();									//���ݴ洢�ĳ�ʼ��
}


StereoCamera::~StereoCamera()
{
}

void StereoCamera::Init(char* IP_A, char* cfgFile_A, char* IP_B, char* cfgFile_B)
{
	CamA.InitCamera(IP_A, cfgFile_A, BallFinderCallBackA);
	CamB.InitCamera(IP_B, cfgFile_B, BallFinderCallBackB);
	
}

void StereoCamera::StartCapture()
{
	//CamA.StartContinuesMode();
	//CamB.StartContinuesMode();
	CamA.StartTRIGGER_Mode();
	CamB.StartTRIGGER_Mode();

	CamA.Translate = false;
	CamB.Translate = false;
	int IDbias = 0;
	for (int i = 0; i < 300000; i++) //�ɼ�һ��ʱ�䣬ʹ�������������֡�ű����ͬ
	{
		Sleep(100);
		IDbias = CamA.frameID - CamB.frameID;
		if (IDbias < 2 && IDbias > -2)
		{
			cout << "Sync OK" << endl;
			CamA.timeStart = CamA.imgTim;
			CamB.timeStart = CamB.imgTim;
			break;
		}
		
	}
	CamA.Translate = false;
	CamB.Translate = false;
}

//void StereoCamera::FindMarch(vector<PointInf> A, vector<PointInf> B)
//{
//	/*int sizeA = A.size();
//	int sizeB = B.size();
//
//	Point3d pnt(0,0,0);
//
//	long dt = 0, dt_last = 0;*/
//	//if (!A.empty() && !B.empty())
//	//{
//	//	uint64_t ta_min = A.front().TimeStep;
//	//	uint64_t tb_min = B.front().TimeStep;
//	//	uint64_t ta_max = A.back().TimeStep;
//	//	uint64_t tb_max = B.back().TimeStep;
//
//	//}
//	
//}

bool StereoCamera::CaptureOnceV2(BallPoint *out)
{
	bool rt = false;
	double ta, tb, dt;
	BallState ballState;
	BallPoint plyout;
	static BallPoint LastOut;

	if (CamA.NewLocation && CamB.NewLocation)
	{
		ta = (CamA.imgTim - CamA.timeStart) * 2 / 100000.0;
		tb = (CamB.imgTim - CamB.timeStart) * 2 / 100000.0;
		if ((CamA.CapedFrameTime - CamB.CapedFrameTime) < 4 && (CamA.CapedFrameTime - CamB.CapedFrameTime) > -4)
		{
			Point3d pnt = ReconstructBall(CamA.BallLocation, CamB.BallLocation);

			//����任 ��������˷���Ϊx������Ϊz
			ballState.x = pnt.y;
			ballState.y = pnt.x;
			ballState.z = -pnt.z;
			ballState.t = ta;

			if (LastOut.x == 0 && LastOut.t == 0)
			{
				LastOut.x = ballState.x;
				LastOut.y = ballState.y;
				LastOut.z = ballState.z;
				LastOut.t = ballState.t;
			}
			else
			{
				dt = ballState.t - LastOut.t;
				out->x = ballState.x;
				out->y = ballState.y;
				out->z = ballState.z;
				out->t = ballState.t;
				out->Vx = (ballState.x - LastOut.x) / dt;
				out->Vy = (ballState.y - LastOut.y) / dt;
				out->Vz = (ballState.z - LastOut.z) / dt;

				LastOut.Copy(*out);
				rt = true;
			}

			
			

	//		ballVector.push_back(ballState);
		//	//------------------------------------------------------------------
		//	//�������͵㣬ɾ����͵�֮ǰ�ĵ�
		//	double zMin = ballVector[0].z;
		//	int zIndex = 0, vLen = ballVector.size();

		//	for (int i = 1; i < vLen; i++)
		//	{
		//		if (ballVector[i].z < zMin)
		//		{
		//			zMin = ballVector[i].z;
		//			zIndex = i;
		//		}
		//	}
		//	if (zIndex != 0 && zIndex != vLen - 1) // ��������͵㣬��ɾ����͵�֮ǰ�����ݡ�
		//	{
		//		ballVector.erase(ballVector.begin() + zIndex, ballVector.end());
		//	}
		//	//--------------------------------------------------------------------
		///*	while (ballVector.size() > 6)
		//	{
		//		ballVector.erase(ballVector.begin());
		//	}
		//	if (ballVector.size() == 6)
		//	{
		//		plyout = polyfit(ballVector);
		//		out->Copy(plyout);
		//		rt = true;
		//	}*/

		//	int tempSize = 5;
		//	while (ballVector.size() > tempSize)
		//	{
		//		ballVector.erase(ballVector.begin());
		//	}
		//	if (ballVector.size() == tempSize)
		//	{
		//		dt = (ballVector[tempSize - 1].t - ballVector[0].t);
		//		out->x = ballState.x;
		//		out->y = ballState.y;
		//		out->z = ballState.z;
		//		out->t = ballState.t;
		//		out->Vx = (ballVector[tempSize - 1].x - ballVector[0].x) / dt;
		//		out->Vy = (ballVector[tempSize - 1].y - ballVector[0].y) / dt;
		//		out->Vz = (ballVector[tempSize - 1].z - ballVector[0].z) / dt;
		//		rt = true;
		//	}
		}

		CamA.NewLocation = false;
		CamB.NewLocation = false;
	}
	return rt;
}

bool StereoCamera::CaptureOnce(BallPoint *out)
{
	bool rt = false;
	double ta, tb;
	BallState ballState;
	BallPoint plyout;
	double dt = 0;


	if (CamA.NewLocation && CamB.NewLocation)
	{
		ta = (CamA.imgTim - CamA.timeStart) * 2 / 100000.0;
		tb = (CamB.imgTim - CamB.timeStart) * 2 / 100000.0;

	//	cout << "a: "<<CamA.CapedFrameTime << " " << CamB.CapedFrameTime << endl;
		if ((CamA.CapedFrameTime - CamB.CapedFrameTime) < 4 && (CamA.CapedFrameTime - CamB.CapedFrameTime) > -4)
		{
			Point3d pnt = ReconstructBall(CamA.BallLocation, CamB.BallLocation);
			
			//����任 ��������˷���Ϊx������Ϊz
			ballState.x = pnt.y; 
			ballState.y = pnt.x;
			ballState.z = -pnt.z;
			ballState.t = ta;
			
			ballVector.push_back(ballState);
			//------------------------------------------------------------------
			//�������͵㣬ɾ����͵�֮ǰ�ĵ�
			double zMin = ballVector[0].z;
			int zIndex = 0, vLen = ballVector.size();

			for (int i = 1; i < vLen; i++)
			{
				if (ballVector[i].z < zMin)
				{
					zMin = ballVector[i].z;
					zIndex = i;
				}
			}
			if (zIndex != 0 && zIndex != vLen - 1) // ��������͵㣬��ɾ����͵�֮ǰ�����ݡ�
			{
				ballVector.erase(ballVector.begin() + zIndex, ballVector.end());
			}
			//--------------------------------------------------------------------

			int tempSize = 5;
			while (ballVector.size() > tempSize)
			{
				ballVector.erase(ballVector.begin());
			}
			if (ballVector.size() == tempSize)
			{
				plyout = polyfit(ballVector);
				out->Copy(plyout);
				rt = true;
			}
		}
		
		CamA.NewLocation = false;
		CamB.NewLocation = false;
	}
	return rt;
}

void StereoCamera::EndCapture()
{
//	CamA.EndContinuesMode();
//	CamB.EndContinuesMode();

	CamA.EndTRIGGERMode();
	CamB.EndTRIGGERMode();
}

Mat StereoCamera::ReconstructBall(Point pntA, Point pntB)
{
//	Mat P = Mat(3, 1, CV_64F);
	double u1 = pntA.x;
	double v1 = pntA.y;
	double u2 = pntB.x;
	double v2 = pntB.y;

	Mat Hl = Mat(4, 4, CV_64F);

	Hl.row(0) = MA.row(2)*u1 - MA.row(0);
	Hl.row(1) = MA.row(2)*v1 - MA.row(1);
	Hl.row(2) = MB.row(2)*u2 - MB.row(0);
	Hl.row(3) = MB.row(2)*v2 - MB.row(1);

	Mat gls = -Hl.col(3);
	Mat Hl1 = Hl.colRange(Range(0, 3));

    Mat P = ((Hl1.t() * Hl1).inv())*Hl1.t()*gls;

	return P;
}


BallPoint StereoCamera::polyfit(vector<BallState> vb)
{
	int size = vb.size();
	Mat Hl;
	Mat gls;

	float t0 = vb[0].t;
	float t;
	float t_avg = 0;
	Mat temp;
	for (int i = 0; i < size; i++)
	{
		t = vb[i].t - t0;
		
		temp = (Mat_<double>(3, 9) <<	1, t, t*t, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 1, t, t*t, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 1, t, t*t);
		Hl.push_back( temp);

		temp = (Mat_<double>(3, 1) << vb[i].x, vb[i].y, vb[i].z);		
		gls.push_back(temp);

		t_avg += t;
	}
	Mat P = ((Hl.t() * Hl).inv())*Hl.t()*gls;

	t_avg /= size;

	BallPoint out;


	double a[9];
	for (int i = 0; i < 9; i++)
	{
		a[i] = P.at<double>(i, 0);
	}

	
	out.x = a[0] + a[1] * t_avg + a[2] * t_avg*t_avg;
	out.Vx = a[1] + 2 * a[2] * t_avg;

	out.y = a[3] + a[4] * t_avg + a[5] * t_avg*t_avg;
	out.Vy = a[4] + 2 * a[5] * t_avg;

	out.z = a[6] + a[7] * t_avg + a[8] * t_avg*t_avg;
	out.Vz = a[7] + 2 * a[8] * t_avg;

	out.t = t_avg + t0;
	
	//out.DispPoint();
	return out;
}