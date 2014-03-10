// Vertex.cpp : subor pre manipulaciu s 3D bodom
#include "stdafx.h"
#include "Vertex.h"

namespace MeshStructures
{
	// konstruktor
	Vertex::Vertex(float x, float y, float z)
	{
		P.Init(x, y, z, 1.0f);
		susedia = new LinkedList<void>();
		quality = new CSDF();
		normal.Init(0.0f, 0.0f, 0.0f);
		has_normal = false;
		texCoord_U = 0.0f;
		texCoord_V = 0.0f;
	}

	Vertex::Vertex(Vector4 position)
	{
		P = position;
		P.W = 1.0f;		// preistotu.. toto su body a teda W = 1
		susedia = new LinkedList<void>();
		quality = new CSDF();
		normal.Init(0.0f, 0.0f, 0.0f);
		has_normal = false;
		texCoord_U = 0.0f;
		texCoord_V = 0.0f;
	}

	// destruktor
	Vertex::~Vertex()
	{
		delete susedia;
		delete quality;
	}

	bool Vertex::HasNormal()
	{
		return has_normal;
	}
	void Vertex::SetNormal(Vector4 normala)
	{
		normal = normala;
		has_normal = true;
	}
	Vector4 Vertex::GetNormal()
	{
		return normal;
	}
}