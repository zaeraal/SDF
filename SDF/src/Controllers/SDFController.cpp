// SDFController.h : subor pre vypocitanie SDF funkcie
#include "stdafx.h"
#include "SDFController.h"
#include "MathHelper.h"

namespace SDFController
{
	// konstruktor
	CSDFController::CSDFController()
	{
		ResetValues();
	}

	// destruktor
	CSDFController::~CSDFController()
	{

	}
		
	// pocitanie funkcie pre vsetky trojuholniky, O(n2)
	void CSDFController::ComputeForAllFaces(LinkedList<Face>* triangles, CAssimp* loggger)
	{
		ResetValues();
		srand (2013);					// initial seed for random number generator
		double n_rays = 30.0;
		double angle = 20.0;
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			// compute tanget space matrix
			Vector4 U = Vector4(tmp->data->v[1]->P - tmp->data->v[0]->P);
			Vector4 V = Vector4(tmp->data->v[2]->P - tmp->data->v[0]->P);
			Vector4 normal = (U % V) * (-1.0);
			normal.Normalize();

			Vector4 tangens = Vector4(tmp->data->v[0]->P - tmp->data->v[2]->P);
			tangens.Normalize();
			Vector4 binormal = tangens % normal;
			binormal.Normalize();
			
			Mat4 t_mat= Mat4(tangens, binormal, normal);
			std::vector<double> rays;
			std::vector<double> weights;
			for(int i = 0; i < n_rays; i++)
			{
				double rndy = rand()%int(angle / 2);
				double rndx = rand()%(360);
				if(rndy == 180.0)
					rndy = 179.5;
				//Vector4 ray = (t_mat * CalcRayFromAngle(rndx, rndy));
				Vector4 ray = (CalcRayFromAngle(rndx, rndy) * t_mat);
				ray.Normalize();
				/*double rndy = 179;
				Vector4 ray = normal;*/
				double dist = 99999.0;
				LinkedList<Face>::Cell<Face>* tmp2 = triangles->start;
				while(tmp2 != NULL)
				{
					if(tmp == tmp2)
					{
						tmp2 = tmp2->next;
						continue;
					}

					double dist2 = 99999.0;
					bool intersected = rayIntersectsTriangle(tmp->data->center, ray, tmp2->data->v[0]->P, tmp2->data->v[1]->P, tmp2->data->v[2]->P, dist2);
					if((intersected == true) && (dist2 < dist))
						dist = dist2;

					tmp2 = tmp2->next;
				}
				if(dist < 99998)
				{
					//loggger->logInfo(MarshalString("pridany ray s dlzkou: " + dist));
					rays.push_back(dist);
					weights.push_back(180.0 - rndy);
				}
			}
			if(rays.size() > 0)
			{
				tmp->data->ComputeSDFValue(rays, weights);
				if(tmp->data->diameter->value < min)
					min = tmp->data->diameter->value;
				if(tmp->data->diameter->value > max)
					max = tmp->data->diameter->value;
			}

			tmp = tmp->next;
		}
		tmp = triangles->start;
		while(tmp != NULL)
		{
			tmp->data->diameter->Normalize(min, max, 4.0);
			if(tmp->data->diameter->normalized < nmin)
				nmin = tmp->data->diameter->normalized;
			if(tmp->data->diameter->normalized > nmax)
				nmax = tmp->data->diameter->normalized;
			tmp = tmp->next;
		}
		loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));
	}

	void CSDFController::ComputeForOctree(Octree* root)
	{

	}

	void CSDFController::ResetValues()
	{
		min = 99999.0;
		max = 0.0;
		nmin = 99999.0;
		nmax = 0.0;
	}
}