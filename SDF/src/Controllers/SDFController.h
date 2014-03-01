// SDFController.h : subor pre vypocitanie SDF funkcie
#pragma once
#include "Assimp.h"
#include "Octree.h"
#include "ROctree.h"
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

		void Compute(LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max);
		void ComputeOpenCL(LinkedList<Vertex>* points, LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max);
		void ComputeOpenCL2(LinkedList<Vertex>* points, LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max, unsigned int nodeCount, unsigned int leafCount, unsigned int triangleCount);
		float* ComputeGaussianKernel(int radius);
		float ComputeGaussian(int radius, float value, float maxvalue);
		void Smooth(Face* tmp, float* kernel, int kernel_size);
		void Smooth2(PPoint* pointik, ROctree* m_root, LinkedList<ROctree>* ro_list, unsigned int poradie);
		HashTable<Face>* GetFaceList(LinkedList<Face>* triangles, Octree* root, Vector4 center, Vector4 ray, Vector4 o_min, Vector4 o_max);
		void ComputeTNB(Face* tmp, Vector4& tang, Vector4& binor, Vector4& norm);
		int first_node(float tx0, float ty0, float tz0, float txm, float tym, float tzm);
		int new_node(float txm, int x, float tym, int y, float tzm, int z);
		int first_node2(Vector4 t0, Vector4 tm);
		void proc_subtree (unsigned char idx, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees);
		void proc_subtree2 (unsigned char idx, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees);
		void proc_subtree3 (Vector4 o, Vector4 or, Vector4 d, Octree* node, LinkedList<Octree>* octrees);
		void proc_subtree4 (unsigned char idx, Vector4 t0, Vector4 t1, Octree* node, LinkedList<Octree>* octrees);
		void ray_octree_traversal(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees, Vector4 o_min, Vector4 o_max);
		void ray_octree_traversal2(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees);
		bool CheckError(int err, char extra_debug[32768] = NULL);
		void UniformPointsOnSphere(float* rndx, float * rndy);
		void RandomPointsOnSphere(float* rndx, float * rndy);
		void InitKernel();
		void EraseKernel();
		bool CheckValid(int mask, int num);
		void DoSmoothing(LinkedList<Face> *triangles, float min, float max);
		void DoSmoothing2(LinkedList<Face> *triangles, float min, float max);
		void RadiusSearch1(Vector4 center, float dist, ROctree* node, LinkedList<ROctree>* octrees);
		void RadiusSearch2(Vector4 center, float dist, ROctree* node, LinkedList<ROctree>* octrees);
		Vector4 ComputePointBoundary(LinkedList<PPoint>* point_list, float &b_size);
		Vector4 ComputePointBoundary2(PPoint **point_list, unsigned int psize, float &b_size);
		ROctree* CreateROctree(LinkedList<PPoint>* point_list, float b_size, Vector4 b_stred);
		ROctree* CreateROctree2(PPoint** pointiky, unsigned int siz, float b_size, Vector4 b_stred, unsigned int &n_pnodes);
		void RandomShuffle(PPoint **c_array, unsigned int size);
	private:
		float diagonal;
		CAssimp* loggger;
		HashTable<Face>* fc_list;			// hashovacia tabulka facov
		//LinkedList<Face>* fc_list;			// prealokovany list facov
		LinkedList<Octree>* oc_list;		// prealokovany list octree
		int kernel_size;					// velkost gaussianu pri vyhladeni
		LinkedList<Face>** gauss_sus;
		unsigned int prealocated_space;

		class CastStackx
		{
		public:
			CastStackx   (void)
			{
				for(int i = 0; i < 10; i++)
				{
					t0stack[i] = Vector4();
					t1stack[i] = Vector4();
					cstack[i] = 0;
					ostack[i] = NULL;
				}
			}

			Octree* read (int idx, Vector4& t0, Vector4& t1, int& cnode) const
			{
				t0 = t0stack[idx];
				t1 = t1stack[idx];
				cnode = cstack[idx];

				return ostack[idx];
			}
			void write (int idx, Vector4 t0, Vector4 t1, int cnode, Octree* node)
			{
				t0stack[idx] = t0;
				t1stack[idx] = t1;
				cstack[idx] = cnode;
				ostack[idx] = node;
			}

		private:
			Vector4         t0stack[10];
			Vector4         t1stack[10];
			int				cstack[10];
			Octree*         ostack[10];
		};
	};
}