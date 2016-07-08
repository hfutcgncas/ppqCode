#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include"cRobotArm.h"

void main(int argc, char *argv[])
{
	cRobotArm robotArm;
	robotArm.ComInit();
	
	if (argc < 8)
	{
		cout << "not enough data" << endl;
		return;;
	}

	double hitx = atof(argv[1]);
	double hity = atof(argv[2]);
	double hitz = atof(argv[3]);
	double hitVx = atof(argv[4]);
	double hitVy = atof(argv[5]);
	double hitVz = atof(argv[6]);
	double hitT = atof(argv[7]);

	robotArm.HitBall(hitx, hity, hitz, hitVx, hitVy, hitVz, hitT, 1);

	
}