// kratky subor pre debugovanie OpenCL kodu
#include "stdafx.h"
#include "OpenCLDebug.h"

namespace OpenCLDebugger
{

	COpenCLDebug::COpenCLDebug()
	{
		args = NULL;
		arg_size = 0;

		nodeCount = 0;
		triangleCount = 0;
		nn_triangles = 0;
		max_outputs = 0;
	}
	
	COpenCLDebug::~COpenCLDebug()
	{
		if(arg_size > 0)
		{
			// maze sa inde
			/*for(int i = 0; i < arg_size; i++)
			{
				delete args[i];
			}*/
			delete [] args;
		}
	}

	void COpenCLDebug::SetArgSize(cl_uint size /* args_size */)
	{
		if(arg_size > 0)
		{
			delete [] args;
		}
		args = new void*[size];
		arg_size = size;
	}
	
	void COpenCLDebug::SetArgValue(cl_uint idx/* arg_index */, void *value /* arg_value */)
	{
		args[idx] = value;
	}
	void COpenCLDebug::ExecuteKernel( size_t global   /* global_work_size */, size_t local   /* local_work_size */)
	{
		//cl_uint kernel_size = (cl_uint)global / (cl_uint)local;
		for(cl_uint i = 0; i < (cl_uint)global; i++)
		{
			sdf((cl_uint3*)args[0], (cl_float4*)args[1], (cl_uint*)args[2], (cl_float4*)args[3], (cl_uint*)args[4], (*(cl_uint4*)(args[5])), (float*)args[6], i);
		}
	}

	void COpenCLDebug::ExecuteKernel2( size_t global   /* global_work_size */, size_t local   /* local_work_size */)
	{
		//cl_uint kernel_size = (cl_uint)global / (cl_uint)local;
		for(cl_uint i = 0; i < (cl_uint)global; i++)
		{
			sdf2((cl_float4*)args[0], (cl_uint*)args[1], (cl_uint*)args[2], (*(cl_float4*)(args[3])), (*(cl_float4*)(args[4])), (*(cl_float*)(args[5])), (cl_float4*)args[6], (*(cl_uint*)(args[7])), (*(cl_uint*)(args[8])), (float*)args[9], i);
		}
	}

	void COpenCLDebug::ExecuteKernel3( size_t global   /* global_work_size */, size_t local   /* local_work_size */)
	{
		for(cl_uint i = 0; i < (cl_uint)global; i++)
		{
			proces((float*)args[0], (float*)args[1], (float*)args[2], (*(cl_uint*)(args[3])), i);
		}
	}

	void COpenCLDebug::ExecuteKernel4( size_t global   /* global_work_size */, size_t local   /* local_work_size */)
	{
		for(cl_uint i = 0; i < (cl_uint)global; i++)
		{
			smooth((cl_float4*)args[0], (cl_float4*)args[1], (cl_uint*)args[2], (cl_float*)(args[3]), (*(cl_float*)(args[4])), (*(cl_uint*)(args[5])), (*(cl_float*)(args[6])), i);
		}
	}

	// pomocne OpenCL funkcie
	cl_float COpenCLDebug::cl_distance(cl_float4 a, cl_float4 b)
	{
		float xd = a.s[0] - b.s[0];
		float yd = a.s[1] - b.s[1];
		float zd = a.s[2] - b.s[2];
		return sqrt(xd*xd + yd*yd  + zd*zd);
	}
	cl_float4 COpenCLDebug::cl_minus(cl_float4 a, cl_float4 b)
	{
		cl_float4 result;
		result.s[0] = a.s[0] - b.s[0];
		result.s[1] = a.s[1] - b.s[1];
		result.s[2] = a.s[2] - b.s[2];
		result.s[3] = a.s[3] - b.s[3];
		return result;
	}
	cl_float4 COpenCLDebug::cl_plus(cl_float4 a, cl_float4 b)
	{
		cl_float4 result;
		result.s[0] = a.s[0] + b.s[0];
		result.s[1] = a.s[1] + b.s[1];
		result.s[2] = a.s[2] + b.s[2];
		result.s[3] = a.s[3] + b.s[3];
		return result;
	}
	cl_float COpenCLDebug::cl_dot(cl_float4 a, cl_float4 b)
	{
		cl_float result;
		result =  a.s[0] * b.s[0] + a.s[1] * b.s[1] + a.s[2] * b.s[2] + a.s[3] * b.s[3];
		return result;
	}
	cl_float4 COpenCLDebug::cl_cross(cl_float4 a, cl_float4 b)
	{
		cl_float4 result;
		result.s[0] = a.s[1] * b.s[2] - a.s[2] * b.s[1];
		result.s[1] = a.s[2] * b.s[0] - a.s[0] * b.s[2];
		result.s[2] = a.s[0] * b.s[1] - a.s[1] * b.s[0];
		result.s[3] = a.s[2];
		return result;
	}
	cl_float4 COpenCLDebug::cl_normalize(cl_float4 a)
	{
		cl_float length = cl_length(a);
		cl_float4 result;
		result.s[0] = a.s[0]/length;
		result.s[1] = a.s[1]/length;
		result.s[2] = a.s[2]/length;
		//result.s[3] = a.s[3]/length;
		return result;
	}
	cl_float COpenCLDebug::cl_length(cl_float4 a)
	{
		return sqrt(a.s[0] * a.s[0] + a.s[1] * a.s[1] + a.s[2] * a.s[2]);
	}
	cl_float4 COpenCLDebug::cl_div(cl_float4 a, cl_float b)
	{
		cl_float4 result;
		result.s[0] = a.s[0]/b;
		result.s[1] = a.s[1]/b;
		result.s[2] = a.s[2]/b;
		result.s[3] = a.s[3]/b;
		return result;
	}
	cl_float4 COpenCLDebug::cl_multiply(cl_float4 a, cl_float b)
	{
		cl_float4 result;
		result.s[0] = a.s[0]*b;
		result.s[1] = a.s[1]*b;
		result.s[2] = a.s[2]*b;
		result.s[3] = a.s[3]*b;
		return result;
	}
	//---------------------------------MOJE OPENCL FUNKCIE---------------------------------
#define FLOAT_MAX  99999.0
#define STACK_SIZE 10
#define MAX_ITERATIONS 1000

	float COpenCLDebug::rayIntersectsTriangle(const cl_float4 p, const cl_float4 d, const cl_float4 v0, const cl_float4 v1, const cl_float4 v2, const cl_float bias)
	{
		cl_float4 e1 = cl_minus(v1, v0);
		cl_float4 e2 = cl_minus(v2, v0);
		float a,f,u,v;

		cl_float4 h = cl_cross(d,e2);
		a = cl_dot(e1,h);

		if (a > -bias && a < bias)
			return FLOAT_MAX;

		f = 1.0f / a;
		cl_float4 s = cl_minus(p, v0);
		u = f * cl_dot(s,h);

		if (u < 0.0f || u > 1.0f)
			return FLOAT_MAX;

		cl_float4 q = cl_cross(s,e1);
		v = f * cl_dot(d,q);

		if (v < 0.0f || u + v > 1.0f)
			return FLOAT_MAX ;

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		f = f * cl_dot(e2,q);

		if (f > bias) // ray intersection (v smere luca)
			return f;

		else // this means that there is a line intersection
				// but not a ray intersection
				return FLOAT_MAX;
	}

	// c_params.s[0] = n_workitems; c_params.s[1] = n_rays; c_params.s[2] = n_prealloc; c_params.s[3] = pocet_trojuholnikov;
	// moja kernel funkcia
	void COpenCLDebug::sdf(const cl_uint3 *c_triangles,
						const cl_float4 *c_vertices,
						const cl_uint *c_origins,
						const cl_float4 *c_rays,
						const cl_uint *c_targets,
						const cl_uint4 c_params,
						float *c_outputs,
						const cl_uint gid
						)
	{
		//const cl_uint gid = get_global_id(0);
		//const int lid = get_local_id(0);
		cl_uint ref_ray = gid;								// zisti kolkaty som luc v poradi
		cl_uint ref_triangle = ref_ray / c_params.s[1];		// na zaklade toho zisti ku ktoremu trojuholniku patrim
	
		if(ref_triangle >= c_params.s[3])					// thready navyse ignorujeme
			return;

		/*c_outputs[ref_ray] = ref_ray;
		return;*/

		float dist = FLOAT_MAX;
		float dist2 = FLOAT_MAX;
		float theta = 0.0f;
		cl_uint i = 0;

		const cl_float4 ray = c_rays[ref_ray];
		cl_float4 v0 = c_vertices[c_triangles[c_origins[ref_triangle]].s[0]];
		cl_float4 v1 = c_vertices[c_triangles[c_origins[ref_triangle]].s[1]];
		cl_float4 v2 = c_vertices[c_triangles[c_origins[ref_triangle]].s[2]];
		const cl_float4 center = cl_div(cl_plus(cl_plus(v0, v1), v2), 3.0f);
		const cl_float4 normal = cl_normalize(cl_multiply(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)), (-1.0f)));
		cl_float4 tnormal;
	
		for(i = 0; i < c_params.s[2]; i++)
		{
			ref_triangle = c_targets[(ref_ray * c_params.s[2]) + i];
			if(ref_triangle == 0)						// uz nemame dalsie trojuholniky v zozname, TODO, dat na -1
				break;

			ref_triangle = ref_triangle - 1;			// naschval posunute aby som mohol pouzit 0 ako neexistujuci trojuholnik

			v0 = c_vertices[c_triangles[ref_triangle].s[0]];
			v1 = c_vertices[c_triangles[ref_triangle].s[1]];
			v2 = c_vertices[c_triangles[ref_triangle].s[2]];
			
			dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2, 0.00001f);
			if(dist2 < FLOAT_MAX)
			{
				tnormal = cl_normalize(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)));
				theta = acos( cl_dot(ray, tnormal) / cl_length(ray) );
				theta = theta * (180.0f / (float)M_PI);
				if((theta < 90.0f) && (dist2 < dist))
					dist = dist2;
			}
		}

		c_outputs[ref_ray] = dist;
	}

	cl_float4 COpenCLDebug::Multiply(cl_float4 a, cl_float4 t, cl_float4 n, cl_float4 b)
	{
		cl_float4 r1; r1.s[0] = t.s[0]; r1.s[1] = n.s[0]; r1.s[2] = b.s[0]; r1.s[3] = 0;
		cl_float4 r2; r2.s[0] = t.s[1]; r2.s[1] = n.s[1]; r2.s[2] = b.s[1]; r2.s[3] = 0;
		cl_float4 r3; r3.s[0] = t.s[2]; r3.s[1] = n.s[2]; r3.s[2] = b.s[2]; r3.s[3] = 0;

		cl_float4 result;
		result.s[0] = cl_dot(a, r1);
		result.s[1] = cl_dot(a, r2);
		result.s[2] = cl_dot(a, r3);
		result.s[3] = 0;

		return cl_normalize(result);
	}

	unsigned char COpenCLDebug::first_node(cl_float4 t0, cl_float4 tm)
	{
		unsigned char answer = 0;   // initialize to 00000000
		// select the entry plane and set bits
		if(t0.s[0] > t0.s[1])
		{
			if(t0.s[0] > t0.s[2])					// PLANE YZ
			{
				if(tm.s[1] < t0.s[0]) answer|=2;    // set bit at position 1
				if(tm.s[2] < t0.s[0]) answer|=1;    // set bit at position 0
				return answer;
			}  
		} else
		{      
			if(t0.s[1] > t0.s[2])					// PLANE XZ
			{
				if(tm.s[0] < t0.s[1]) answer|=4;    // set bit at position 2
				if(tm.s[2] < t0.s[1]) answer|=1;    // set bit at position 0
				return answer;
			}
		}
		// PLANE XY
		if(tm.s[0] < t0.s[2]) answer|=4;			// set bit at position 2
		if(tm.s[1] < t0.s[2]) answer|=2;			// set bit at position 1
		return answer;
	}

	unsigned char COpenCLDebug::new_node(float txm, unsigned char x, float tym, unsigned char y, float tzm, unsigned char z)
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

	bool COpenCLDebug::CheckValid(unsigned char mask, unsigned char num)
	{
		return((mask >> num) & 1);
	}

	typedef struct {
		cl_float4       t0stack[STACK_SIZE];
		cl_float4       t1stack[STACK_SIZE];
		int				cstack[STACK_SIZE];
		unsigned int	ostack[STACK_SIZE];
	}CastStack;

	unsigned int read (CastStack *stack, int idx, cl_float4 *t0, cl_float4 *t1, int *cnode)
	{
		if(idx < 0)
			assert(false);
		if(idx >= STACK_SIZE)
			assert(false);
		(*t0) = (*stack).t0stack[idx];
		(*t1) = (*stack).t1stack[idx];
		(*cnode) = (*stack).cstack[idx];

		return (*stack).ostack[idx];
	}

	void write (CastStack *stack, int idx, cl_float4 t0, cl_float4 t1, int cnode, unsigned int node)
	{
		if(idx < 0)
			assert(false);
		if(idx >= STACK_SIZE)
			assert(false);
		(*stack).t0stack[idx] = t0;
		(*stack).t1stack[idx] = t1;
		(*stack).cstack[idx] = cnode;
		(*stack).ostack[idx] = node;
	}
	// funguje to tak ze zistim kolko som uz presiel
	// tj ak moje idx je 3, tak viem ze som uz presiel (1,2,3)
	// to odratam 8-3 a zmazem vrchnych 5 bitov
	// tak zistim kolko som presiel a kam sa dalej posunut
	const unsigned char c_popc8LUT[] =
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
	unsigned char popc8(unsigned char mask)
	{
		return c_popc8LUT[mask];
	}
	unsigned char popc88(unsigned char sons, unsigned char idx)
	{
		int idxx = 0;
		for(int i = 0; i < idx; i++)
		{
			if((sons >> i) & 1)
				idxx++;
		}
		return idxx;
		//return c_popc8LUT[mask];
	}
	bool CanAdd(cl_uint8 *short_shack, unsigned char *ss_idx, unsigned int triangle)
	{
		if((*short_shack).s[0] == triangle) return false;
		if((*short_shack).s[1] == triangle) return false;
		if((*short_shack).s[2] == triangle) return false;
		if((*short_shack).s[3] == triangle) return false;
		if((*short_shack).s[4] == triangle) return false;
		if((*short_shack).s[5] == triangle) return false;
		if((*short_shack).s[6] == triangle) return false;
		if((*short_shack).s[7] == triangle) return false;

		(*ss_idx) = (*ss_idx) + 1; (*ss_idx) = (*ss_idx) % 8;
		(*short_shack).s[(*ss_idx)] = triangle;
		return true;
	}
	void COpenCLDebug::proc_subtree(const cl_float4 *c_triangles,
									const cl_uint *c_nodes,
									const cl_uint *c_node_tria,
									const cl_float4 ray,
									const cl_float4 center,
									const cl_uint ref_triangle,
									const cl_float bias,
									cl_float *dist,
									unsigned char idx,
									cl_float4 t0,
									cl_float4 t1
									)
	{
		CastStack stack;
		cl_float4 tm;
		int currNode = -1;
		int scale = 0;
		unsigned int node = c_nodes[0];
		unsigned char sons = (node & 0xFF);
		unsigned int tindex;
		unsigned int size = 0;
		unsigned int i = 0;
		cl_uint8 short_shack;
		short_shack.s[0] = ref_triangle;
		short_shack.s[1] = ref_triangle;
		short_shack.s[2] = ref_triangle;
		short_shack.s[3] = ref_triangle;
		short_shack.s[4] = ref_triangle;
		short_shack.s[5] = ref_triangle;
		short_shack.s[6] = ref_triangle;
		short_shack.s[7] = ref_triangle;
		unsigned char ss_idx = 0;
		float dist2 = FLOAT_MAX;
		float theta = 0.0f;
		cl_float4 v0, v1, v2;
		cl_float4 tnormal;
		unsigned char popik;
		while (scale < STACK_SIZE)
		{
			// konec toho spodneho switchu, berem otca
			if(currNode == 8)
			{
				if(scale > 0)
				{
					scale--;
					node = read(&stack, scale, &t0, &t1, &currNode);
					sons = (node & 0xFF);
					continue;
				}
				else
					break;
			}
			// mame novy nody
			else if(currNode == -1)
			{
				if((t1.s[0] < 0.0) || (t1.s[1] < 0.0) || (t1.s[2] < 0.0))
				{
					if(scale > 0)
					{
						scale--;
						node = read(&stack, scale, &t0, &t1, &currNode);
						sons = (node & 0xFF);
						continue;
					}
					else
						break;
				}
				if(sons == 0)
				{
					tindex = node>>8;
					size = c_node_tria[tindex];
					for(i = 1; i <= size; i++)
					{
						if(CanAdd(&short_shack, &ss_idx, c_node_tria[tindex+i]))
						{
							v0 = c_triangles[c_node_tria[tindex+i] * 3 + 0];
							v1 = c_triangles[c_node_tria[tindex+i] * 3 + 1];
							v2 = c_triangles[c_node_tria[tindex+i] * 3 + 2];

							dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2, bias);
							if(dist2 < FLOAT_MAX)
							{
								tnormal = cl_normalize(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)));
								theta = acos( cl_dot(ray, tnormal) / cl_length(ray) );
								theta = theta * (180.0f / (float)M_PI);
								if((theta < 90.0f) && (dist2 < (*dist)))
									(*dist) = dist2;
							}
						}
					}
					if((*dist) < FLOAT_MAX)
						return;
					
					scale--;
					node = read(&stack, scale, &t0, &t1, &currNode);
					sons = (node & 0xFF);
					size = 0;
					continue;
				}
				tm = cl_multiply((cl_plus(t0,t1)), 0.5f);
				currNode = first_node(t0,tm);
			}
			else
				tm = cl_multiply((cl_plus(t0,t1)), 0.5f);

			switch (currNode)
			{
			case 0:
				if(CheckValid(sons, idx))
				{
					currNode = new_node(tm.s[0],4,tm.s[1],2,tm.s[2],1);
					write(&stack, scale, t0, t1, currNode, node);
					t1 = tm;
					//proc_subtree4(tx0,ty0,tz0,txm,tym,tzm,node->son[aa], octrees);
					//currNode = new_node(txm,4,tym,2,tzm,1);
					currNode = -1;
					sons = sons << (8 - (idx));
					sons = sons >> (8 - (idx));
					popik = popc8(sons);
					//popik = popc88(sons, idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(tm.s[0],4,tm.s[1],2,tm.s[2],1);
					scale--;
				}
				break;
			case 1: 
				if(CheckValid(sons, 1^idx))
				{
					currNode = new_node(tm.s[0],5,tm.s[1],3,t1.s[2],8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[2] = tm.s[2];
					t1.s[0] = tm.s[0];
					t1.s[1] = tm.s[1];
					//proc_subtree4(tx0,ty0,tzm,txm,tym,tz1,node->son[1^aa], octrees);
					//currNode = new_node(txm,5,tym,3,tz1,8);
					currNode = -1;
					sons = sons << (8 - (1^idx));
					sons = sons >> (8 - (1^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 1^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(tm.s[0],5,tm.s[1],3,t1.s[2],8);
					scale--;
				}
				break;
			case 2:
				if(CheckValid(sons, 2^idx))
				{
					currNode = new_node(tm.s[0],6,t1.s[1],8,tm.s[2],3);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[1] = tm.s[1];
					t1.s[0] = tm.s[0];
					t1.s[2] = tm.s[2];
					//proc_subtree4(tx0,tym,tz0,txm,ty1,tzm,node->son[2^aa], octrees);
					//currNode = new_node(txm,6,ty1,8,tzm,3);
					currNode = -1;
					sons = sons << (8 - (2^idx));
					sons = sons >> (8 - (2^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 2^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(tm.s[0],6,t1.s[1],8,tm.s[2],3);
					scale--;
				}
				break;
			case 3:
				if(CheckValid(sons, 3^idx))
				{
					currNode = new_node(tm.s[0],7,t1.s[1],8,t1.s[2],8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[1] = tm.s[1];
					t0.s[2] = tm.s[2];
					t1.s[0] = tm.s[0];
					//proc_subtree4(tx0,tym,tzm,txm,ty1,tz1,node->son[3^aa], octrees);
					//currNode = new_node(txm,7,ty1,8,tz1,8);
					currNode = -1;
					sons = sons << (8 - (3^idx));
					sons = sons >> (8 - (3^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 3^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(tm.s[0],7,t1.s[1],8,t1.s[2],8);
					scale--;
				}
				break;
			case 4:
				if(CheckValid(sons, 4^idx))
				{
					currNode = new_node(t1.s[0],8,tm.s[1],6,tm.s[2],5);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[0] = tm.s[0];
					t1.s[1] = tm.s[1];
					t1.s[2] = tm.s[2];
					//proc_subtree4(txm,ty0,tz0,tx1,tym,tzm,node->son[4^aa], octrees);
					//currNode = new_node(tx1,8,tym,6,tzm,5);
					currNode = -1;
					sons = sons << (8 - (4^idx));
					sons = sons >> (8 - (4^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 4^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(t1.s[0],8,tm.s[1],6,tm.s[2],5);
					scale--;
				}
				break;
			case 5:
				if(CheckValid(sons, 5^idx))
				{
					currNode = new_node(t1.s[0],8,tm.s[1],7,t1.s[2],8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[0] = tm.s[0];
					t0.s[2] = tm.s[2];
					t1.s[1] = tm.s[1];
					//proc_subtree4(txm,ty0,tzm,tx1,tym,tz1,node->son[5^aa], octrees);
					//currNode = new_node(tx1,8,tym,7,tz1,8);
					currNode = -1;
					sons = sons << (8 - (5^idx));
					sons = sons >> (8 - (5^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 5^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(t1.s[0],8,tm.s[1],7,t1.s[2],8);
					scale--;
				}
				break;
			case 6:
				if(CheckValid(sons, 6^idx))
				{
					currNode = new_node(t1.s[0],8,t1.s[1],8,tm.s[2],7);
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[0] = tm.s[0];
					t0.s[1] = tm.s[1];
					t1.s[2] = tm.s[2];
					//proc_subtree4(txm,tym,tz0,tx1,ty1,tzm,node->son[6^aa], octrees);
					//currNode = new_node(tx1,8,ty1,8,tzm,7);
					currNode = -1;
					sons = sons << (8 - (6^idx));
					sons = sons >> (8 - (6^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 6^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
				}
				else
				{
					currNode = new_node(t1.s[0],8,t1.s[1],8,tm.s[2],7);
					scale--;
				}
				break;
			case 7:
				if(CheckValid(sons, 7^idx))
				{
					currNode = 8;
					write(&stack, scale, t0, t1, currNode, node);
					t0.s[0] = tm.s[0];
					t0.s[1] = tm.s[1];
					t0.s[2] = tm.s[2];
					//proc_subtree4(txm,tym,tzm,tx1,ty1,tz1,node->son[7^aa], octrees);
					//currNode = 8;
					currNode = -1;
					sons = sons << (8 - (7^idx));
					sons = sons >> (8 - (7^idx));
					popik = popc8(sons);
					//popik = popc88(sons, 7^idx);
					node = c_nodes[(node >> 8) + popik];
					sons = (node & 0xFF);
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

	void COpenCLDebug::OctreeTraversal( const cl_float4 *c_triangles,
										const cl_uint *c_nodes,
										const cl_uint *c_node_tria,
										const cl_float4 o_min,
										const cl_float4 o_max,
										cl_float4 ray,
										const cl_float4 center,
										const cl_uint ref_triangle,
										const cl_float bias,
										cl_float *dist)
	{
		unsigned char idx = 0;

		// avoid division by zero
		if (fabs(ray.s[0]) < bias) ray.s[0] = ray.s[0] < 0.0 ? -bias : bias;
		if (fabs(ray.s[1]) < bias) ray.s[1] = ray.s[1] < 0.0 ? -bias : bias;
		if (fabs(ray.s[2]) < bias) ray.s[2] = ray.s[2] < 0.0 ? -bias : bias;

		float invdirx = 1.0f / fabs(ray.s[0]);
		float invdiry = 1.0f / fabs(ray.s[1]);
		float invdirz = 1.0f / fabs(ray.s[2]);

		cl_float4 t0, t1;
		t0.s[3] = 0; t1.s[3] = 0;

		// fixes for rays with negative direction
		if(ray.s[0] < 0.0)
		{
			t0.s[0] = (o_max.s[0] - center.s[0]) * -invdirx;
			t1.s[0] = (o_min.s[0] - center.s[0]) * -invdirx;
			idx |= 4 ; //bitwise OR (latest bits are XYZ)
		}
		else
		{
			t0.s[0] = (o_min.s[0] - center.s[0]) * invdirx;
			t1.s[0] = (o_max.s[0] - center.s[0]) * invdirx;
		}
		if(ray.s[1] < 0.0)
		{
			t0.s[1] = (o_max.s[1] - center.s[1]) * -invdiry;
			t1.s[1] = (o_min.s[1] - center.s[1]) * -invdiry;
			idx |= 2 ;
		}
		else
		{
			t0.s[1] = (o_min.s[1] - center.s[1]) * invdiry;
			t1.s[1] = (o_max.s[1] - center.s[1]) * invdiry;
		}
		if(ray.s[2] < 0.0)
		{
			t0.s[2] = (o_max.s[2] - center.s[2]) * -invdirz;
			t1.s[2] = (o_min.s[2] - center.s[2]) * -invdirz;
			idx |= 1 ;
		}
		else
		{
			t0.s[2] = (o_min.s[2] - center.s[2]) * invdirz;
			t1.s[2] = (o_max.s[2] - center.s[2]) * invdirz;
		}

		if( max(max(t0.s[0],t0.s[1]),t0.s[2]) < min(min(t1.s[0],t1.s[1]),t1.s[2]) )
		{ 
			proc_subtree(c_triangles, c_nodes, c_node_tria, ray, center, ref_triangle, bias, dist, idx, t0, t1);
		}
	}

	// moja kernel funkcia 2
	void COpenCLDebug::sdf3(const cl_float4 *c_triangles,
							const cl_uint *c_nodes,
							const cl_uint *c_node_tria,
							const cl_float4 o_min,
							const cl_float4 o_max,
							const cl_float bias,
							const cl_float4 *c_rays,
							const cl_uint n_rays,
							const cl_uint n_triangles,
							float *c_outputs,
							const cl_uint gid
							)
	{
		//const cl_uint gid = get_global_id(0);
		cl_uint ref_ray = gid;											// zisti kolkaty som luc v poradi
		cl_uint ref_triangle = (cl_uint)(ref_ray / n_rays);		// na zaklade toho zisti ku ktoremu trojuholniku patrim
	
		if(ref_triangle > n_triangles)								// thready navyse ignorujeme
			return;

		/*c_outputs[ref_ray] = ref_ray;
		return;*/
		// premenne zadefinovat dopredu
		const cl_float4 v0 = c_triangles[ref_triangle * 3];
		const cl_float4 v1 = c_triangles[ref_triangle * 3 + 1];
		const cl_float4 v2 = c_triangles[ref_triangle * 3 + 2];
		const cl_float4 center = cl_div(cl_plus(cl_plus(v0, v1), v2), 3.0f);
		const cl_float4 normal = cl_normalize(cl_multiply(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)), (-1.0f)));
		const cl_float4 tangens = cl_normalize(cl_minus(v0,v2));
		const cl_float4 binormal = cl_normalize(cl_cross(tangens, normal));

		cl_float4 ray = Multiply(c_rays[ref_ray % n_rays], tangens, normal, binormal);

		float dist = FLOAT_MAX;
		OctreeTraversal(c_triangles, c_nodes, c_node_tria,o_min,o_max,ray,center,ref_triangle, bias, &dist);

		c_outputs[gid] = dist;
	}

	// moja kernel funkcia 2
	void COpenCLDebug::sdf2(const cl_float4 *c_triangles,
							const cl_uint *c_nodes,
							const cl_uint *c_node_tria,
							const cl_float4 o_min,
							const cl_float4 o_max,
							const cl_float bias,
							const cl_float4 *c_rays,
							const cl_uint n_rays,
							const cl_uint n_triangles,
							float *c_outputs,
							const cl_uint gid
							)
	{
		//const cl_uint gid = get_global_id(0);
		//cl_uint ITERATIONS = 0;
		cl_uint ref_ray = gid;											// zisti kolkaty som luc v poradi
		cl_uint ref_triangle = (cl_uint)(ref_ray / n_rays);		// na zaklade toho zisti ku ktoremu trojuholniku patrim
	
		if(ref_triangle > n_triangles)								// thready navyse ignorujeme
			return;

		/*c_outputs[ref_ray] = ref_ray;
		return;*/
		// premenne zadefinovat dopredu
		if((ref_triangle * 3 + 2) >= nn_triangles)
			assert(false);

		const cl_float4 v0 = c_triangles[ref_triangle * 3];
		const cl_float4 v1 = c_triangles[ref_triangle * 3 + 1];
		const cl_float4 v2 = c_triangles[ref_triangle * 3 + 2];
		const cl_float4 center = cl_div(cl_plus(cl_plus(v0, v1), v2), 3.0f);
		const cl_float4 normal = cl_normalize(cl_multiply(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)), (-1.0f)));
		const cl_float4 tangens = cl_normalize(cl_minus(v0,v2));
		const cl_float4 binormal = cl_normalize(cl_cross(tangens, normal));

		if((ref_ray % n_rays) >= n_rays)
			assert(false);

		cl_float4 ray = Multiply(c_rays[ref_ray % n_rays], tangens, normal, binormal);

		float dist = FLOAT_MAX;
		unsigned char idx = 0;

		// avoid division by zero
		if (fabs(ray.s[0]) < bias) ray.s[0] = ray.s[0] < 0.0 ? -bias : bias;
		if (fabs(ray.s[1]) < bias) ray.s[1] = ray.s[1] < 0.0 ? -bias : bias;
		if (fabs(ray.s[2]) < bias) ray.s[2] = ray.s[2] < 0.0 ? -bias : bias;

		float invdirx = 1.0f / fabs(ray.s[0]);
		float invdiry = 1.0f / fabs(ray.s[1]);
		float invdirz = 1.0f / fabs(ray.s[2]);

		cl_float4 t0, t1;
		t0.s[3] = 0; t1.s[3] = 0;

		// fixes for rays with negative direction
		if(ray.s[0] < 0.0)
		{
			t0.s[0] = (o_max.s[0] - center.s[0]) * -invdirx;
			t1.s[0] = (o_min.s[0] - center.s[0]) * -invdirx;
			idx |= 4 ; //bitwise OR (latest bits are XYZ)
		}
		else
		{
			t0.s[0] = (o_min.s[0] - center.s[0]) * invdirx;
			t1.s[0] = (o_max.s[0] - center.s[0]) * invdirx;
		}
		if(ray.s[1] < 0.0)
		{
			t0.s[1] = (o_max.s[1] - center.s[1]) * -invdiry;
			t1.s[1] = (o_min.s[1] - center.s[1]) * -invdiry;
			idx |= 2 ;
		}
		else
		{
			t0.s[1] = (o_min.s[1] - center.s[1]) * invdiry;
			t1.s[1] = (o_max.s[1] - center.s[1]) * invdiry;
		}
		if(ray.s[2] < 0.0)
		{
			t0.s[2] = (o_max.s[2] - center.s[2]) * -invdirz;
			t1.s[2] = (o_min.s[2] - center.s[2]) * -invdirz;
			idx |= 1 ;
		}
		else
		{
			t0.s[2] = (o_min.s[2] - center.s[2]) * invdirz;
			t1.s[2] = (o_max.s[2] - center.s[2]) * invdirz;
		}

		if( max(max(t0.s[0],t0.s[1]),t0.s[2]) < min(min(t1.s[0],t1.s[1]),t1.s[2]) )
		{ 
			CastStack stack;
			cl_float4 tm;
			int currNode = -1;
			int scale = 0;
			unsigned int node = c_nodes[0];
			unsigned char sons = (node & 0xFF);
			unsigned int tindex;
			unsigned int size = 0;
			unsigned int i = 0;
			cl_uint8 short_shack;
			short_shack.s[0] = ref_triangle;
			short_shack.s[1] = ref_triangle;
			short_shack.s[2] = ref_triangle;
			short_shack.s[3] = ref_triangle;
			short_shack.s[4] = ref_triangle;
			short_shack.s[5] = ref_triangle;
			short_shack.s[6] = ref_triangle;
			short_shack.s[7] = ref_triangle;
			unsigned char ss_idx = 0;
			float dist2 = FLOAT_MAX;
			float theta = 0.0f;
			cl_float4 v0, v1, v2;
			cl_float4 tnormal;
			unsigned char popik;
			while (scale < STACK_SIZE)
			{
				/*ITERATIONS++;
				if(ITERATIONS > MAX_ITERATIONS)
					break;*/
				// konec toho spodneho switchu, berem otca
				if(currNode == 8)
				{
					if(scale > 0)
					{
						scale--;
						node = read(&stack, scale, &t0, &t1, &currNode);
						sons = (node & 0xFF);
						continue;
					}
					else
						break;
				}
				// mame novy nody
				else if(currNode == -1)
				{
					if((t1.s[0] < 0.0) || (t1.s[1] < 0.0) || (t1.s[2] < 0.0))
					{
						if(scale > 0)
						{
							scale--;
							node = read(&stack, scale, &t0, &t1, &currNode);
							sons = (node & 0xFF);
							continue;
						}
						else
							break;
					}
					if(sons == 0)
					{
						tindex = node>>8;
						if(tindex >= triangleCount)
							assert(false);
						size = c_node_tria[tindex];
						for(i = 1; i <= size; i++)
						{
							if((tindex+i) >= triangleCount)
								assert(false);
							if(CanAdd(&short_shack, &ss_idx, c_node_tria[tindex+i]))
							{
								//ITERATIONS++;
								if((c_node_tria[tindex+i] * 3 + 2) >= nn_triangles)
									assert(false);

								v0 = c_triangles[c_node_tria[tindex+i] * 3 + 0];
								v1 = c_triangles[c_node_tria[tindex+i] * 3 + 1];
								v2 = c_triangles[c_node_tria[tindex+i] * 3 + 2];

								dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2, bias);
								if(dist2 < FLOAT_MAX)
								{
									tnormal = cl_normalize(cl_cross(cl_minus(v1,v0), cl_minus(v2,v0)));
									theta = acos( cl_dot(ray, tnormal) / cl_length(ray) );
									theta = theta * (180.0f / (float)M_PI);
									if((theta < 90.0f) && (dist2 < dist))
										dist = dist2;
								}
							}
						}
						if(dist < (FLOAT_MAX - 1.0f))
							break;

						scale--;
						node = read(&stack, scale, &t0, &t1, &currNode);
						sons = (node & 0xFF);
						size = 0;
						continue;
					}
					tm = cl_multiply((cl_plus(t0,t1)), 0.5f);
					currNode = first_node(t0,tm);
				}
				else
					tm = cl_multiply((cl_plus(t0,t1)), 0.5f);

				switch (currNode)
				{
				case 0:
					if(CheckValid(sons, idx))
					{
						currNode = new_node(tm.s[0],4,tm.s[1],2,tm.s[2],1);
						write(&stack, scale, t0, t1, currNode, node);
						t1 = tm;
						//proc_subtree4(tx0,ty0,tz0,txm,tym,tzm,node->son[aa], octrees);
						//currNode = new_node(txm,4,tym,2,tzm,1);
						currNode = -1;
						sons = sons << (8 - (idx));
						sons = sons >> (8 - (idx));
						popik = popc8(sons);
						//popik = popc88(sons, idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(tm.s[0],4,tm.s[1],2,tm.s[2],1);
						scale--;
					}
					break;
				case 1: 
					if(CheckValid(sons, 1^idx))
					{
						currNode = new_node(tm.s[0],5,tm.s[1],3,t1.s[2],8);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[2] = tm.s[2];
						t1.s[0] = tm.s[0];
						t1.s[1] = tm.s[1];
						//proc_subtree4(tx0,ty0,tzm,txm,tym,tz1,node->son[1^aa], octrees);
						//currNode = new_node(txm,5,tym,3,tz1,8);
						currNode = -1;
						sons = sons << (8 - (1^idx));
						sons = sons >> (8 - (1^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 1^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(tm.s[0],5,tm.s[1],3,t1.s[2],8);
						scale--;
					}
					break;
				case 2:
					if(CheckValid(sons, 2^idx))
					{
						currNode = new_node(tm.s[0],6,t1.s[1],8,tm.s[2],3);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[1] = tm.s[1];
						t1.s[0] = tm.s[0];
						t1.s[2] = tm.s[2];
						//proc_subtree4(tx0,tym,tz0,txm,ty1,tzm,node->son[2^aa], octrees);
						//currNode = new_node(txm,6,ty1,8,tzm,3);
						currNode = -1;
						sons = sons << (8 - (2^idx));
						sons = sons >> (8 - (2^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 2^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(tm.s[0],6,t1.s[1],8,tm.s[2],3);
						scale--;
					}
					break;
				case 3:
					if(CheckValid(sons, 3^idx))
					{
						currNode = new_node(tm.s[0],7,t1.s[1],8,t1.s[2],8);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[1] = tm.s[1];
						t0.s[2] = tm.s[2];
						t1.s[0] = tm.s[0];
						//proc_subtree4(tx0,tym,tzm,txm,ty1,tz1,node->son[3^aa], octrees);
						//currNode = new_node(txm,7,ty1,8,tz1,8);
						currNode = -1;
						sons = sons << (8 - (3^idx));
						sons = sons >> (8 - (3^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 3^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(tm.s[0],7,t1.s[1],8,t1.s[2],8);
						scale--;
					}
					break;
				case 4:
					if(CheckValid(sons, 4^idx))
					{
						currNode = new_node(t1.s[0],8,tm.s[1],6,tm.s[2],5);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[0] = tm.s[0];
						t1.s[1] = tm.s[1];
						t1.s[2] = tm.s[2];
						//proc_subtree4(txm,ty0,tz0,tx1,tym,tzm,node->son[4^aa], octrees);
						//currNode = new_node(tx1,8,tym,6,tzm,5);
						currNode = -1;
						sons = sons << (8 - (4^idx));
						sons = sons >> (8 - (4^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 4^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(t1.s[0],8,tm.s[1],6,tm.s[2],5);
						scale--;
					}
					break;
				case 5:
					if(CheckValid(sons, 5^idx))
					{
						currNode = new_node(t1.s[0],8,tm.s[1],7,t1.s[2],8);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[0] = tm.s[0];
						t0.s[2] = tm.s[2];
						t1.s[1] = tm.s[1];
						//proc_subtree4(txm,ty0,tzm,tx1,tym,tz1,node->son[5^aa], octrees);
						//currNode = new_node(tx1,8,tym,7,tz1,8);
						currNode = -1;
						sons = sons << (8 - (5^idx));
						sons = sons >> (8 - (5^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 5^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(t1.s[0],8,tm.s[1],7,t1.s[2],8);
						scale--;
					}
					break;
				case 6:
					if(CheckValid(sons, 6^idx))
					{
						currNode = new_node(t1.s[0],8,t1.s[1],8,tm.s[2],7);
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[0] = tm.s[0];
						t0.s[1] = tm.s[1];
						t1.s[2] = tm.s[2];
						//proc_subtree4(txm,tym,tz0,tx1,ty1,tzm,node->son[6^aa], octrees);
						//currNode = new_node(tx1,8,ty1,8,tzm,7);
						currNode = -1;
						sons = sons << (8 - (6^idx));
						sons = sons >> (8 - (6^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 6^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
					}
					else
					{
						currNode = new_node(t1.s[0],8,t1.s[1],8,tm.s[2],7);
						scale--;
					}
					break;
				case 7:
					if(CheckValid(sons, 7^idx))
					{
						currNode = 8;
						write(&stack, scale, t0, t1, currNode, node);
						t0.s[0] = tm.s[0];
						t0.s[1] = tm.s[1];
						t0.s[2] = tm.s[2];
						//proc_subtree4(txm,tym,tzm,tx1,ty1,tz1,node->son[7^aa], octrees);
						//currNode = 8;
						currNode = -1;
						sons = sons << (8 - (7^idx));
						sons = sons >> (8 - (7^idx));
						popik = popc8(sons);
						//popik = popc88(sons, 7^idx);
						if(((node >> 8) + popik) >= nodeCount)
							assert(false);
						node = c_nodes[(node >> 8) + popik];
						sons = (node & 0xFF);
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
		if(gid >= max_outputs)
			assert(false);
		c_outputs[gid] = dist;
	}

	#define N_RAYS 30
	void COpenCLDebug::proces(float *c_outputs,
		float *c_results,
		float *weights,
		const cl_uint n_triangles,
		const cl_uint gid
		)
	{
		cl_uint ref_triangle = gid;									// kolkaty som luc v poradi
	
		if(ref_triangle > n_triangles)								// thready navyse ignorujeme
			return;

		float cache[N_RAYS];
		int i = 0;
		int j = 0;
		float mean = 0.0;
		for(i = 0; i < N_RAYS; i++)
		{
			cache[i] = c_outputs[(ref_triangle * N_RAYS) +  i];
			if(cache[i] >= (FLOAT_MAX - 1.0f))
				cache[i] = 0;
			mean += cache[i];
		}

		// bubble Sort
		float tmp = 0.0f;
		for(int i = 0; i < N_RAYS - 1; i++)
		{
			for(int j = 0; j < N_RAYS - i - 1; j++)
			{
				if(cache[j+1] < cache[j])
				{
					tmp = cache[j + 1];
					cache[j + 1] = cache[j];
					cache[j] = tmp;
				}   
			}   
		}

		float median = 0.0f;
		if (N_RAYS  % 2 == 0)
		{
			median = (cache[N_RAYS / 2 - 1] + cache[N_RAYS / 2]) / 2;
		}
		else 
		{
			median = cache[N_RAYS / 2];
		}

		mean = mean / (float)N_RAYS;

		float deviation = 0.0f;

		for(unsigned int i = 0; i < N_RAYS; i++)
		{
			deviation += (cache[i] - mean) * (cache[i] - mean);
		}
		deviation = sqrt(deviation / (float)N_RAYS);

		float sum_values = 0.0;
		float sum_weights = 0.0;
		for(unsigned int i = 0; i < N_RAYS; i++)
		{
			if(fabs(cache[i] - median) <= deviation)
			{
				sum_values += cache[i] * weights[i];
				sum_weights += weights[i];
			}
		}
		c_results[ref_triangle] = sum_values / sum_weights;           
	}
	#define M_PI_F 3.1415
	#define SQRT_THREE 1.7320508075f
	// vypocitaj gaussian hodnotu pre dane x
	float ComputeGaussian(float val, float maxval)
	{
		float num = min((val / maxval), 1.0f);
		return ((1.0f / sqrt(2*M_PI_F)) * exp( (-num*num)/2.0f) * 100.0f);
	}

	typedef struct {
		unsigned int	ostack[STACK_SIZE];
		unsigned char	tstack[STACK_SIZE];
	}CastStackz;

	inline unsigned int read (CastStackz *stack, int idx, unsigned char *id)
	{
		(*id) = (*stack).tstack[idx];
		return (*stack).ostack[idx];
	}

	inline void write (CastStackz *stack, int idx, unsigned int node, unsigned char id)
	{
		(*stack).tstack[idx] = id;
		(*stack).ostack[idx] = node;
	}

	// funguje to tak ze mi to najde dalsi index na ktory mam ist
	const unsigned char c_pipc8LUT[] =
	{
		8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	};

	unsigned char pipc8(unsigned char mask)
	{
		return c_pipc8LUT[mask];
	}

	void COpenCLDebug::smooth(const cl_float4 *c_points,
		const cl_float4 *c_node_origins,
		const cl_uint *c_nodes,
		float *c_results,
		const float weight,
		const cl_uint n_points,
		const float o_size,
		const cl_uint gid
		)
	{
		cl_uint ref_point = gid;										// kolkaty som point v poradi

		if(ref_point > n_points)										// thready navyse ignorujeme (nemali by byt)
			return;

		//Smooth2(point_array[i], m_root, ro_list, i);
		cl_float4 pointik1 = c_points[ref_point];
		float value1 = fabs(pointik1.s[3]);
		pointik1.s[3] = 1.0f;

		cl_float4 pointik2;
		float value2 = 0;

		float maxval = value1 * 0.25f * weight;
		float distanc = 0;

		float gweight = ComputeGaussian(0, maxval);
		float sum_values = value1 * gweight;
		float sum_weights = gweight;

		if(ref_point < 10000)
		{
			sum_values = 0;
			sum_weights = 0;
		}

		CastStackz stack;
		int idx = 0;
		unsigned int tmp = c_nodes[0];
		unsigned char sons = (tmp & 0xFF);
		unsigned char id = pipc8(sons);;
		unsigned int node = 0;

		float o_size_array[STACK_SIZE];
		o_size_array[0] = o_size;
		for(int i = 1; i < STACK_SIZE; i++)
		{
			o_size_array[i] = o_size_array[i - 1] * 0.5f;
		}

		write(&stack, idx, tmp, id);
		while (idx >= 0)
		{
			tmp = read(&stack, idx, &id);	// nacitame si momentalne robeny node
			sons = (tmp & 0xFF);
			if(sons == 0)					// leaf
			{
				tmp = tmp >> 8;
				pointik2 = c_points[tmp];
				value2 = fabs(pointik2.s[3]);
				pointik2.s[3] = 1.0f;
				while(value2 >= 0.0f)
				{
					distanc = cl_distance(pointik1, pointik2);
					if(distanc <= maxval)
					{
						gweight = ComputeGaussian(distanc, maxval);
						sum_values += value2 * gweight;
						sum_weights += gweight;
					}
					tmp = tmp + 1;

					if((tmp >= n_points) || (tmp >= 10000))
						break;

					pointik2 = c_points[tmp];
					value2 = pointik2.s[3];
					pointik2.s[3] = 1.0f;
				}
				idx--;
			}
			else
			{
				if(id >= 8)
				{
					idx--;
					continue;
				}
				//ziskaj nove tmp
				node = tmp >> 8;

				sons = sons << (8 - id);
				sons = sons >> (8 - id);
				node = node + popc8(sons);		// idem na dalsi node

				// vzdialenost od mojho bodu k synovy nodu
				distanc = cl_distance(pointik1, c_node_origins[node]) - (o_size_array[idx+1] * SQRT_THREE);
				if(distanc <= maxval)
				{
					write(&stack, idx + 1, c_nodes[node], pipc8(c_nodes[node] & 0xFF));
					sons = (tmp & 0xFF);
					sons = sons >> (id + 1);
					sons = sons << (id + 1);
					id = pipc8(sons);
					write(&stack, idx, tmp, id);
					idx = idx + 1;
				}
				else
				{
					sons = (tmp & 0xFF);
					sons = sons >> (id + 1);
					sons = sons << (id + 1);
					id = pipc8(sons);
					write(&stack, idx, tmp, id);
				}
			}
		}

		c_results[ref_point] = sum_values / sum_weights;
	}
}