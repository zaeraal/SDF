// SDFController.h : subor pre vypocitanie SDF funkcie
#pragma once
#include "Assimp.h"
#include "Octree.h"


namespace SDFController
{
	using namespace MeshStructures;
	using namespace AssimpFileHandler;

	public class CSDFController
	{
	public:
		CSDFController(double dia);
		~CSDFController();

		void ComputeForAllFaces(LinkedList<Face>* triangles, CAssimp* loggger);
		void ComputeForOctree(Octree* root);

	private:
		double diagonal;
	};
}