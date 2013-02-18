// Face.cpp : subor pre pracu s facetmi
#include "stdafx.h"
#include "Face.h"

namespace Model
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
	}

	//destruktor
	Face::~Face()
	{
		// mnou alokovane (new) premenne
		//delete normal;
		//delete center;

		// osetrene v LinkdListe ze sa zmaze cely
		delete susedia;
	}

	void Face::ComputeNormal()
	{
		center = Vector4((v[0]->P.X + v[1]->P.X + v[2]->P.X) / 3.0,
							 (v[0]->P.Y + v[1]->P.Y + v[2]->P.Y) / 3.0,
							 (v[0]->P.Z + v[1]->P.Z + v[2]->P.Z) / 3.0);

		// cross product
		Vector4 U = Vector4(v[1]->P.X - v[0]->P.X, v[1]->P.Y - v[0]->P.Y, v[1]->P.Z - v[0]->P.Z);
		Vector4 V = Vector4(v[2]->P.X - v[0]->P.X, v[2]->P.Y - v[0]->P.Y, v[2]->P.Z - v[0]->P.Z);
		double x = (U.Y * V.Z) - (U.Z * V.Y);
		double y = (U.Z * V.X) - (U.X * V.Z);
		double z = (U.X * V.Y) - (U.Y * V.X);

		double length = sqrt(x * x + y * y + z * z);

		normal = Vector4(x / length, y / length, z / length);
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