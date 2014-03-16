// Octree.h : subor obsahujuci zakladnu octree strukturu
#pragma once
#include "PPoint.h"

namespace MeshStructures
{
	using namespace std;
	using namespace GenericStructures;

	public class ROctree
	{
	public:
		ROctree(const int dep, const float siz, Vector4 ori);
		~ROctree();

		void Build(PPoint** pointiky, unsigned int length);
		void Build2(PPoint** pointiky, unsigned int start, unsigned int length, unsigned int &NodeCount, unsigned int &PointCount, unsigned int &LeafCount);
		void InserToEnd(PPoint** pointiky, unsigned int idx, byte code, int (&tabulka)[8][2]);
		void FwdMove(PPoint** pointiky, unsigned int idx, byte code, int (&tabulka)[8][2], bool added);
		byte GetCode(const Vector4 pt);
		bool isLeaf() { return (sons != 0 ? false : true);};
		void GetBoundary(float &siz, float &x, float &y, float &z);
		void HLSToRGB(float SDF_value, GLubyte &R, GLubyte &G, GLubyte &B);
		void DrawOctree(bool recursive);
		void DrawAxes();
		bool CheckValid(int num);
		void DoValueSmoothing();

		ROctree** son;						// synovia
		float value;
		Vector4 value_center;
		unsigned int count;					// pocet trojuholnikov, hranica pre robenie synov je Nastavenia->OCTREE_Threshold
		unsigned int depth;					// ako hlboko sme v octree, max je Nastavenia->OCTREE_Depth
		float size;							// hranica kocky
		Vector4 origin;						// stred kocky
		unsigned char sons;					// tabulka platnosti synov
	};
}