// Face.h : subor pre pracu s facetmi
#pragma once
#include "Vertex.h"

namespace Model
{
	using namespace std;

	public class Face
	{
	public:
		Face();
		Face(Vertex* v1, Vertex* v2, Vertex* v3);
		~Face();

		void ComputeNormal();
		void AddSused(Face* sused);
		void SetColor(unsigned int color);

		Vertex*					v[3];
		Vector4*				normal;
		Vector4*				center;
		vector<Face*>			susedia;					// susedia
		unsigned int			farba;						// pre picking
		//SDF					value;
	};
}