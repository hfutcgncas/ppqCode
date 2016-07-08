#pragma once

#include "stdio.h"
#include <iostream>
#include <io.h> //_access
#include"GlobleSrouce.h"
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include"dataStruct.h"

using namespace std;

class cRobotArm
{
public:
	cRobotArm();
	~cRobotArm();
	bool ComInit();
	bool HitBall(double X_hit, double Y_hit, double Z_hit, double hit_vx0, double hit_vy0, double hit_vz0, double t_predict, bool flag_alreadySend);
	bool cRobotArm::HitBall(BallPoint hitPoint, bool flag_alreadySend);
};

