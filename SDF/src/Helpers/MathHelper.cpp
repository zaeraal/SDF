// kratky subor s funkciami na matiku
#include "stdafx.h"
#include "MathHelper.h"

// get radians from degrees
void GetRadians(double& AngleX, double& Angley)
{
	AngleX = AngleX * (M_PI / 180.0);
	Angley = Angley * (M_PI / 180.0);
}

//get degrees from radians
void GetDegrees(double& AngleX, double& Angley)
{
	AngleX = AngleX * (180.0 / M_PI);
	Angley = Angley * (180.0 / M_PI);
}

Vector4 CalcRayFromAngle(double AngleX, double Angley)
{
	Vector4 result;
	GetRadians(AngleX, Angley);
	result.X = sin(Angley) * sin(AngleX);
	result.Y = cos(Angley);
	result.Z = sin(Angley) * cos(AngleX);
	return result;
}

void CalcAnglesFromRay(Vector4 ray, double& AngleX, double& Angley)
{
	double r = sqrt(ray.X*ray.X + ray.Y*ray.Y + ray.Z*ray.Z);
	AngleX = atan2(ray.Z, ray.X);
	Angley = acos(ray.Y / r);
}

// prienik luca a trojuholnika
// p - zdroj luca, d - smer luca, v0,v1,v2 - body trojuholniku
bool rayIntersectsTriangle(Vector4 p, Vector4 d, Vector4 v0, Vector4 v1, Vector4 v2, double& t)
{
	Vector4 e1 = v1 - v0;
	Vector4 e2 = v2 - v0;
	double a,f,u,v;

	Vector4 h = d % e2;
	a = e1 * h;

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1.0 / a;
	Vector4 s = p - v0;
	u = f * (s*h);

	if (u < 0.0 || u > 1.0)
		return false;

	Vector4 q = s % e1;
	v = f * (d*q);

	if (v < 0.0 || u + v > 1.0)
		return false ;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * (e2*q);

	if (t > 0.00001) // ray intersection (v smere luca)
		return true;

	else // this means that there is a line intersection
		 // but not a ray intersection
		 return false;
}