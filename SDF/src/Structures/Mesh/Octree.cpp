// Octree.cpp : subor obsahujuci zakladnu octree strukturu
#include "stdafx.h"
#include "Octree.h"
#include "MathHelper.h"

namespace MeshStructures
{
	enum Flags
	{
		FLAG0 = 0x0,
		FLAG1 = 0x1,
		FLAG2 = 0x2,
		FLAG4 = 0x4,
		FLAG8 = 0x8
	};
	Octree::Octree(const int dep, const float siz, Vector4 ori, Octree* par)
	{
		// Front half			Back half (viewed from front)
		// -----------------	-----------------
		// |       |       |	|       |       |
		// |   2   |   6   |	|   3   |   7   |
		// |       |       |	|       |       |
		// -----------------	-----------------
		// |       |       |	|       |       |
		// |   0   |   4   |	|   1   |   5   |
		// |       |       |	|       |       |
		// -----------------	-----------------
		// x smeruje doprava, y smeruje hore, z ODOMNA!! - bacha opengl ma Z inak
		// tabulka offsetov
		float Table2[8][3] =
        {
            {-1.0, -1.0, -1.0},
            {-1.0, -1.0, +1.0},
            {-1.0, +1.0, -1.0},
            {-1.0, +1.0, +1.0},
            {+1.0, -1.0, -1.0},
            {+1.0, -1.0, +1.0},
            {+1.0, +1.0, -1.0},
            {+1.0, +1.0, +1.0}
        };
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 3; j++)
				Table[i][j] = Table2[i][j];

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
			isLeaf = true;
		}
		else
		{
			isLeaf = false;
			count = 0;
			int new_depth = depth + 1;
			float new_size = size / 2.0f;

			LinkedList<Face>** son_tria = new LinkedList<Face>* [8];
			for(unsigned int i = 0; i < 8; i++)
				son_tria[i] = NULL; // preistotu

			// zisti kam patria trojuholniky
			for(unsigned int i = 0; i < length; i++)
			{
				byte code1 = GetCode(tria[i]->v[0]->P);
				byte code2 = GetCode(tria[i]->v[1]->P);
				byte code3 = GetCode(tria[i]->v[2]->P);
				byte code = FLAG8;

				if((code1 == code2) && (code1 == code3))
				{
					code = code1;
					if(son_tria[code] == NULL)
						son_tria[code] = new LinkedList<Face>(tria[i]);
					else
						son_tria[code]->InsertToEnd(tria[i]);
				}
				else
				{
					// moc neurychli
					/*bool crosses [8] = {0,0,0,0,0,0,0,0};
					bool crosses1 [8] = {0,0,0,0,0,0,0,0};
					bool crosses2 [8] = {0,0,0,0,0,0,0,0};
					bool crosses3 [8] = {0,0,0,0,0,0,0,0};

					if(!(code1 & FLAG1) || !(code2 & FLAG1) || !(code3 & FLAG1))
						{ crosses1[0] = true; crosses1[2] = true; crosses1[4] = true; crosses1[6] = true; }
					if((code1 & FLAG1) || (code2 & FLAG1) || (code3 & FLAG1))
						{ crosses1[1] = true; crosses1[3] = true; crosses1[5] = true; crosses1[7] = true; }
					if(!(code1 & FLAG2) || !(code2 & FLAG2) || !(code3 & FLAG2))
						{ crosses2[0] = true; crosses2[4] = true; crosses2[1] = true; crosses2[5] = true; }
					if((code1 & FLAG2) || (code2 & FLAG2) || (code3 & FLAG2))
						{ crosses2[2] = true; crosses2[6] = true; crosses2[3] = true; crosses2[7] = true; }
					if(!(code1 & FLAG4) || !(code2 & FLAG4) || !(code3 & FLAG4))
						{ crosses3[0] = true; crosses3[1] = true; crosses3[2] = true; crosses3[3] = true; }
					if((code1 & FLAG4) || (code2 & FLAG4) || (code3 & FLAG4))
						{ crosses3[4] = true; crosses3[5] = true; crosses3[6] = true; crosses3[7] = true; }
					for(byte j = 0; j < 8; j++)
					{
						if((crosses1[j] == true) && (crosses2[j] == true) && (crosses3[j] == true) )
							crosses[j] = true;
					}*/
					for(byte j = 0; j < 8; j++)
					{
						/*if(crosses[j] == false)
							continue;*/
						Vector4 tmpv = Vector4(origin.X + new_size*Table[j][0],
											   origin.Y + new_size*Table[j][1],
											   origin.Z + new_size*Table[j][2]);
						if(triBoxOverlap(tmpv, new_size, tria[i]->v[0]->P, tria[i]->v[1]->P, tria[i]->v[2]->P))
						{
							code = j;
							if(son_tria[code] == NULL)
								son_tria[code] = new LinkedList<Face>(tria[i]);
							else
								son_tria[code]->InsertToEnd(tria[i]);
						}
					}
				}
			}

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

			// zmazanie pola, kedze vytvaram vlastne
			delete [] tria;

			// zmazanie pomocnych poli kedze som urobil vlastne
			for(unsigned int i = 0; i < 8; i++)
			{
				delete son_tria[i];
				son_tria[i] = NULL;		// preistotu
			}
			delete [] son_tria;
		}
	}

	void Octree::Build2(Face** tria, unsigned int* mtria, unsigned int start, unsigned int length)
	{
		/*//debug kktina
		unsigned int u [500];
		for(unsigned int ii=0; ii<500;ii++)
			u[ii] = tria[ii]->number;
		if(u[0] == 1000)
			return;*/

		if(length == 0)
			return;

		if((depth >= max_depth) || (length <= min_count))
		{
			count = length;
			triangles = new Face* [length];
			for(unsigned int i = start; i < start+length; i++)
				triangles[i - start] = tria[i];
			isLeaf = true;
		}
		else
		{
			isLeaf = false;
			count = 0;
			int new_depth = depth + 1;
			float new_size = size / 2.0f;

			int tabulka[8][2];			// start, size
			for(unsigned int i = 0; i < 8; i++)
			{
				tabulka[i][0] = start;
				tabulka[i][1] = 0;
			}
			// zisti kam patria trojuholniky
			unsigned int cislo = (unsigned int)pow(2, depth);
			for(unsigned int i = start; i < start+length; i++)
			{
				byte code1 = GetCode(tria[i]->v[0]->P);
				byte code2 = GetCode(tria[i]->v[1]->P);
				byte code3 = GetCode(tria[i]->v[2]->P);
				byte code = FLAG8;

				if((code1 == code2) && (code1 == code3))
				{
					code = code1;
					mtria[i] &= ~cislo;
					InserToEnd(tria, mtria, i, code, tabulka);
				}
				else
				{
					for(byte j = 0; j < 8; j++)
					{
						Vector4 tmpv = Vector4(origin.X + new_size*Table[j][0],
											   origin.Y + new_size*Table[j][1],
											   origin.Z + new_size*Table[j][2]);
						if(triBoxOverlap(tmpv, new_size, tria[i]->v[0]->P, tria[i]->v[1]->P, tria[i]->v[2]->P))
						{
							code = j;
							mtria[i] |= cislo;
							InserToEnd(tria, mtria, i, code, tabulka);
							break;
						}
					}
					if(code == FLAG8)
						InserToEnd(tria, mtria, i, 0, tabulka);
				}
			}

			// vytvor octree a uloz ich tam
			for(unsigned int i = 0; i < 8; i++)
			{
				Vector4 tmpv = Vector4(origin.X + new_size*Table[i][0],
									   origin.Y + new_size*Table[i][1],
									   origin.Z + new_size*Table[i][2]);
				son[i] = new Octree(new_depth, new_size, tmpv, this);
				if(i > 0)
				{
					Check(tria, mtria, i, tabulka, new_size, cislo);
				}

				son[i]->Build2(tria, mtria, tabulka[i][0], tabulka[i][1]);
			}
		}
	}

	void Octree::InserToEnd(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2])
	{
		bool added = tabulka[code][1] > 0 ? false : true;

		Face* ftmp = tria[tabulka[code][0]];
		unsigned int itmp = mtria[tabulka[code][0]];

		tria[tabulka[code][0]] = tria[idx];
		mtria[tabulka[code][0]] = mtria[idx];

		tria[idx] = ftmp;
		mtria[idx] = itmp;

		tabulka[code][1] += 1;
		FwdMove(tria, mtria, idx, code+1, tabulka, added);
	}

	void Octree::InserToStart(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2])
	{
		if(tabulka[code][0] == idx)
			return;

		tabulka[code][1] += 1;
		tabulka[code][0] -= 1;

		Face* ftmp = tria[tabulka[code][0]];
		unsigned int itmp = mtria[tabulka[code][0]];

		tria[tabulka[code][0]] = tria[idx];
		mtria[tabulka[code][0]] = mtria[idx];

		tria[idx] = ftmp;
		mtria[idx] = itmp;

		// je to ok, kedze nekontrolujem znova
		tabulka[code-1][1] -= 1;
		//BwdMove(tria, mtria, idx, code+1, tabulka);
	}

	void Octree::FwdMove(Face** tria, unsigned int* mtria, unsigned int idx, byte code, int (&tabulka)[8][2], bool added)
	{
		if(code >= 8)
			return;
		if(added == false)
		{
			if(tabulka[code][1] > 0)
			{
				Face* ftmp = tria[tabulka[code][0]];
				unsigned int itmp = mtria[tabulka[code][0]];

				tria[tabulka[code][0]] = tria[idx];
				mtria[tabulka[code][0]] = mtria[idx];

				tria[idx] = ftmp;
				mtria[idx] = itmp;
			}
			tabulka[code][0] += 1;
		}
		else
		{
			if(tabulka[code][1] > 0)
			{
				Face* ftmp = tria[tabulka[code][0] + tabulka[code][1]];
				unsigned int itmp = mtria[tabulka[code][0] + tabulka[code][1]];

				tria[tabulka[code][0] + tabulka[code][1]] = tria[idx];
				mtria[tabulka[code][0] + tabulka[code][1]] = mtria[idx];

				tria[idx] = ftmp;
				mtria[idx] = itmp;
			}
			tabulka[code][0] += 1;
		}
		FwdMove(tria, mtria, idx, code+1, tabulka, added);
	}

	void Octree::Check(Face** tria, unsigned int* mtria, byte code, int (&tabulka)[8][2], float new_size, unsigned int cislo)
	{
		int length = 0;
		for(unsigned int j = 0; j < code; j++)
		{
			length += tabulka[j][1];
		}

		// zisti kam patria trojuholniky
		int idx = tabulka[0][0];
		int koniec = tabulka[0][0]+length;
		while(idx < koniec)
		{
			// musime skontrolovat
			if(mtria[idx] & cislo)
			{
				Vector4 tmpv = Vector4(origin.X + new_size*Table[code][0],
										origin.Y + new_size*Table[code][1],
										origin.Z + new_size*Table[code][2]);
				if(triBoxOverlap(tmpv, new_size, tria[idx]->v[0]->P, tria[idx]->v[1]->P, tria[idx]->v[2]->P))
				{
					InserToStart(tria, mtria, idx, code, tabulka);
					idx--;
					koniec--;
				}
			}
			idx++;
		}
	}

	// vrati poziciu v octree
	byte Octree::GetCode(const Vector4 pt)
	{
		byte result = FLAG0;
		if (pt.X > origin.X) result |= FLAG4;
		if (pt.Y > origin.Y) result |= FLAG2;
		if (pt.Z > origin.Z) result |= FLAG1;

		return result;
	}

	void Octree::GetBoundary(float &siz, float &x, float &y, float &z)
	{
		siz = size;
		x = origin.X;
		y = origin.Y;
		z = origin.Z;
	}

	void Octree::DrawOctree(bool recursive)
	{
		if((recursive == true) && (depth > 5))
			return;

		glBegin(GL_LINES);
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

		if((isLeaf == false) && (recursive == true))
		{
			for(int i = 0; i < 8; i++)
			{
				son[i]->DrawOctree(recursive);
			}
		}
	}
}