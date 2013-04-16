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
		ComputeNormal();
		diameter = new CSDF();
		//assimp_ref = NULL;
	}

	//destruktor
	Face::~Face()
	{
		// osetrene v LinkdListe ze sa zmaze cely
		delete diameter;
	}

	void Face::ComputeNormal()
	{
		center = Vector4((v[0]->P.X + v[1]->P.X + v[2]->P.X) / 3.0f,
							 (v[0]->P.Y + v[1]->P.Y + v[2]->P.Y) / 3.0f,
							 (v[0]->P.Z + v[1]->P.Z + v[2]->P.Z) / 3.0f,
							 1.0f);

		// cross product
		Vector4 U = v[1]->P - v[0]->P;
		Vector4 V = v[2]->P - v[0]->P;
		normal = U % V;
		normal.Normalize();
	}

	void Face::SetColor(int color)
	{
		farba = color;
	}

	void Face::ComputeSDFValue(const std::vector<float> values, const std::vector<float> inverse_Yangles)
	{
		diameter->ComputeValue(values, inverse_Yangles);
	}

	LinkedList<Face>* Face::GetSusedia()
	{
		return susedia;
	}
	void Face::ComputeSusedov()
	{
		susedia = new LinkedList<Face>();
		for(int i = 0; i < 3; i++)
		{
			LinkedList<void>::Cell<void>* tmp = v[i]->susedia->start;
			while(tmp != NULL)
			{
				if(!susedia->Contains((Face*)tmp->data))
					susedia->InsertToEnd((Face*)tmp->data);
				tmp = tmp->next;
			}
		}
		//return result;
	}
}