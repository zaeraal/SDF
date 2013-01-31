// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"

namespace Model
{
	using namespace Mathematics;

	public class Vertex
	{
	public:
		Vertex();
		Vertex(double x, double y, double z);
		~Vertex();

		Vector4 *P;
	};
}