// SDFController.h : subor pre vypocitanie SDF funkcie
#include "stdafx.h"
#include "SDFController.h"
#include "MathHelper.h"

#define DOUBLE_MAX  99999.0
#define FLD_NODE 0
#define BLD_NODE 1
#define FLT_NODE 2
#define BLT_NODE 3
#define FRD_NODE 4
#define BRD_NODE 5
#define FRT_NODE 6
#define BRT_NODE 7

namespace SDFController
{
	// konstruktor
	CSDFController::CSDFController(double dia, CAssimp* logg)
	{
		diagonal = dia;
		loggger = logg;
		//loggger->logInfo(MarshalString("diagonal: " + diagonal));
	}

	// destruktor
	CSDFController::~CSDFController()
	{

	}
		
	// pocitanie funkcie pre vsetky trojuholniky, O(n2)
	void CSDFController::Compute(LinkedList<Face>* triangles, Octree* root)
	{
		double min = DOUBLE_MAX;
		double max = 0.0;
		srand (2013);					// initial seed for random number generator
		double n_rays = 30.0;
		double angle = 120.0;
		int counter = 0;
		LinkedList<Face>::Cell<Face>* tmp = triangles->start;
		while(tmp != NULL)
		{
			// vypocet TNB vektorov a matice
			Vector4 tangens, normal, binormal;
			ComputeTNB(tmp->data, tangens, normal, binormal);
			Mat4 t_mat= Mat4(tangens, normal, binormal);

			std::vector<double> rays;
			std::vector<double> weights;
			for(int i = 0; i < n_rays; i++)
			{
				double rndy = rand()%int(angle / 2);
				double rndx = rand()%(360);
				if(rndy == 0.0)
					rndy = 0.5;

				Vector4 ray = CalcRayFromAngle(rndx, rndy) * t_mat;
				ray.Normalize();

				double dist = DOUBLE_MAX;
				LinkedList<Face>* tmpp = GetFaceList(triangles, root, tmp->data->center, ray);
				LinkedList<Face>::Cell<Face>* tmp2 = tmpp->start;
				while(tmp2 != NULL)
				{
					if(tmp == tmp2)
					{
						tmp2 = tmp2->next;
						continue;
					}

					double dist2 = DOUBLE_MAX;
					bool intersected = rayIntersectsTriangle(tmp->data->center, ray, tmp2->data->v[0]->P, tmp2->data->v[1]->P, tmp2->data->v[2]->P, dist2);
					if(intersected == true)
					{
						double theta = acos( (ray * (tmp->data->normal * (-1))) / (ray.Length() * tmp->data->normal.Length()) );
						theta = theta * (180.0 / M_PI);
						//loggger->logInfo(MarshalString("pridany ray s thetou: " + theta));
						if((theta < 90.0) && (dist2 < dist))
							dist = dist2;
					}

					tmp2 = tmp2->next;
				}
				if(dist < (DOUBLE_MAX - 1.0))
				{
					//loggger->logInfo(MarshalString("pridany ray s dlzkou: " + dist));
					rays.push_back(dist);
					weights.push_back(180.0 - rndy);
				}
				if(root != NULL)
					delete tmpp;						// generated list
			}
			if(rays.size() > 0)
			{
				tmp->data->ComputeSDFValue(rays, weights);
				if(tmp->data->diameter->value < min)
					min = tmp->data->diameter->value;
				if(tmp->data->diameter->value > max)
					max = tmp->data->diameter->value;
			}
			counter++;
			tmp = tmp->next;
		}
		// postprocessing - smoothing and normalization
		int kernel_size = 2;
		//double kernel[] = {1.0,4.0,6.0,4.0,1.0};
		double* kernel = ComputeGaussianKernel(kernel_size);
		tmp = triangles->start;
		while(tmp != NULL)
		{
			Smooth(tmp->data, kernel, kernel_size);
			tmp->data->diameter->Normalize1(min, max, 4.0);
			tmp->data->diameter->Normalize2(0, max, 4.0);
			//tmp->data->diameter->Normalize2(0, diagonal, 4.0);

			tmp = tmp->next;
		}
		delete kernel;
		//loggger->logInfo(MarshalString("pocet: " + counter));
		//loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		//loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));
	}

	// vypocitaj normalizovany 1D kernel pre gaussian
	double* CSDFController::ComputeGaussianKernel(int radius)
	{
		double* matrix = new double [radius*2+1];
		double sigma = (double)radius/2.0;
		double norm = 1.0 / (sqrt(2*M_PI) * sigma);
		double coeff = 2*sigma*sigma;
		double total=0;
		for(int x = -radius; x <= radius; x++)
		{
			double g = norm * exp( (-x*x)/coeff );
			matrix[x+radius] = g;
			total+=g;
		}
		for(int x=0; x<=2*radius; x++)
			matrix[x]=(matrix[x]/total) * 1000.0;

		return matrix;
	}

	void CSDFController::Smooth(Face* tmp, double* kernel, int kernel_size)
	{
		LinkedList<Face>** sus = new LinkedList<Face>*[kernel_size + 1];
		sus[0] = new LinkedList<Face>(tmp);
		for(int i=1; i <= kernel_size; i++)				// bacha na posunutie
		{
			sus[i] = new LinkedList<Face>();
			LinkedList<Face>::Cell<Face>* tm = sus[i-1]->start;
			while(tm != NULL)
			{
				LinkedList<Face>* t = tm->data->GetSusedia();
				LinkedList<Face>::Cell<Face>* tc = t->start;
				while(tc != NULL)
				{
					bool pokracuj = false;
					for(int j = 0; j <= i; j++)
					{
						if(sus[j]->Contains(tc->data))
						{
							pokracuj = true;
							break;
						}
					}
					if(pokracuj)
					{
						tc = tc->next;
						continue;
					}
					sus[i]->InsertToEnd(tc->data);
					tc = tc->next;
				}
				delete t;
				tm = tm->next;
			}
		}
		std::vector<double> _values;
		std::vector<double> _weights;

		for(int i=0; i <= kernel_size; i++)
		{
			int _size = sus[i]->GetSize();
			if(_size != 0)
			{
				double _weight = kernel[kernel_size + i];// / (double)_size;
				LinkedList<Face>::Cell<Face>* tc = sus[i]->start;
				while(tc != NULL)
				{
					_values.push_back(tc->data->diameter->value);
					_weights.push_back(_weight);
					tc = tc->next;
				}
			}
			delete sus[i];
		}
		delete [] sus;

		tmp->diameter->Smooth(_values, _weights);
	}

	LinkedList<Face>* CSDFController::GetFaceList(LinkedList<Face>* triangles, Octree* root, Vector4 center, Vector4 ray)
	{
		if (root == NULL)
			return triangles;

		LinkedList<Octree>* octrees = new LinkedList<Octree>();
		LinkedList<Face>* faces = new LinkedList<Face>();
		//center = center - (ray * diagonal);						// hack

		ray_octree_traversal(root, ray, center, octrees);

		// create triangle list
		LinkedList<Octree>::Cell<Octree>* tmp = octrees->start;
		while(tmp != NULL)
		{
			for(unsigned int i = 0; i < tmp->data->count; i++)
			{
				if(!faces->Contains(tmp->data->triangles[i]))
					faces->InsertToEnd(tmp->data->triangles[i]);
			}
			tmp = tmp->next;
		}
		delete octrees;
		return faces;
	}

	void CSDFController::ComputeTNB(Face* tmp, Vector4& tang, Vector4& norm, Vector4& binor)
	{
		// compute tanget space matrix
		Vector4 U = Vector4(tmp->v[1]->P - tmp->v[0]->P);
		Vector4 V = Vector4(tmp->v[2]->P - tmp->v[0]->P);
		norm = (U % V) * (-1.0);
		norm.Normalize();

		tang = Vector4(tmp->v[0]->P - tmp->v[2]->P);
		tang.Normalize();
		binor = tang % norm;
		binor.Normalize();
	}

	int CSDFController::first_node(double tx0, double ty0, double tz0, double txm, double tym, double tzm)
	{
		unsigned char answer = 0;   // initialize to 00000000
		// select the entry plane and set bits
		if(tx0 > ty0)
		{
			if(tx0 > tz0)					// PLANE YZ
			{
				if(tym < tx0) answer|=2;    // set bit at position 1
				if(tzm < tx0) answer|=1;    // set bit at position 0
				return (int) answer;
			}  
		} else
		{      
			if(ty0 > tz0)					// PLANE XZ
			{
				if(txm < ty0) answer|=4;    // set bit at position 2
				if(tzm < ty0) answer|=1;    // set bit at position 0
				return (int) answer;
			}
		}
											// PLANE XY
		if(txm < tz0) answer|=4;			// set bit at position 2
		if(tym < tz0) answer|=2;			// set bit at position 1
		return (int) answer;
	}

	int CSDFController::new_node(double txm, int x, double tym, int y, double tzm, int z)
	{
		if(txm < tym)
		{
			if(txm < tzm) return x;			// YZ plane
		}
		else
		{
			if(tym < tzm) return y;			// XZ plane
		}
		return z;							// XY plane;
	}

	void CSDFController::proc_subtree (double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		double txm, tym, tzm;
		int currNode;

		if((tx1 < 0.0) || (ty1 < 0.0) || (tz1 < 0.0)) return;

		if(node->isLeaf)
		{
			//loggger->logInfo("Reached leaf node");
			octrees->InsertToEnd(node);
			return;
		}
		
		txm = 0.5*(tx0 + tx1);
		tym = 0.5*(ty0 + ty1);
		tzm = 0.5*(tz0 + tz1);
		currNode = first_node(tx0,ty0,tz0,txm,tym,tzm);
		do{
			switch (currNode)
			{
			case 0: {
				proc_subtree(tx0,ty0,tz0,txm,tym,tzm,node->son[a], octrees);
				currNode = new_node(txm,4,tym,2,tzm,1);
				break;}
			case 1: {
				proc_subtree(tx0,ty0,tzm,txm,tym,tz1,node->son[1^a], octrees);
				currNode = new_node(txm,5,tym,3,tz1,8);
				break;}
			case 2: {
				proc_subtree(tx0,tym,tz0,txm,ty1,tzm,node->son[2^a], octrees);
				currNode = new_node(txm,6,ty1,8,tzm,3);
				break;}
			case 3: {
				proc_subtree(tx0,tym,tzm,txm,ty1,tz1,node->son[3^a], octrees);
				currNode = new_node(txm,7,ty1,8,tz1,8);
				break;}
			case 4: {
				proc_subtree(txm,ty0,tz0,tx1,tym,tzm,node->son[4^a], octrees);
				currNode = new_node(tx1,8,tym,6,tzm,5);
				break;}
			case 5: {
				proc_subtree(txm,ty0,tzm,tx1,tym,tz1,node->son[5^a], octrees);
				currNode = new_node(tx1,8,tym,7,tz1,8);
				break;}
			case 6: {
				proc_subtree(txm,tym,tz0,tx1,ty1,tzm,node->son[6^a], octrees);
				currNode = new_node(tx1,8,ty1,8,tzm,7);
				break;}
			case 7: {
				proc_subtree(txm,tym,tzm,tx1,ty1,tz1,node->son[7^a], octrees);
				currNode = 8;
				break;}
			}
		} while (currNode < 8);
	}

	void CSDFController::ray_octree_traversal(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees)
	{
		a = 0;
		double o_size = 0.0;
		double o_X = 0.0;
		double o_Y = 0.0;
		double o_Z = 0.0;
		octree->GetBoundary(o_size, o_X, o_Y, o_Z);
		double to_size = o_size * 2.0;

		// avoid division by zero
		double Bias = 0.0001;
		if (fabs(ray.X) < Bias) ray.X = ray.X < 0.0 ? -Bias : Bias;
		if (fabs(ray.Y) < Bias) ray.Y = ray.Y < 0.0 ? -Bias : Bias;
		if (fabs(ray.Z) < Bias) ray.Z = ray.Z < 0.0 ? -Bias : Bias;

		double invdirx = 1.0 / fabs(ray.X);
		double invdiry = 1.0 / fabs(ray.Y);
		double invdirz = 1.0 / fabs(ray.Z);

		// TODO: pridat do octree a nie sem
		Vector4 o_min = Vector4(o_X - o_size, o_Y - o_size, o_Z - o_size, 1.0);
		Vector4 o_max = Vector4(o_X + o_size, o_Y + o_size, o_Z + o_size, 1.0);
		double tx0 ,tx1, ty0, ty1, tz0, tz1;

		// fixes for rays with negative direction
		if(ray.X < 0.0)
		{
			//Center.X = o_size / 4.0 - Center.X;
			//ray.X *= -1.0;
			tx0 = (o_max.X - Center.X) * -invdirx;
			tx1 = (o_min.X - Center.X) * -invdirx;
			a |= 4 ; //bitwise OR (latest bits are XYZ)
		}
		else
		{
			tx0 = (o_min.X - Center.X) * invdirx;
			tx1 = (o_max.X - Center.X) * invdirx;
		}
		if(ray.Y < 0.0)
		{
			//Center.Y = o_size / 4.0 - Center.Y;
			//ray.Y *= -1.0;
			ty0 = (o_max.Y - Center.Y) * -invdiry;
			ty1 = (o_min.Y - Center.Y) * -invdiry;
			a |= 2 ;
		}
		else
		{
			ty0 = (o_min.Y - Center.Y) * invdiry;
			ty1 = (o_max.Y - Center.Y) * invdiry;
		}
		if(ray.Z < 0.0)
		{
			//Center.Z = o_size / 4.0 - Center.Z;
			//ray.Z *= -1.0;
			tz0 = (o_max.Z - Center.Z) * -invdirz;
			tz1 = (o_min.Z - Center.Z) * -invdirz;
			a |= 1 ;
		}
		else
		{
			tz0 = (o_min.Z - Center.Z) * invdirz;
			tz1 = (o_max.Z - Center.Z) * invdirz;
		}



		/*double tx0 = (o_min.X - Center.X) * invdirx;
		double tx1 = (o_max.X - Center.X) * invdirx;
		double ty0 = (o_min.Y - Center.Y) * invdiry;
		double ty1 = (o_max.Y - Center.Y) * invdiry;
		double tz0 = (o_min.Z - Center.Z) * invdirz;
		double tz1 = (o_max.Z - Center.Z) * invdirz;*/

		/*double tx0 = ((o_X - o_size)) * invdirx;
		double tx1 = ((o_X + o_size)) * invdirx;
		double ty0 = ((o_Y - o_size)) * invdiry;
		double ty1 = ((o_Y + o_size)) * invdiry;
		double tz0 = ((o_Z - o_size)) * invdirz;
		double tz1 = ((o_Z + o_size)) * invdirz;*/

        /*double tx0 = -Center.X * invdirx;
        double tx1 = (to_size - Center.X) * invdirx;
 
        double ty0 = -Center.Y * invdiry;
        double ty1 = (to_size - Center.Y) * invdiry;

        double tz0 = -Center.Z * invdirz;
        double tz1 = (to_size - Center.Z) * invdirz;*/

		/*if (ray.X < Bias)
        {
                if (tx0 < 0) tx0 = -INT_MAX;
                else tx0 = INT_MAX;
                if (tx1 < 0) tx1 = -INT_MAX;
                else tx1 = INT_MAX;
        }
        if (ray.Y < Bias)
        {
                if (ty0 < 0) ty0 = -INT_MAX;
                else ty0 = INT_MAX;
                if (ty1 < 0) ty1 = -INT_MAX;
                else ty1 = INT_MAX;
        }
        if (ray.Z < Bias)
        {
                if (tz0 < 0) tz0 = -INT_MAX;
                else tz0 = INT_MAX;
                if (tz1 < 0) tz1 = -INT_MAX;
                else tz1 = INT_MAX;
        }*/

		if( max(max(tx0,ty0),tz0) < min(min(tx1,ty1),tz1) )
		{ 
			proc_subtree(tx0,ty0,tz0,tx1,ty1,tz1,octree, octrees);
		}
	}

	struct ActualState
	{
		double tx0;
		double tx1;
		double ty0;
		double ty1;
		double tz0;
		double tz1;
	};
	void CSDFController::proc_subtree2 (double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		if((tx1 < 0.0) || (ty1 < 0.0) || (tz1 < 0.0)) return;

		if(node->isLeaf)
		{
			//loggger->logInfo("Reached leaf node");
			octrees->InsertToEnd(node);
			return;
		}

		int currNode = 0;
		double txm, tym, tzm;
	
		ActualState state;
		ActualState newState;

		state.tx0 = tx0; state.ty0 = ty0; state.tz0 = tz0;
		state.tx1 = tx1; state.ty1 = ty1; state.tz1 = tz1;	

		int stateLookUp = 0;

		txm = 0.5f * (state.tx0 + state.tx1);
		tym = 0.5f * (state.ty0 + state.ty1);
		tzm = 0.5f * (state.tz0 + state.tz1);

		currNode = first_node(state.tx0, state.ty0, state.tz0, txm, tym, tzm);
		do 
		{
			int case1 = 8;
			int case2 = 8;
			int case3 = 8;		
			switch (currNode)
			{
				case FLD_NODE:
					newState.tx0 = state.tx0; newState.ty0 = state.ty0; newState.tz0 = state.tz0;
					newState.tx1 = txm;		  newState.ty1 = tym;		newState.tz1 = tzm;						
					stateLookUp = FLD_NODE ^ a;
					case1 = FRD_NODE; case2 = FLT_NODE; case3 = BLD_NODE;
					break;
				case BLD_NODE:
					newState.tx0 = state.tx0; newState.ty0 = state.ty0; newState.tz0 = tzm;
					newState.tx1 = txm;		  newState.ty1 = tym;		newState.tz1 = state.tz1;										
					stateLookUp = BLD_NODE ^ a;
					case1 = BRD_NODE; case2 = BLT_NODE;
					break;
				case FLT_NODE:
					newState.tx0 = state.tx0; newState.ty0 = tym;		newState.tz0 = state.tz0;
					newState.tx1 = txm;		  newState.ty1 = state.ty1; newState.tz1 = tzm;							
					stateLookUp = FLT_NODE ^ a;
					case1 = FRT_NODE; case3 = BLT_NODE;
					break;
				case BLT_NODE:
					newState.tx0 = state.tx0; newState.ty0 = tym;		newState.tz0 = tzm;
					newState.tx1 = txm;		  newState.ty1 = state.ty1; newState.tz1 = state.tz1;							
					stateLookUp = BLT_NODE ^ a;
					case1 = BRT_NODE;
					break;
				case FRD_NODE:
					newState.tx0 = txm;		  newState.ty0 = state.ty0; newState.tz0 = state.tz0;
					newState.tx1 = state.tx1; newState.ty1 = tym;		newState.tz1 = tzm;						
					stateLookUp = FRD_NODE ^ a;
					case2 = FRT_NODE; case3 = BRD_NODE;
					break;
				case BRD_NODE:
					newState.tx0 = txm;		  newState.ty0 = state.ty0; newState.tz0 = tzm;
					newState.tx1 = state.tx1; newState.ty1 = tym;		newState.tz1 = state.tz1;						
					stateLookUp = BRD_NODE ^ a;
					case2 = BRT_NODE;
					break;
				case FRT_NODE:
					newState.tx0 = txm;		  newState.ty0 = tym;		newState.tz0 = state.tz0;
					newState.tx1 = state.tx1; newState.ty1 = state.ty1; newState.tz1 = tzm;						
					stateLookUp = FRT_NODE ^ a;
					case3 = BRT_NODE;
					break;
				case BRT_NODE:
					newState.tx0 = txm;		  newState.ty0 = tym;		newState.tz0 = tzm;
					newState.tx1 = state.tx1; newState.ty1 = state.ty1; newState.tz1 = state.tz1;						
					stateLookUp = BRT_NODE ^ a;
					break;
			}

			currNode = new_node(newState.tx1, case1, 
								newState.ty1, case2,
								newState.tz1, case3);
			proc_subtree2(newState.tx0, newState.ty0, newState.tz0, newState.tx1, newState.ty1, newState.tz1,node->son[stateLookUp], octrees);
		} while (currNode < 8);
	}
}