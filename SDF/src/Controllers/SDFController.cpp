// SDFController.h : subor pre vypocitanie SDF funkcie
#include "stdafx.h"
#include "SDFController.h"
#include "MathHelper.h"
#include "SDFOpenCL.h"

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
		prealocated_space = 100;
		/*fc_list = new LinkedList<Face>();
		fc_list->Preallocate(prealocated_space);*/
		fc_list = new HashTable<Face>(prealocated_space);
		oc_list = new LinkedList<Octree>();
		oc_list->Preallocate(100);
		InitKernel();
		//loggger->logInfo(MarshalString("diagonal: " + diagonal));
	}

	// destruktor
	CSDFController::~CSDFController()
	{
		delete fc_list;
		delete oc_list;

		EraseKernel();
	}
		
	// pocitanie funkcie pre vsetky trojuholniky, O(n2)
	void CSDFController::Compute(LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max)
	{
		int ticks1 = GetTickCount();
		float min = FLOAT_MAX;
		float max = 0.0;
		
		const unsigned int n_rays = Nastavenia->SDF_Rays;
		unsigned int counter = 0;

		//------------------prealocated variables------------------
		Vector4 tangens, normal, binormal;
		Mat4 t_mat;
		std::vector<float> rays;
		std::vector<float> weights;

		float* rndy = new float[n_rays];
		float* rndx = new float[n_rays];
		if(Nastavenia->SDF_Distribution == true)
			UniformPointsOnSphere(rndx, rndy);
		else
			RandomPointsOnSphere(rndx, rndy);

		for(unsigned int i = 0; i < n_rays; i++)
		{
			weights.push_back(180.0f - rndy[i]);
		}

		float dist = FLOAT_MAX;
		float dist2 = FLOAT_MAX;
		float theta = 0.0f;
		bool intersected = false;

		HashTable<Face>* face_list = NULL;
		Face** intersected_face = NULL;
		//------------------prealocated variables------------------

		LinkedList<Face>::Cell<Face>* current_face = triangles->start;
		int min_tri = 99999;
		int max_tri = 0;
		float avg = 0;
		float vypis_pocet = 0.0f;
		float total_pocet = (float)triangles->GetSize();
		while(current_face != NULL)
		{
			vypis_pocet = vypis_pocet + 1;
			// vypocet TNB vektorov a matice
			ComputeTNB(current_face->data, tangens, binormal, normal);
			t_mat = Mat4(tangens, normal, binormal);

			rays.clear();
			Nastavenia->SDF_STATUS = (unsigned int)((vypis_pocet / total_pocet) * 100.0f);
			for(unsigned int i = 0; i < n_rays; i++)
			{
				unsigned int repeat = 1;
				if(Nastavenia->SDF_Revert_Rays == true)
					repeat = 2;
				
				Vector4 ray = CalcRayFromAngle(rndx[i], rndy[i]) * t_mat;
				ray.Normalize();
				ray = ray * (-1.0f);

				dist = FLOAT_MAX;
				while(repeat > 0)
				{
					repeat--;
					ray = ray * (-1.0f);
					fc_list->Clear();
					oc_list->Clear();
					//fc_list->InsertToStart(current_face->data);
					fc_list->Insert(current_face->data);
					face_list = GetFaceList(triangles, root, current_face->data->center, ray, o_min, o_max);
					face_list->Delete(current_face->data);

					int sizi = face_list->GetSize();
					if(sizi < min_tri)
						min_tri = sizi;
					if(sizi > max_tri)
						max_tri = sizi;
					avg += sizi;

					intersected_face = face_list->start;
					if(face_list->GetSize() > 0)
					{
						for(unsigned int idx = 0; idx < face_list->prealocated; idx++)
						{				
							if(intersected_face[idx] == NULL)
								continue;
							dist2 = FLOAT_MAX;
							intersected = rayIntersectsTriangle(current_face->data->center, ray, intersected_face[idx]->v[0]->P, intersected_face[idx]->v[1]->P, intersected_face[idx]->v[2]->P, dist2);
							if(intersected == true)
							{
								if(Nastavenia->SDF_Revert_Rays == true)
								{
									if((dist2 > 0.0001f) && (dist2 < dist))
										dist = dist2;
								}
								else
								{
									theta = acos( (ray * intersected_face[idx]->normal) / (ray.Length() * intersected_face[idx]->normal.Length()) );
									theta = theta * float(180.0 / M_PI);
									//loggger->logInfo(MarshalString("pridany ray s thetou: " + theta));
									if((theta < 90.0f) && (dist2 > 0.0001f) && (dist2 < dist))
										dist = dist2;
								}
							}
							//intersected_face = intersected_face->next;
						}
					}
				}
				//if(dist < (FLOAT_MAX - 1.0f))
				{
					//loggger->logInfo(MarshalString("pridany ray s dlzkou: " + dist));
					if(dist >= (FLOAT_MAX - 1.0f))
						dist = 0;
					rays.push_back(dist);
				}
				//if(root != NULL)
					//delete face_list;						// generated list, bez prealokovania
			}
			if(rays.size() > 0)
			{
				current_face->data->ComputeSDFValue(rays, weights);
				if(current_face->data->quality->value < min)
					min = current_face->data->quality->value;
				if(current_face->data->quality->value > max)
					max = current_face->data->quality->value;
			}
			counter = counter + 1;
			current_face = current_face->next;
		}
		fc_list->Clear();
		oc_list->Clear();
		delete [] rndy;
		delete [] rndx;

		int ticks2 = GetTickCount();
		// postprocessing - smoothing and normalization
		//float kernel[] = {1.0,4.0,6.0,4.0,1.0};
		EraseKernel();
		InitKernel();
		float* kernel = ComputeGaussianKernel(kernel_size);
		current_face = triangles->start;
		while(current_face != NULL)
		{
			Smooth(current_face->data, kernel, kernel_size);
			current_face->data->quality->Normalize(min, max, 4.0);
			// pokus pouzit diagonalu, pripadne avg a pod, ale dako nefungovalo
			//tmp->data->diameter->Normalizex(0, diagonal, 4.0);

			current_face = current_face->next;
		}
		Nastavenia->DEBUG_Min_SDF = min;
		Nastavenia->DEBUG_Max_SDF = max;
		delete kernel;
		int ticks3 = GetTickCount();

		loggger->logInfo(MarshalString("SDF vypocitane v case: " + (ticks2 - ticks1)+ "ms"));
		loggger->logInfo(MarshalString("SDF hodnoty vyhladene v case: " + (ticks3 - ticks2)+ "ms"));
		loggger->logInfo(MarshalString("Celkovy vypocet trval: " + (ticks3 - ticks1)+ "ms, pre " + counter + " trojuholnikov"));
		//loggger->logInfo(MarshalString("pocet: " + counter));
		//loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		//loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));
		avg = avg / (triangles->GetSize() * n_rays);
		loggger->logInfo(MarshalString("min trianglov na luc: " + min_tri));
		loggger->logInfo(MarshalString("max trianglov na luc: " + max_tri));
		loggger->logInfo(MarshalString("avg trianglov na luc: " + avg));
	}

	// pocitanie funkcie pre vsetky trojuholniky v OpenCL
	void CSDFController::ComputeOpenCL(LinkedList<Vertex>* points, LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max)
	{
		using namespace OpenCLForm;
		int ticks1 = GetTickCount();

		//-------------------------------------------
		//---------------INIT OpenCL-------Begin-----
		//-------------------------------------------
		COpenCL* OpenCLko = new COpenCL();

		int err = EXIT_SUCCESS;
		err = OpenCLko->InitOpenCL();
		if(!CheckError(err)) return;

		err = OpenCLko->LoadKernel("sdf.cl");
		if(!CheckError(err)) return;

		err = OpenCLko->BuildKernel();
		if(!CheckError(err, OpenCLko->debug_buffer)) return;

		err = OpenCLko->GetGPUVariables();
		if(!CheckError(err)) return;

		//-------------------------------------------
		//---------------INIT OpenCL-------End-------
		//-------------------------------------------

		// IMPORTANT!! Variables for memory allocation
		size_t n_workitems = (size_t)(OpenCLko->num_1D_work_items * Nastavenia->GPU_Work_Items);
		if(OpenCLko->num_ND_work_items[0] < n_workitems)
			n_workitems = (size_t)(OpenCLko->num_ND_work_items[0] * Nastavenia->GPU_Work_Items);

		size_t n_workgroups = (size_t)(OpenCLko->num_cores * Nastavenia->GPU_Work_Groups);
		
		OpenCLko->global = n_workgroups * n_workitems;
		OpenCLko->local = n_workitems;

		const unsigned int n_rays = Nastavenia->SDF_Rays;
		const unsigned int n_prealloc = prealocated_space;

		unsigned int n_triangles = triangles->GetSize();
		unsigned int n_vertices = points->GetSize();

		unsigned int n_rays_per_kernel = n_workitems * n_workgroups;
		unsigned int n_triangles_per_kernel = (unsigned int)(n_rays_per_kernel / n_rays);
		unsigned int n_triangles_at_end = n_triangles % n_triangles_per_kernel;
		unsigned int n_rays_at_end = n_triangles_at_end * n_rays;

		unsigned int n_kernels = (unsigned int)(n_triangles / n_triangles_per_kernel);
		if(n_triangles_at_end > 0) n_kernels++;

		//-------------------------------------------
		//---------------Memory Alloc------Begin-----
		//-------------------------------------------

		cl_uint3	*c_triangles;			// zoznam trojuholnikov obsahujucich IDX 3 vertexov
		cl_float4	*c_vertices;			// zoznam vertexov obsahujuci ich poziciu
		cl_uint4	c_params;				// potrebujem vediet n_workitems, n_rays, n_prealloc, max pocet lucov
		cl_uint		**c_origins;			// zoznamy trojuholnikov na strielanie lucov, ktore sa postupne vkladaju do OpenCL
		cl_float4	**c_rays;				// zoznamy lucov, ktore sa postupne vkladaju do OpenCL
		cl_uint		**c_targets;			// zoznamy trojuholnikov na kontrolu, ktore sa postupne vkladaju do OpenCL
		cl_float	**c_outputs;			// vzdialenost a vaha pre kazdy luc, ktore je mojim vysledkom co si zapisem

		unsigned int s_triangles = n_triangles * sizeof(cl_uint3);									// pocet trojuholnikov * 4 (nie 3!) * int
		unsigned int s_vertices = n_vertices * sizeof(cl_float4);									// pocet vertexov * 4 * float
		unsigned int s_origins = n_triangles_per_kernel * sizeof(cl_uint);							// trojuholniky pre kernel * int
		unsigned int s_rays = n_triangles_per_kernel * n_rays * sizeof(cl_float4);					// trojuholniky pre kernel * 30 * 4 * float
		unsigned int s_targets = n_triangles_per_kernel * n_rays * n_prealloc * sizeof(cl_uint);	// trojuholniky pre kernel * 30 * 100 * int
		unsigned int s_outputs = n_triangles_per_kernel * n_rays * sizeof(cl_float);				// trojuholniky pre kernel * 30  * float

		int ticks2 = GetTickCount();
		err = OpenCLko->SetupMemory(s_triangles, s_vertices, s_origins, s_rays, s_targets, s_outputs);
		if(!CheckError(err)) return;
		int ticks3 = GetTickCount();

		c_triangles = (cl_uint3*) malloc(s_triangles);
		c_vertices = (cl_float4*) malloc(s_vertices);
		c_origins = (cl_uint**) calloc(n_kernels, sizeof(cl_uint*));
		c_rays = (cl_float4**) calloc(n_kernels, sizeof(cl_float4*));
		c_targets = (cl_uint**) calloc(n_kernels, sizeof(cl_uint*));
		c_outputs = (cl_float**) calloc(n_kernels, sizeof(cl_float*));

		for(unsigned int i = 0; i < n_kernels; i++)
		{
			c_origins[i] = (cl_uint*) calloc(s_origins, 1);
			c_rays[i] = (cl_float4*) calloc(s_rays, 1);
			c_targets[i] = (cl_uint*) calloc(s_targets, 1);
			c_outputs[i] = (cl_float*) calloc(s_outputs, 1);
		}
		//-------------------------------------------
		//---------------Memory Alloc------End-------
		//-------------------------------------------

		//------------------prealocated variables------------------	
		std::vector<float> weights;

		Vector4 tangens, normal, binormal;
		Mat4 t_mat;

		float* rndy = new float[n_rays];
		float* rndx = new float[n_rays];
		if(Nastavenia->SDF_Distribution == true)
			UniformPointsOnSphere(rndx, rndy);
		else
			RandomPointsOnSphere(rndx, rndy);

		for(unsigned int i = 0; i < Nastavenia->SDF_Rays; i++)
		{
			weights.push_back(180.0f - rndy[i]);
		}

		//------------------prealocated variables------------------

		// -----------------------------------------------
		// vypocitaj dopredu zoznamy trojuholnikov a lucov
		// -----------------------------------------------
		int ticks4 = GetTickCount();
		//---------------copy variables--------------
		unsigned int pocet = 0;
		LinkedList<Face>::Cell<Face>* tmp_face = triangles->start;
		while(tmp_face != NULL)
		{
			c_triangles[pocet].s[0] = tmp_face->data->v[0]->number;
			c_triangles[pocet].s[1] = tmp_face->data->v[1]->number;
			c_triangles[pocet].s[2] = tmp_face->data->v[2]->number;
			tmp_face = tmp_face->next;
			pocet++;
		}

		pocet = 0;
		LinkedList<Vertex>::Cell<Vertex>* tmp_points = points->start;
		while(tmp_points != NULL)
		{
			c_vertices[pocet].s[0] = tmp_points->data->P.X;
			c_vertices[pocet].s[1] = tmp_points->data->P.Y;
			c_vertices[pocet].s[2] = tmp_points->data->P.Z;
			c_vertices[pocet].s[3] = tmp_points->data->P.W;
			tmp_points = tmp_points->next;
			pocet++;
		}
		//---------------copy variables--------------
		int ticks5 = GetTickCount();
		pocet = 0;
		LinkedList<Face>::Cell<Face>* current_face = triangles->start;
		Face** tmp_faces = NULL;
		while(current_face != NULL)
		{
			unsigned int kernel_num = (unsigned int)(pocet / n_triangles_per_kernel);
			unsigned int in_kernel_num = (unsigned int)(pocet % n_triangles_per_kernel);

			// TODO: zjednodusit
			c_origins[kernel_num][in_kernel_num] = pocet;

			// vypocet TNB vektorov a matice
			ComputeTNB(current_face->data, tangens, binormal, normal);
			t_mat = Mat4(tangens, normal, binormal);

			for(unsigned int i = 0; i < n_rays; i++)
			{
				Vector4 ray = CalcRayFromAngle(rndx[i], rndy[i]) * t_mat;
				ray.Normalize();

				c_rays[kernel_num][i+(n_rays*in_kernel_num)].s[0] = ray.X;
				c_rays[kernel_num][i+(n_rays*in_kernel_num)].s[1] = ray.Y;
				c_rays[kernel_num][i+(n_rays*in_kernel_num)].s[2] = ray.Z;
				c_rays[kernel_num][i+(n_rays*in_kernel_num)].s[3] = ray.W;

				fc_list->Clear();
				oc_list->Clear();
				//fc_list->InsertToStart(current_face->data);
				fc_list->Insert(current_face->data);
				HashTable<Face>* face_list = GetFaceList(triangles, root, current_face->data->center, ray, o_min, o_max);
				face_list->Delete(current_face->data);

				tmp_faces = face_list->start;
				unsigned int c_pocet = 0;
				if(face_list->GetSize() > 0)
				{
					for(unsigned int idx = 0; idx < face_list->prealocated; idx++)
					{	
						if(tmp_faces[idx] != NULL)
						{
							c_targets[kernel_num][c_pocet+(i+(n_rays*in_kernel_num))*n_prealloc] = tmp_faces[idx]->number + 1;		// naschval posunute
							//tmp_face = tmp_face->next;
							c_pocet++;
							if(c_pocet == prealocated_space) break;
						}
					}
				}
			}
			pocet = pocet + 1;
			current_face = current_face->next;
		}
		fc_list->Clear();
		oc_list->Clear();
		delete [] rndy;
		delete [] rndx;
		// -----------------------------------------------
		// vypocitaj dopredu zoznamy trojuholnikov a lucov
		// -----------------------------------------------


		// v tomto bode je uz pamet pripravena a nacitana
		// je nutne poslat ju do OpenCL a zahajit vypocet
		int ticks6 = GetTickCount();
		unsigned int pocet_trojuholnikov = n_triangles_per_kernel;
		for(unsigned int i = 0; i < n_kernels; i++)
		{
			if(i == (n_kernels-1)) 
				pocet_trojuholnikov = n_triangles_at_end;
			c_params.s[0] = n_workitems; c_params.s[1] = n_rays; c_params.s[2] = n_prealloc; c_params.s[3] = pocet_trojuholnikov;
			err = OpenCLko->LaunchKernel(c_triangles, c_vertices, c_origins[i], c_rays[i], c_targets[i], c_outputs[i], c_params);
			if(!CheckError(err)) return;
		}
		OpenCLko->WaitForFinish();

		int ticks7 = GetTickCount();

		// spracuj ziskane hodnoty
		float min = FLOAT_MAX;
		float max = 0.0;
		unsigned int cpocet = 0;
		pocet_trojuholnikov = n_triangles_per_kernel;
		std::vector<float> rays;
		std::vector<float> weightsx;
		current_face = triangles->start;
		for(unsigned int i = 0; i < n_kernels; i++)
		{
			if(i == (n_kernels-1)) 
				pocet_trojuholnikov = n_triangles_at_end;

			for(unsigned int j = 0; j < pocet_trojuholnikov; j++)
			{
				for(unsigned int k = 0; k < n_rays; k++)
				{
					float dist = c_outputs[i][k+j*n_rays];
					//loggger->logInfo(MarshalString("triangle: "+(i*n_triangles_per_kernel + j)+ " val: " + dist));
					if(dist < (FLOAT_MAX - 1.0f))
					{
						rays.push_back(c_outputs[i][k+j*n_rays]);
						weightsx.push_back(weights[k]);
					}
				}
				if(rays.size() > 0)
				{
					current_face->data->ComputeSDFValue(rays, weightsx);
					if(current_face->data->quality->value < min)
						min = current_face->data->quality->value;
					if(current_face->data->quality->value > max)
						max = current_face->data->quality->value;
				}
				cpocet++;
				current_face = current_face->next;
				rays.clear();
				weightsx.clear();
			}
		}
		int ticks8 = GetTickCount();

		// postprocessing - smoothing and normalization
		//float kernel[] = {1.0,4.0,6.0,4.0,1.0};
		EraseKernel();
		InitKernel();
		float* kernel = ComputeGaussianKernel(kernel_size);
		current_face = triangles->start;
		while(current_face != NULL)
		{
			Smooth(current_face->data, kernel, kernel_size);
			current_face->data->quality->Normalize(min, max, 4.0);
			// pokus pouzit diagonalu, pripadne avg a pod, ale dako nefungovalo
			//tmp->data->diameter->Normalizex(0, diagonal, 4.0);

			current_face = current_face->next;
		}
		Nastavenia->DEBUG_Min_SDF = min;
		Nastavenia->DEBUG_Max_SDF = max;
		delete kernel;
		int ticks9 = GetTickCount();


		loggger->logInfo(MarshalString("Inicializacia OpenCL: " + (ticks2 - ticks1)+ "ms"));
		loggger->logInfo(MarshalString("Alokovanie pamete v OpenCL: " + (ticks3 - ticks2)+ "ms"));
		loggger->logInfo(MarshalString("Alokovanie pamete v PC: " + (ticks4 - ticks3)+ "ms"));
		loggger->logInfo(MarshalString("Nacitanie zoznamu trojuholnikov: " + (ticks5 - ticks4)+ "ms"));
		loggger->logInfo(MarshalString("Prehladavanie Octree: " + (ticks6 - ticks5)+ "ms"));
		loggger->logInfo(MarshalString("!!VYPOCET OpenCL!!: " + (ticks7 - ticks6)+ "ms"));
		loggger->logInfo(MarshalString("Spracovanie: " + (ticks8 - ticks7)+ "ms"));
		loggger->logInfo(MarshalString("Smoothing: " + (ticks9 - ticks8)+ "ms"));
		loggger->logInfo(MarshalString("Celkovy vypocet trval: " + (ticks9 - ticks1)+ "ms, pre " + pocet + " trojuholnikov"));
		//loggger->logInfo(MarshalString("pocet: " + pocet));
		//loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		//loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));


		// Delete OpenCL to free GPU
		delete OpenCLko;

		// Free host memory
		for(unsigned int i = 0; i < n_kernels; i++)
		{
			free(c_origins[i]);
			free(c_rays[i]);
			free(c_targets[i]);
			free(c_outputs[i]);
		}
		free(c_triangles);
		free(c_vertices);
		free(c_origins);
		free(c_rays);
		free(c_targets);
		free(c_outputs);
	}

	// vypocitaj normalizovany 1D kernel pre gaussian
	float* CSDFController::ComputeGaussianKernel(int radius)
	{
		float* matrix = new float [radius*2+1];
		if(radius == 0)
		{
			matrix[0] = 1.0f;
			return matrix;
		}
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
		gauss_sus[0]->InsertToEnd(tmp);
		for(int i=1; i <= kernel_size; i++)				// bacha na posunutie
		{
			//gauss_sus[i] = new LinkedList<Face>();
			LinkedList<Face>::Cell<Face>* tm = gauss_sus[i-1]->start;
			while(tm != NULL)
			{
				LinkedList<Face>* t = tm->data->GetSusedia();
				LinkedList<Face>::Cell<Face>* tc = t->start;
				while(tc != NULL)
				{
					bool pokracuj = false;
					for(int j = 0; j <= i; j++)
					{
						if(gauss_sus[j]->Contains(tc->data))
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
					gauss_sus[i]->InsertToEnd(tc->data);
					tc = tc->next;
				}
				//delete t;
				tm = tm->next;
			}
		}
		std::vector<float> _values;
		std::vector<float> _weights;

		for(int i=0; i <= kernel_size; i++)
		{
			int _size = gauss_sus[i]->GetSize();
			if(_size != 0)
			{
				float _weight = kernel[kernel_size + i];// / (float)_size;
				LinkedList<Face>::Cell<Face>* tc = gauss_sus[i]->start;
				while(tc != NULL)
				{
					_values.push_back(tc->data->quality->value);
					_weights.push_back(_weight);
					tc = tc->next;
				}
			}
			gauss_sus[i]->Clear();
		}
		//delete [] gauss_sus;

		tmp->quality->Smooth(_values, _weights);
	}

	HashTable<Face>* CSDFController::GetFaceList(LinkedList<Face>* triangles, Octree* root, Vector4 center, Vector4 ray, Vector4 o_min, Vector4 o_max)
	{
		/*if (root == NULL)
			return triangles;*/
		if (root == NULL)
			return NULL;

		LinkedList<Octree>* octrees = oc_list;
		HashTable<Face>* faces = fc_list;
		//octrees->Clear();
		//faces->Clear();
		//center = center - (ray * diagonal);						// hack

		ray_octree_traversal(root, ray, center, octrees, o_min, o_max);
		//ray_octree_traversal2(root, ray, center, octrees);

		// create triangle list
		LinkedList<Octree>::Cell<Octree>* tmp = octrees->start;
		while(tmp != NULL)
		{
			for(unsigned int i = 0; i < tmp->data->count; i++)
			{
				if(faces->GetSize() >= prealocated_space)
					break;
				//if(!faces->Contains(tmp->data->triangles[i]))
				faces->Insert(tmp->data->triangles[i]);
			}
			if(faces->GetSize() >= prealocated_space)
				break;
			tmp = tmp->next;
		}
		//delete octrees;								// bez prealokovania
		return faces;
	}

	void CSDFController::ComputeTNB(Face* tmp, Vector4& tang, Vector4& binor, Vector4& norm)
	{
		// compute tanget space matrix
		/*Vector4 U = Vector4(tmp->v[1]->P - tmp->v[0]->P);
		Vector4 V = Vector4(tmp->v[2]->P - tmp->v[0]->P);
		norm = (U % V) * (-1.0);
		norm.Normalize();*/
		norm = tmp->normal * (-1.0);

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

	int CSDFController::first_node2(Vector4 t0, Vector4 tm)
	{
		unsigned char answer = 0;   // initialize to 00000000
		// select the entry plane and set bits
		if(t0.X > t0.Y)
		{
			if(t0.X > t0.Z)					// PLANE YZ
			{
				if(tm.Y < t0.X) answer|=2;    // set bit at position 1
				if(tm.Z < t0.X) answer|=1;    // set bit at position 0
				return (int) answer;
			}  
		} else
		{      
			if(t0.Y > t0.Z)					// PLANE XZ
			{
				if(tm.X < t0.Y) answer|=4;    // set bit at position 2
				if(tm.Z < t0.Y) answer|=1;    // set bit at position 0
				return (int) answer;
			}
		}
		// PLANE XY
		if(tm.X < t0.Z) answer|=4;			// set bit at position 2
		if(tm.Y < t0.Z) answer|=2;			// set bit at position 1
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

	void CSDFController::proc_subtree (unsigned char idx, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		if(node == NULL)
			return;

		float txm, tym, tzm;
		int currNode;

		if((tx1 < 0.0) || (ty1 < 0.0) || (tz1 < 0.0)) return;

		if(node->isLeaf)
		{
			//loggger->logInfo("Reached leaf node");
			if(node->count > 0)
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
				proc_subtree(idx, tx0,ty0,tz0,txm,tym,tzm,node->son[idx], octrees);
				currNode = new_node(txm,4,tym,2,tzm,1);
				break;}
			case 1: {
				proc_subtree(idx, tx0,ty0,tzm,txm,tym,tz1,node->son[1^idx], octrees);
				currNode = new_node(txm,5,tym,3,tz1,8);
				break;}
			case 2: {
				proc_subtree(idx, tx0,tym,tz0,txm,ty1,tzm,node->son[2^idx], octrees);
				currNode = new_node(txm,6,ty1,8,tzm,3);
				break;}
			case 3: {
				proc_subtree(idx, tx0,tym,tzm,txm,ty1,tz1,node->son[3^idx], octrees);
				currNode = new_node(txm,7,ty1,8,tz1,8);
				break;}
			case 4: {
				proc_subtree(idx, txm,ty0,tz0,tx1,tym,tzm,node->son[4^idx], octrees);
				currNode = new_node(tx1,8,tym,6,tzm,5);
				break;}
			case 5: {
				proc_subtree(idx, txm,ty0,tzm,tx1,tym,tz1,node->son[5^idx], octrees);
				currNode = new_node(tx1,8,tym,7,tz1,8);
				break;}
			case 6: {
				proc_subtree(idx, txm,tym,tz0,tx1,ty1,tzm,node->son[6^idx], octrees);
				currNode = new_node(tx1,8,ty1,8,tzm,7);
				break;}
			case 7: {
				proc_subtree(idx, txm,tym,tzm,tx1,ty1,tz1,node->son[7^idx], octrees);
				currNode = 8;
				break;}
			}
		} while (currNode < 8);
	}

	void CSDFController::ray_octree_traversal(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees, Vector4 o_min, Vector4 o_max)
	{
		unsigned char idx = 0;

		// avoid division by zero
		float Bias = 0.0001f;
		if (fabs(ray.X) < Bias) ray.X = ray.X < 0.0 ? -Bias : Bias;
		if (fabs(ray.Y) < Bias) ray.Y = ray.Y < 0.0 ? -Bias : Bias;
		if (fabs(ray.Z) < Bias) ray.Z = ray.Z < 0.0 ? -Bias : Bias;

		float invdirx = 1.0f / fabs(ray.X);
		float invdiry = 1.0f / fabs(ray.Y);
		float invdirz = 1.0f / fabs(ray.Z);

		float tx0 ,tx1, ty0, ty1, tz0, tz1;

		// fixes for rays with negative direction
		if(ray.X < 0.0)
		{
			tx0 = (o_max.X - Center.X) * -invdirx;
			tx1 = (o_min.X - Center.X) * -invdirx;
			idx |= 4 ; //bitwise OR (latest bits are XYZ)
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
			idx |= 2 ;
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
			idx |= 1 ;
		}
		else
		{
			tz0 = (o_min.Z - Center.Z) * invdirz;
			tz1 = (o_max.Z - Center.Z) * invdirz;
		}

		if( max(max(tx0,ty0),tz0) < min(min(tx1,ty1),tz1) )
		{ 
//			proc_subtree(idx, tx0,ty0,tz0,tx1,ty1,tz1,octree, octrees);
			proc_subtree4(idx, Vector4(tx0,ty0,tz0),Vector4(tx1,ty1,tz1),octree, octrees);
		}
	}
	void CSDFController::ray_octree_traversal2(Octree* octree, Vector4 ray, Vector4 Center, LinkedList<Octree>* octrees)
	{
		proc_subtree3(Center,Center,ray,octree, octrees);
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
	void CSDFController::proc_subtree2 (unsigned char idx, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, Octree* node, LinkedList<Octree>* octrees)
	{
		if(node == NULL)
			return;

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
					stateLookUp = FLD_NODE ^ idx;
					case1 = FRD_NODE; case2 = FLT_NODE; case3 = BLD_NODE;
					break;
				case BLD_NODE:
					newState.tx0 = state.tx0; newState.ty0 = state.ty0; newState.tz0 = tzm;
					newState.tx1 = txm;		  newState.ty1 = tym;		newState.tz1 = state.tz1;										
					stateLookUp = BLD_NODE ^ idx;
					case1 = BRD_NODE; case2 = BLT_NODE;
					break;
				case FLT_NODE:
					newState.tx0 = state.tx0; newState.ty0 = tym;		newState.tz0 = state.tz0;
					newState.tx1 = txm;		  newState.ty1 = state.ty1; newState.tz1 = tzm;							
					stateLookUp = FLT_NODE ^ idx;
					case1 = FRT_NODE; case3 = BLT_NODE;
					break;
				case BLT_NODE:
					newState.tx0 = state.tx0; newState.ty0 = tym;		newState.tz0 = tzm;
					newState.tx1 = txm;		  newState.ty1 = state.ty1; newState.tz1 = state.tz1;							
					stateLookUp = BLT_NODE ^ idx;
					case1 = BRT_NODE;
					break;
				case FRD_NODE:
					newState.tx0 = txm;		  newState.ty0 = state.ty0; newState.tz0 = state.tz0;
					newState.tx1 = state.tx1; newState.ty1 = tym;		newState.tz1 = tzm;						
					stateLookUp = FRD_NODE ^ idx;
					case2 = FRT_NODE; case3 = BRD_NODE;
					break;
				case BRD_NODE:
					newState.tx0 = txm;		  newState.ty0 = state.ty0; newState.tz0 = tzm;
					newState.tx1 = state.tx1; newState.ty1 = tym;		newState.tz1 = state.tz1;						
					stateLookUp = BRD_NODE ^ idx;
					case2 = BRT_NODE;
					break;
				case FRT_NODE:
					newState.tx0 = txm;		  newState.ty0 = tym;		newState.tz0 = state.tz0;
					newState.tx1 = state.tx1; newState.ty1 = state.ty1; newState.tz1 = tzm;						
					stateLookUp = FRT_NODE ^ idx;
					case3 = BRT_NODE;
					break;
				case BRT_NODE:
					newState.tx0 = txm;		  newState.ty0 = tym;		newState.tz0 = tzm;
					newState.tx1 = state.tx1; newState.ty1 = state.ty1; newState.tz1 = state.tz1;						
					stateLookUp = BRT_NODE ^ idx;
					break;
			}

			currNode = new_node(newState.tx1, case1, 
								newState.ty1, case2,
								newState.tz1, case3);
			proc_subtree2(idx, newState.tx0, newState.ty0, newState.tz0, newState.tx1, newState.ty1, newState.tz1,node->son[stateLookUp], octrees);
		} while (currNode < 8);
	}

	bool CSDFController::CheckError(int err, char extra_debug[32768])
	{
		switch(err)
		{
			case 0: return true;
			case 1: loggger->logInfo("Error: Failed to find a platform!"); return false;
			case 2: loggger->logInfo("Error: Failed to create a device group!"); return false;
			case 3: loggger->logInfo("Error: Failed to create a compute context!"); return false;
			case 4: loggger->logInfo("Error: Failed to create a command commands!"); return false;
			case 5: loggger->logInfo("Error: Failed to create compute program!"); return false;
			case 6: loggger->logInfo("Error: Failed to build program executable!"); loggger->logInfo(extra_debug); return false;
			case 7: loggger->logInfo("Error: Failed to create compute kernel!"); return false;
			case 8: loggger->logInfo("Error: Failed to get device info!"); return false;
			case 9: loggger->logInfo("Error: Failed to allocate memory!"); return false;
			case 10: loggger->logInfo("Error: Failed to write to source array!"); return false;
			case 11: loggger->logInfo("Error: Failed to set kernel arguments!"); return false;
			case 12: loggger->logInfo("Error: Failed to execute kernel!"); return false;
			case 13: loggger->logInfo("Error: Failed to Load .cl file!"); return false;
			case 14: loggger->logInfo("Error: Failed to read from source array!"); return false;
			default: return true;
		}
	}

	void CSDFController::UniformPointsOnSphere(float* rndx, float * rndy)
	{
		unsigned int n_rays = Nastavenia->SDF_Rays;
		float N = (float)n_rays;

		float inc = (float)M_PI * (3.0f - sqrt(5.0f));
		float del = (360.0f / Nastavenia->SDF_Cone);
		float off = (2.0f / N) / del;
		for(unsigned int k = 0; k < n_rays; k++)
		{
			float y = k * off - 1 + (off / 2.0f) * del;
			float r = sqrt(1 - y*y);
			float phi = k * inc;
			Vector4 ray = Vector4(cos(phi)*r, -y, sin(phi)*r);
			ray.Normalize();
			CalcAnglesFromRay(ray, rndx[k], rndy[k]);
			GetDegrees(rndx[k], rndy[k]);
			if(rndy[k] < 0.5)
				rndy[k] = 0.5;
		}
	}

	void CSDFController::RandomPointsOnSphere(float* rndx, float * rndy)
	{
		unsigned int n_rays = Nastavenia->SDF_Rays;
		// precompute those N rays
		srand (123);											// initial seed for random number generator
		for(unsigned int i = 0; i < n_rays; i++)
		{
			rndy[i] = float(rand()%int(Nastavenia->SDF_Cone / 2));
			rndx[i] = float(rand()%(360));
			if(rndy[i] < 0.5)
				rndy[i] = 0.5;
		}
	}
	void CSDFController::InitKernel()
	{
		kernel_size = Nastavenia->SDF_Smoothing_Radius;
		gauss_sus = new LinkedList<Face>*[kernel_size + 1];
		// preallocate smoothing arrays
		for(int i=0; i <= kernel_size; i++)				// bacha na posunutie
		{
			gauss_sus[i] = new LinkedList<Face>();
			switch(i)
			{
				case 0: gauss_sus[i]->Preallocate(1); break;
				case 1: gauss_sus[i]->Preallocate(10); break;
				case 2: gauss_sus[i]->Preallocate(100); break;
				default: gauss_sus[i]->Preallocate(1000); break;
			}
		}
	}
	void CSDFController::EraseKernel()
	{
		// delete smoothing arrays
		for(int i=0; i <= kernel_size; i++)
		{
			delete gauss_sus[i];
		}
		delete [] gauss_sus;
	}



	typedef unsigned char       U8;
	typedef unsigned short      U16;
	typedef unsigned int        U32;
	typedef signed char         S8;
	typedef signed short        S16;
	typedef signed int          S32;
	typedef float               F32;
	typedef double              F64;
	typedef unsigned long       U64;
	typedef signed long         S64;
	typedef void                (*FuncPtr)(void);
	int c_popc8LUT[] =
	{
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
	};
	inline int popc8(U32 mask)
	{
		//((void)0);
		return c_popc8LUT[mask];
	}
	int __float_as_int(float in)
	{
		union fi { int i; float f; } conv;
		conv.f = in;
		return conv.i;
	}
	float __int_as_float(int a)
	{
		union {int a; float b;} u;
		u.a = a;
		return u.b;
	}
	#define CAST_STACK_DEPTH        23
	#define MAX_RAYCAST_ITERATIONS  10000
	struct cll_float3
	{
		float       x;
		float       y;
		float       z;
	};
	struct cll_int2
	{
		int       x;
		int       y;
	};
	cll_float3 make_float3(float xx, float yy, float zz)
	{
		cll_float3 result;
		result.x = xx;
		result.y = yy;
		result.z = zz;
		return result;
	};
	cll_int2 make_int2(int xx, int yy)
	{
		cll_int2 result;
		result.x = xx;
		result.y = yy;
		return result;
	};
	struct Ray
	{
		cll_float3      orig;
		cll_float3      orig_real;
		cll_float3      dir;
	};
	struct CastResult
	{
		float       t;
		cll_float3      pos;
		int         iter;

		void*        node;
		int         childIdx;
		int         stackPtr;
	};

	//------------------------------------------------------------------------

	class CastStack
	{
	public:
		CastStack   (void)
		{
			for(int i = 0; i < 24; i++)
			{
				tstack[i] = 0;
				ostack[i] = NULL;
			}
		}

		//Octree* read        (int idx, F32& tmax) const      { U64 e = stack[idx]; tmax = __int_as_float((U32)(e >> 32)); return (Octree*)(U32)e; }
		//void write       (int idx, Octree* node, F32 tmax)  { stack[idx] = (U32)node | ((U64)__float_as_int(tmax) << 32); }
		Octree* read     (int idx, F32& tmax) const         { tmax = tstack[idx]; return ostack[idx]; }
		void write       (int idx, Octree* node, F32 tmax)  { ostack[idx] = node; tstack[idx] = tmax; }

	private:
		F32             tstack[24];
		Octree*         ostack[24];
	};
	float copysignf(float a, float b)
	{
		if(b < 0)
			return -a;
		else
			return a;
	};
	float fmaxf(float a, float b)
	{
		if(a > b)
			return a;
		else
			return b;
	};
	float fminf(float a, float b)
	{
		if(a < b)
			return a;
		else
			return b;
	};
	void CSDFController::proc_subtree3 (Vector4 o, Vector4 or, Vector4 d, Octree* node, LinkedList<Octree>* octrees)
	{
		Ray ray;
		ray.orig.x = o.X; ray.orig.y = o.Y; ray.orig.z = o.Z;
		ray.orig_real.x = or.X; ray.orig_real.y = or.Y; ray.orig_real.z = or.Z;
		ray.dir.x = d.X; ray.dir.y = d.Y; ray.dir.z = d.Z;

		CastStack stack;
		const float epsilon = 0.000001f;
		//float ray_orig_sz = ray.orig_sz;
		int iter = 0;

		// Get rid of small ray direction components to avoid division by zero.

		if (fabsf(ray.dir.x) < epsilon) ray.dir.x = copysignf(epsilon, ray.dir.x);
		if (fabsf(ray.dir.y) < epsilon) ray.dir.y = copysignf(epsilon, ray.dir.y);
		if (fabsf(ray.dir.z) < epsilon) ray.dir.z = copysignf(epsilon, ray.dir.z);

		// Precompute the coefficients of tx(x), ty(y), and tz(z).
		// The octree is assumed to reside at coordinates [1, 2].

		float tx_coef = 1.0f / -fabs(ray.dir.x);
		float ty_coef = 1.0f / -fabs(ray.dir.y);
		float tz_coef = 1.0f / -fabs(ray.dir.z);

		float tx_bias = tx_coef * ray.orig.x;
		float ty_bias = ty_coef * ray.orig.y;
		float tz_bias = tz_coef * ray.orig.z;

		// Select octant mask to mirror the coordinate system so
		// that ray direction is negative along each axis.

		int octant_mask = 7;
		if (ray.dir.x > 0.0f) octant_mask ^= 1, tx_bias = 3.0f * tx_coef - tx_bias;
		if (ray.dir.y > 0.0f) octant_mask ^= 2, ty_bias = 3.0f * ty_coef - ty_bias;
		if (ray.dir.z > 0.0f) octant_mask ^= 4, tz_bias = 3.0f * tz_coef - tz_bias;

		// Initialize the active span of t-values.

		float t_min = fmaxf(fmaxf(2.0f * tx_coef - tx_bias, 2.0f * ty_coef - ty_bias), 2.0f * tz_coef - tz_bias);
		float t_max = fminf(fminf(tx_coef - tx_bias, ty_coef - ty_bias), tz_coef - tz_bias);
		float h = t_max;
		t_min = fmaxf(t_min, 0.0f);
		t_max = fminf(t_max, 1.0f);

		// Initialize the current voxel to the first child of the root.
		// sme prvym synom roota, tj parent = m_root, zistime ktory sme az neskor
		Octree*   parent        = node;
		int child_descriptor = 0; // invalid until fetched
		int    idx              = 0;
		cll_float3 pos          = make_float3(1.0f, 1.0f, 1.0f);
		int    scale            = CAST_STACK_DEPTH - 1;
		float  scale_exp2       = 0.5f; // exp2f(scale - s_max)

		if (1.5f * tx_coef - tx_bias > t_min) idx ^= 1, pos.x = 1.5f;
		if (1.5f * ty_coef - ty_bias > t_min) idx ^= 2, pos.y = 1.5f;
		if (1.5f * tz_coef - tz_bias > t_min) idx ^= 4, pos.z = 1.5f;

		// Traverse voxels along the ray as long as the current voxel
		// stays within the octree.

		while (scale < CAST_STACK_DEPTH)
		{
			iter++;
			if (iter > MAX_RAYCAST_ITERATIONS)
				break;

			// Fetch child descriptor unless it is already valid.
			// nacitame si hodnoty nasho otca, tj link na prveho syna a ci su synovia validny/listnaty
			if (child_descriptor == 0)
			{
				//child_descriptor = (*parent)<<24;
				child_descriptor = parent->sons;
			}
			if (child_descriptor == 0)
			{
				//sme v leafe
				// ray intersect
				octrees->InsertToEnd(parent);
			}
			// Determine maximum t-value of the cube by evaluating
			// tx(), ty(), and tz() at its corner.

			float tx_corner = pos.x * tx_coef - tx_bias;
			float ty_corner = pos.y * ty_coef - ty_bias;
			float tz_corner = pos.z * tz_coef - tz_bias;
			float tc_max = fminf(fminf(tx_corner, ty_corner), tz_corner);

			// Process voxel if the corresponding bit in valid mask is set
			// and the active t-span is non-empty.

			int child_shift = idx ^ octant_mask; // permute child slots based on the mirroring
			int child_masks = child_descriptor << child_shift;
			if (((child_masks & 0x0080) != 0) && (t_min <= t_max))
			{
				// Terminate if the voxel is small enough.

				/*if (tc_max * ray.dir_sz + ray_orig_sz >= scale_exp2)
					break; // at t_min*/

				// INTERSECT
				// Intersect active t-span with the cube and evaluate
				// tx(), ty(), and tz() at the center of the voxel.

				float tv_max = fminf(t_max, tc_max);
				float half = scale_exp2 * 0.5f;
				float tx_center = half * tx_coef + tx_corner;
				float ty_center = half * ty_coef + ty_corner;
				float tz_center = half * tz_coef + tz_corner;

				// Descend to the first child if the resulting t-span is non-empty.

				if (t_min <= tv_max)
				{
					// Terminate if the corresponding bit in the non-leaf mask is not set.
					// sme v dakom leafe
					/*if (child_descriptor == 0)
						break; // at t_min (overridden with tv_min).*/

					// PUSH
					// Write current parent to the stack.

					if (tc_max < h)
					{
						stack.write(scale, parent, t_max);
					}
					h = tc_max;

					// Find child descriptor corresponding to the current voxel.

					/*int ofs = (unsigned int)(*parent) >> 8; // child pointer
					ofs += popc8((child_masks<<8) & 0x7F);
					parent += ofs;*/
					int ofs = popc8((child_masks) & 0x7F);
					parent = parent->son[ofs];

					// Select child voxel that the ray enters first.

					idx = 0;
					scale--;
					scale_exp2 = half;

					if (tx_center > t_min) idx ^= 1, pos.x += scale_exp2;
					if (ty_center > t_min) idx ^= 2, pos.y += scale_exp2;
					if (tz_center > t_min) idx ^= 4, pos.z += scale_exp2;

					// Update active t-span and invalidate cached child descriptor.

					t_max = tv_max;
					child_descriptor = 0;
					continue;
				}
			}

			// ADVANCE
			// Step along the ray.


			int step_mask = 0;
			if (tx_corner <= tc_max) step_mask ^= 1, pos.x -= scale_exp2;
			if (ty_corner <= tc_max) step_mask ^= 2, pos.y -= scale_exp2;
			if (tz_corner <= tc_max) step_mask ^= 4, pos.z -= scale_exp2;

			// Update active t-span and flip bits of the child slot index.

			t_min = tc_max;
			idx ^= step_mask;

			// Proceed with pop if the bit flips disagree with the ray direction.

			if ((idx & step_mask) != 0)
			{
				// POP
				// Find the highest differing bit between the two positions.

				unsigned int differing_bits = 0;
				if ((step_mask & 1) != 0) differing_bits |= __float_as_int(pos.x) ^ __float_as_int(pos.x + scale_exp2);
				if ((step_mask & 2) != 0) differing_bits |= __float_as_int(pos.y) ^ __float_as_int(pos.y + scale_exp2);
				if ((step_mask & 4) != 0) differing_bits |= __float_as_int(pos.z) ^ __float_as_int(pos.z + scale_exp2);
				scale = (__float_as_int((float)differing_bits) >> 23) - 127; // position of the highest bit
				scale_exp2 = __int_as_float((scale - CAST_STACK_DEPTH + 127) << 23); // exp2f(scale - s_max)

				// Restore parent voxel from the stack.

				parent = stack.read(scale, t_max);

				// Round cube position and extract child slot index.

				int shx = __float_as_int(pos.x) >> scale;
				int shy = __float_as_int(pos.y) >> scale;
				int shz = __float_as_int(pos.z) >> scale;
				pos.x = __int_as_float(shx << scale);
				pos.y = __int_as_float(shy << scale);
				pos.z = __int_as_float(shz << scale);
				idx  = (shx & 1) | ((shy & 1) << 1) | ((shz & 1) << 2);

				// Prevent same parent from being stored again and invalidate cached child descriptor.

				h = 0.0f;
				child_descriptor = 0;
			}
		}
	}

	bool CSDFController::CheckValid(int mask, int num)
	{
		return((mask >> num) & 1);
	}
	void CSDFController::proc_subtree4 (unsigned char idx, Vector4 t0, Vector4 t1, Octree* node, LinkedList<Octree>* octrees)
	{
		Vector4 tm = Vector4();
		int currNode = -1;
		CastStackx stack;
		int scale = 0;
		while (true)
		{
			// konec toho spodneho switchu, berem otca
			if(currNode == 8)
			{
				if(scale > 0)
				{
					scale--;
					node = stack.read(scale, t0, t1, currNode);
					continue;
				}
				else
					break;
			}
			// mame novy nody
			else if(currNode == -1)
			{
				if((t1.X < 0.0) || (t1.Y < 0.0) || (t1.Z < 0.0))
				{
					if(scale > 0)
					{
						scale--;
						node = stack.read(scale, t0, t1, currNode);
						continue;
					}
					else
						break;
				}
				unsigned char synovia = node->sons;
				if(synovia == 0)
				{
					//loggger->logInfo("Reached leaf node");
					//if(node->count > 0)
					octrees->InsertToEnd(node);

					scale--;
					node = stack.read(scale, t0, t1, currNode);
					continue;
				}
				tm = 0.5*(t0 + t1);
				
				currNode = first_node2(t0,tm);
			}
			else
				tm = 0.5*(t0 + t1);

			switch (currNode)
			{
			case 0:
				if(CheckValid(node->sons, idx))
				{
					currNode = new_node(tm.X,4,tm.Y,2,tm.Z,1);
					stack.write(scale, t0, t1, currNode, node);
					t1 = tm;
					//proc_subtree4(tx0,ty0,tz0,txm,tym,tzm,node->son[aa], octrees);
					//currNode = new_node(txm,4,tym,2,tzm,1);
					currNode = -1;
					node = node->son[idx];
				}
				else
				{
					currNode = new_node(tm.X,4,tm.Y,2,tm.Z,1);
					scale--;
				}
				break;
			case 1: 
				if(CheckValid(node->sons, 1^idx))
				{
					currNode = new_node(tm.X,5,tm.Y,3,t1.Z,8);
					stack.write(scale, t0, t1, currNode, node);
					t0.Z = tm.Z;
					t1.X = tm.X;
					t1.Y = tm.Y;
					//proc_subtree4(tx0,ty0,tzm,txm,tym,tz1,node->son[1^aa], octrees);
					//currNode = new_node(txm,5,tym,3,tz1,8);
					currNode = -1;
					node = node->son[1^idx];
				}
				else
				{
					currNode = new_node(tm.X,5,tm.Y,3,t1.Z,8);
					scale--;
				}
				break;
			case 2:
				if(CheckValid(node->sons, 2^idx))
				{
					currNode = new_node(tm.X,6,t1.Y,8,tm.Z,3);
					stack.write(scale, t0, t1, currNode, node);
					t0.Y = tm.Y;
					t1.X = tm.X;
					t1.Z = tm.Z;
					//proc_subtree4(tx0,tym,tz0,txm,ty1,tzm,node->son[2^aa], octrees);
					//currNode = new_node(txm,6,ty1,8,tzm,3);
					currNode = -1;
					node = node->son[2^idx];
				}
				else
				{
					currNode = new_node(tm.X,6,t1.Y,8,tm.Z,3);
					scale--;
				}
				break;
			case 3:
				if(CheckValid(node->sons, 3^idx))
				{
					currNode = new_node(tm.X,7,t1.Y,8,t1.Z,8);
					stack.write(scale, t0, t1, currNode, node);
					t0.Y = tm.Y;
					t0.Z = tm.Z;
					t1.X = tm.X;
					//proc_subtree4(tx0,tym,tzm,txm,ty1,tz1,node->son[3^aa], octrees);
					//currNode = new_node(txm,7,ty1,8,tz1,8);
					currNode = -1;
					node = node->son[3^idx];
				}
				else
				{
					currNode = new_node(tm.X,7,t1.Y,8,t1.Z,8);
					scale--;
				}
				break;
			case 4:
				if(CheckValid(node->sons, 4^idx))
				{
					currNode = new_node(t1.X,8,tm.Y,6,tm.Z,5);
					stack.write(scale, t0, t1, currNode, node);
					t0.X = tm.X;
					t1.Y = tm.Y;
					t1.Z = tm.Z;
					//proc_subtree4(txm,ty0,tz0,tx1,tym,tzm,node->son[4^aa], octrees);
					//currNode = new_node(tx1,8,tym,6,tzm,5);
					currNode = -1;
					node = node->son[4^idx];
				}
				else
				{
					currNode = new_node(t1.X,8,tm.Y,6,tm.Z,5);
					scale--;
				}
				break;
			case 5:
				if(CheckValid(node->sons, 5^idx))
				{
					currNode = new_node(t1.X,8,tm.Y,7,t1.Z,8);
					stack.write(scale, t0, t1, currNode, node);
					t0.X = tm.X;
					t0.Z = tm.Z;
					t1.Y = tm.Y;
					//proc_subtree4(txm,ty0,tzm,tx1,tym,tz1,node->son[5^aa], octrees);
					//currNode = new_node(tx1,8,tym,7,tz1,8);
					currNode = -1;
					node = node->son[5^idx];
				}
				else
				{
					currNode = new_node(t1.X,8,tm.Y,7,t1.Z,8);
					scale--;
				}
				break;
			case 6:
				if(CheckValid(node->sons, 6^idx))
				{
					currNode = new_node(t1.X,8,t1.Y,8,tm.Z,7);
					stack.write(scale, t0, t1, currNode, node);
					t0.X = tm.X;
					t0.Y = tm.Y;
					t1.Z = tm.Z;
					//proc_subtree4(txm,tym,tz0,tx1,ty1,tzm,node->son[6^aa], octrees);
					//currNode = new_node(tx1,8,ty1,8,tzm,7);
					currNode = -1;
					node = node->son[6^idx];
				}
				else
				{
					currNode = new_node(t1.X,8,t1.Y,8,tm.Z,7);
					scale--;
				}
				break;
			case 7:
				if(CheckValid(node->sons, 7^idx))
				{
					currNode = 8;
					stack.write(scale, t0, t1, currNode, node);
					t0.X = tm.X;
					t0.Y = tm.Y;
					t0.Z = tm.Z;
					//proc_subtree4(txm,tym,tzm,tx1,ty1,tz1,node->son[7^aa], octrees);
					//currNode = 8;
					currNode = -1;
					node = node->son[7^idx];
				}
				else
				{
					currNode = 8;
					scale--;
				}
				break;
			}
			
			scale++;
		}
	}

	void CSDFController::ComputeOpenCL2(LinkedList<Vertex>* points, LinkedList<Face>* triangles, Octree* root, Vector4 o_min, Vector4 o_max, unsigned int nodeCount, unsigned int leafCount, unsigned int triangleCount)
	{
		using namespace OpenCLForm;
		int ticks1 = GetTickCount();

		//-------------------------------------------
		//---------------INIT OpenCL-------Begin-----
		//-------------------------------------------
		COpenCL* OpenCLko = new COpenCL();

		int err = EXIT_SUCCESS;
		err = OpenCLko->InitOpenCL();
		if(!CheckError(err)) return;

		err = OpenCLko->LoadKernel("sdf2.cl");
		if(!CheckError(err)) return;

		err = OpenCLko->BuildKernel();
		if(!CheckError(err, OpenCLko->debug_buffer)) return;

		err = OpenCLko->GetGPUVariables();
		if(!CheckError(err)) return;

		//-------------------------------------------
		//---------------INIT OpenCL-------End-------
		//-------------------------------------------


		const unsigned int n_rays = Nastavenia->SDF_Rays;
		const unsigned int n_triangles = triangles->GetSize();
		const unsigned int n_nodes = nodeCount;
		const unsigned int n_leaves = leafCount;
		const unsigned int n_node_tria = triangleCount;
		//unsigned int n_vertices = points->GetSize();

		OpenCLko->global = (n_triangles * n_rays);

		//-------------------------------------------
		//---------------Memory Alloc------Begin-----
		//-------------------------------------------

		cl_float4	*c_triangles;			// zoznam trojuholnikov obsahujucich 3 vertexy
		cl_uint		*c_nodes;				// zoznam nodov v octree
		cl_uint		*c_node_tria;			// zoznam trojuholnikov v nodoch v octree
		cl_float4	*c_rays;				// zoznamy 30 lucov, ktore sa postupne vkladaju do OpenCL
		cl_float	*c_outputs;			// vzdialenost a vaha pre kazdy luc, ktore je mojim vysledkom co si zapisem

		unsigned int s_triangles = n_triangles * 3 * sizeof(cl_float4);					// pocet trojuholnikov * 4 * 3 vertexy * float
		unsigned int s_nodes = n_nodes * sizeof(cl_uint);								// pocet nodov * int
		unsigned int s_node_tria = (n_leaves +  n_node_tria) * sizeof(cl_uint);			// (pocet nodov (velkosti) + pocet trojuholnikov) * int
		unsigned int s_rays = n_rays * sizeof(cl_float4);								// 30 * 4 * float
		unsigned int s_outputs = n_triangles * n_rays * sizeof(cl_float);				// trojuholniky * 30  * float

		int ticks2 = GetTickCount();
		err = OpenCLko->SetupMemory2(s_triangles, s_nodes, s_node_tria, s_rays, s_outputs);
		if(!CheckError(err)) return;
		int ticks3 = GetTickCount();

		c_triangles = (cl_float4*) malloc(s_triangles);
		c_nodes = (cl_uint*) malloc(s_nodes);//o_array;
		c_node_tria = (cl_uint*) malloc(s_node_tria);//t_array;
		c_rays = (cl_float4*) malloc(s_rays);
		c_outputs = (cl_float*) malloc(s_outputs);

		//-------------------------------------------
		//---------------Memory Alloc------End-------
		//-------------------------------------------

		//------------------prealocated variables------------------	
		float *weights = new float[n_rays];

		Vector4 tangens, normal, binormal;
		Mat4 t_mat;

		float* rndy = new float[n_rays];
		float* rndx = new float[n_rays];
		if(Nastavenia->SDF_Distribution == true)
			UniformPointsOnSphere(rndx, rndy);
		else
			RandomPointsOnSphere(rndx, rndy);

		for(unsigned int i = 0; i < n_rays; i++)
		{
			Vector4 ray = CalcRayFromAngle(rndx[i], rndy[i]);
			ray.Normalize();
			c_rays[i].s[0] = ray.X;
			c_rays[i].s[1] = ray.Y;
			c_rays[i].s[2] = ray.Z;
			c_rays[i].s[3] = 0.0f;
			weights[i] = 180.0f - rndy[i];
		}
		delete [] rndy;
		delete [] rndx;
		//------------------prealocated variables------------------

		// -----------------------------------------------
		// vypocitaj dopredu zoznamy trojuholnikov a lucov
		// -----------------------------------------------
		int ticks4 = GetTickCount();
		//---------------copy variables--------------
		unsigned int pocet = 0;
		LinkedList<Face>::Cell<Face>* tmp_face = triangles->start;
		while(tmp_face != NULL)
		{
			for(int ii = 0; ii < 3; ii++)
			{
				c_triangles[pocet].s[0] = tmp_face->data->v[ii]->P.X;
				c_triangles[pocet].s[1] = tmp_face->data->v[ii]->P.Y;
				c_triangles[pocet].s[2] = tmp_face->data->v[ii]->P.Z;
				c_triangles[pocet].s[3] = tmp_face->data->v[ii]->P.W;
				pocet++;
			}
			tmp_face = tmp_face->next;
		}


		{
			Octree** oc_array = new Octree*[nodeCount];

			Octree* node = root;
			unsigned int end = 0;
			oc_array[0] = node;
			unsigned int tidx = 0;
			bool jeden_krat = true;
			for(unsigned int idx = 0; idx < nodeCount; idx++)
			{
				node = oc_array[idx];
				if(node->isLeaf)
				{
					unsigned int safe_tidx = tidx << 8;
					c_nodes[idx] = safe_tidx;
					c_node_tria[tidx] = node->count;
					tidx++;
					for(unsigned int j = 0; j < node->count; j++)
					{
						c_node_tria[tidx] = node->triangles[j]->number;
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
							c_nodes[idx] = safe_end;
							jeden_krat = false;
						}
						oc_array[end] = node->son[i];
					}
				}
			}
			delete [] oc_array;
		}
		//---------------copy variables--------------
		

		// v tomto bode je uz pamet pripravena a nacitana
		// je nutne poslat ju do OpenCL a zahajit vypocet
		int ticks5 = GetTickCount();
		cl_float4 oo_min; oo_min.s[0] = o_min.X; oo_min.s[1] = o_min.Y; oo_min.s[2] = o_min.Z; oo_min.s[3] = o_min.W;
		cl_float4 oo_max; oo_max.s[0] = o_max.X; oo_max.s[1] = o_max.Y; oo_max.s[2] = o_max.Z; oo_max.s[3] = o_max.W;
		cl_float bias = root->size * 2.0f * 0.00001f;

		OpenCLko->debugger->max_outputs = n_triangles * n_rays;
		OpenCLko->debugger->nodeCount = n_nodes;
		OpenCLko->debugger->triangleCount = n_leaves +  n_node_tria;
		OpenCLko->debugger->nn_triangles = n_triangles * 3;

		err = OpenCLko->LaunchKernel2(c_triangles, c_nodes, c_node_tria, oo_min, oo_max, bias, c_rays, n_rays, n_triangles, c_outputs);
		if(!CheckError(err)) return;

		OpenCLko->WaitForFinish();

		int ticks6 = GetTickCount();

		// spracuj ziskane hodnoty
		float min = FLOAT_MAX;
		float max = 0.0f;
		float dist = 0.0f;
		std::vector<float> rays;
		std::vector<float> weightsx;
		LinkedList<Face>::Cell<Face>* current_face = triangles->start;
		for(unsigned int ii = 0; ii < n_triangles; ii++)
		{
			for(unsigned int kk = 0; kk < n_rays; kk++)
			{
				dist = c_outputs[(ii * n_rays) + kk];
				//loggger->logInfo(MarshalString("triangle: "+i+ " val: " + dist));
				if(dist >= (FLOAT_MAX - 1.0f))
					dist = 0;

				rays.push_back(dist);
				weightsx.push_back(weights[kk]);
			}
			if(rays.size() > 0)
			{
				current_face->data->ComputeSDFValue(rays, weightsx);
				if(current_face->data->quality->value < min)
					min = current_face->data->quality->value;
				if(current_face->data->quality->value > max)
					max = current_face->data->quality->value;
			}
			current_face = current_face->next;
			rays.clear();
			weightsx.clear();
		}

		int ticks7 = GetTickCount();

		// postprocessing - smoothing and normalization
		//float kernel[] = {1.0,4.0,6.0,4.0,1.0};
		EraseKernel();
		InitKernel();
		float* kernel = ComputeGaussianKernel(kernel_size);
		current_face = triangles->start;
		while(current_face != NULL)
		{
			Smooth(current_face->data, kernel, kernel_size);
			current_face->data->quality->Normalize(min, max, 4.0);
			// pokus pouzit diagonalu, pripadne avg a pod, ale dako nefungovalo
			//tmp->data->diameter->Normalizex(0, diagonal, 4.0);

			current_face = current_face->next;
		}
		Nastavenia->DEBUG_Min_SDF = min;
		Nastavenia->DEBUG_Max_SDF = max;
		delete kernel;
		int ticks8 = GetTickCount();


		loggger->logInfo(MarshalString("Inicializacia OpenCL: " + (ticks2 - ticks1)+ "ms"));
		loggger->logInfo(MarshalString("Alokovanie pamete v OpenCL: " + (ticks3 - ticks2)+ "ms"));
		loggger->logInfo(MarshalString("Alokovanie pamete v PC: " + (ticks4 - ticks3)+ "ms"));
		loggger->logInfo(MarshalString("Nacitanie zoznamu trojuholnikov: " + (ticks5 - ticks4)+ "ms"));
		loggger->logInfo(MarshalString("!!VYPOCET OpenCL!!: " + (ticks6 - ticks5)+ "ms"));
		loggger->logInfo(MarshalString("Spracovanie: " + (ticks7 - ticks6)+ "ms"));
		loggger->logInfo(MarshalString("Smoothing: " + (ticks8 - ticks7)+ "ms"));
		loggger->logInfo(MarshalString("Celkovy vypocet trval: " + (ticks8 - ticks1)+ "ms, pre " + n_triangles + " trojuholnikov"));
		//loggger->logInfo(MarshalString("pocet: " + pocet));
		//loggger->logInfo(MarshalString("min a max pre SDF su: " + min + ", "+max));
		//loggger->logInfo(MarshalString("nmin a nmax pre SDF su: " + nmin + ", "+nmax));


		// Delete OpenCL to free GPU
		delete OpenCLko;
		delete weights;

		// Free host memory
		free(c_triangles);
		free(c_rays);
		free(c_outputs);
		free(c_nodes);
		free(c_node_tria);
	}
}