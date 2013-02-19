// kratky subor s funkciami na matiku
#pragma once
#include "Mat4.h"

using namespace MathStructures;

void GetRadians(double& AngleX, double& Angley);
void GetDegrees(double& AngleX, double& Angley);

Vector4 CalcRayFromAngle(double AngleX, double Angley);
void CalcAnglesFromRay(Vector4 ray, double& AngleX, double& Angley);