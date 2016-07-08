#ifndef _GLOBLESROUCE_H_
#define _GLOBLESROUCE_H_
#pragma once

#include "time.h" // time
#include "stdio.h"

#include <io.h> //_access
#include"string.h"
#include "direct.h" // mkdir
#include "time.h" // time

#include "conio.h"//键盘

#include"dataStruct.h"
#include <Winsock2.h>

#include<fstream>
#pragma comment(lib, "ws2_32.lib")


#define PREDICT_NORMAL 1
#define PREDICT_NOREBOND 2
#define PREDICT_TIMEOUT 3

//using namespace std;

enum dataSaveFlag { CLOSE_FILE = 1, NEW_SAVE = 88 };

//时钟================================
class cClock
{
public:
	LARGE_INTEGER litmp;
	LONGLONG timeSysStart,timeSysNow;
	double dfFreq,dfMinus; 

	cClock();
	double now();
	void reset();
};

//配置参数============================
class cConfiger
{
public:
	int fileIndex;

	char ball_3D_TraceFile[100];
	char HitPointFile[100];
	char CamAPointFile[100];
	char CamBPointFile[100];

	char	iniName[100];
	char	DirName[100];

	//string imgPointFileBase;
	//string ball_3D_TraceFile;
	//string HitPointFile;


	cConfiger(char* iniFile)
	{ 
		fileIndex = 0;
		sprintf(iniName, iniFile);

		char iniData[100];
		GetPrivateProfileString("SaveFilePara", "FileIndex", "0", iniData, 100, iniName);
		fileIndex = atoi(iniData);


		
		struct tm *localTime;
		time_t ti;
		char strTime[100];
		strTime[0] = '0';
		ti = time(NULL);
		localTime = localtime(&ti);	//获取日期 

		if (_access("historyData", 0) == -1)
		{//文件夹不存在
			_mkdir("historyData");
		}

		sprintf(DirName, "historyData\\球轨迹数据_%04d_%02d_%02d",
			localTime->tm_year + 1900,
			localTime->tm_mon + 1,
			localTime->tm_mday);
		if (_access(DirName, 0) == -1)
		{
			_mkdir(DirName);
			writeCfgFile();
			//DeleteFile(iniName);
			//新建文件夹
		}
	}

	bool writeCfgFile()
	{
		char iniData[100];
		sprintf(iniData, "%d", fileIndex);
		WritePrivateProfileString("SaveFilePara", "FileIndex", iniData, iniName);
		WritePrivateProfileString("SaveFilePara", "DirName", DirName, iniName);

		return true;
	}

	bool loadCfgFile()
	{
		//读取ini文件
		char iniData[100];
		GetPrivateProfileString("SaveFilePara", "FileIndex", "0", iniData, 100, iniName);
		fileIndex = atoi(iniData);

		sprintf(ball_3D_TraceFile, "%s\\%d_3DTrace.txt", DirName, fileIndex);
		//ball_3D_TraceFile[0] = '\0';
		//strcat(ball_3D_TraceFile, DirName);
		//strcat(ball_3D_TraceFile, "\\3DTrace_");
		//strcat(ball_3D_TraceFile, iniData);
		//strcat(ball_3D_TraceFile, ".txt");

		sprintf(HitPointFile, "%s\\%d_HitPoint.txt", DirName, fileIndex);
		sprintf(CamAPointFile, "%s\\%d_imgPointA.txt", DirName, fileIndex);
		sprintf(CamBPointFile, "%s\\%d_imgPointB.txt", DirName, fileIndex);


	/*	CamAPointFile[0] = '\0';
		strcat(CamAPointFile, DirName);
		strcat(CamAPointFile, "\\imgPointA_");
		strcat(CamAPointFile, iniData);
		strcat(CamAPointFile, ".txt");

		CamBPointFile[0] = '\0';
		strcat(CamBPointFile, DirName);
		strcat(CamBPointFile, "\\imgPointB_");
		strcat(CamBPointFile, iniData);
		strcat(CamBPointFile, ".txt");*/
		
		return true;
	}

};


//记录文件===========================

//class DataFile
//{
//public:
//	string fileName;
//	fstream file;
//	bool WriteFile();
//	bool ColseNewFile();
//};

//shell===========================
class cShell
{
public:
	char key;

	char getCommand();
};
void RunMatlab();




class cFile
{
	char	fileName[100];
	FILE	*fp;
	bool	bFileCreate  ;
	int		fileNameIndex;
	char	DirName[100];
public:
	cFile();

	bool m_bStartSave;
	bool FileInit();
	void SaveTraceData(double x, double y, double z, double t, double vx, double vy, double vz, dataSaveFlag flag);
};
//=================================================================================================

//
//class cBallModel
//{
//	inline double signFun(double a)
//	{
//		if (a == 0)
//		{
//			return 0.0;
//		}
//		if (a> 0)
//		{
//			return a;
//		}
//		else
//			return -a;
//	}
//public:
//	bool predict_OneStep(BallPoint In, BallPoint* pOut, double dt);
//	int predict(BallPoint startPoint, BallPoint* pendPoint, double stopPlane);
//	bool predict_Rebound(BallPoint In, BallPoint* pOut);
//};

//=================================================================================================
//=================================================================================================
//=================================================================================================

class cBallFilter
{
public:
	float lambda_Last;
	float lambda_Now;
	BallPoint LastPoint;
	BallPoint currentPoint;

	bool isInited = false;
	void resetFilter(void);
	bool updateState(BallPoint NewPoint, float NewLambda);
};

bool isInScope(BallPoint hitpoint);


#endif

