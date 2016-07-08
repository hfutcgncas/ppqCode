#pragma once
#include <vector>

using std::vector;

/*************************************************
class:				    BallPoint
Description:			乒乓球的状态
修改记录：               2015/11/30 创建
*************************************************/
class BallPoint
{
public:
#pragma region State x,y,z,Vx,Vy,Vz,Wx,Wy,Wz,t
	double x; double y; double z;

	double Vx; double Vy; double Vz;

	double Wx; double Wy; double Wz;

	double t;
#pragma endregion

#pragma region operator
	void Copy(BallPoint src);
	BallPoint operator*(double fac);
	BallPoint operator+(BallPoint &A);
	BallPoint operator-(BallPoint &A);
#pragma endregion

	BallPoint();
	void DispPoint();

	bool isNan();

};
/*************************************************
class:				    cBallPosTemp
Description:			乒乓球的状态的队列
修改记录：               2015/11/30 创建
*************************************************/
class cBallPosTemp
{
public:
	vector<double> tempX;
	vector<double> tempY;
	vector<double> tempZ;
	vector<double> tempVx;
	vector<double> tempVy;
	vector<double> tempVz;
	vector<double> tempWx;
	vector<double> tempWy;
	vector<double> tempWz;
	vector<double> tempTime;

	void push_back(double x, double y, double z, double vx, double vy, double vz, double wx, double wy, double wz, double time);
	void push_back(BallPoint newPoint);

	void clear();
	int  size();
	void  del();//删除队列头部的数据
	void copy(cBallPosTemp src);
};