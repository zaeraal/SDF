// Octree.cpp : subor obsahujuci zakladnu octree strukturu
#include "stdafx.h"
#include "Octree.h"

/*		int **ptr;
		ptr = new int*[n_faces];*/

namespace MeshStructures
{
	Octree::Octree(const int dep, const double siz, Vector4 ori, Octree* par)
	{
		isLeaf = true;
		depth = dep;
		size = siz;
		origin = ori;
		parent = par;
		triangles = NULL;
		count = 0;

		son = new Octree* [8];
	}

	Octree::~Octree()
	{
		if(count > 0)
			delete [] triangles;

		if(!isLeaf)
		{
			for(int i = 0; i < 8; i++)
			{
					delete son[i];
			}
		}
		delete [] son;
	}

	void Octree::Build(Face** tria, unsigned int length)
	{
		if(length == 0)
			return;

		if((depth >= max_depth) || (length <= min_count))
		{
			count = length;
			triangles = tria;
		}
		else
		{
			int new_depth = depth + 1;
			double new_size = size / 2.0;

			LinkedList<Face>** son_tria = new LinkedList<Face>* [9];
			for(unsigned int i = 0; i < 9; i++)
				son_tria[i] = NULL; // preistotu

			// zisti kam patria trojuholniky
			for(unsigned int i = 0; i < length; i++)
			{
				int code1 = GetCode(tria[i]->v[0]->P);
				int code2 = GetCode(tria[i]->v[1]->P);
				int code3 = GetCode(tria[i]->v[2]->P);
				int code = 8;

				if((code1 == code2) && (code1 == code3))
				{
					isLeaf = false;
					code = code1;
				}

				if(son_tria[code] == NULL)
					son_tria[code] = new LinkedList<Face>(tria[i]);
				else
					son_tria[code]->InsertToEnd(tria[i]);

			}
			/*
			// old tabulka offsetov
			double Table[8][3] =
            {
                {-1.0, -1.0, -1.0},
                {+1.0, -1.0, -1.0},
                {-1.0, +1.0, -1.0},
                {+1.0, +1.0, -1.0},
                {-1.0, -1.0, +1.0},
                {+1.0, -1.0, +1.0},
                {-1.0, +1.0, +1.0},
                {+1.0, +1.0, +1.0}
            };*/
			// tabulka offsetov
			double Table[8][3] =
            {
                {-1.0, -1.0, +1.0},
                {-1.0, -1.0, -1.0},
                {-1.0, +1.0, +1.0},
                {-1.0, +1.0, -1.0},
                {+1.0, -1.0, +1.0},
                {+1.0, -1.0, -1.0},
                {+1.0, +1.0, +1.0},
                {+1.0, +1.0, -1.0}
            };

			count = (son_tria[8] == NULL ? 0 : son_tria[8]->GetSize());
			if(count > 0)
			{
				triangles = new Face* [count];

				LinkedList<Face>::Cell<Face>* tmp = son_tria[8]->start;
				for(unsigned int i = 0; i < count; i++)
				{
					triangles[i] = tmp->data;
					tmp = tmp->next;
				}
			}

			if(isLeaf != true)
			{
				// vytvor octree a uloz ich tam
				for(int i = 0; i < 8; i++)
				{
					Vector4 tmpv = Vector4(origin.X + new_size*Table[i][0],
										   origin.Y + new_size*Table[i][1],
										   origin.Z + new_size*Table[i][2]);
					son[i] = new Octree(new_depth, new_size, tmpv, this);

					unsigned int velkost = (son_tria[i] == NULL ? 0 : son_tria[i]->GetSize());
					Face** tmp_zoznam = NULL;
					if(velkost > 0)
					{
						tmp_zoznam = new Face* [velkost];

						LinkedList<Face>::Cell<Face>* tmp = son_tria[i]->start;
						for(unsigned int j = 0; j < velkost; j++)
						{
							tmp_zoznam[j] = tmp->data;
							tmp = tmp->next;
						}
					}
					son[i]->Build(tmp_zoznam, velkost);
				}
			}

			// zmazanie pola, kedze vytvaram vlastne
			delete [] tria;

			// zmazanie pomocnych poli kedze som urobil vlastne
			for(unsigned int i = 0; i < 9; i++)
			{
				delete son_tria[i];
				son_tria[i] = NULL;		// preistotu
			}
			delete [] son_tria;
		}
	}

	// vrati poziciu v octree
	int Octree::GetCode(const Vector4 pt)
	{
		int result = 0;
		if (pt.X > origin.X) result |= 4;
		if (pt.Y > origin.Y) result |= 2;
		if (pt.Z <= origin.Z) result |= 1;

		return result;
	}

	void Octree::GetBoundary(double &siz, double &x, double &y, double &z)
	{
		siz = size;
		x = origin.X;
		y = origin.Y;
		z = origin.Z;
	}

	void Octree::DrawOctree()
	{
		glBegin(GL_LINES);
			double Table[8][3] =
            {
                {-1.0, -1.0, +1.0},
                {-1.0, -1.0, -1.0},
                {-1.0, +1.0, +1.0},
                {-1.0, +1.0, -1.0},
                {+1.0, -1.0, +1.0},
                {+1.0, -1.0, -1.0},
                {+1.0, +1.0, +1.0},
                {+1.0, +1.0, -1.0}
            };
			glVertex3d(origin.X + size*Table[0][0], origin.Y + size*Table[0][1], origin.Z + size*Table[0][2]);
			glVertex3d(origin.X + size*Table[1][0], origin.Y + size*Table[1][1], origin.Z + size*Table[1][2]);

			glVertex3d(origin.X + size*Table[0][0], origin.Y + size*Table[0][1], origin.Z + size*Table[0][2]);
			glVertex3d(origin.X + size*Table[2][0], origin.Y + size*Table[2][1], origin.Z + size*Table[2][2]);

			glVertex3d(origin.X + size*Table[0][0], origin.Y + size*Table[0][1], origin.Z + size*Table[0][2]);
			glVertex3d(origin.X + size*Table[4][0], origin.Y + size*Table[4][1], origin.Z + size*Table[4][2]);

			glVertex3d(origin.X + size*Table[3][0], origin.Y + size*Table[3][1], origin.Z + size*Table[3][2]);
			glVertex3d(origin.X + size*Table[7][0], origin.Y + size*Table[7][1], origin.Z + size*Table[7][2]);
			
			glVertex3d(origin.X + size*Table[3][0], origin.Y + size*Table[3][1], origin.Z + size*Table[3][2]);
			glVertex3d(origin.X + size*Table[2][0], origin.Y + size*Table[2][1], origin.Z + size*Table[2][2]);

			glVertex3d(origin.X + size*Table[3][0], origin.Y + size*Table[3][1], origin.Z + size*Table[3][2]);
			glVertex3d(origin.X + size*Table[1][0], origin.Y + size*Table[1][1], origin.Z + size*Table[1][2]);

			glVertex3d(origin.X + size*Table[6][0], origin.Y + size*Table[6][1], origin.Z + size*Table[6][2]);
			glVertex3d(origin.X + size*Table[7][0], origin.Y + size*Table[7][1], origin.Z + size*Table[7][2]);

			glVertex3d(origin.X + size*Table[6][0], origin.Y + size*Table[6][1], origin.Z + size*Table[6][2]);
			glVertex3d(origin.X + size*Table[2][0], origin.Y + size*Table[2][1], origin.Z + size*Table[2][2]);

			glVertex3d(origin.X + size*Table[6][0], origin.Y + size*Table[6][1], origin.Z + size*Table[6][2]);
			glVertex3d(origin.X + size*Table[4][0], origin.Y + size*Table[4][1], origin.Z + size*Table[4][2]);

			glVertex3d(origin.X + size*Table[5][0], origin.Y + size*Table[5][1], origin.Z + size*Table[5][2]);
			glVertex3d(origin.X + size*Table[7][0], origin.Y + size*Table[7][1], origin.Z + size*Table[7][2]);

			glVertex3d(origin.X + size*Table[5][0], origin.Y + size*Table[5][1], origin.Z + size*Table[5][2]);
			glVertex3d(origin.X + size*Table[1][0], origin.Y + size*Table[1][1], origin.Z + size*Table[1][2]);

			glVertex3d(origin.X + size*Table[5][0], origin.Y + size*Table[5][1], origin.Z + size*Table[5][2]);
			glVertex3d(origin.X + size*Table[4][0], origin.Y + size*Table[4][1], origin.Z + size*Table[4][2]);
		glEnd();

		if(isLeaf != true)
		{
			for(int i = 0; i < 8; i++)
			{
				son[i]->DrawOctree();
			}
		}
	}
}