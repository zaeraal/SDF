// Model.cpp : subor pre kontrolu modelov
#include "stdafx.h"
#include "Model.h"

namespace ModelController
{
	CModel::CModel()
	{
		Assimp = new CAssimp();
		SDF_control = NULL;
		m_root = NULL;
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

		Assimp->Import3DFromFile(Filename);
		Assimp->LoadData(triangles, points);
		loaded = true;

		ComputeBoundary();
		CreateOctree();
		SetColors();
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

		ComputeBoundary();
		CreateOctree();
		SetColors();
	}

	/*double CModel::GetSDF(const struct aiFace* face, bool smoothed)
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

	double* CModel::GetSDF(int& size, bool smoothed)
	{
		LinkedList<Vertex>::Cell<Vertex>* tmp = points->start;
		size = points->GetSize();
		double* values = new double[size];
		for(int i = 0; i < size; i++)
		{
			double hodnota = 0;
			double total = tmp->data->susedia->GetSize();
			LinkedList<void>::Cell<void>* tm = tmp->data->susedia->start;
			while(tm != NULL)
			{
				if(smoothed)
					hodnota += ((Face*)tm->data)->diameter->smoothed;
				else
					hodnota += ((Face*)tm->data)->diameter->value;
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

	// vypocita rozmery modelu
	void CModel::ComputeBoundary()
	{
		double minx = 99999.0, miny = 99999.0, minz = 99999.0;
		double maxx = -99999.0, maxy = -99999.0, maxz = -99999.0;

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

		b_stred = Vector4((minx+maxx) / 2.0, (miny+maxy) / 2.0, (minz+maxz) / 2.0);
		double sizex = 0;
		double sizey = 0;
		double sizez = 0;

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
		b_size = max(max(sizex, sizey), sizez);

		b_sf = b_size / 10.0;											// 1 / 10 velkosti modelu budu tie vektory
		b_max = sqrt(3.0) * b_size;										// diagonala kocky
		b_size = b_size / 2.0;

		SDF_control = new CSDFController(b_max, Assimp);
	}

	// vytvori Octree strukturu
	void CModel::CreateOctree()
	{
		m_root = new Octree(0, b_size, b_stred, NULL);

		unsigned int siz = triangles->GetSize();
		if(siz > 0)
		{
			Face** tria = new Face* [siz];
			LinkedList<Face>::Cell<Face>* tmp2 = triangles->start;
			int i = 0;
			while(tmp2 != NULL)
			{
				tria[i] = tmp2->data;
				tmp2 = tmp2->next;
				i++;
			}
			m_root->Build(tria, siz);
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

	// pre spravne vycentrovanie pohladu ked sa nacita CModel
	void CModel::GetBoundary(double &siz, double &x, double &y, double &z)
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
					HLSToRGB(tmp->data->diameter->normalized2, r, g, b);
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

			/*glColor3f(0.5f,0.5f,1.0f);
			glBegin(GL_LINES);
				for(int i = 0; i < 30; i++)
				{
					double rndy = rand()%int(120 / 2);
					double rndx = rand()%(360);
					if(rndy == 180.0)
						rndy = 179.5;
					Vector4 ray = (CalcRayFromAngle(rndx, rndy) * t_mat);
					//Vector4 ray = CalcRayFromAngle(rndx, rndy);
					ray.Normalize();// ray = ray *  b_sf;
					glVertex3d(selected->center.X, selected->center.Y, selected->center.Z);
					glVertex3d( ray.X *  b_sf + selected->center.X,
								ray.Y *  b_sf + selected->center.Y,
								ray.Z *  b_sf + selected->center.Z);
				}
			glEnd();*/

			Vector4 Center = selected->center;// - (normal * b_max);
			double o_size = 0.0;
			double o_X = 0.0;
			double o_Y = 0.0;
			double o_Z = 0.0;
			m_root->GetBoundary(o_size, o_X, o_Y, o_Z);

			// fixes for rays with negative direction
			/*if(normal.X < 0)
			{
				Center.X = (o_size) - Center.X;
				normal.X = - normal.X;
			}
			if(normal.Y < 0)
			{
				Center.Y = (o_size) - Center.Y;
				normal.Y = - normal.Y;
			}
			if(normal.Z < 0)
			{
				Center.Z = (o_size) - Center.Z;
				normal.Z = - normal.Z;
			}*/

			double divx = 1 / normal.X; // IEEE stability fix
			double divy = 1 / normal.Y;
			double divz = 1 / normal.Z;

			double tx0 = ((o_X - o_size) - Center.X) * divx;
			double tx1 = ((o_X + o_size) - Center.X) * divx;
			double ty0 = ((o_Y - o_size) - Center.Y) * divy;
			double ty1 = ((o_Y + o_size) - Center.Y) * divy;
			double tz0 = ((o_Z - o_size) - Center.Z) * divz;
			double tz1 = ((o_Z + o_size) - Center.Z) * divz;

			/*double tx0 = -Center.X * divx;
			double tx1 = (o_size - Center.X) * divx;
 
			double ty0 = -Center.Y * divy;
			double ty1 = (o_size - Center.Y) * divy;

			double tz0 = -Center.Z * divz;
			double tz1 = (o_size - Center.Z) * divz;*/

			if (normal.X == 0.0f)
			{
					if (tx0 < 0) tx0 = -9.0;
					else tx0 = 9.0;
					if (tx1 < 0) tx1 = -9.0;
					else tx1 = 9.0;
			}
			if (normal.Y == 0.0f)
			{
					if (ty0 < 0) ty0 = -9.0;
					else ty0 = 9.0;
					if (ty1 < 0) ty1 = -9.0;
					else ty1 = 9.0;
			}
			if (normal.Z == 0.0f)
			{
					if (tz0 < 0) tz0 = -9.0;
					else tz0 = 9.0;
					if (tz1 < 0) tz1 = -9.0;
					else tz1 = 9.0;
			}

			/*glBegin(GL_LINES);
				glVertex3d(tx0, ty0, tz0);
				glVertex3d(tx1, ty1, tz1);
			glEnd();*/
			Vector4 norm = (U % V) * (-1.0);
			norm.Normalize();
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

	void CModel::HLSToRGB(double SDF_value, GLubyte &R, GLubyte &G, GLubyte &B)
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

	int CModel::GetTriangleCount()
	{
		return triangles->GetSize();
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
		SDF_control->Compute(triangles, m_root);
	}
}