// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "Vertex.h"

namespace MeshStructures
{
	// konstruktor
	Vertex::Vertex(float x, float y, float z)
	{
		P.Init(x, y, z, 1.0);
		susedia = new LinkedList<void>();
	}

	Vertex::Vertex(Vector4 position)
	{
		P = position;
		P.W = 1.0;		// preistotu.. toto su body a teda W = 1
		susedia = new LinkedList<void>();
	}

	// destruktor
	Vertex::~Vertex()
	{
		delete susedia;
	}
}