// Octree.h : subor obsahujuci zakladnu octree strukturu
#pragma once
#include "Face.h"

namespace MeshStructures
{
	using namespace std;
	using namespace GenericStructures;

	const int max_depth = 8;
	const int min_count = 2;

	public class Octree
	{
	public:
		Octree(const int dep, const float siz, Vector4 ori, Octree* par = NULL);
		~Octree();

		void Build(Face** tria, unsigned int length);
		void Build2(Face** tria, unsigned int* mtria, unsigned int start, unsigned int length);
		void InserToEnd(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2]);
		void InserToStart(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2]);
		void FwdMove(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2], bool added);
		void Check(Face** tria, unsigned int* mtria, byte code, int (&tabulka)[8][2], float new_size, unsigned int cislo);
		byte GetCode(const Vector4 pt);
		bool isLeaf;
		void GetBoundary(float &siz, float &x, float &y, float &z);
		void DrawOctree(bool recursive);

		Octree* parent;						// otec, NULL ak sme hlavny vrchol
		Octree** son;						// synovia
		Face** triangles;					// nase trojuholnicky
		unsigned int count;					// pocet trojuholnikov, hranica pre robenie synov je min_count
	private:
		int depth;							// ako hlboko sme v octree, max je max_depth
		float size;							// hranica kocky
		Vector4 origin;						// stred kocky
		float Table[8][3];					// tabulka offsetov
	};
}