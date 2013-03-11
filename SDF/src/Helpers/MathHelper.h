// kratky subor s funkciami na matiku
#pragma once
#include "Mat4.h"

using namespace MathStructures;

void GetRadians(double& AngleX, double& Angley);
void GetDegrees(double& AngleX, double& Angley);

Vector4 CalcRayFromAngle(double AngleX, double Angley);
void CalcAnglesFromRay(Vector4 ray, double& AngleX, double& Angley);

bool rayIntersectsTriangle(Vector4 p, Vector4 d, Vector4 v0, Vector4 v1, Vector4 v2, double& t);

/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
void FINDMINMAX(double X, double Y, double Z, double& min, double& max);
void getVMinVMax(double normal, double maxbox, double& vmin, double& vmax);
bool planeBoxOverlap(Vector4 normal, Vector4 vert, double maxbox);
bool AXISTEST(double a, double b, double fa, double fb, double boxhalfsize, double k1, double k2, double l1, double l2);
bool triBoxOverlap(Vector4 boxcenter, double boxhalfsize, Vector4 p1, Vector4 p2, Vector4 p3);