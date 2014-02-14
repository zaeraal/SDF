// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"
#include "Face.h"

namespace MeshStructures
{
	using namespace MathStructures;

	public class PPoint
	{
	public:
		PPoint(float x, float y, float z, Face* refference);
		PPoint(Vector4 position, Face* refference);

		Vector4 P;
		unsigned int			number;						// cislo v zozname
		float					diameter;
		Face*					ref;
	};
}