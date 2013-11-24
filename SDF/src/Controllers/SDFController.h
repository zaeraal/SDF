// SDFController.h : subor pre vypocitanie SDF funkcie
#pragma once
#include "Assimp.h"
#include "Octree.h"
#include "HashTable.h"

namespace SDFController
{
	using namespace MeshStructures;
	using namespace AssimpFileHandler;

	public class CSDFController
	{
	public:
		CSDFController(float dia, CAssimp* logg);
		~CSDFController();

		void Compute(LinkedList<Face>* triangles, Octree* root);
		void ComputeOpenCL(LinkedList<Vertex>* points, LinkedList<Face>* triangles, Octree* root);
		float* ComputeGaussianKernel(int radius);
		void Smooth(Face* tmp, float* kernel, int kernel_size);
		HashTable<Face>* GetFaceList(LinkedList<Face>* triangles, Octree* root, Vector4 center, Vector4 ray);
		void ComputeTNB(Face* tmp, Vector4& tang, Vector4& binor, Vector4& norm);
		int first_node(float tx0, float ty0, float tz0, float txm, float tym, float tzm);
		int new_node(float txm, int x, float tym, int y, float tzm, int z);
		void proc_subtree (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees);
		void proc_subtree2 (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees);
		void ray_octree_traversal(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees);
		bool CheckError(int err, std::string extra_debug = "");
		void UniformPointsOnSphere(float* rndx, float * rndy);
		void RandomPointsOnSphere(float* rndx, float * rndy);
		void InitKernel();
		void EraseKernel();
	private:
		float diagonal;
		unsigned char a;					// for octree traveersal
		CAssimp* loggger;
		HashTable<Face>* fc_list;			// hashovacia tabulka facov
		//LinkedList<Face>* fc_list;			// prealokovany list facov
		LinkedList<Octree>* oc_list;		// prealokovany list octree
		int kernel_size;					// velkost gaussianu pri vyhladeni
		LinkedList<Face>** gauss_sus;
		unsigned int prealocated_space;
	};
}