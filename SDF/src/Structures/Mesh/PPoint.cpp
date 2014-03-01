// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "PPoint.h"

namespace MeshStructures
{
	// konstruktor
	PPoint::PPoint(float x, float y, float z, Face* refference)
	{
		P.Init(x, y, z, 1.0f);
		ref = refference;
		diameter = 0.0f;
		number = 0;
	}

	PPoint::PPoint(Vector4 position, Face* refference)
	{
		P = position;
		P.W = 1.0f;		// preistotu.. toto su body a teda W = 1
		ref = refference;
		diameter = 0.0f;
	}
}