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
		CSDFController(double dia, CAssimp* logg);
		~CSDFController();

		void Compute(LinkedList<Face>* triangles, Octree* root);
		double* ComputeGaussianKernel(int radius);
		void Smooth(Face* tmp, double* kernel, int kernel_size);
		LinkedList<Face>* GetFaceList(LinkedList<Face>* triangles, Octree* root, Vector4 center, Vector4 ray);
		void ComputeTNB(Face* tmp, Vector4& tang, Vector4& norm, Vector4& binor);
		int first_node(double tx0, double ty0, double tz0, double txm, double tym, double tzm);
		int new_node(double txm, int x, double tym, int y, double tzm, int z);
		void proc_subtree (double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, Octree* node, LinkedList<Octree>* octrees);
		void ray_octree_traversal(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees);
	private:
		double diagonal;
		unsigned char a;					// for octree traveersal
		CAssimp* loggger;
	};
}