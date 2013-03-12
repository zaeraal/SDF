// Octree.h : subor obsahujuci zakladnu octree strukturu
#pragma once
#include "Face.h"

namespace MeshStructures
{
	using namespace std;
	using namespace GenericStructures;

	const int max_depth = 4;
	const int min_count = 2;

	public class Octree
	{
	public:
		Octree(const int dep, const double siz, Vector4 ori, Octree* par = NULL);
		~Octree();

		void Build(Face** tria, unsigned int length);
		int GetCode(const Vector4 pt);
		bool isLeaf;
		void GetBoundary(double &siz, double &x, double &y, double &z);
		void DrawOctree(bool recursive);

		Octree* parent;						// otec, NULL ak sme hlavny vrchol
		Octree** son;						// synovia
		Face** triangles;					// nase trojuholnicky
		unsigned int count;					// pocet trojuholnikov, hranica pre robenie synov je min_count
	private:
		int depth;							// ako hlboko sme v octree, max je max_depth
		double size;						// hranica kocky
		Vector4 origin;						// stred kocky
	};
}