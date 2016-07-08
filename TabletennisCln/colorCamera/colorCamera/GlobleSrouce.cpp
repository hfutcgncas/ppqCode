#include"GlobleSrouce.h"
#include"math.h"

#include"iostream"
//#include"engine.h"
using namespace std;
//globle ver--------------------
cFile  dataFile;
cClock SystemClock;
//Engine* ep;
//-----------------------------------------



cClock::cClock()
{
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}
void cClock::reset()
{
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}
double cClock::now()
//��ǰʱ�䵥λΪms
{
	double nowRead;
	QueryPerformanceCounter(&litmp);
	timeSysNow = litmp.QuadPart; 
	timeSysNow -= timeSysStart;
	if(timeSysNow<0)timeSysNow += 0x7FFFFFFF;  //LONGLONG Ϊ64λ���Σ�����˽�λ
	nowRead    = timeSysNow/dfFreq*1000.0;
	if(nowRead>1000000)
	{
		nowRead -= 1000000;
		this->reset();
	}
	return nowRead;
}

//==============================================

char cShell::getCommand()
{
	key = '0';
	if (_kbhit())
	{//�а���
		key = _getch();
		switch(key)
		{
		case 'q':
			dataFile.SaveTraceData(0, 0, 0, 0, 0, 0, 0, CLOSE_FILE);
			printf("\n�˳�����.\n");
			break;
		case 's':
			//�����ļ�,��һ��S������һ�Σ������������
			if (!dataFile.m_bStartSave)
			{
				dataFile.m_bStartSave = true;
				printf("Begin to Save File.\n");
			}
			else
			{
				dataFile.m_bStartSave = false;
				//dataFile.SaveTraceData(0, 0, 0, 0, 0, 0, 0, CLOSE_FILE);//�ر��ļ�
				printf("Stop Save File.\n");
			//	RunMatlab();
			}
			break;
		default:
			printf("\n�������.\n");
		}
	}
	return key;

}


//void RunMatlab()
//{
//
//
//	if (ep == NULL)return;
//	
//	engEvalString(ep, "plotData");
//	//engClose(ep);
//}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cFile::cFile()
{
	m_bStartSave  = false ;
	bFileCreate   = false ;
	fileNameIndex = 0     ;
}

//===============================================================
void cFile::SaveTraceData(double x, double y, double z, double t, double vx, double vy, double vz, dataSaveFlag flag)
{//��������
	//flag = 88:����88������ 98:����98������,�������ر��ļ�
	if (flag == CLOSE_FILE)
	{//�ر�
		if (bFileCreate)
		{
			fclose(fp);
			bFileCreate = false;
			char data[100];
			sprintf(data,"%d",++fileNameIndex);
			WritePrivateProfileString("SaveFilePara","FileIndex",data, "ballData\\sys.ini");
		}
		return;
	}
	if (flag == NEW_SAVE)
	{
		if (bFileCreate==false)
		{
			//��ȡini�ļ�
			char iniData[100];
			GetPrivateProfileString("SaveFilePara","FileIndex","0",iniData,100,"ballData\\sys.ini");
			fileNameIndex = atoi(iniData);		
			sprintf(fileName,"%s\\ballData%d.txt",DirName,fileNameIndex);
			if((fp=fopen(fileName,"at"))==NULL)
			{//����
				printf("\nCreate File Failed!\n");
				return;
			}
			bFileCreate = true;
		}
		else
		{//д��
			fprintf(fp,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f\n", x,y,z,t,vx,vy,vz);
		}
	}
}
//======================================
bool cFile::FileInit()
{
	struct tm *localTime;
	time_t ti;
	char strTime[100];
	strTime[0]='0';
	ti=time(NULL); 
	localTime=localtime(&ti);	//��ȡ���� 
	
	if(_access("ballData",0)==-1)
	{//�ļ��в�����
		_mkdir("ballData");
	}
	
	sprintf(DirName,"ballData\\��켣����_%04d_%02d_%02d",
		localTime->tm_year+1900,
		localTime->tm_mon+1,
		localTime->tm_mday);
	if (_access(DirName,0)==-1)
	{
		_mkdir(DirName); 
		DeleteFile("ballData\\sys.ini");
		//�½��ļ���
	}
	return 1;
}
//=================================================================================================
//=================================================================================================
//=================================================================================================
void cBallFilter::resetFilter()
{
	BallPoint ZeroPoint;//ȫ���
	lambda_Last = 0.8;
	lambda_Now = 0.2;
	LastPoint.Copy(ZeroPoint);
	isInited = false;

}
bool cBallFilter::updateState(BallPoint NewPoint, float NewLambda)
{
	if (NewLambda < 0 || NewLambda>1)
	{
		cout << "wrong lambda" << endl;
		return false;
	}
	if (NewPoint.isNan()) //Ԥ�������ʱ��ⲻ���𰸣���ʱ������ʷֵ��
	{
		currentPoint.Copy(LastPoint);
		return false;
	}
	else if (isInited == false)
	{
		currentPoint.Copy(NewPoint);
		LastPoint.Copy(currentPoint);
		isInited = true;
		return true;
	}
	else
	{
		//lambda_Now = NewLambda;
		double t = NewPoint.t;
		currentPoint = NewPoint*NewLambda + LastPoint*(1 - NewLambda);
		currentPoint.t = t;
		LastPoint.Copy(currentPoint);
		return true;
	}
}

//=================================================================================================
bool isInScope(BallPoint hitpoint)
{
	double x = (hitpoint.y)* 1000; //����������ϵx
	double y = (1.37 - hitpoint.x) * 1000;
	double z = hitpoint.z * 1000; //����������ϵz
	double Vx = hitpoint.Vy * 1000;//����������ϵx
	double Vy = -hitpoint.Vx * 1000;//����������ϵy
	double Vz = hitpoint.Vz * 1000;//����������ϵz

	if (   x < -720 ||   x > 720
		|| y > 900 || y < -145
		|| z > 410 || z < 190)
	{
		return false;
	}
	return true;
}