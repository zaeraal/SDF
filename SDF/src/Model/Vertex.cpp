// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "Vertex.h"

namespace Model
{
	// konstruktor
	Vertex::Vertex(double x, double y, double z)
	{
		P = new Vector4(x, y, z);
	}

	// destruktor
	Vertex::~Vertex()
	{
		delete P;
	}
}