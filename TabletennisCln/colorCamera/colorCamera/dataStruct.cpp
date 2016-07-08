#include"dataStruct.h"
#include"iostream"
using namespace std;




//初始化，全部赋值为0
BallPoint::BallPoint()
{
	x = 0;
	y = 0;
	z = 0;
	Vx = 0;
	Vy = 0;
	Vz = 0;
	Wx = 0;
	Wy = 0;
	Wz = 0;
	t = 0;
}

BallPoint BallPoint::operator * (double fac)
{
	BallPoint out;
	out.x = x*fac;
	out.y = y*fac;
	out.z = z*fac;
	out.Vx = Vx*fac;
	out.Vy = Vy*fac;
	out.Vz = Vz*fac;
	out.Wx = Wx*fac;
	out.Wy = Wy*fac;
	out.Wz = Wz*fac;
	out.t = t*fac;

	return out;
}
BallPoint BallPoint::operator + (BallPoint &A)
{
	BallPoint out;
	out.x = x + A.x;
	out.y = y + A.y;
	out.z = z + A.z;
	out.Vx = Vx + A.Vx;
	out.Vy = Vy + A.Vy;
	out.Vz = Vz + A.Vz;
	out.Wx = Wx + A.Wx;
	out.Wy = Wy + A.Wy;
	out.Wz = Wz + A.Wz;
	out.t = t + A.t;
	return out;
}
BallPoint BallPoint::operator - (BallPoint &A)
{
	BallPoint out;
	out.x = x - A.x;
	out.y = y - A.y;
	out.z = z - A.z;
	out.Vx = Vx - A.Vx;
	out.Vy = Vy - A.Vy;
	out.Vz = Vz - A.Vz;
	out.Wx = Wx - A.Wx;
	out.Wy = Wy - A.Wy;
	out.Wz = Wz - A.Wz;
	out.t = t - A.t;
	return out;
}

void  BallPoint::Copy(BallPoint src)
{
	x = src.x;
	y = src.y;
	z = src.z;
	Vx = src.Vx;
	Vy = src.Vy;
	Vz = src.Vz;
	Wx = src.Wx;
	Wy = src.Wy;
	Wz = src.Wz;
	t = src.t;
}
void  BallPoint::DispPoint()
{
	cout << "pos:" << endl;
	cout << x << " " << y << " " << z << " " << Vx << " "<<Vy << " " << t << endl;
}

bool BallPoint::isNan()
{
	return isnan(x) && isnan(y) && isnan(z) && isnan(Vx) && isnan(Vy) && isnan(Vz) && isnan(t);
}
//=================================================================================================
//=================================================================================================
//=================================================================================================

////==========================================================================================
//乒乓球3D位置队列cHis3DTemp的实现
//==========================================================================================
void cBallPosTemp::push_back(double x, double y, double z, double vx, double vy, double vz, double wx, double wy, double wz, double time)
{
	tempX.push_back(x);
	tempY.push_back(y);
	tempZ.push_back(z);
	tempVx.push_back(vx);
	tempVy.push_back(vy);
	tempVz.push_back(vz);
	tempWx.push_back(wx);
	tempWy.push_back(wy);
	tempWz.push_back(wz);
	tempTime.push_back(time);
}
void cBallPosTemp::push_back(BallPoint newPoint)
{
	tempX.push_back(newPoint.x);
	tempY.push_back(newPoint.y);
	tempZ.push_back(newPoint.z);
	tempVx.push_back(newPoint.Vx);
	tempVy.push_back(newPoint.Vy);
	tempVz.push_back(newPoint.Vz);
	tempWx.push_back(newPoint.Wx);
	tempWy.push_back(newPoint.Wy);
	tempWz.push_back(newPoint.Wz);
	tempTime.push_back(newPoint.t);

}

void cBallPosTemp::clear()
{
	tempX.clear();
	tempY.clear();
	tempZ.clear();
	tempVx.clear();
	tempVy.clear();
	tempVz.clear();
	tempWx.clear();
	tempWy.clear();
	tempWz.clear();
	tempTime.clear();
}
int cBallPosTemp::size()
{
	return (int)(tempX.size());
}
void cBallPosTemp::del()
{
	tempX.erase(tempX.begin());
	tempY.erase(tempY.begin());
	tempZ.erase(tempZ.begin());
	tempVx.erase(tempVx.begin());
	tempVy.erase(tempVy.begin());
	tempVz.erase(tempVz.begin());
	tempWx.erase(tempWx.begin());
	tempWy.erase(tempWy.begin());
	tempWz.erase(tempWz.begin());
	tempTime.erase(tempTime.begin());
}
void cBallPosTemp::copy(cBallPosTemp src)
{
	tempX = src.tempX;
	tempY = src.tempY;
	tempZ = src.tempZ;
	tempVx = src.tempVx;
	tempVy = src.tempVy;
	tempVz = src.tempVz;
	tempWx = src.tempWx;
	tempWy = src.tempWy;
	tempWz = src.tempWz;
	tempTime = src.tempTime;
}
