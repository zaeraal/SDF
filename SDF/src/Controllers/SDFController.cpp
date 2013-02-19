// SDFController.h : subor pre vypocitanie SDF funkcie
#include "stdafx.h"
#include "SDFController.h"
#include "MathHelper.h"

namespace SDFController
{
	// konstruktor
	CSDFController::CSDFController()
	{
		min = 0.0;
		max = 0.0;
		nmin = 0.0;
		nmax = 0.0;
	}

	// destruktor
	CSDFController::~CSDFController()
	{

	}
		
	// pocitanie funkcie pre vsetky trojuholniky, O(n2)
	void CSDFController::ComputeForAllFaces(LinkedList<Face>* triangles)
	{
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{

		}

	}

	void CSDFController::ComputeForOctree(Octree* root)
	{

	}
}