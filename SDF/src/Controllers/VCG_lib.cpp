// VCG_lib.cpp : subor pre nacitanie / ulozenie modelov

#include "stdafx.h"
#include "VCG_lib.h"
#define FLOAT_MAX  99999.0

namespace VCGFileHandler
{
	void CVCG::Import3DFromFile(const string filename)
	{
		char *fname = new char [filename.size()+1];
		strcpy_s (fname, filename.size()+1, filename.c_str());
		vcg::tri::io::Importer<MyMesh>::Open(m, fname);
		//vcg::tri::UpdateNormal<MyMesh>::PerVertexPerFace(m);
	}
	int CVCG::GetPosition(MyMesh::VertexPointer vertex_)
	{
		int i = 0;
		for(MyMesh::VertexIterator it= m.vert.begin(); it != m.vert.end(); it++)
		{
			if(&(*it) == vertex_)
				return i;
			i++;
		}
		return 0;
	}
	void CVCG::LoadData(LinkedList<Face>* fc, LinkedList<Vertex>* pts)
	{
		float q_min = FLOAT_MAX;
		float q_max = 0;
		bool has_quality = false;

		Vertex** tmp_points = new Vertex* [m.vert.size()]; unsigned int i = 0;
		for(MyMesh::VertexIterator it= m.vert.begin(); it != m.vert.end(); it++)
		{
			tmp_points[i] = new Vertex(it->P().X(), it->P().Y(), it->P().Z());
			
			if(it->HasQuality())
			{
				tmp_points[i]->quality->value = it->Q();
				tmp_points[i]->quality->smoothed = it->Q();
				if(it->Q() < q_min)
					q_min = it->Q();
				if(it->Q() > q_max)
					q_max = it->Q();
				has_quality = true;
			}
			it->Q() = MyMesh::VertexType::QualityType (i);
			pts->InsertToEnd(tmp_points[i]);
			i++;
		}
		if(has_quality == true)
		{
			for(i = 0; i < m.vert.size(); i++)
			{
				tmp_points[i]->quality->Normalize(q_min, q_max, 4.0);
			}
			Nastavenia->DEBUG_Min_SDF = q_min;
			Nastavenia->DEBUG_Max_SDF = q_max;
		}

		i = 0;
		for(MyMesh::FaceIterator it = m.face.begin(); it != m.face.end(); it++)
		{
			MyMesh::VertexPointer v0 = it->cV(0);
			MyMesh::VertexPointer v1 = it->cV(1);
			MyMesh::VertexPointer v2 = it->cV(2);
			int vv0 = (int)v0->Q();
			int vv1 = (int)v1->Q();
			int vv2 = (int)v2->Q();

			Face* face = new Face(tmp_points[vv0],tmp_points[vv1], tmp_points[vv2]);
			tmp_points[vv0]->susedia->InsertToEnd(face);
			tmp_points[vv1]->susedia->InsertToEnd(face);
			tmp_points[vv2]->susedia->InsertToEnd(face);

			if(it->HasQuality())
			{
				face->quality->value = it->Q();
				face->quality->smoothed = it->Q();
			}
			/*if(it->HasNormal())
				face->normal = Vector4(it->N().X(), it->N().Y(), it->N().Z(), 0.0f);*/

			fc->InsertToEnd(face);
		}
		delete [] tmp_points;
	}
}