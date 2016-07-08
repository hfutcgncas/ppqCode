

/*���ļ����ڶ��屾PC���˶�����PC���ͨѶ����������
1��MotionNetInit()����ʼ�����˶�����PC����������
2��Cmd2Motion���������˶�����PC����Ԥ��Ļ���㼰����ʱ��
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
	int ret = ioctlsocket(sockClient_Motion, FIONBIO, (unsigned long *)&ul);//���óɷ�����ģʽ��

	if (ret == SOCKET_ERROR)//����ʧ�ܡ�
	{
		cout << "setting Error" << endl;
	}
	return true;
}


bool cRobotArm::HitBall(double X_hit, double Y_hit, double Z_hit, double hit_vx0, double hit_vy0, double hit_vz0, double t_predict, bool flag_alreadySend)
{
	double Sendbuff[13];

	Sendbuff[0] = (flag_alreadySend) ? 1.0 : 2.0;
	Sendbuff[1] = X_hit * 1000; //����������ϵx
	//X_hit�ķ�Χ���в���
	Sendbuff[2] = Y_hit * 1000;

	Sendbuff[3] = Z_hit * 1000; //����������ϵz
	Sendbuff[4] = hit_vy0 * 1000;//����������ϵx
	Sendbuff[5] = -hit_vx0 * 1000;//����������ϵy
	Sendbuff[6] = hit_vz0 * 1000;//����������ϵz
	Sendbuff[7] = 0;
	Sendbuff[8] = 0.2; //����ʱ��,�������ʱ�̣����ػ�����������
	Sendbuff[9] = 0;
	Sendbuff[10] = 0;
	Sendbuff[11] = 0;
	Sendbuff[12] = t_predict; //ms



	//for (int k = 0;k<13;k++)
	//{
	//	printf(" %4f",Sendbuff[k]);
	//	printf("\n");
	//}

	//�˶���Χ�޶�
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
		cout << "�����˶���Χ" << endl;
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
			//printf("����ǰ�������ݳɹ�!\n");
		}
		else
		{
			//printf("����ǰ��������ʧ��!\n");
		}
		return 1;
	}

};
