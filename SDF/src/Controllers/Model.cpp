// Model.cpp : subor pre kontrolu modelov
#include "stdafx.h"
#include "Model.h"
#include "PointCloudTriangulation.h"

namespace ModelController
{
	CModel::CModel()
	{
		Assimp = new CAssimp();
		SDF_control = NULL;
		m_root = NULL;
//		transform_matica = 
		triangles = new LinkedList<Face>();
		points = new LinkedList<Vertex>();
		ResetSettings();
	}

	CModel::~CModel()
	{
		delete Assimp;
		delete SDF_control;

		/*
		/// -------------------------------------
		/// STRASNE DLHO TRVA ZATVORENIE PROGRAMU
		/// DOCASNE ZAKOMENTOVANE
		/// -------------------------------------

		// bacha na deletovanie v spravnom poradi
		if(m_root != NULL)
			delete m_root;

		// delete actual faces and vertices
		triangles->CompleteDelete();
		delete triangles;

		points->CompleteDelete();
		delete points;*/
	}

	// zapis info log do suboru
	void CModel::logInfo(std::string logString)
	{
		Assimp->logInfo(logString);
	}

	// zapis debug log do suboru
	void CModel::logDebug(std::string logString)
	{
		Assimp->logDebug(logString);
	}

	// nacita subor
	void CModel::LoadFile(std::string Filename)
	{
		int ticks1 = GetTickCount();
		ResetSettings();

		// ak znovu nacitavame, premaz povodne udaje
		if(m_root != NULL)
			delete m_root;

		if(SDF_control != NULL)
			delete SDF_control;

		// delete actual faces and vertices
		triangles->CompleteDelete();
		delete triangles;

		points->CompleteDelete();
		delete points;

		m_root = NULL;
		SDF_control = NULL;
		triangles = new LinkedList<Face>();
		points = new LinkedList<Vertex>();
		int ticks2 = GetTickCount();

		Assimp->Import3DFromFile(Filename);

		int ticks3 = GetTickCount();
		Assimp->LoadData(triangles, points);
		loaded = true;

		int ticks4 = GetTickCount();
		AssignNumber();
		ComputeBoundary();
		CreateOctree();

		int ticks5 = GetTickCount();

		SetColors();
		ComputeSusedov();

		int ticks6 = GetTickCount();

		logInfo(MarshalString("Vycistenie starych zaznamov: " + (ticks2 - ticks1)+ "ms"));
		logInfo(MarshalString("Nacitanie modelu do Assimpu: " + (ticks3 - ticks2)+ "ms"));
		logInfo(MarshalString("Nacitanie Assimpu do mojich objektov: " + (ticks4 - ticks3)+ "ms"));
		logInfo(MarshalString("Vytvorenie Octree: " + (ticks5 - ticks4)+ "ms"));
		logInfo(MarshalString("Vytvorenie susedov: " + (ticks6 - ticks5)+ "ms"));
		logInfo(MarshalString("Celkovy cas nacitania: " + (ticks6 - ticks1)+ "ms"));

		Nastavenia->INFO_Total_Triangles = triangles->GetSize();
		Nastavenia->INFO_Total_Vertices = points->GetSize();
	}

	// nacita priamo Assimp
	void CModel::LoadAssimp(aiScene* scene)
	{
		ResetSettings();

		// ak znovu nacitavame, premaz povodne udaje
		if(m_root != NULL)
			delete m_root;

		if(SDF_control != NULL)
			delete SDF_control;

		// delete actual faces and vertices
		triangles->CompleteDelete();
		delete triangles;

		points->CompleteDelete();
		delete points;

		m_root = NULL;
		SDF_control = NULL;
		triangles = new LinkedList<Face>();
		points = new LinkedList<Vertex>();

		Assimp->SetScene(scene);
		Assimp->LoadData(triangles, points);
		loaded = true;

		AssignNumber();
		ComputeBoundary();
		CreateOctree();
		SetColors();
		ComputeSusedov();

		Nastavenia->INFO_Total_Triangles = triangles->GetSize();
		Nastavenia->INFO_Total_Vertices = points->GetSize();
	}

	/*float CModel::GetSDF(const struct aiFace* face, bool smoothed)
	{
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			if(tmp->data->assimp_ref == face)
			{
				if(smoothed)
					return tmp->data->diameter->smoothed;
				else
					return tmp->data->diameter->value;
			}
			tmp = tmp->next;
		}
		return 0;
	}*/

	float* CModel::GetSDF(int& size, bool smoothed)
	{
		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		size = points->GetSize();
		float* values = new float[size];
		for(int i = 0; i < size; i++)
		{
			float hodnota = 0;
			float total = float(tmp->data->susedia->GetSize());
			LinkedList<void>::Cell<void>* tm = tmp->data->susedia->start;
			while(tm != NULL)
			{
				if(smoothed)
					hodnota += ((Face*)tm->data)->quality->smoothed;
				else
					hodnota += ((Face*)tm->data)->quality->value;
				tm = tm->next;
			}
			hodnota = hodnota / total;
			values[i] = hodnota;
			tmp = tmp->next;
		}
		return values;
	}

	// resetuje "show" nastavenia
	void CModel::ResetSettings()
	{
		loaded = false;
		draw_mode = 1;
		show_octree = false;
		show_normals = false;
		selected = NULL;

		b_size = 0.0;
		b_sf = 1.0;
		b_max = 0.0;
	}

	// priradi poradie
	void CModel::AssignNumber()
	{
		LinkedList<Vertex>::Cell<Vertex>* tmp1 = points->start;
		int pocet = 0;
		while(tmp1 != NULL)
		{
			tmp1->data->number = pocet;
			tmp1 = tmp1->next;
			pocet++;
		}
		LinkedList<Face>::Cell<Face>* tmp2 = triangles->start;
		pocet = 0;
		while(tmp2 != NULL)
		{
			tmp2->data->number = pocet;
			tmp2 = tmp2->next;
			pocet++;
		}
	}

	// vypocita rozmery modelu
	void CModel::ComputeBoundary()
	{
		float minx = 99999.0, miny = 99999.0, minz = 99999.0;
		float maxx = -99999.0, maxy = -99999.0, maxz = -99999.0;

		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		while(tmp != NULL)
		{
			if(tmp->data->P.X < minx)
				minx = tmp->data->P.X;
			if(tmp->data->P.Y < miny)
				miny = tmp->data->P.Y;
			if(tmp->data->P.Z < minz)
				minz = tmp->data->P.Z;

			if(tmp->data->P.X > maxx)
				maxx = tmp->data->P.X;
			if(tmp->data->P.Y > maxy)
				maxy = tmp->data->P.Y;
			if(tmp->data->P.Z > maxz)
				maxz = tmp->data->P.Z;

			tmp = tmp->next;
		}

		b_stred = Vector4((minx+maxx) / 2.0f, (miny+maxy) / 2.0f, (minz+maxz) / 2.0f);
		float sizex = 0;
		float sizey = 0;
		float sizez = 0;

		if(((minx<=0.0)&&(maxx<=0.0)) || ((minx>=0.0)&&(maxx>=0.0)))
			sizex = abs(minx+maxx);
		else
			sizex = abs(minx-maxx);

		if(((miny<=0.0)&&(maxy<=0.0)) || ((miny>=0.0)&&(maxy>=0.0)))
			sizey = abs(miny+maxy);
		else
			sizey = abs(miny-maxy);

		if(((minz<=0.0)&&(maxz<=0.0)) || ((minz>=0.0)&&(maxz>=0.0)))
			sizez = abs(minz+maxz);
		else
			sizez = abs(minz-maxz);

		//NormalizeMatrix(
		b_size = max(max(sizex, sizey), sizez);

		b_sf = b_size / 10.0f;											// 1 / 10 velkosti modelu budu tie vektory
		b_max = sqrt(3.0f) * b_size;									// diagonala kocky
		b_size = b_size / 2.0f;

		SDF_control = new CSDFController(b_max, Assimp);
	}

	// vytvori Octree strukturu
	void CModel::CreateOctree()
	{
		m_root = new Octree(1, b_size, b_stred, NULL);

		unsigned int siz = triangles->GetSize();
		if(siz > 0)
		{
			/*Face** tria = new Face* [siz];
			LinkedList<Face>::Cell<Face>* tmp2 = triangles->start;
			int i = 0;
			while(tmp2 != NULL)
			{
				tria[i] = tmp2->data;
				tmp2 = tmp2->next;
				i++;
			}
			m_root->Build(tria, siz);*/
			Face** tria = new Face* [siz];
			unsigned int* mtria = new unsigned int [siz];
			LinkedList<Face>::Cell<Face>* tmp2 = triangles->start;
			int i = 0;
			while(tmp2 != NULL)
			{
				tria[i] = tmp2->data;
				mtria[i] = 0;
				tmp2 = tmp2->next;
				i++;
			}

			m_root->Build2(tria, mtria, 0, siz);
			delete [] tria;
			delete mtria;
		}
		else
			m_root->Build(NULL, 0);
	}

	// nastavi farby pre picking
	void CModel::SetColors()
	{
		int col;
		int r = 0, g = 0, b = 0;
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			r = r + color_step;
			if(r >= 256) { r = r % 256; g = g + color_step;}
			if(g >= 256) { g = g % 256; b = b + color_step;}
			if(b >= 256) { b = b % 256; logInfo("MAXIMUM FARIEB PRE PICKING DOSIAHNUTE!!");}

			col = r + 256 * g + 256 * 256 * b;
			if(tmp->data != NULL)
				tmp->data->SetColor(col);
			//logInfo(MarshalString("farba: "+col+", RGB: " + r+" "+g+" "+b));

			tmp = tmp->next;
		}
	}

	// nastavi susedov pre facy
	void CModel::ComputeSusedov()
	{
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			tmp->data->ComputeSusedov();
			tmp = tmp->next;
		}
	}

	// pre spravne vycentrovanie pohladu ked sa nacita CModel
	void CModel::GetBoundary(float &siz, float &x, float &y, float &z)
	{
		siz = b_size;
		x = b_stred.X;
		y = b_stred.Y;
		z = b_stred.Z;
	}

	// vykresli CModel
	void CModel::DrawModel()
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
			LinkedList<Face>::Cell<Face>* tmp = triangles->start;
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
				else if((draw_mode == 0) || (draw_mode == 4))
				{
					GLubyte r = 0, g = 0, b = 0;
					ColorToRGB(tmp->data->farba, r,g,b);
					glColor3ub(r, g, b);
					//logInfo(MarshalString("red: "+r+", green: "+g+", blue: "+b));
				}
				// nech vykresli hodnoty SDF funkcie
				else if(draw_mode == 2)
				{
					GLubyte r = 0, g = 0, b = 0;
					HLSToRGB(tmp->data->quality->normalized2, r, g, b);
					glColor3ub(r, g, b);
				}

				glBegin(draw_mode == 3 ? GL_LINE_LOOP : GL_TRIANGLES);
					glNormal3d(tmp->data->normal.X, tmp->data->normal.Y, tmp->data->normal.Z);
					for(unsigned int i = 0; i < 3; i++)
					{
						glVertex3d(tmp->data->v[i]->P.X, tmp->data->v[i]->P.Y, tmp->data->v[i]->P.Z);
					}
				glEnd();
				if((show_normals == true) && (draw_mode != 4))
				{
					glDisable(GL_LIGHTING);
					glColor3f(1.0f,0.0f,0.0f);		// red
					glBegin(GL_LINES);
						//glNormal3d(tmp->data->normal.X, tmp->data->normal.Y, tmp->data->normal.Z);
						glVertex3d(tmp->data->center.X, tmp->data->center.Y, tmp->data->center.Z);
						glVertex3d( tmp->data->normal.X * b_sf + tmp->data->center.X,
									tmp->data->normal.Y * b_sf + tmp->data->center.Y,
									tmp->data->normal.Z * b_sf + tmp->data->center.Z);
					glEnd();
					glColor3f(1.0f,1.0f,1.0f);		// biela farba
					if((draw_mode == 1) || (draw_mode == 3))
						glEnable(GL_LIGHTING);
				}
				tmp = tmp->next;
			}
		}
		if(draw_mode != 4)
		{
			glDisable(GL_LIGHTING);
			glColor3f(0.5f,0.5f,0.5f);							// seda farba
			if(show_octree == true)
				m_root->DrawOctree(true);
		}
		if(selected != NULL)
		{
			Vector4 U = Vector4(selected->v[1]->P - selected->v[0]->P);
			Vector4 V = Vector4(selected->v[2]->P - selected->v[0]->P);
			Vector4 normal = (U % V) * (-1.0);
			normal.Normalize();

			Vector4 tangens = Vector4(selected->v[0]->P - selected->v[2]->P);
			tangens.Normalize();
			Vector4 binormal = tangens % normal;
			binormal.Normalize();
			Mat4 t_mat= Mat4(tangens, normal, binormal);
			glColor3f(0.0f,1.0f,0.0f);
			glBegin(GL_LINES);
				glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3d( normal.X * 5.0 + selected->center.X,
							normal.Y * 5.0 + selected->center.Y,
							normal.Z * 5.0 + selected->center.Z);
				glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3d( binormal.X * b_sf + selected->center.X,
							binormal.Y * b_sf + selected->center.Y,
							binormal.Z * b_sf + selected->center.Z);
				glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3d( tangens.X * b_sf + selected->center.X,
							tangens.Y * b_sf + selected->center.Y,
							tangens.Z * b_sf + selected->center.Z);
			glEnd();

			glColor3f(0.5f,0.5f,1.0f);
			glBegin(GL_LINES);
				unsigned int n_rays = 30;
				float N = (float)n_rays;

				float inc = (float)M_PI * (3.0f - sqrt(5.0f));
				float del = (360.0f / 120) * 2;
				float off = (2.0f / N) / del;
				for(unsigned int k = 0; k < n_rays; k++)
				{
					float y = k * off - 1 + (off / 2.0f) * del;
					float r = sqrt(1 - y*y);
					float phi = k * inc;
					Vector4 ray = Vector4(cos(phi)*r, -y, sin(phi)*r) * t_mat;
					ray.Normalize();
					glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
					glVertex3d( ray.X *  b_sf + selected->center.X,
								ray.Y *  b_sf + selected->center.Y,
								ray.Z *  b_sf + selected->center.Z);
				}

				srand (123);	
				for(int i = 0; i < 30; i++)
				{
					float rndy = rand()%int(120 / 2);
					float rndx = rand()%(360);
					if(rndy < 0.5)
						rndy = 0.5;
					Vector4 ray = (CalcRayFromAngle(rndx, rndy) * t_mat);
					ray.Normalize();
					glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
					glVertex3d( ray.X *  b_sf + selected->center.X,
								ray.Y *  b_sf + selected->center.Y,
								ray.Z *  b_sf + selected->center.Z);
				}
			glEnd();

			Vector4 norm = selected->normal * (-1.0);
			LinkedList<Octree>* octrees = new LinkedList<Octree>();
			SDF_control->ray_octree_traversal(m_root, norm, selected->center, octrees);
			LinkedList<Octree>::Cell<Octree>* tmp = octrees->start;
			glColor3f(1.0f,0.0f,0.0f);
			if(tmp != NULL)
			{
				tmp->data->DrawOctree(false);
				tmp = tmp->next;
			}
			glColor3f(1.0f,0.5f,0.5f);
			while(tmp != NULL)
			{
				tmp->data->DrawOctree(false);
				tmp = tmp->next;
			}
		}
	}

	void CModel::ColorToRGB(int color, GLubyte &R, GLubyte &G, GLubyte &B)
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

	void CModel::HLSToRGB(float SDF_value, GLubyte &R, GLubyte &G, GLubyte &B)
	{
		//#pragma comment( lib, "shlwapi.lib" )  // needed for the ColorHLSToRGB() function
		
		int hue, lum, sat;
		hue = int(SDF_value * 240.0);
		lum = 120;
		sat = 240;

		COLORREF rgbColor = ColorHLSToRGB( hue, lum, sat );
		R = GetRValue(rgbColor);
		G = GetGValue(rgbColor);
		B = GetBValue(rgbColor);
		//logInfo(MarshalString("value: " + SDF_value + ", R: " + R + ", G: "+G+", B: "+B));
	}

	void CModel::setDrawMode(int mode)
	{
		draw_mode = mode;
	}
	int CModel::getDrawMode()
	{
		return draw_mode;
	}

	void CModel::ProcessPick(int x, int y)
	{
		GLint viewport[4];
		GLubyte pixel[3];

		glGetIntegerv(GL_VIEWPORT,viewport);

		glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

		logInfo(MarshalString("Selected Pixel at x: " + x + ", y: "+y+" is: " + pixel[0]+" "+pixel[1]+" "+pixel[2]));
		
		selected = NULL;
		int color = pixel[0] + 256 * int(pixel[1]) + 256 * 256 * int(pixel[2]);
		logInfo(MarshalString("Selected Pixel farba: "+color));

		if(color > 0)
		{
			LinkedList<Face>::Cell<Face>* tmp = triangles->start;
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
	void CModel::ComputeSDF()
	{
		if(Nastavenia->SDF_Mode == SDF_CPU)
			SDF_control->Compute(triangles, m_root);
		else
			SDF_control->ComputeOpenCL(points, triangles, m_root);		
	}
	void CModel::TriangulatePoints()
	{
		PointCloudTriangulation::DeleunayTriangulator *pTriangulator = new PointCloudTriangulation::DeleunayTriangulator();
		//pTriangulator->setKNeighParams(0.02, 8, 12);
		//pTriangulator->setCenterFactorParams(0.2, 0.5, 2.0);

		int numOfIndices = 0;
		int * indices = NULL;
		unsigned int numOfVertices = points->GetSize();
		Vertex** tmp_points = new Vertex* [numOfVertices];

		// prekopcime do pola, koli lepsiemu pristupu
		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		unsigned int i = 0;
		while(tmp != NULL)
		{
			tmp_points[i] = tmp->data;
			tmp = tmp->next;
			i++;
		}

		// zoznam ktory davam do Madovej libky
		float * verts = new float[numOfVertices * 3];
		for(unsigned int ii = 0; ii < numOfVertices; ii++)
		{
			verts[ii * 3] = tmp_points[ii]->P.X;
			verts[ii * 3 + 1] = tmp_points[ii]->P.Y;
			verts[ii * 3 + 2] = tmp_points[ii]->P.Z;
		}

		// triangulacia v Madovej libke
		pTriangulator->computeGlobalTriangulationFromPoints(numOfVertices, verts, numOfIndices, &indices);
		
		loaded = false;

		// vymazanie struktur
		for(unsigned int iii = 0; iii < numOfVertices; iii++)
		{
			delete tmp_points[iii]->susedia;
			tmp_points[iii]->susedia = new LinkedList<void>();
		}
		selected = NULL;

		if(m_root != NULL)
			delete m_root;

		triangles->CompleteDelete();
		delete triangles;
		triangles = new LinkedList<Face>();

		// prelinkovanie struktur
		for (int l=0; l<numOfIndices/3; l++)
		{
			int v1 = indices[l * 2];
			int v2 = indices[l * 2 + 2];
			int v3 = indices[l * 2 + 4];
			Face* novy_face = new Face(tmp_points[v1], tmp_points[v2], tmp_points[v3]);
			triangles->InsertToEnd(novy_face);
			tmp_points[v1]->susedia->InsertToEnd(novy_face);
			tmp_points[v2]->susedia->InsertToEnd(novy_face);
			tmp_points[v3]->susedia->InsertToEnd(novy_face);
		}

		loaded = true;

		// postprocessing ako Octree a pod.
		AssignNumber();
		CreateOctree();
		SetColors();
		ComputeSusedov();

		Nastavenia->INFO_Total_Triangles = triangles->GetSize();
	}
	void CModel::ReloadOctreeData()
	{
		if(m_root != NULL)
			delete m_root;

		m_root = NULL;

		CreateOctree();
	}
}