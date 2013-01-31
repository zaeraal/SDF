// Octree.h : subor obsahujuci zakladnu octree strukturu
#pragma once
#include "Face.h"

namespace Model
{
	using namespace std;
	using namespace Mathematics;

	const int max_depth = 6;
	const int min_count = 2;

	public class Octree
	{
	public:
		Octree(int dep, double siz, Vector4* ori, Octree* par = NULL);
		~Octree();

		void Build(vector<Face*> tria);
		int GetCode(Vector4* pt);
		bool isLeaf();
		void GetBoundary(double &siz, double &x, double &y, double &z);
	private:
		int depth;							// ako hlboko sme v octree, max je max_depth
		double size;						// hranica kocky
		Vector4* origin;					// stred kocky
		Octree* son[8];						// synovia
		Octree* parent;						// otec, NULL ak sme hlavny vrchol
		vector<Face*> triangles;			// nase trojuholnicky
	};
}