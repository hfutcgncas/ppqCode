

/*本文件用于定义本PC与运动控制PC间的通讯函数。包括
1、MotionNetInit()，初始化与运动控制PC的网络连接
2、Cmd2Motion（），向运动控制PC发送预测的击打点及击打时间
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include "cRobotArm.h"
#include"dataStruct.h"
#include"GlobleSrouce.h"

cRobotArm::cRobotArm()
{
}


cRobotArm::~cRobotArm()
{
}


SOCKET sockClient_Motion;
SOCKADDR_IN addrSrv_Motion;

bool cRobotArm::ComInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}
	sockClient_Motion = socket(AF_INET, SOCK_DGRAM, 0);
	//addrSrv_Motion.sin_addr.S_un.S_addr=inet_addr("192.168.64.82"); //视觉台式机
	//addrSrv_Motion.sin_addr.S_un.S_addr=inet_addr("192.168.64.86"); //笔记本
	addrSrv_Motion.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
	addrSrv_Motion.sin_family = AF_INET;
//	addrSrv_Motion.sin_family = AF_INET;
	addrSrv_Motion.sin_port = htons(7000);

	unsigned long ul = 1;
	int ret = ioctlsocket(sockClient_Motion, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。

	if (ret == SOCKET_ERROR)//设置失败。
	{
		cout << "setting Error" << endl;
	}
	return true;
}


extern cClock SystemClock;
bool cRobotArm::HitBall(double X_hit, double Y_hit, double Z_hit, double hit_vx0, double hit_vy0, double hit_vz0, double t_predict, bool flag_alreadySend)
{
	double Sendbuff[13];

	Sendbuff[0] = (flag_alreadySend) ? 1.0 : 2.0;
	Sendbuff[1] = (Y_hit) * 1000; //机器人坐标系x
	Sendbuff[2] = (1.37-X_hit) * 1000;//机器人坐标系y
	Sendbuff[3] = Z_hit * 1000; //机器人坐标系z
	Sendbuff[4] = hit_vy0 * 1000;//机器人坐标系x
	Sendbuff[5] = -hit_vx0 * 1000;//机器人坐标系y
	Sendbuff[6] = hit_vz0 * 1000;//机器人坐标系z
	Sendbuff[7] = 0;
	Sendbuff[8] = SystemClock.now(); //发送时刻,如果发送时刻，工控机将保存数据
	Sendbuff[9] = 0;
	Sendbuff[10] = 0;
	Sendbuff[11] = 0;
	Sendbuff[12] = t_predict - SystemClock.now() / 1000; //ms 变为相对预测时间。


	//运动范围限定
	if (Sendbuff[1] < -720 || Sendbuff[1] > 720)
	{
		cout << "超过x(左右)范围" << endl;
		cout << "x: " << Sendbuff[1] << endl;
		return 0;
	}
	if (Sendbuff[2] > 900  || Sendbuff[2] < -145)
	{
		cout << "超过y(前后)范围" << endl;
		cout << "y: " << Sendbuff[2] << endl;
		return 0;
	}
	if (  Sendbuff[3] > 410 || Sendbuff[3] < 190)
	{
		cout << "超过z(上下)范围" << endl;
		cout << "z: " << Sendbuff[3] << endl;
		return 0;
	}
	else
	{
		//cout << "x = " << Sendbuff[1] << " \t y = " << Sendbuff[2] << "\t z = " << Sendbuff[3]
		//	<< "\t Vx= " << Sendbuff[4] << "\t Vy= " << Sendbuff[5] << "\t Vz= " << Sendbuff[6] << endl
		//	<< "t = " << Sendbuff[12] << endl;
		//cout << "HitPara sended" << endl;
		cout << "o";
		

		//getchar();

		if (sendto(sockClient_Motion, (char *)Sendbuff, sizeof(Sendbuff) * 13, 0,
			(SOCKADDR*)&addrSrv_Motion, sizeof(SOCKADDR)))
		{
			//printf("反弹前发送数据成功!\n");
			return 1;
		}
		else
		{
			//printf("反弹前发送数据失败!\n");
			return 0;
		}
		
	}

};

bool cRobotArm::HitBall(BallPoint hitPoint, bool flag_alreadySend)
{
	return HitBall(hitPoint.x, hitPoint.y, hitPoint.z, hitPoint.Vx, hitPoint.Vy, hitPoint.Vz, hitPoint.t, flag_alreadySend);
};