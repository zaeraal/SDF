// Face.cpp : subor pre pracu s facetmi
#include "stdafx.h"
#include "Vertex.h"
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
	Face::Face(Vertex* v1, Vertex* v2, Vertex* v3, Vertex* normala, vector<Face*> sused)
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		normal = normala;
		susedia = sused;
		farba = 0;
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
		susedia.clear();
	}
}