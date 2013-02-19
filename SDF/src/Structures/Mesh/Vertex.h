// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"

namespace MeshStructures
{
	using namespace MathStructures;

	public class Vertex
	{
	public:
		Vertex(double x, double y, double z);
		Vertex(Vector4 position);
		~Vertex();

		Vector4 P;
	};
}