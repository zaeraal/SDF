// VCG_lib.h : subor pre nacitanie / ulozenie modelov
#pragma once
#include "StringHelper.h"
#include "Face.h"
#include <wrap/io_trimesh/import.h>

namespace VCGFileHandler
{
	using namespace std;
	using namespace GenericStructures;
	using namespace MeshStructures;

	public class CVCG
	{
	public:
		void Import3DFromFile( const string filename);
		void LoadData(LinkedList<Face>* fc, LinkedList<Vertex>* pts);
	private:
		class MyVertex; class MyEdge; class MyFace;
		struct MyUsedTypes :
			public vcg::UsedTypes<vcg::Use<MyVertex> ::AsVertexType,
			vcg::Use<MyEdge> ::AsEdgeType,
			vcg::Use<MyFace> ::AsFaceType>{};
		class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Qualityf, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
		class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::Qualityf, vcg::face::BitFlags > {};
		class MyEdge : public vcg::Edge< MyUsedTypes> {};
		class MyMesh : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace> , std::vector<MyEdge> > {};

		MyMesh m;
		int GetPosition(MyMesh::VertexPointer vertex_);
	};
}