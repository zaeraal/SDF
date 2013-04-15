// kratky subor s funkciami na matiku
#pragma once
#include "Mat4.h"

using namespace MathStructures;

void GetRadians(float& AngleX, float& Angley);
void GetDegrees(float& AngleX, float& Angley);

Vector4 CalcRayFromAngle(float AngleX, float Angley);
void CalcAnglesFromRay(Vector4 ray, float& AngleX, float& Angley);

bool rayIntersectsTriangle(Vector4 p, Vector4 d, Vector4 v0, Vector4 v1, Vector4 v2, float& t);

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
void FINDMINMAX(float X, float Y, float Z, float& min, float& max);
void getVMinVMax(float normal, float maxbox, float& vmin, float& vmax);
bool planeBoxOverlap(Vector4 normal, Vector4 vert, float maxbox);
bool AXISTEST(float a, float b, float fa, float fb, float boxhalfsize, float k1, float k2, float l1, float l2);
bool triBoxOverlap(Vector4 boxcenter, float boxhalfsize, Vector4 p1, Vector4 p2, Vector4 p3);