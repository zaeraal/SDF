// SDFController.h : subor pre vypocitanie SDF funkcie
#pragma once
#include "Octree.h"

namespace SDFController
{
	using namespace MeshStructures;

	public class CSDFController
	{
	public:
		CSDFController();
		~CSDFController();

		void ComputeForAllFaces(LinkedList<Face>* triangles);
		void ComputeForOctree(Octree* root);

		double min;
		double max;
		double nmin;
		double nmax;
	};
}