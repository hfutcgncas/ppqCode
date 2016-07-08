
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "StereoCamera.h"
#include"BallFinderCamera.h"
#include "GxIAPI.h"
#include "iostream"
#include "MERCamera.h"
#include "GlobleSrouce.h"
#include"cRobotArm.h"
#include"cBallModel.h"
#include<fstream>
//#include"engine.h"


using namespace std;
using namespace cv;


//globle ver--------------------
extern cClock SystemClock;
extern cFile  dataFile;
//extern Engine* ep;
//------------------------------

enum eGatherState
{
	Stage0, //不确定
	Stage1, //机器人开始运动之前
	Stage2, //按照之前的预测开始运动
	Stage3  //机器人已经开始运动且乒乓球未到达击打点
};

struct saveImgLoc {
	int camID;
	saveImgLoc(const int prefix) : camID(prefix) {}
	void operator() (PointInf elem) {
		dataFile.SaveTraceData(elem.pnt.x, elem.pnt.y, camID, elem.TimeStep, 0, 0, 0, NEW_SAVE);
	}
};

struct save3DLoc {
	int camID;
	//save3DLoc(const int prefix) : camID(prefix) {}
	void operator() (BallPoint elem) {
		dataFile.SaveTraceData(elem.x, elem.y, elem.z, elem.t, elem.Vx, elem.Vy, elem.Vz, NEW_SAVE);
	}
};


bool isNewTrace(BallPoint NewPoint, BallPoint LastPoint)
{
	double dt = NewPoint.t - LastPoint.t;
	if (dt > 0.5)return true;

	bool dir = (NewPoint.Vx > 0);
	if (dir && (NewPoint.x - LastPoint.x < -10))return true;  //方向错误，留了10的余量
	if ((!dir) && (NewPoint.x - LastPoint.x > 10))return true;  //方向错误，留了10的余量

	return false;
}



BallPoint polyfit(vector<BallPoint> vb)
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

		temp = (Mat_<double>(3, 9) << 1, t, t*t, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, t, t*t, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, t, t*t);
		Hl.push_back(temp);

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

int main(int argc, char* argv[])
{
#pragma region initial 

	cBallFilter FilterA, FilterB;
	FilterA.resetFilter();
	FilterB.resetFilter();

	cBallModel  ballModel; //乒乓球递推模型

	BallPoint startPoint;
	BallPoint hitPoint;
	BallPoint LastPoint;

	BallPoint hitOutPoint;
	BallPoint LastHitPoint;
	BallPoint ZeroPoint; //全0点

	int Predicted = 0;
	double st1 = -0.3;
	double st2 = 1.1;
	eGatherState gatherState = Stage1;

	bool flag_alreadySend = false;

	bool hitflag = false;
	int rt2 = -1;

	int hitCount = 0; //记录接到的轨迹数量	
	//数据存储空间---------------------------------------------------------------------------
	vector<BallPoint> StoreV;			//重建得到的乒乓球点状态

	vector<BallPoint> StoreHitPoint;	//预测的击打点
	vector<double> cmdSendTime; //传送预测点的时间

	vector<BallPoint> capedTrace;		//用于滤波
	bool caped = false;


	//---------------------------------------------------------------------------------------
	//各部分初始化
	//摄像头
	StereoCamera BallFinder;
	BallFinder.Init("192.168.64.108", "./Cam100.txt", "192.168.64.109", "./Cam101.txt");
	
	
	BallFinder.StartCapture();
	SystemClock.reset(); //本地时钟与摄像机时钟同步。

	//运动机构接口
	cRobotArm	robotArm; 
	robotArm.ComInit();
	
	//按键监控及文件存储
	cShell  shell;
	dataFile.FileInit();
	//---------------------------------------------------------------------------------------

#pragma endregion

	cConfiger cfg(".\\historyData\\cfg.ini");

	cfg.writeCfgFile();
	cfg.loadCfgFile();

	cout << cfg.HitPointFile << endl;

	char cmd;
	while (true) //主循环
	{
		cmd = shell.getCommand();
		if (cmd == 'q')break;
		//--------------------------------------------------
//#pragma region SaveData 
//		if (cmd == 's')
//		{
//			if (dataFile.m_bStartSave)
//			{
//				BallFinder.CamA.saveData = true;
//				BallFinder.CamB.saveData = true;
//			}
//			else
//			{
//				BallFinder.CamA.saveData = false;
//				BallFinder.CamB.saveData = false;
//			}
//		}
//
//		if (!dataFile.m_bStartSave && !StoreV.empty())
//		{
//			//for (vector<BallPoint>::iterator ite = StoreV.begin(); ite != StoreV.end(); ite++)
//			//{
//			//	dataFile.SaveTraceData((*ite).x, (*ite).y, (*ite).z, (*ite).t, (*ite).Vx, (*ite).Vy, (*ite).Vz, NEW_SAVE);
//			//}
//			//dataFile.SaveTraceData(0, 0, 0, 0, 0, 0, 0, CLOSE_FILE);//关闭文件
//			//
//			//StoreV.clear();			
//			//-------------------------------------------
//			cout << "3D Trave: " << StoreV.size() << endl;
//			ofstream file3D;
//			file3D.open("./temporaryData/3Dtrace.txt");
//			for (vector<BallPoint>::iterator ite = StoreV.begin(); ite != StoreV.end(); ite++)
//			{
//				file3D << (*ite).x << " " << (*ite).y << " " << (*ite).z << " " << (*ite).t
//					<< " " << (*ite).Vx << " " << (*ite).Vy << " " << (*ite).Vz << endl;
//			}
//			file3D.close();
//			StoreV.clear();
//		}
//
//		if (!dataFile.m_bStartSave && !StoreHitPoint.empty())
//		{
//			int size = StoreHitPoint.size();
//			cout << "HitPoint: " << size << endl;
//			ofstream fileHitPoint;
//			fileHitPoint.open("./temporaryData/HitPoint.txt");
//			//for (vector<BallPoint>::iterator ite = StoreHitPoint.begin(); ite != StoreHitPoint.end(); ite++)
//			//{
//			//	fileHitPoint << (*ite).x << " " << (*ite).y << " " << (*ite).z << " " << (*ite).t
//			//		<< " " << (*ite).Vx << " " << (*ite).Vy << " " << (*ite).Vz << endl;
//			//}
//			for (int i = 0; i < size; i++)
//			{
//				fileHitPoint << StoreHitPoint[i].x << " " << StoreHitPoint[i].y << " " << StoreHitPoint[i].z << " " << StoreHitPoint[i].t
//					<< " " << StoreHitPoint[i].Vx << " " << StoreHitPoint[i].Vy << " " << StoreHitPoint[i].Vz << " " << cmdSendTime[i]
//					<<endl;
//			}
//			fileHitPoint.close();
//			StoreHitPoint.clear();
//			cmdSendTime.clear();
//		}
//
//		if (!dataFile.m_bStartSave && !BallFinder.CamA.pntQueue.empty())
//		{
//			cout << "A: " << BallFinder.CamA.pntQueue.size() << endl;
//			//	fstream fileA = BallFinder.CamA.file;
//			BallFinder.CamA.file.open(BallFinder.CamA.fileName);
//
//			for (vector<PointInf>::iterator ite = BallFinder.CamA.pntQueue.begin(); ite != BallFinder.CamA.pntQueue.end(); ite++)
//			{
//				BallFinder.CamA.file << ite->pnt.x << " " << ite->pnt.y << " " << ite->TimeStep << endl;
//			}
//			BallFinder.CamA.file.close();
//
//			BallFinder.CamA.pntQueue.clear();
//		}
//
//		if (!dataFile.m_bStartSave && !BallFinder.CamB.pntQueue.empty())
//		{
//			cout << "B: " << BallFinder.CamB.pntQueue.size() << endl;
//			//	fstream fileA = BallFinder.CamA.file;
//			BallFinder.CamB.file.open(BallFinder.CamB.fileName);
//
//			for (vector<PointInf>::iterator ite = BallFinder.CamB.pntQueue.begin(); ite != BallFinder.CamB.pntQueue.end(); ite++)
//			{
//				BallFinder.CamB.file << ite->pnt.x << " " << ite->pnt.y << " " << ite->TimeStep << endl;
//			}
//			BallFinder.CamB.file.close();
//			BallFinder.CamB.pntQueue.clear();
//		}
//
//#	pragma endregion
		//--------------------------------------------------
		//reconstruct 消耗时间小于0.3ms

#pragma region SaveData 
		if (cmd == 's')
		{
			if (dataFile.m_bStartSave)
			{
				BallFinder.CamA.saveData = true;
				BallFinder.CamB.saveData = true;

				cfg.fileIndex++;
				cfg.writeCfgFile();
				cfg.loadCfgFile();
			}
			else
			{
				BallFinder.CamA.saveData = false;
				BallFinder.CamB.saveData = false;
			}
		}

		if (!dataFile.m_bStartSave && !StoreV.empty())
		{
			//for (vector<BallPoint>::iterator ite = StoreV.begin(); ite != StoreV.end(); ite++)
			//{
			//	dataFile.SaveTraceData((*ite).x, (*ite).y, (*ite).z, (*ite).t, (*ite).Vx, (*ite).Vy, (*ite).Vz, NEW_SAVE);
			//}
			//dataFile.SaveTraceData(0, 0, 0, 0, 0, 0, 0, CLOSE_FILE);//关闭文件
			//
			//StoreV.clear();			
			//-------------------------------------------
			cout << "3D Trace: " << StoreV.size() << endl;
			ofstream file3D;
			file3D.open(cfg.ball_3D_TraceFile);
			for (vector<BallPoint>::iterator ite = StoreV.begin(); ite != StoreV.end(); ite++)
			{
				file3D << (*ite).x << " " << (*ite).y << " " << (*ite).z << " " << (*ite).t
					<< " " << (*ite).Vx << " " << (*ite).Vy << " " << (*ite).Vz << endl;
			}
			file3D.close();
			StoreV.clear();
		}

		if (!dataFile.m_bStartSave && !StoreHitPoint.empty())
		{
			int size = StoreHitPoint.size();
			cout << "HitPoint: " << size << endl;
			ofstream fileHitPoint;
			fileHitPoint.open(cfg.HitPointFile);
			//for (vector<BallPoint>::iterator ite = StoreHitPoint.begin(); ite != StoreHitPoint.end(); ite++)
			//{
			//	fileHitPoint << (*ite).x << " " << (*ite).y << " " << (*ite).z << " " << (*ite).t
			//		<< " " << (*ite).Vx << " " << (*ite).Vy << " " << (*ite).Vz << endl;
			//}
			for (int i = 0; i < size; i++)
			{
				fileHitPoint << StoreHitPoint[i].x << " " << StoreHitPoint[i].y << " " << StoreHitPoint[i].z << " " << StoreHitPoint[i].t
					<< " " << StoreHitPoint[i].Vx << " " << StoreHitPoint[i].Vy << " " << StoreHitPoint[i].Vz << " " << cmdSendTime[i]
					<< endl;
			}
			fileHitPoint.close();
			StoreHitPoint.clear();
			cmdSendTime.clear();
		}

		if (!dataFile.m_bStartSave && !BallFinder.CamA.pntQueue.empty())
		{
			cout << "A: " << BallFinder.CamA.pntQueue.size() << endl;
			//	fstream fileA = BallFinder.CamA.file;
			BallFinder.CamA.fileName = cfg.CamAPointFile;
			BallFinder.CamA.file.open(BallFinder.CamA.fileName);

			for (vector<PointInf>::iterator ite = BallFinder.CamA.pntQueue.begin(); ite != BallFinder.CamA.pntQueue.end(); ite++)
			{
				BallFinder.CamA.file << ite->pnt.x << " " << ite->pnt.y << " " << ite->TimeStep << endl;
			}
			BallFinder.CamA.file.close();

			BallFinder.CamA.pntQueue.clear();
		}

		if (!dataFile.m_bStartSave && !BallFinder.CamB.pntQueue.empty())
		{
			cout << "B: " << BallFinder.CamB.pntQueue.size() << endl;
			//	fstream fileA = BallFinder.CamA.file;
			BallFinder.CamB.fileName = cfg.CamBPointFile;
			BallFinder.CamB.file.open(BallFinder.CamB.fileName);

			for (vector<PointInf>::iterator ite = BallFinder.CamB.pntQueue.begin(); ite != BallFinder.CamB.pntQueue.end(); ite++)
			{
				BallFinder.CamB.file << ite->pnt.x << " " << ite->pnt.y << " " << ite->TimeStep << endl;
			}
			BallFinder.CamB.file.close();
			BallFinder.CamB.pntQueue.clear();
		}

#	pragma endregion

#pragma region reconstruct

		caped = BallFinder.CaptureOnceV2(&startPoint);

		if (caped)
		{
			startPoint.x = startPoint.x * 0.001;
			startPoint.y = startPoint.y * 0.001;
			startPoint.z = startPoint.z * 0.001;
			startPoint.t = startPoint.t * 0.001;


			if (dataFile.m_bStartSave)
			{
				StoreV.push_back(startPoint);
				//dataFile.SaveTraceData(startPoint.x, startPoint.y, startPoint.z, startPoint.t, startPoint.Vx, startPoint.Vy, startPoint.Vz, NEW_SAVE);	
			}

			//验证球的状态是否需要预测,若数据明显不对，则不做处理
			if (startPoint.x < -1.1 || startPoint.x > 1			//x轴超限
				|| startPoint.y <-0.75 || startPoint.y > 0.75	//y轴超限
				|| startPoint.z <-0.1 || startPoint.z > 2.0		//z轴超限
				|| startPoint.Vx < 0 || abs(startPoint.Vx) < 1) //正向运动且速度不是太低
			{
				continue;
			}
#pragma endregion 
		//--------------------------------------------------
		//predict & send		
#pragma region predict & send

			//-------------------------
			//轨迹分割 若进入新轨迹，则清空滤波器历史数据。
			if (isNewTrace(startPoint, LastPoint))
			{
				hitCount++;
				cout << "Trace " << hitCount << endl;
				cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << endl;
				BallFinder.ballVector.clear();
				FilterA.resetFilter();
				FilterB.resetFilter();
				gatherState = Stage0;
				flag_alreadySend = false;
				capedTrace.clear();
				st2 = 1.1;
				//	RecordedTrace.clear();
			}
			LastPoint.Copy(startPoint);
			//-------------------------
			capedTrace.push_back(startPoint);

			//------------------------------------------------------------------
			//如果有最低点，删除最低点之前的点
			double zMin = capedTrace[0].z;
			int zIndex = 0, vLen = capedTrace.size();

			for (int i = 1; i < vLen; i++)
			{
				if (capedTrace[i].z < zMin)
				{
					zMin = capedTrace[i].z;
					zIndex = i;
				}
			}
			if (zIndex != 0 && zIndex != vLen - 1)
			{
				capedTrace.erase(capedTrace.begin(), capedTrace.begin() + zIndex);
			}
			//--------------------------------------------------------------------

			while (capedTrace.size() > 5)
			{
				capedTrace.erase(capedTrace.begin());
			}
			if (capedTrace.size() == 5)
			{
				startPoint = polyfit(capedTrace);
				double confidence; //数据置信度

				if (!flag_alreadySend)
				{
					//-------------------------------------------

					rt2 = ballModel.predict_Mut(startPoint, &hitPoint, &confidence); //选择击球点
					if (rt2 != -1)
					{
						
						if (dataFile.m_bStartSave)
						{
							StoreHitPoint.push_back(hitPoint);
							cmdSendTime.push_back(SystemClock.now());
						}				
						if (robotArm.HitBall(hitPoint, flag_alreadySend))
						{
							st2 = ballModel.hitplane[rt2];
							cout << "击打平面 = " << st2 << endl;
							flag_alreadySend = true;
						}
					}

					//-------------------------------------------

					//rt2 = ballModel.predict(startPoint, &hitPoint, &confidence, st2);//从起始预测点接着预测到接打点
					//if (rt2 == PREDICT_NORMAL || rt2 == PREDICT_NOREBOND)
					//{
					//	//滤波----------------
					//	//这里可以加上对反弹后的点以及位置信息所对应的权重调整
					//	//FilterA.updateState(hitPoint, 0.2);
					//	//hitPoint.Copy(FilterA.currentPoint);
					//	//-------------------
					//	if (dataFile.m_bStartSave)
					//	{
					//		StoreHitPoint.push_back(hitPoint);
					//		cmdSendTime.push_back(SystemClock.now());
					//	}
					//	if (robotArm.HitBall(hitPoint, flag_alreadySend))
					//	{

					//		flag_alreadySend = true;
					//	}
					//}

				}
				else
				{
					rt2 = ballModel.predict(startPoint, &hitPoint, &confidence, st2);//从起始预测点接着预测到接打点
					if (rt2 == PREDICT_NORMAL || rt2 == PREDICT_NOREBOND)
					{
						//滤波----------------
						//这里可以加上对反弹后的点以及位置信息所对应的权重调整
						//FilterA.updateState(hitPoint, 0.2);
						//hitPoint.Copy(FilterA.currentPoint);
						//-------------------
						if (dataFile.m_bStartSave)
						{
							StoreHitPoint.push_back(hitPoint);
							cmdSendTime.push_back(SystemClock.now());
						}
						if (robotArm.HitBall(hitPoint, flag_alreadySend))
						{
				
							flag_alreadySend = true;
						}
					}
				}				
			}

#pragma endregion

		}
	}
	BallFinder.EndCapture();
	getchar();
	return 0;
}


