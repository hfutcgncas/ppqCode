

/*本文件用于定义本PC与运动控制PC间的通讯函数。包括
1、MotionNetInit()，初始化与运动控制PC的网络连接
2、Cmd2Motion（），向运动控制PC发送预测的击打点及击打时间
*/

#include "cRobotArm.h"


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
	//addrSrv_Motion.sin_addr.S_un.S_addr=inet_addr("192.168.64.82");
	addrSrv_Motion.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv_Motion.sin_family = AF_INET;
	addrSrv_Motion.sin_family = AF_INET;
	addrSrv_Motion.sin_port = htons(7000);

	unsigned long ul = 1;
	int ret = ioctlsocket(sockClient_Motion, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。

	if (ret == SOCKET_ERROR)//设置失败。
	{
		cout << "setting Error" << endl;
	}
	return true;
}


bool cRobotArm::HitBall(double X_hit, double Y_hit, double Z_hit, double hit_vx0, double hit_vy0, double hit_vz0, double t_predict, bool flag_alreadySend)
{
	double Sendbuff[13];

	Sendbuff[0] = (flag_alreadySend) ? 1.0 : 2.0;
	Sendbuff[1] = X_hit * 1000; //机器人坐标系x
	//X_hit的范围进行补偿
	Sendbuff[2] = Y_hit * 1000;

	Sendbuff[3] = Z_hit * 1000; //机器人坐标系z
	Sendbuff[4] = hit_vy0 * 1000;//机器人坐标系x
	Sendbuff[5] = -hit_vx0 * 1000;//机器人坐标系y
	Sendbuff[6] = hit_vz0 * 1000;//机器人坐标系z
	Sendbuff[7] = 0;
	Sendbuff[8] = 0.2; //发送时刻,如果发送时刻，工控机将保存数据
	Sendbuff[9] = 0;
	Sendbuff[10] = 0;
	Sendbuff[11] = 0;
	Sendbuff[12] = t_predict; //ms



	//for (int k = 0;k<13;k++)
	//{
	//	printf(" %4f",Sendbuff[k]);
	//	printf("\n");
	//}

	//运动范围限定
	if (Sendbuff[1] < -720
		|| Sendbuff[1] > 720
		|| Sendbuff[2] > 900
		|| Sendbuff[2] < -145
		|| Sendbuff[3] > 450
		|| Sendbuff[3] < 250)
	{
		/*cout << "x = " << Sendbuff[1] << " \t y = " << Sendbuff[2] << "\t z = " << Sendbuff[3]
		<< "\t Vx= " << Sendbuff[4] << "\t Vy= " << Sendbuff[5] << "\t Vz= " << Sendbuff[6] << endl
		<< "t = " << Sendbuff[12] << endl;*/
		cout << "超过运动范围" << endl;
		cout << "--------" << endl;
		return 0;
	}
	else
	{
		/*cout << "x = " << Sendbuff[1] << "\t y = " << Sendbuff[2] << "\t z = " << Sendbuff[3]
		<< "\t Vx= " << Sendbuff[4] << "\t Vy= " << Sendbuff[5] << "\t Vz= " << Sendbuff[6] << endl
		<< "t = " << Sendbuff[12] << endl;*/

		cout << "HitPara sended" << endl;
		cout << "--------" << endl;

		//getchar();

		if (sendto(sockClient_Motion, (char *)Sendbuff, sizeof(Sendbuff) * 13, 0,
			(SOCKADDR*)&addrSrv_Motion, sizeof(SOCKADDR)))
		{
			//printf("反弹前发送数据成功!\n");
		}
		else
		{
			//printf("反弹前发送数据失败!\n");
		}
		return 1;
	}

};
