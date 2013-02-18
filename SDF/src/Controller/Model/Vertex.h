// Vertex.h : subor pre manipulaciu s 3D bodom
#pragma once
#include "Vector4.h"

namespace Model
{
	using namespace Structures;

	public class Vertex
	{
	public:
		Vertex(double x, double y, double z);
		~Vertex();

		Vector4 P;
	};
}