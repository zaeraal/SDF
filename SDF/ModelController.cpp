// ModelController.cpp : subor pre kontrolu modelov
#include "stdafx.h"
#include "ModelController.h"

namespace Controller
{
	ModelController::ModelController()
	{
		Assimp = new CAssimp();
		m_root = NULL;
		loaded = false;
	}

	ModelController::~ModelController()
	{
		delete Assimp;

		// bacha na deletovanie v spravnom poradi
		if(m_root != NULL)
			delete m_root;

		for(unsigned int i = 0; i < triangles.size(); i++)
		{
			delete triangles[i];
			triangles[i] = NULL;
		}

		for(unsigned int i = 0; i < points.size(); i++)
		{
			delete points[i];
			points[i] = NULL;
		}

		triangles.clear();
		points.clear();
	}

	// zapis info log do suboru
	void ModelController::logInfo(std::string logString)
	{
		Assimp->logInfo(logString);
	}

	// zapis debug log do suboru
	void ModelController::logDebug(std::string logString)
	{
		Assimp->logDebug(logString);
	}

	// nacita subor
	void ModelController::LoadFile(std::string Filename)
	{
		Assimp->Import3DFromFile(Filename);
		Assimp->LoadData(triangles, points);
		loaded = true;

		CreateOctree();
		SetColors();
	}

	// vytvori Octree strukturu
	void ModelController::CreateOctree()
	{
		double minx = 9999.0, miny = 9999.0, minz = 9999.0;
		double maxx = -9999.0, maxy = -9999.0, maxz = -9999.0;

		for(unsigned int i = 0; i < points.size(); i++)
		{
			if(points[i]->P->X < minx)
				minx = points[i]->P->X;
			if(points[i]->P->Y < miny)
				miny = points[i]->P->Y;
			if(points[i]->P->Z < minz)
				minz = points[i]->P->Z;

			if(points[i]->P->X > maxx)
				maxx = points[i]->P->X;
			if(points[i]->P->Y > maxy)
				maxy = points[i]->P->Y;
			if(points[i]->P->Z > maxz)
				maxz = points[i]->P->Z;
		}

		Vector4* center = new Vector4((minx+maxx) / 2.0, (miny+maxy) / 2.0, (minz+maxz) / 2.0);
		double sizex = 0;
		double sizey = 0;
		double sizez = 0;

		if(((minx<=0.0)&&(maxx<=0.0)) || ((minx>=0.0)&&(maxx>=0.0)))
			sizex = abs(minx+maxx) / 2.0;
		else
			sizex = abs(minx-maxx) / 2.0;

		if(((minx<=0.0)&&(maxx<=0.0)) || ((minx>=0.0)&&(maxx>=0.0)))
			sizey = abs(minx+maxx) / 2.0;
		else
			sizey = abs(minx-maxx) / 2.0;

		if(((minx<=0.0)&&(maxx<=0.0)) || ((minx>=0.0)&&(maxx>=0.0)))
			sizez = abs(minx+maxx) / 2.0;
		else
			sizez = abs(minx-maxx) / 2.0;
		double size = max(max(sizex, sizey), sizez);

		m_root = new Octree(0, size, center, NULL);
		m_root->Build(triangles);
	}

	// nastavi farby pre picking
	void ModelController::SetColors()
	{
		unsigned int col = 0;
		const unsigned int step = 8;
		for(unsigned int i = 0; i < triangles.size(); i++)
		{
			triangles[i]->SetColor(col);
			col += step;
		}
	}

	// pre spravne vycentrovanie pohladu ked sa nacita model
	void ModelController::GetBoundary(double &siz, double &x, double &y, double &z)
	{
		m_root->GetBoundary(siz, x, y, z);
	}

	// vykresli model
	void ModelController::DrawModel()
	{
		glColor3f(1.0f,1.0f,1.0f);							// biela farba
		glBegin(GL_TRIANGLES);
			for (std::vector<Face*>::iterator it = triangles.begin() ; it != triangles.end(); ++it)
			{
				//logInfo(MarshalString("farba: "+(*it)->farba));
				GLdouble r = 0.0, g = 0.0, b = 0.0;
				r = (*it)->farba % 256;
				g = int((*it)->farba / 256)*8;
				if(g >= 256.0)
				{
					b = int((g / 256.0) * 8);
					g = int(g) % 256;
				}
				glColor3d(r / 256.0, g / 256.0, b / 256.0);
				//logInfo(MarshalString("red: "+r+", green: "+g+", blue: "+b));
				for(unsigned int i = 0; i < 3; i++)
				{
					glVertex3d((*it)->v[i]->P->X, (*it)->v[i]->P->Y, (*it)->v[i]->P->Z);
				}
			}
		glEnd();
	}

}