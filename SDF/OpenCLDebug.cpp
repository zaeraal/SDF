// kratky subor pre debugovanie OpenCL kodu
#include "stdafx.h"
#include "OpenCLDebug.h"

namespace OpenCLDebugger
{

	COpenCLDebug::COpenCLDebug()
	{
		args = NULL;
		arg_size = 0;
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
			delete args;
		}
	}

	void COpenCLDebug::SetArgSize(cl_uint size /* args_size */)
	{
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

	// pomocne OpenCL funkcie
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

float COpenCLDebug::rayIntersectsTriangle(cl_float4 p, cl_float4 d, cl_float4 v0, cl_float4 v1, cl_float4 v2)
{
	cl_float4 e1 = cl_minus(v1, v0);
	cl_float4 e2 = cl_minus(v2, v0);
	float a,f,u,v;

	cl_float4 h = cl_cross(d,e2);
	a = cl_dot(e1,h);

	if (a > -0.00001f && a < 0.00001f)
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
	float result = f * cl_dot(e2,q);

	if (result > 0.00001f) // ray intersection (v smere luca)
		return result;

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
	
	for(i = 0; i < c_params.s[2]; i++)
	{
		ref_triangle = c_targets[(ref_ray * c_params.s[2]) + i];
		if(ref_triangle == 0)						// uz nemame dalsie trojuholniky v zozname, TODO, dat na -1
			break;

		ref_triangle = ref_triangle - 1;			// naschval posunute aby som mohol pouzit 0 ako neexistujuci trojuholnik

		v0 = c_vertices[c_triangles[ref_triangle].s[0]];
		v1 = c_vertices[c_triangles[ref_triangle].s[1]];
		v2 = c_vertices[c_triangles[ref_triangle].s[2]];

		dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2);
		if(dist2 < FLOAT_MAX)
		{
			theta = acos( cl_dot(ray, normal) / cl_length(ray) );
			theta = theta * (180.0f / (float)M_PI);
			if((theta < 90.0f) && (dist2 < dist))
				dist = dist2;
		}
	}

	c_outputs[ref_ray] = dist;
}
}