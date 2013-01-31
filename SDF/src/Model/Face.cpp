// Face.cpp : subor pre pracu s facetmi
#include "stdafx.h"
#include "Face.h"

namespace Model
{
	// default konstruktor
	Face::Face()
	{
		v[0] = NULL;
		v[1] = NULL;
		v[2] = NULL;
		normal = NULL;
		farba = 0;
	}

	// konstruktor
	Face::Face(Vertex* v1, Vertex* v2, Vertex* v3)
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		farba = 0;
		ComputeNormal();
	}

	//destruktor
	Face::~Face()
	{
		for(int i = 0; i<3; i++)
		{
			v[i] = NULL;
		}
		if(normal != NULL)
			delete normal;

		// malo by byt OK kedze odkladam pointer, nemalo by zmazat samotne data
		for(unsigned int i = 0; i < susedia.size(); i++)
			susedia[i] = NULL;

		susedia.clear();
	}

	void Face::ComputeNormal()
	{
		center = new Vector4((v[0]->P->X + v[1]->P->X + v[2]->P->X) / 3.0,
							 (v[0]->P->Y + v[1]->P->Y + v[2]->P->Y) / 3.0,
							 (v[0]->P->Z + v[1]->P->Z + v[2]->P->Z) / 3.0);

		Vector4* U = new Vector4(v[1]->P->X - v[0]->P->X, v[1]->P->Y - v[0]->P->Y, v[1]->P->Z - v[0]->P->Z);
		Vector4* V = new Vector4(v[2]->P->X - v[0]->P->X, v[2]->P->Y - v[0]->P->Y, v[2]->P->Z - v[0]->P->Z);

		(*U) %= (*V);
		normal = new Vector4(U->X, U->Y, U->Z);

		delete U;
		delete V;
	}

	void Face::AddSused(Face* sused)
	{
		for(unsigned int i = 0; i < susedia.size(); i++)
		{
			if(susedia[i] == sused)
				return;
		}
		susedia.push_back(sused);
	}

	void Face::SetColor(unsigned int color)
	{
		farba = color;
	}
}