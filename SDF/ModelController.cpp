// ModelController.cpp : subor pre kontrolu modelov
#include "stdafx.h"
#include "ModelController.h"

namespace Controller
{
	ModelController::ModelController()
	{
		Assimp = new CAssimp();
		m_root = NULL;
		triangles = new LinkedList<Face>(NULL);
		points = new LinkedList<Vertex>(NULL);
		loaded = false;
		draw_mode = 0;
		show_octree = false;
		selected = NULL;
	}

	ModelController::~ModelController()
	{
		delete Assimp;

		/*
		/// -------------------------------------
		/// STRASNE DLHO TRVA ZATVORENIE PROGRAMU
		/// DOCASNE ZAKOMENTOVANE
		/// -------------------------------------

		// bacha na deletovanie v spravnom poradi
		if(m_root != NULL)
			delete m_root;

		// delete actual faces and vertices
		LinkedList<Face>* tmp1 = triangles;
		while(tmp1 != NULL)
		{
			if(tmp1->data != NULL)
				delete tmp1->data;
			tmp1 = tmp1->next;
		}
		DeleteList(triangles);

		LinkedList<Vertex>* tmp2 = points;
		while(tmp2 != NULL)
		{
			if(tmp2->data != NULL)
				delete tmp2->data;
			tmp2 = tmp2->next;
		}
		DeleteList(points);*/
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
		loaded = false;
		draw_mode = 0;
		show_octree = false;
		selected = NULL;
		// ak znovu nacitavame, premaz povodne udaje
		if(m_root != NULL)
			delete m_root;

		// delete actual faces and vertices
		LinkedList<Face>* tmp1 = triangles;
		while(tmp1 != NULL)
		{
			if(tmp1->data != NULL)
				delete tmp1->data;
			tmp1 = tmp1->next;
		}
		DeleteList(triangles);

		LinkedList<Vertex>* tmp2 = points;
		while(tmp2 != NULL)
		{
			if(tmp2->data != NULL)
				delete tmp2->data;
			tmp2 = tmp2->next;
		}
		DeleteList(points);

		m_root = NULL;
		triangles = new LinkedList<Face>(NULL);
		points = new LinkedList<Vertex>(NULL);

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

		LinkedList<Vertex>* tmp = points->next;
		while(tmp != NULL)
		{
			if(tmp->data->P->X < minx)
				minx = tmp->data->P->X;
			if(tmp->data->P->Y < miny)
				miny = tmp->data->P->Y;
			if(tmp->data->P->Z < minz)
				minz = tmp->data->P->Z;

			if(tmp->data->P->X > maxx)
				maxx = tmp->data->P->X;
			if(tmp->data->P->Y > maxy)
				maxy = tmp->data->P->Y;
			if(tmp->data->P->Z > maxz)
				maxz = tmp->data->P->Z;

			tmp = tmp->next;
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

		unsigned int siz = triangles->GetSize();
		if(siz > 1)
		{
			Face** tria = new Face* [siz];
			LinkedList<Face>* tmp2 = triangles->next;
			int i = 0;
			while(tmp2 != NULL)
			{
				tria[i] = tmp2->data;
				tmp2 = tmp2->next;
				i++;
			}
			m_root->Build(tria, siz-1);
		}
		else
			m_root->Build(NULL, 0);
	}

	// nastavi farby pre picking
	void ModelController::SetColors()
	{
		int col;
		int r = 0, g = 0, b = 0;
		LinkedList<Face>* tmp = triangles;
		while(tmp != NULL)
		{
			r = r + color_step;
			if(r >= 256) { r = r % 256; g = g + color_step;}
			if(g >= 256) { g = g % 256; b = b + color_step;}
			if(b >= 256) { b = b % 256; logInfo("MAXIMUM FARIEB PRE PICKING DOSIAHNUTE!!");}

			col = r + 256 * g + 256 * 256 * b;
			if(tmp->data != NULL)
				tmp->data->SetColor(col);
			logInfo(MarshalString("farba: "+col+", RGB: " + r+" "+g+" "+b));

			tmp = tmp->next;
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
		if((draw_mode == 1) || (draw_mode == 3))
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);

		if(draw_mode == 4)
			glDisable(GL_DITHER);
		else
			glEnable(GL_DITHER);

		glColor3f(1.0f,1.0f,1.0f);							// biela farba
		{
			LinkedList<Face>* tmp = triangles;
			while(tmp != NULL)
			{
				if(tmp->data == NULL)
				{
					tmp = tmp->next;
					continue;
				}

				// nech vykresli selectnuty trojuholnik
				if(draw_mode == 1)
				{
					if(tmp->data == selected)
						glColor3f(1.0f,0.5f,0.0f);		// orange
					else
						glColor3f(1.0f,1.0f,1.0f);		// white
				}
				// nech zorbazi picking hodnoty
				if((draw_mode == 0) || (draw_mode == 4))
				{
					GLubyte r = 0, g = 0, b = 0;
					ColorToRGB(tmp->data->farba, r,g,b);
					glColor3ub(r, g, b);
					//logInfo(MarshalString("red: "+r+", green: "+g+", blue: "+b));
				}

				glBegin(draw_mode == 3 ? GL_LINE_LOOP : GL_TRIANGLES);
					glNormal3d(tmp->data->normal->X, tmp->data->normal->Y, tmp->data->normal->Z);
					for(unsigned int i = 0; i < 3; i++)
					{
						glVertex3d(tmp->data->v[i]->P->X, tmp->data->v[i]->P->Y, tmp->data->v[i]->P->Z);
					}
				glEnd();

				tmp = tmp->next;
			}
		}
		if(draw_mode != 4)
		{
			glDisable(GL_LIGHTING);
			glColor3f(0.5f,0.5f,0.5f);							// seda farba
			if(show_octree == true)
				m_root->DrawOctree();
		}
	}

	void ModelController::ColorToRGB(int color, GLubyte &R, GLubyte &G, GLubyte &B)
	{
		R = color % 256;
		int g = int((color - R) / 256);
		if(g >= 256)
		{
			G = g % 256;
			B = int((g - G) / 256);
		}
		else G = g;
	}

	void ModelController::setDrawMode(int mode)
	{
		draw_mode = mode;
	}
	int ModelController::getDrawMode()
	{
		return draw_mode;
	}

	int ModelController::GetTriangleCount()
	{
		return triangles->GetSize() - 1;
	}

	void ModelController::ProcessPick(int x, int y)
	{
		GLint viewport[4];
		GLubyte pixel[3];

		glGetIntegerv(GL_VIEWPORT,viewport);

		glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

		logInfo(MarshalString("Selected Pixel at x: " + x + ", y: "+y+" is: " + pixel[0]+" "+pixel[1]+" "+pixel[2]));
		
		selected = NULL;
		int color = pixel[0] + 256 * int(pixel[1]) + 256 * 256 * int(pixel[2]);
		logInfo(MarshalString("farba: "+color));
		if(color > 0)
		{
			LinkedList<Face>* tmp = triangles->next;
			while(tmp != NULL)
			{
				if(color == tmp->data->farba)
				{
					selected = tmp->data;
					break;
				}
				tmp = tmp->next;
			}
		}
	}
}