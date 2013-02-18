// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "Vertex.h"

namespace Model
{
	// konstruktor
	Vertex::Vertex(double x, double y, double z)
	{
		P.Init(x, y, z);
	}

	Vertex::Vertex(Vector4 position)
	{
		P = position;
	}

	// destruktor
	Vertex::~Vertex()
	{
		// P deleted automatically
	}
}