// Face.h : subor pre pracu s facetmi
#pragma once
#include "Vertex.h"
#include "CSDF.h"

namespace MeshStructures
{
	using namespace std;
	using namespace GenericStructures;
	using namespace SDFStructures;

	public class Face
	{
	public:
		Face(Vertex* v1, Vertex* v2, Vertex* v3);
		~Face();

		void ComputeNormal();
		void SetColor(int color);
		void ComputeSDFValue(const std::vector<double> values, const std::vector<double> inverse_Yangles);
		LinkedList<Face>* GetSusedia();

		Vertex*					v[3];
		Vector4					normal;
		Vector4					center;
		int						farba;						// pre picking
		CSDF*					diameter;
	};
}