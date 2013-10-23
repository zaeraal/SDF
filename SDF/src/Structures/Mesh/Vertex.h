// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"
#include "LinkedList.h"

namespace MeshStructures
{
	using namespace MathStructures;
	using namespace GenericStructures;

	public class Vertex
	{
	public:
		Vertex(float x, float y, float z);
		Vertex(Vector4 position);
		~Vertex();

		Vector4 P;
		LinkedList<void>*		susedia;					// susedia
		unsigned int			number;						// cislo v zozname
	};
}