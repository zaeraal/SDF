// Octree.cpp : subor obsahujuci zakladnu octree strukturu
#include "stdafx.h"
#include "Octree.h"

namespace Model
{
	Octree::Octree(int dep, double siz, Vector4* ori, Octree* par)
	{
		depth = dep;
		size = siz;
		origin = ori;
		parent = par;

		for(int i = 0; i < 8; i++)
		{
			son[i] = NULL;
		}
	}

	Octree::~Octree()
	{
		parent = NULL;
		delete origin;
		origin = NULL;
		for(int i = 0; i < 8; i++)
		{
			if(son[i] != NULL)
				delete son[i];
			son[i] = NULL;
		}
		triangles.clear();
	}

	void Octree::Build(vector<Face*> tria)
	{
		if((depth == max_depth) || (tria.size() <= min_count))
		{
			for(unsigned int i = 0; i < tria.size(); i++)
			{
				triangles.push_back(tria[i]);
			}
		}
		else
		{
			int new_depth = depth + 1;
			double new_size = size / 2.0;
			vector<Face*> son_tria[8];


			// zisti kam patria trojuholniky
			for(unsigned int i = 0; i < tria.size(); i++)
			{
				int code1 = GetCode(tria[i]->v[0]->P);
				int code2 = GetCode(tria[i]->v[0]->P);
				int code3 = GetCode(tria[i]->v[0]->P);
				int code = 8;

				if(code1 == code2 == code3)
					code = code1;

				if(code != 8)
					son_tria[code].push_back(tria[i]);
				else
					triangles.push_back(tria[i]);
			}

			// tabulka offsetov
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
            };

			// vytvor octree a uloz ich tam
			for(int i = 0; i < 8; i++)
			{
				if(son_tria[i].size() > 0)
				{
					son[i] = new Octree(new_depth,
										new_size,
										new Vector4(origin->X + new_size*Table[i][0],
													origin->Y + new_size*Table[i][1],
													origin->Z + new_size*Table[i][2]),
										this);
					son[i]->Build(son_tria[i]);
				}
			}
		}
	}

	// vrati poziciu v octree
	int Octree::GetCode(Vector4* pt)
	{
		int result = 0;
		if (pt->X > origin->X) result |= 1;
		if (pt->Y > origin->Y) result |= 2;
		if (pt->Z > origin->Y) result |= 4;

		return result;
	}

}