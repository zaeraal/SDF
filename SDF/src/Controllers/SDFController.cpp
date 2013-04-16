// SDFController.h : subor pre vypocitanie SDF funkcie
#include "stdafx.h"
#include "SDFController.h"
#include "MathHelper.h"

#define FLOAT_MAX  99999.0
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
	CSDFController::CSDFController(float dia, CAssimp* logg)
	{
		diagonal = dia;
		loggger = logg;
		fc_list = new LinkedList<Face>();
		fc_list->Preallocate(100);
		oc_list = new LinkedList<Octree>();
		oc_list->Preallocate(100);
		//loggger->logInfo(MarshalString("diagonal: " + diagonal));
	}

	// destruktor
	CSDFController::~CSDFController()
	{
		delete fc_list;
		delete oc_list;
	}
		
	// pocitanie funkcie pre vsetky trojuholniky, O(n2)
	void CSDFController::Compute(LinkedList<Face>* triangles, Octree* root)
	{
		time_t timer1 = time(NULL);
		float min = FLOAT_MAX;
		float max = 0.0;
		
		unsigned int n_rays = 30;
		float angle = 120.0f;
		unsigned int counter = 0;

		//------------------prealocated variables------------------
		Vector4 tangens, normal, binormal;
		Mat4 t_mat;
		std::vector<float> rays;
		std::vector<float> weights;

		// precompute those N rays
		srand (123);											// initial seed for random number generator
		float* rndy = new float[n_rays];
		float* rndx = new float[n_rays];
		for(unsigned int i = 0; i < n_rays; i++)
		{
			rndy[i] = float(rand()%int(angle / 2));
			rndx[i] = float(rand()%(360));
			if(rndy[i] == 0.0)
				rndy[i] = 0.5;
		}

		float dist = FLOAT_MAX;
		float dist2 = FLOAT_MAX;
		float theta = 0.0f;
		bool intersected = false;

		LinkedList<Face>* face_list = NULL;
		LinkedList<Face>::Cell<Face>* intersected_face = NULL;
		//------------------prealocated variables------------------

		LinkedList<Face>::Cell<Face>* current_face = triangles->start;
		while(current_face != NULL)
		{
			// vypocet TNB vektorov a matice
			ComputeTNB(current_face->data, tangens, normal, binormal);
			t_mat = Mat4(tangens, normal, binormal);

			rays.clear();
			weights.clear();
			for(unsigned int i = 0; i < n_rays; i++)
			{
				Vector4 ray = CalcRayFromAngle(rndx[i], rndy[i]) * t_mat;
				ray.Normalize();

				dist = FLOAT_MAX;
				face_list = GetFaceList(triangles, root, current_face->data->center, ray);
				intersected_face = face_list->start;
				while(intersected_face != NULL)
				{
					if(current_face == intersected_face)
					{
						intersected_face = intersected_face->next;
						continue;
					}

					dist2 = FLOAT_MAX;
					intersected = rayIntersectsTriangle(current_face->data->center, ray, intersected_face->data->v[0]->P, intersected_face->data->v[1]->P, intersected_face->data->v[2]->P, dist2);
					if(intersected == true)
					{
						theta = acos( (ray * (current_face->data->normal * (-1))) / (ray.Length() * current_face->data->normal.Length()) );
						theta = theta * float(180.0 / M_PI);
						//loggger->logInfo(MarshalString("pridany ray s thetou: " + theta));
						if((theta < 90.0f) && (dist2 < dist))
							dist = dist2;
					}

					intersected_face = intersected_face->next;
				}
				if(dist < (FLOAT_MAX - 1.0f))
				{
					//loggger->logInfo(MarshalString("pridany ray s dlzkou: " + dist));
					rays.push_back(dist);
					weights.push_back(180.0f - rndy[i]);
				}
				//if(root != NULL)
					//delete face_list;						// generated list, bez prealokovania
			}
			if(rays.size() > 0)
			{
				current_face->data->ComputeSDFValue(rays, weights);
				if(current_face->data->diameter->value < min)
					min = current_face->data->diameter->value;
				if(current_face->data->diameter->value > max)
					max = current_face->data->diameter->value;
			}
			counter = counter + 1;
			current_face = current_face->next;
		}
		fc_list->Clear();
		oc_list->Clear();
		delete [] rndy;
		delete [] rndx;

		time_t timer2 = time(NULL);
		// postprocessing - smoothing and normalization
		int kernel_size = 2;
		//float kernel[] = {1.0,4.0,6.0,4.0,1.0};
		float* kernel = ComputeGaussianKernel(kernel_size);
		current_face = triangles->start;
		while(current_face != NULL)
		{
			Smooth(current_face->data, kernel, kernel_size);
			current_face->data->diameter->Normalize1(min, max, 4.0);
			current_face->data->diameter->Normalize2(0, max, 4.0);
			//tmp->data->diameter->Normalize2(0, diagonal, 4.0);

			current_face = current_face->next;
		}
		delete kernel;
		time_t timer3 = time(NULL);

		loggger->logInfo(MarshalString("SDF vypocitane v case: " + (timer2 - timer1)+ "s"));
		loggger->logInfo(MarshalString("SDF hodnoty vyhladene v case: " + (timer3 - timer2)+ "s"));
		loggger->logInfo(MarshalString("Celkovy vypocet trval: " + (timer3 - timer1)+ "s, pre " + counter + " trojuholnikov"));
		//loggger->logInfo(MarshalString("pocet: " + counter));
		//loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		//loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));
	}

	// vypocitaj normalizovany 1D kernel pre gaussian
	float* CSDFController::ComputeGaussianKernel(int radius)
	{
		float* matrix = new float [radius*2+1];
		float sigma = (float)radius/2.0f;
		float norm = 1.0f / float(sqrt(2*M_PI) * sigma);
		float coeff = 2*sigma*sigma;
		float total=0;
		for(int x = -radius; x <= radius; x++)
		{
			float g = norm * exp( (-x*x)/coeff );
			matrix[x+radius] = g;
			total+=g;
		}
		for(int x=0; x<=2*radius; x++)
			matrix[x]=(matrix[x]/total) * 1000.0f;

		return matrix;
	}

	void CSDFController::Smooth(Face* tmp, float* kernel, int kernel_size)
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
		std::vector<float> _values;
		std::vector<float> _weights;

		for(int i=0; i <= kernel_size; i++)
		{
			int _size = sus[i]->GetSize();
			if(_size != 0)
			{
				float _weight = kernel[kernel_size + i];// / (float)_size;
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

		LinkedList<Octree>* octrees = oc_list;
		LinkedList<Face>* faces = fc_list;
		octrees->Clear();
		faces->Clear();
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
		//delete octrees;								// bez prealokovania
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

	int CSDFController::first_node(float tx0, float ty0, float tz0, float txm, float tym, float tzm)
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

	int CSDFController::new_node(float txm, int x, float tym, int y, float tzm, int z)
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

	void CSDFController::proc_subtree (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		float txm, tym, tzm;
		int currNode;

		if((tx1 < 0.0) || (ty1 < 0.0) || (tz1 < 0.0)) return;

		if(node->isLeaf)
		{
			//loggger->logInfo("Reached leaf node");
			octrees->InsertToEnd(node);
			return;
		}
		
		txm = 0.5f*(tx0 + tx1);
		tym = 0.5f*(ty0 + ty1);
		tzm = 0.5f*(tz0 + tz1);
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
		float o_size = 0.0;
		float o_X = 0.0;
		float o_Y = 0.0;
		float o_Z = 0.0;
		octree->GetBoundary(o_size, o_X, o_Y, o_Z);
		float to_size = o_size * 2.0f;

		// avoid division by zero
		float Bias = 0.0001f;
		if (fabs(ray.X) < Bias) ray.X = ray.X < 0.0 ? -Bias : Bias;
		if (fabs(ray.Y) < Bias) ray.Y = ray.Y < 0.0 ? -Bias : Bias;
		if (fabs(ray.Z) < Bias) ray.Z = ray.Z < 0.0 ? -Bias : Bias;

		float invdirx = 1.0f / fabs(ray.X);
		float invdiry = 1.0f / fabs(ray.Y);
		float invdirz = 1.0f / fabs(ray.Z);

		// TODO: pridat do octree a nie sem
		Vector4 o_min = Vector4(o_X - o_size, o_Y - o_size, o_Z - o_size, 1.0);
		Vector4 o_max = Vector4(o_X + o_size, o_Y + o_size, o_Z + o_size, 1.0);
		float tx0 ,tx1, ty0, ty1, tz0, tz1;

		// fixes for rays with negative direction
		if(ray.X < 0.0)
		{
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
			tz0 = (o_max.Z - Center.Z) * -invdirz;
			tz1 = (o_min.Z - Center.Z) * -invdirz;
			a |= 1 ;
		}
		else
		{
			tz0 = (o_min.Z - Center.Z) * invdirz;
			tz1 = (o_max.Z - Center.Z) * invdirz;
		}

		/*float tx0 = (o_min.X - Center.X) * invdirx;
		float tx1 = (o_max.X - Center.X) * invdirx;
		float ty0 = (o_min.Y - Center.Y) * invdiry;
		float ty1 = (o_max.Y - Center.Y) * invdiry;
		float tz0 = (o_min.Z - Center.Z) * invdirz;
		float tz1 = (o_max.Z - Center.Z) * invdirz;*/

		if( max(max(tx0,ty0),tz0) < min(min(tx1,ty1),tz1) )
		{ 
			proc_subtree(tx0,ty0,tz0,tx1,ty1,tz1,octree, octrees);
		}
	}

	struct ActualState
	{
		float tx0;
		float tx1;
		float ty0;
		float ty1;
		float tz0;
		float tz1;
	};
	void CSDFController::proc_subtree2 (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		if((tx1 < 0.0) || (ty1 < 0.0) || (tz1 < 0.0)) return;

		if(node->isLeaf)
		{
			//loggger->logInfo("Reached leaf node");
			octrees->InsertToEnd(node);
			return;
		}

		int currNode = 0;
		float txm, tym, tzm;
	
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