// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"
#include "LinkedList.h"
#include "CSDF.h"

namespace MeshStructures
{
	using namespace MathStructures;
	using namespace GenericStructures;
	using namespace SDFStructures;

	public class Vertex
	{
	public:
		Vertex(float x, float y, float z);
		Vertex(Vector4 position);
		~Vertex();
		bool HasNormal();
		void SetNormal(Vector4 normala);
		Vector4 GetNormal();
		

		Vector4 P;
		LinkedList<void>*		susedia;					// susedia
		unsigned int			number;						// cislo v zozname
		CSDF*					quality;
	private:
		bool					has_normal;
		Vector4					normal;
	};
}