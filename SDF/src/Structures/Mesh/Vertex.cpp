// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "Vertex.h"

namespace MeshStructures
{
	// konstruktor
	Vertex::Vertex(double x, double y, double z)
	{
		P.Init(x, y, z, 1.0);
	}

	Vertex::Vertex(Vector4 position)
	{
		P = position;
		P.W = 1.0;		// preistotu.. toto su body a teda W = 1
	}

	// destruktor
	Vertex::~Vertex()
	{
		// P deleted automatically
	}
}