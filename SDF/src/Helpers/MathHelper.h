// kratky subor s funkciami na matiku
#pragma once
#include "Mat4.h"

using namespace MathStructures;

void GetRadians(double& AngleX, double& Angley);
void GetDegrees(double& AngleX, double& Angley);

Vector4 CalcRayFromAngle(double AngleX, double Angley);
void CalcAnglesFromRay(Vector4 ray, double& AngleX, double& Angley);

bool rayIntersectsTriangle(Vector4 p, Vector4 d, Vector4 v0, Vector4 v1, Vector4 v2, double& t);