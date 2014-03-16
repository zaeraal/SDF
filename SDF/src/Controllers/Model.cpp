// Model.cpp : subor pre kontrolu modelov
#include "stdafx.h"
#include "Model.h"
#include "PointCloudTriangulation.h"

namespace ModelController
{
	CModel::CModel()
	{
		Assimp = new CAssimp();
		VCGlib = new CVCG();
		SDF_control = NULL;
		m_root = NULL;
		triangles = new LinkedList<Face>();
		points = new LinkedList<Vertex>();
		ResetSettings();
	}

	CModel::~CModel()
	{
		delete Assimp;
		delete VCGlib;
		delete SDF_control;

		/*
		/// -------------------------------------
		/// STRASNE DLHO TRVA ZATVORENIE PROGRAMU
		/// NATRVALO ZAKOMENTOVANE, neni nutne :D
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

	// nacita subor cez assimp
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
		ComputeBoundary();
		EraseIdenticalVertices();
		loaded = true;

		int ticks4 = GetTickCount();
		AssignNumber();
		CreateOctree();

		int ticks5 = GetTickCount();

		SetColors();
		ComputeSusedov();
		ComputeSoftNormals();
		NormalizeTextureCoords();

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

	// nacita subor cez VCG lib
	void CModel::LoadFileVCG(std::string Filename)
	{
		int ticks1 = GetTickCount();
		ResetSettings();

		// ak znovu nacitavame, premaz povodne udaje
		if(m_root != NULL)
			delete m_root;

		if(SDF_control != NULL)
			delete SDF_control;

		LinkedList<Face>* triangles_backup;
		LinkedList<Vertex>* points_backup;

		// delete actual faces and vertices
		/*triangles->CompleteDelete();
		delete triangles;

		points->CompleteDelete();
		delete points;*/

		triangles_backup = triangles;
		points_backup = points;

		m_root = NULL;
		SDF_control = NULL;
		triangles = new LinkedList<Face>();
		points = new LinkedList<Vertex>();
		int ticks2 = GetTickCount();

		VCGlib->Import3DFromFile(Filename);

		int ticks3 = GetTickCount();
		VCGlib->LoadData(triangles, points);
		ComputeBoundary();
		//EraseIdenticalVertices();
		CopySDF_Vertices_to_Faces();
		loaded = true;

		int ticks4 = GetTickCount();
		AssignNumber();
		CreateOctree();

		int ticks5 = GetTickCount();

		SetColors();
		ComputeSusedov();
		ComputeSoftNormals();
		NormalizeTextureCoords();

		int ticks6 = GetTickCount();

		logInfo(MarshalString("Vycistenie starych zaznamov: " + (ticks2 - ticks1)+ "ms"));
		logInfo(MarshalString("Nacitanie modelu do Assimpu: " + (ticks3 - ticks2)+ "ms"));
		logInfo(MarshalString("Nacitanie Assimpu do mojich objektov: " + (ticks4 - ticks3)+ "ms"));
		logInfo(MarshalString("Vytvorenie Octree: " + (ticks5 - ticks4)+ "ms"));
		logInfo(MarshalString("Vytvorenie susedov: " + (ticks6 - ticks5)+ "ms"));
		logInfo(MarshalString("Celkovy cas nacitania: " + (ticks6 - ticks1)+ "ms"));

		Nastavenia->INFO_Total_Triangles = triangles->GetSize();
		Nastavenia->INFO_Total_Vertices = points->GetSize();

		if((triangles->GetSize() == triangles_backup->GetSize()) && (points->GetSize() == points_backup->GetSize()))
		{
			MergeResults(triangles_backup, points_backup);
		}
		triangles_backup->CompleteDelete();
		delete triangles_backup;

		points_backup->CompleteDelete();
		delete points_backup;
	}

	void CModel::SaveModel(std::string Filename)
	{
		VCGlib->SavetoFile(Filename, triangles, points);
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
		ComputeBoundary();
		EraseIdenticalVertices();
		loaded = true;

		AssignNumber();
		CreateOctree();
		SetColors();
		ComputeSusedov();
		ComputeSoftNormals();
		NormalizeTextureCoords();

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
					return tmp->data->quality->smoothed;
				else
					return tmp->data->quality->value;
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
			if(smoothed)
				values[i] = tmp->data->quality->smoothed;
			else
				values[i] = tmp->data->quality->value;
			tmp = tmp->next;
		}
		return values;
	}

	void CModel::CopySDF_Vertices_to_Faces()
	{
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			float h_smoothed	= 0;
			float h_value		= 0;
			float h_normalized1 = 0;
			float h_normalized2 = 0;
			float h_normalized3 = 0;
			float h_normalized4 = 0;

			float total = 3.0f;
			for(int i = 0; i<3; i++)
			{
				h_smoothed += tmp->data->v[i]->quality->smoothed;
				h_value += tmp->data->v[i]->quality->value;
				h_normalized1 += tmp->data->v[i]->quality->normalized1;
				h_normalized2 += tmp->data->v[i]->quality->normalized2;
				h_normalized3 += tmp->data->v[i]->quality->normalized3;
				h_normalized4 += tmp->data->v[i]->quality->normalized4;
			}
			h_smoothed = h_smoothed / total;
			h_value = h_value / total;
			h_normalized1 = h_normalized1 / total;
			h_normalized2 = h_normalized2 / total;
			h_normalized3 = h_normalized3 / total;
			h_normalized4 = h_normalized4 / total;

			tmp->data->quality->smoothed = h_smoothed;
			tmp->data->quality->value = h_value;
			tmp->data->quality->normalized1 = h_normalized1;
			tmp->data->quality->normalized2 = h_normalized2;
			tmp->data->quality->normalized3 = h_normalized3;
			tmp->data->quality->normalized4 = h_normalized4;

			tmp = tmp->next;
		}
	}
	void CModel::CopySDF_Faces_to_Vertices()
	{
		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		while(tmp != NULL)
		{
			float h_smoothed	= 0;
			float h_value		= 0;
			float h_normalized1 = 0;
			float h_normalized2 = 0;
			float h_normalized3 = 0;
			float h_normalized4 = 0;

			float total = float(tmp->data->susedia->GetSize());
			LinkedList<void>::Cell<void>* tm = tmp->data->susedia->start;
			while(tm != NULL)
			{
				h_smoothed += ((Face*)tm->data)->quality->smoothed;
				h_value += ((Face*)tm->data)->quality->value;
				h_normalized1 += ((Face*)tm->data)->quality->normalized1;
				h_normalized2 += ((Face*)tm->data)->quality->normalized2;
				h_normalized3 += ((Face*)tm->data)->quality->normalized3;
				h_normalized4 += ((Face*)tm->data)->quality->normalized4;
				tm = tm->next;
			}
			h_smoothed = h_smoothed / total;
			h_value = h_value / total;
			h_normalized1 = h_normalized1 / total;
			h_normalized2 = h_normalized2 / total;
			h_normalized3 = h_normalized3 / total;
			h_normalized4 = h_normalized4 / total;

			tmp->data->quality->smoothed = h_smoothed;
			tmp->data->quality->value = h_value;
			tmp->data->quality->normalized1 = h_normalized1;
			tmp->data->quality->normalized2 = h_normalized2;
			tmp->data->quality->normalized3 = h_normalized3;
			tmp->data->quality->normalized4 = h_normalized4;

			tmp = tmp->next;
		}
	}
	// resetuje "show" nastavenia
	void CModel::ResetSettings()
	{
		loaded = false;
		selected = NULL;

		b_size = 0.0;
		b_sf = 1.0;
		b_max = 0.0;
		o_min = Vector4();
		o_max = Vector4();
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
			sizex = abs(maxx-minx);
		else
			sizex = abs(minx-maxx);

		if(((miny<=0.0)&&(maxy<=0.0)) || ((miny>=0.0)&&(maxy>=0.0)))
			sizey = abs(maxy-miny);
		else
			sizey = abs(miny-maxy);

		if(((minz<=0.0)&&(maxz<=0.0)) || ((minz>=0.0)&&(maxz>=0.0)))
			sizez = abs(maxz-minz);
		else
			sizez = abs(minz-maxz);

		b_size = max(max(sizex, sizey), sizez);

		b_sf = b_size / 10.0f;											// 1 / 10 velkosti modelu budu tie vektory
		b_max = sqrt(3.0f) * b_size;									// diagonala kocky
		b_size = b_size / 2.0f + 0.005f;

		SDF_control = new CSDFController(b_max, Assimp);
	}

	// vytvori Octree strukturu
	void CModel::CreateOctree()
	{
		m_root = new Octree(1, b_size, b_stred);

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
			nodeCount = 0, triangleCount = 0, leafCount = 0;
			m_root->Build2(tria, mtria, 0, siz, nodeCount, triangleCount, leafCount);
			o_min = Vector4(m_root->origin.X - m_root->size, m_root->origin.Y - m_root->size, m_root->origin.Z - m_root->size, 1.0);
			o_max = Vector4(m_root->origin.X + m_root->size, m_root->origin.Y + m_root->size, m_root->origin.Z + m_root->size, 1.0);
			delete [] tria;
			delete mtria;
		}
		else
			m_root->Build(NULL, 0);

		//BuildArrays();
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
		Vector4 look = Vector4(Look_X, Look_Y, Look_Z, 0.0f);
		Vector4 upp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
		Vector4 rightt = look % upp; rightt.Normalize();
		upp = look % rightt; upp.Normalize();
		Vector4 projected;

		if((Nastavenia->VISUAL_State == VISUAL_DEFAULT) || (Nastavenia->VISUAL_State == VISUAL_SDF))
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);

		if(Nastavenia->VISUAL_State == VISUAL_PICKING)
			glDisable(GL_DITHER);
		else
			glEnable(GL_DITHER);

		float alpha = (float)Nastavenia->VISUAL_Alpha / 255.0f;
		glColor4f(0.75f,0.75f,0.75f,alpha);							// biela farba
		{
			LinkedList<Face>::Cell<Face>* tmp = triangles->start;
			while(tmp != NULL)
			{
				if(tmp->data == NULL)
				{
					tmp = tmp->next;
					continue;
				}
				if((Nastavenia->VISUAL_State != VISUAL_PICKING) && (Nastavenia->VISUAL_Points == true))
				{
					if(Nastavenia->SDF_Smoothing_Radius < 2)
						glColor4f(0.0f,0.0f,0.0f,1.0f);			// black
					else
					{
						GLubyte r = 0, g = 0, b = 0;
						switch(Nastavenia->VISUAL_SDF_Type)
						{
						case VISUAL_NORMALIZED_0_1: HLSToRGB(tmp->data->quality->normalized1, r, g, b); break;
						case VISUAL_NORMALIZED_MIN_1: HLSToRGB(tmp->data->quality->normalized2, r, g, b); break;
						case VISUAL_NORMALIZED_0_MAX: HLSToRGB(tmp->data->quality->normalized3, r, g, b); break;
						case VISUAL_NORMALIZED_MIN_MAX: HLSToRGB(tmp->data->quality->normalized4, r, g, b); break;
						default: break;
						}
						glColor4ub(r, g, b, 255);
					}
					glBegin(GL_QUADS);
					float t_sf = b_sf * 0.02f;
					glNormal3f(Look_X, Look_Y, Look_Z);
					projected = tmp->data->center - tmp->data->normal * tmp->data->quality->smoothed * 0.5f;
					glVertex3f(projected.X - (rightt.X + upp.X) * t_sf, projected.Y - (rightt.Y + upp.Y) * t_sf, projected.Z - (rightt.Z + upp.Z) * t_sf);
					glVertex3f(projected.X + (rightt.X - upp.X) * t_sf, projected.Y + (rightt.Y - upp.Y) * t_sf, projected.Z + (rightt.Z - upp.Z) * t_sf);
					glVertex3f(projected.X + (rightt.X + upp.X) * t_sf, projected.Y + (rightt.Y + upp.Y) * t_sf, projected.Z + (rightt.Z + upp.Z) * t_sf);
					glVertex3f(projected.X - (rightt.X - upp.X) * t_sf, projected.Y - (rightt.Y - upp.Y) * t_sf, projected.Z - (rightt.Z - upp.Z) * t_sf);
					glEnd();
					glColor4f(0.75f,0.75f,0.75f,alpha);
				}
				// nech vykresli selectnuty trojuholnik
				if(Nastavenia->VISUAL_State == VISUAL_DEFAULT)
				{
					if(tmp->data == selected)
						glColor4f(1.0f,0.5f,0.0f,alpha);		// orange
					else
						glColor4f(0.75f,0.75f,0.75f,alpha);		// white
				}
				// nech zorbazi picking hodnoty
				else if(Nastavenia->VISUAL_State == VISUAL_PICKING)
				{
					GLubyte r = 0, g = 0, b = 0;
					ColorToRGB(tmp->data->farba, r,g,b);
					glColor3ub(r, g, b);
					//logInfo(MarshalString("red: "+r+", green: "+g+", blue: "+b));
				}
				// nech vykresli hodnoty SDF funkcie
				else if((Nastavenia->VISUAL_State == VISUAL_SDF) && (Nastavenia->VISUAL_Smoothed == false))
				{
					GLubyte r = 0, g = 0, b = 0;
					switch(Nastavenia->VISUAL_SDF_Type)
					{
						case VISUAL_NORMALIZED_0_1: HLSToRGB(tmp->data->quality->normalized1, r, g, b); break;
						case VISUAL_NORMALIZED_MIN_1: HLSToRGB(tmp->data->quality->normalized2, r, g, b); break;
						case VISUAL_NORMALIZED_0_MAX: HLSToRGB(tmp->data->quality->normalized3, r, g, b); break;
						case VISUAL_NORMALIZED_MIN_MAX: HLSToRGB(tmp->data->quality->normalized4, r, g, b); break;
						default: break;
					}
					glColor4ub(r, g, b, Nastavenia->VISUAL_Alpha);
				}
				if(Nastavenia->VISUAL_State != VISUAL_NONE)
				{
					glBegin(Nastavenia->VISUAL_State == VISUAL_WIREFRAME ? GL_LINE_LOOP : GL_TRIANGLES);
						glNormal3f(tmp->data->normal.X, tmp->data->normal.Y, tmp->data->normal.Z);
						for(unsigned int i = 0; i < 3; i++)
						{
							if((Nastavenia->VISUAL_State == VISUAL_SDF) && (Nastavenia->VISUAL_Smoothed == true))
							{
								GLubyte r = 0, g = 0, b = 0;
								switch(Nastavenia->VISUAL_SDF_Type)
								{
									case VISUAL_NORMALIZED_0_1: HLSToRGB(tmp->data->v[i]->quality->normalized1, r, g, b); break;
									case VISUAL_NORMALIZED_MIN_1: HLSToRGB(tmp->data->v[i]->quality->normalized2, r, g, b); break;
									case VISUAL_NORMALIZED_0_MAX: HLSToRGB(tmp->data->v[i]->quality->normalized3, r, g, b); break;
									case VISUAL_NORMALIZED_MIN_MAX: HLSToRGB(tmp->data->v[i]->quality->normalized4, r, g, b); break;
									default: break;
								}
								glColor4ub(r, g, b, Nastavenia->VISUAL_Alpha);
							}
							if(tmp->data->v[i]->HasNormal() && (Nastavenia->VISUAL_Smoothed == true))
								glNormal3f(tmp->data->v[i]->GetNormal().X, tmp->data->v[i]->GetNormal().Y, tmp->data->v[i]->GetNormal().Z);
							glVertex3f(tmp->data->v[i]->P.X, tmp->data->v[i]->P.Y, tmp->data->v[i]->P.Z);
						}
					glEnd();
				}
				if((Nastavenia->VISUAL_Normals == true) && (Nastavenia->VISUAL_State != VISUAL_PICKING))
				{
					glDisable(GL_LIGHTING);
					glColor4f(1.0f,0.0f,0.0f,1.0f);		// red
					glBegin(GL_LINES);
						//glNormal3f(tmp->data->normal.X, tmp->data->normal.Y, tmp->data->normal.Z);
						glVertex3f(tmp->data->center.X, tmp->data->center.Y, tmp->data->center.Z);
						glVertex3f( tmp->data->normal.X * b_sf + tmp->data->center.X,
									tmp->data->normal.Y * b_sf + tmp->data->center.Y,
									tmp->data->normal.Z * b_sf + tmp->data->center.Z);
					glEnd();
					glColor4f(1.0f,1.0f,1.0f, alpha);		// biela farba
					if((Nastavenia->VISUAL_State == VISUAL_DEFAULT) || (Nastavenia->VISUAL_State == VISUAL_WIREFRAME))
						glEnable(GL_LIGHTING);
				}
				tmp = tmp->next;
			}
		}
		glDisable(GL_LIGHTING);
		if(Nastavenia->VISUAL_State != VISUAL_PICKING)
		{
			if(Nastavenia->VISUAL_Octree == true)
			{
				/*int tmp_threshold = Nastavenia->OCTREE_Threshold;
				Nastavenia->OCTREE_Threshold = ((triangles->GetSize() / 450) > 4) ? (triangles->GetSize() / 450) : 4;
				Nastavenia->OCTREE_Depth = Nastavenia->OCTREE_Depth - 2;
				
				LinkedList<Face>::Cell<Face>* current_face = triangles->start;
				LinkedList<PPoint>* point_list = new LinkedList<PPoint>();
				while(current_face != NULL)
				{
					// projektnute body
					PPoint* tmpp = new PPoint(current_face->data->center + ((current_face->data->normal * -1.0f) * current_face->data->quality->value) / 2.0f, current_face->data);
					tmpp->diameter = current_face->data->quality->value;
				
					if(tmpp->diameter >= 0.1f)
						point_list->InsertToEnd(tmpp);
				
					current_face = current_face->next;
				}
				
				float b_size; unsigned int n_pnodes;
				Vector4 b_stred = SDF_control->ComputePointBoundary(point_list, b_size);
				ROctree* mm_root = SDF_control->CreateROctree(point_list, b_size, b_stred, n_pnodes);
				
				mm_root->DrawOctree(true);
				
				point_list->CompleteDelete();
				Nastavenia->OCTREE_Depth = Nastavenia->OCTREE_Depth + 2;
				Nastavenia->OCTREE_Threshold = tmp_threshold;*/
				glColor4f(0.5f,0.5f,0.5f,1.0f);							// seda farba
				m_root->DrawOctree(true);
			}
			if(Nastavenia->VISUAL_Axes == true)
			{
				m_root->DrawAxes();
			}
		}
		if((selected != NULL) && (Nastavenia->VISUAL_State != VISUAL_PICKING))
		{
			Vector4 normal = selected->normal * (-1.0);
			/*Vector4 U = Vector4(selected->v[1]->P - selected->v[0]->P);
			Vector4 V = Vector4(selected->v[2]->P - selected->v[0]->P);
			Vector4 normal = (U % V) * (-1.0);
			normal.Normalize();*/

			Vector4 tangens = Vector4(selected->v[0]->P - selected->v[2]->P);
			tangens.Normalize();
			Vector4 binormal = tangens % normal;
			binormal.Normalize();
			Mat4 t_mat= Mat4(tangens, normal, binormal);
			glColor4f(0.0f,1.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3f( normal.X * 5.0f + selected->center.X,
							normal.Y * 5.0f + selected->center.Y,
							normal.Z * 5.0f + selected->center.Z);
				glVertex3f(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3f( binormal.X * b_sf + selected->center.X,
							binormal.Y * b_sf + selected->center.Y,
							binormal.Z * b_sf + selected->center.Z);
				glVertex3f(selected->center.X, selected->center.Y, selected->center.Z);
				glVertex3f( tangens.X * b_sf + selected->center.X,
							tangens.Y * b_sf + selected->center.Y,
							tangens.Z * b_sf + selected->center.Z);
			glEnd();

			glColor4f(0.5f,0.5f,1.0f,1.0f);
			glBegin(GL_LINES);
				unsigned int n_rays = Nastavenia->SDF_Rays;
				if(Nastavenia->SDF_Distribution == true)
				{
					float N = (float)n_rays;

					float mull = 4.0f / (2.0f * (1.0f - cos( (Nastavenia->SDF_Cone / 2.0f) * (float)M_PI / 180.0f )));
					N = N * mull;

					float inc = (float)M_PI * (3.0f - sqrt(5.0f));
					float off = (2.0f / N);
					for(unsigned int k = 0; k < n_rays; k++)
					{
						float y = k * off - 1 + (off / 2.0f);
						float r = sqrt(1 - y*y);
						float phi = k * inc;
						Vector4 ray = Vector4(cos(phi)*r, -y, sin(phi)*r) * t_mat;
						ray.Normalize();
						glVertex3f(selected->center.X, selected->center.Y, selected->center.Z);
						glVertex3f( ray.X *  b_sf + selected->center.X,
									ray.Y *  b_sf + selected->center.Y,
									ray.Z *  b_sf + selected->center.Z);
					}
				}
				else
				{
					srand (123);	
					for(unsigned int i = 0; i < n_rays; i++)
					{
						float rndy = (float)(rand()%int(Nastavenia->SDF_Cone / 2.0f));
						float rndx = (float)(rand()%(360));
						if(rndy < 0.5)
							rndy = 0.5;
						Vector4 ray = (CalcRayFromAngle(rndx, rndy) * t_mat);
						ray.Normalize();
						glVertex3f(selected->center.X, selected->center.Y, selected->center.Z);
						glVertex3f( ray.X *  b_sf + selected->center.X,
									ray.Y *  b_sf + selected->center.Y,
									ray.Z *  b_sf + selected->center.Z);
					}
				}
			glEnd();

			Vector4 norm = selected->normal * (-1.0);
			LinkedList<Octree>* octrees = new LinkedList<Octree>();
			SDF_control->ray_octree_traversal(m_root, norm, selected->center, octrees, o_min, o_max);
			LinkedList<Octree>::Cell<Octree>* tmp = octrees->start;
			glColor4f(1.0f,0.0f,0.0f,1.0f);
			if(tmp != NULL)
			{
				tmp->data->DrawOctree(false);
				tmp = tmp->next;
			}
			glColor4f(1.0f,0.5f,0.5f,1.0f);
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
		lum = 120;//int(SDF_value * 120.0);
		sat = 240;

		COLORREF rgbColor = ColorHLSToRGB( hue, lum, sat );
		R = GetRValue(rgbColor);
		G = GetGValue(rgbColor);
		B = GetBValue(rgbColor);
		//logInfo(MarshalString("value: " + SDF_value + ", R: " + R + ", G: "+G+", B: "+B));
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
		Nastavenia->SDF_STATUS = 0;

		if(Nastavenia->SDF_Mode == SDF_CPU)
			SDF_control->Compute(triangles, m_root, o_min, o_max);
		else
			//SDF_control->ComputeOpenCL(points, triangles, m_root, o_min, o_max);
			SDF_control->ComputeOpenCL2(points, triangles, m_root, o_min, o_max, nodeCount, leafCount, triangleCount);

		CopySDF_Faces_to_Vertices();
		Nastavenia->SDF_STATUS = 100;
	}
	void CModel::TriangulatePoints()
	{
		loaded = false;

		// vymazanie struktur
		selected = NULL;

		if(m_root != NULL)
			delete m_root;

		triangles->CompleteDelete();
		delete triangles;
		triangles = new LinkedList<Face>();

		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		while(tmp != NULL)
		{
			tmp->data->susedia->Clear();
			delete tmp->data->susedia;

			tmp = tmp->next;
		}

		DeleteIdenticalVertices();

		PointCloudTriangulation::DeleunayTriangulator *pTriangulator = new PointCloudTriangulation::DeleunayTriangulator();
		//pTriangulator->setKNeighParams(0.02, 8, 12);
		//pTriangulator->setCenterFactorParams(0.2, 0.5, 2.0);

		int numOfTriangles = 0;
		int *triangleverts = NULL;
		float *normals = NULL;
		unsigned int numOfVertices = points->GetSize();
		Vertex** tmp_points = new Vertex* [numOfVertices];

		// prekopcime do pola, koli lepsiemu pristupu
		tmp = points->start;
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

		for(unsigned int iii = 0; iii < numOfVertices; iii++)
		{
			delete tmp_points[iii]->susedia;
			tmp_points[iii]->susedia = new LinkedList<void>();
		}
		
		// triangulacia v Madovej libke
		pTriangulator->computeGlobalTriangulationFromPoints(numOfVertices, verts, numOfTriangles, &triangleverts, &normals, false);
		
		// kopcenie normal
		for (unsigned int l=0; l<numOfVertices; l++)
		{
			Vector4 normala = Vector4(normals[l * 3 + 0], normals[l * 3 + 1], normals[l * 3 + 2]);
			tmp_points[l]->SetNormal(normala);
		}

		// prelinkovanie struktur
		for (int l=0; l<numOfTriangles; l++)
		{
			int v1 = triangleverts[l * 3 + 0];
			int v2 = triangleverts[l * 3 + 1];
			int v3 = triangleverts[l * 3 + 2];		
			Face* novy_face = new Face(tmp_points[v1], tmp_points[v2], tmp_points[v3]);
			Vector4 t_nor = tmp_points[v1]->GetNormal() + tmp_points[v2]->GetNormal() + tmp_points[v3]->GetNormal();
			t_nor.Normalize();
			float theta = acos(novy_face->normal * t_nor);
			theta = theta * float(180.0 / M_PI);
			if(theta > 90.0f)
			{
				novy_face->v[0] = tmp_points[v3];
				novy_face->v[2] = tmp_points[v1];
			}
			novy_face->normal = t_nor;
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
		Nastavenia->INFO_Total_Vertices = points->GetSize();

		delete pTriangulator;
		delete [] verts;
		delete [] triangleverts;
		delete [] normals;
		delete [] tmp_points;
	}

	void CModel::DeleteIdenticalVertices()
	{
		float delta = b_size * 2 * 0.00001f;
		LinkedList<Vertex>* new_vertices = new LinkedList<Vertex>();
		LinkedList<Vertex>::Cell<Vertex>* tmp1 = points->start;
		while(tmp1 != NULL)
		{
			bool add = true;
			LinkedList<Vertex>::Cell<Vertex>* tmp2 = points->start;
			while(tmp2 != NULL)
			{
				if(tmp1 == tmp2)
					break;
				float distance = tmp1->data->P.Dist(tmp2->data->P);
				if(distance < delta)
				{
					add = false;
					break;
				}	
				tmp2 = tmp2->next;
			}
			if(add == true)
				new_vertices->InsertToEnd(tmp1->data);
			tmp1 = tmp1->next;
		}
		delete points;
		points = new_vertices;
	}

	void CModel::EraseIdenticalVertices()
	{
		float delta = b_size * 2 * 0.00001f;
		LinkedList<Vertex>* new_vertices = new LinkedList<Vertex>();
		LinkedList<Face>::Cell<Face>* tmp1 = triangles->start;
		while(tmp1 != NULL)
		{
			for(int i = 0; i < 3; i++)
			{
				bool add = true;
				LinkedList<Vertex>::Cell<Vertex>* tmp2 = new_vertices->start;
				while(tmp2 != NULL)
				{
					// nemalo by nastat kedze vytvaram uplne nove
					if(tmp1->data->v[i] == tmp2->data)
						break;
					float distance = tmp1->data->v[i]->P.Dist(tmp2->data->P);
					if(distance < delta)
					{
						if(abs(tmp2->data->texCoord_U - tmp1->data->v[i]->texCoord_U) > 0.01f)
						{
							tmp2 = tmp2->next;
							continue;
						}
						if(abs(tmp2->data->texCoord_V - tmp1->data->v[i]->texCoord_V) > 0.01f)
						{
							tmp2 = tmp2->next;
							continue;
						}
						tmp1->data->v[i] = tmp2->data;
						add = false;
						break;
					}	
					tmp2 = tmp2->next;
				}
				if(add == true)
				{
					Vertex* new_vert = new Vertex(tmp1->data->v[i]->P);
					new_vert->texCoord_U = tmp1->data->v[i]->texCoord_U;
					new_vert->texCoord_V = tmp1->data->v[i]->texCoord_V;
					if(tmp1->data->v[i]->HasNormal())
					{
						new_vert->SetNormal(tmp1->data->v[i]->GetNormal());
					}
					new_vert->susedia->InsertToEnd(tmp1->data);
					tmp1->data->v[i] = new_vert;
					new_vertices->InsertToEnd(new_vert);
					
				}
			}
			tmp1 = tmp1->next;
		}
		points->CompleteDelete();
		delete points;

		points = new_vertices;
	}

	void CModel::ReloadOctreeData()
	{
		int ticks1 = GetTickCount();

		if(m_root != NULL)
			delete m_root;

		m_root = NULL;

		int ticks2 = GetTickCount();
		CreateOctree();

		int ticks3 = GetTickCount();

		logInfo(MarshalString("Zmazanie stareho Octree: " + (ticks2 - ticks1)+ "ms"));
		logInfo(MarshalString("Znovu vytvorenie Octree: " + (ticks3 - ticks2)+ "ms"));
	}

	void CModel::BuildArrays()
	{
		/*if(m_root == NULL)
			return;
		if(o_array != NULL)
			delete [] o_array;
		if(t_array != NULL)
			delete [] t_array;

		Octree** oc_array = new Octree*[m_root->nodeCount];
		o_array = new unsigned int[m_root->nodeCount];
		t_array = new unsigned int[m_root->leafCount + m_root->triangleCount];	// pocet + trojuholniky

		logDebug(MarshalString("node count: "+ m_root->nodeCount));
		logDebug(MarshalString("triangle count: "+ m_root->triangleCount));
		logDebug(MarshalString("spolu v t_array: "+ (m_root->leafCount + m_root->triangleCount)));

		Octree* node = m_root;
		unsigned int end = 0;
		oc_array[0] = node;
		unsigned int tidx = 0;
		bool jeden_krat = true;
		unsigned int najvecsi_o = 0;
		unsigned int najvecsi_t = 0;
		for(unsigned int idx = 0; idx < (m_root->leafCount + m_root->triangleCount); idx++)
		{
			t_array[idx] = 0;
		}
		for(unsigned int idx = 0; idx < m_root->nodeCount; idx++)
		{
			node = oc_array[idx];
			if(node->isLeaf)
			{
				if(node->count == 0)
					assert("daco sa povrzalo v octree");
				unsigned int safe_tidx = tidx << 8;
				o_array[idx] = safe_tidx;
				t_array[tidx] = node->count;
				tidx++;
				for(unsigned int j = 0; j < node->count; j++)
				{
					t_array[tidx] = node->triangles[j]->number;
					if(tidx > najvecsi_t)
						najvecsi_t = tidx;
					tidx++;
				}
				continue;
			}
			jeden_krat = true;
			for(int i = 0; i < 8; i++)
			{
				if((node->sons >> i) & 1)
				{
					end++;
					if(jeden_krat == true)
					{
						unsigned int safe_end = end<<8;
						safe_end = safe_end +  node->sons;
						o_array[idx] = safe_end;
						jeden_krat = false;
					}
					oc_array[end] = node->son[i];
					if(end > najvecsi_o)
						najvecsi_o = end;
				}
			}
		}
		delete [] oc_array;
		logDebug(MarshalString("najvecsi_t: "+ najvecsi_t));
		logDebug(MarshalString("najvecsi_o: "+ najvecsi_o));*/
	}
	void CModel::ComputeSoftNormals()
	{
		LinkedList<Vertex>::Cell<Vertex>* tmp1 = points->start;
		while(tmp1 != NULL)
		{
			LinkedList<void>::Cell<void>* tmp2 = tmp1->data->susedia->start;
			Vector4 normala;
			while(tmp2 != NULL)
			{
				normala = normala + ((Face*)tmp2->data)->normal;
				tmp2 = tmp2->next;
			}
			normala.W = 0;
			normala.Normalize();
			tmp1->data->SetNormal(normala);

			tmp1 = tmp1->next;
		}
	}
	void CModel::MergeResults(LinkedList<Face>* triangles_backup, LinkedList<Vertex>* points_backup)
	{
		logInfo(MarshalString("robim rozdiel hodnot SDF"));
		float min = 99999.0f;
		float maxx = -99999.0f;
		float avg = 0.0f;

		LinkedList<Vertex>::Cell<Vertex>* tmp1 = points->start;
		LinkedList<Vertex>::Cell<Vertex>* tmp2 = points_backup->start;
		while(tmp1 != NULL)
		{
			float maxik = 1.6f;
			float val1 = tmp1->data->quality->value > maxik ? maxik : tmp1->data->quality->value;
			float val2 = tmp2->data->quality->value > maxik ? maxik : tmp2->data->quality->value;
			float alfa = 4.0f;

			val1 = (log(((val1 - 0.0f) / (maxik - 0.0f)) * alfa + 1.0f) / log(alfa + 1.0f));
			val2 = (log(((val2 - 0.0f) / (maxik - 0.0f)) * alfa + 1.0f) / log(alfa + 1.0f));

			tmp1->data->quality->value = abs(val1 - val2) * 100.0f;
			tmp1->data->quality->smoothed = abs(val1 - val2) * 100.0f;

			if(tmp1->data->quality->value < min)
				min = tmp1->data->quality->value;

			if(tmp1->data->quality->value > maxx)
				maxx = tmp1->data->quality->value;

			avg += tmp1->data->quality->value;

			tmp1 = tmp1->next;
			tmp2 = tmp2->next;
		}
		avg = avg / (float)(points->GetSize());

		logInfo(MarshalString("min: " + min));
		logInfo(MarshalString("max: " + maxx));
		logInfo(MarshalString("avg: " + avg));

		LinkedList<Face>::Cell<Face>* tmp1f = triangles->start;
		LinkedList<Face>::Cell<Face>* tmp2f = triangles_backup->start;
		while(tmp1f != NULL)
		{
			tmp1f->data->quality->value = tmp1f->data->quality->value - tmp2f->data->quality->value;
			tmp1f->data->quality->smoothed = tmp1f->data->quality->smoothed - tmp2f->data->quality->smoothed;

			tmp1f = tmp1f->next;
			tmp2f = tmp2f->next;
		}
	}
	void CModel::RecomputeSmoothing()
	{
		float min = 99999.0f;
		float max = -99999.0f;
		LinkedList<Face>::Cell<Face>* tmp1 = triangles->start;
		while(tmp1 != NULL)
		{
			if(tmp1->data->quality->value < min)
				min = tmp1->data->quality->value;

			if(tmp1->data->quality->value > max)
				max = tmp1->data->quality->value;

			tmp1 = tmp1->next;
		}

		if(Nastavenia->SDF_Smooth_Projected == true)
		{
			int tmp_threshold = Nastavenia->OCTREE_Threshold;
			Nastavenia->OCTREE_Threshold = (triangles->GetSize() / 450) < 4 ? 4 : (triangles->GetSize() / 450);
			Nastavenia->OCTREE_Depth = Nastavenia->OCTREE_Depth - 2;
			SDF_control->DoSmoothing2(triangles, min, max);
			Nastavenia->OCTREE_Depth = Nastavenia->OCTREE_Depth + 2;
			Nastavenia->OCTREE_Threshold = tmp_threshold;
		}
		else
		{
			SDF_control->DoSmoothing(triangles, min, max);
		}

		CopySDF_Faces_to_Vertices();
	}
	unsigned char*** CModel::GetTexture()
	{
		float** texttur = SDF_control->GetTexture(triangles, true);

		unsigned char R, G, B;
		unsigned char*** result = new unsigned char**[Nastavenia->SDF_Smoothing_Texture];
		for(unsigned int i = 0; i < Nastavenia->SDF_Smoothing_Texture; i++)
		{
			result[i] = new unsigned char*[Nastavenia->SDF_Smoothing_Texture];
		}

		for(unsigned int i = 0; i < Nastavenia->SDF_Smoothing_Texture; i++)
		{
			for(unsigned int j = 0; j < Nastavenia->SDF_Smoothing_Texture; j++)
			{
				result[i][j] = new unsigned char[3];
				if(texttur[i][j] < 0.0f)
				{
					result[i][j][0] = 0;
					result[i][j][1] = 0;
					result[i][j][2] = 0;
				}
				else
				{
					R = 0; G = 0; B = 0;
					HLSToRGB(texttur[i][j], R, G, B);
					result[i][j][0] = R;
					result[i][j][1] = G;
					result[i][j][2] = B;
				}
			}
		}

		for(unsigned int i = 0; i < Nastavenia->SDF_Smoothing_Texture; i++)
		{
			delete [] texttur[i];
		}
		delete [] texttur;

		return result;
	}
	void CModel::NormalizeTextureCoords()
	{
		float min_u = 99999.0f;
		float max_u = -99999.0f;
		float min_v = 99999.0f;
		float max_v = -99999.0f;
		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		while(tmp != NULL)
		{
			if(tmp->data->texCoord_U < min_u)
				min_u = tmp->data->texCoord_U;
			if(tmp->data->texCoord_V < min_v)
				min_v = tmp->data->texCoord_V;

			if(tmp->data->texCoord_U > max_u)
				max_u = tmp->data->texCoord_U;
			if(tmp->data->texCoord_V > max_v)
				max_v = tmp->data->texCoord_V;

			tmp = tmp->next;
		}
		if(abs(max_u - min_u) < 0.0001f)
			return;
		if(abs(max_v - min_v) < 0.0001f)
			return;

		tmp = points->start;
		while(tmp != NULL)
		{
			tmp->data->texCoord_U = (tmp->data->texCoord_U - min_u) / (max_u - min_u);
			tmp->data->texCoord_V = (tmp->data->texCoord_V - min_v) / (max_v - min_v);

			tmp = tmp->next;
		}
	}

	void CModel::SmoothTexture()
	{
		bool normalized = false;
		float** textur = SDF_control->GetTexture(triangles, normalized);
		SDF_control->SmoothTexture(textur);
		//SDF_control->SmoothTexture(textur, triangles);
		/*SDF_control->ApplyTexture(triangles, textur, normalized);
		CopySDF_Faces_to_Vertices();*/
		SDF_control->ApplyTexture(points, textur, normalized);
		CopySDF_Vertices_to_Faces();

		for(unsigned int i = 0; i < Nastavenia->SDF_Smoothing_Texture; i++)
		{
			delete [] textur[i];
		}
		delete [] textur;
	}
}