// Face.cpp : subor pre pracu s facetmi
#include "stdafx.h"
#include "Face.h"

namespace MeshStructures
{
	// konstruktor
	Face::Face(Vertex* v1, Vertex* v2, Vertex* v3)
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		farba = 0;
		susedia = NULL;
		ComputeNormal();
		diameter = new CSDF();
	}

	//destruktor
	Face::~Face()
	{
		// osetrene v LinkdListe ze sa zmaze cely
		delete susedia;
		delete diameter;
	}

	void Face::ComputeNormal()
	{
		center = Vector4((v[0]->P.X + v[1]->P.X + v[2]->P.X) / 3.0,
							 (v[0]->P.Y + v[1]->P.Y + v[2]->P.Y) / 3.0,
							 (v[0]->P.Z + v[1]->P.Z + v[2]->P.Z) / 3.0,
							 1.0);

		// cross product
		Vector4 U = Vector4(v[1]->P.X - v[0]->P.X, v[1]->P.Y - v[0]->P.Y, v[1]->P.Z - v[0]->P.Z);
		Vector4 V = Vector4(v[2]->P.X - v[0]->P.X, v[2]->P.Y - v[0]->P.Y, v[2]->P.Z - v[0]->P.Z);
		normal = U % V;
		normal.Normalize();
	}

	void Face::AddSused(Face* sused)
	{
		if(susedia == NULL)
			susedia = new LinkedList<Face>(sused);
		else
			susedia->InsertToEnd(sused);
	}

	void Face::SetColor(int color)
	{
		farba = color;
	}
}