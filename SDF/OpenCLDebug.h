// kratky subor pre debugovanie OpenCL kodu
#pragma once

namespace OpenCLDebugger
{
	public class COpenCLDebug
	{
	public:
		COpenCLDebug();
		~COpenCLDebug();

		void SetArgSize(cl_uint size /* args_size */);
		void SetArgValue(cl_uint /* arg_index */, void * /* arg_value */);
		void ExecuteKernel( size_t   /* global_work_size */, size_t   /* local_work_size */);
	private:
		void** args;
		cl_uint arg_size;
		// pomocne OpenCL funkcie
		cl_float4 cl_minus(cl_float4 a, cl_float4 b);
		cl_float4 cl_plus(cl_float4 a, cl_float4 b);
		cl_float cl_dot(cl_float4 a, cl_float4 b);
		cl_float4 cl_cross(cl_float4 a, cl_float4 b);
		cl_float4 cl_normalize(cl_float4 a);
		cl_float cl_length(cl_float4 a);
		cl_float4 cl_div(cl_float4 a, cl_float b);
		cl_float4 cl_multiply(cl_float4 a, cl_float b);
		//---------------------------------MOJE OPENCL FUNKCIE---------------------------------
		float rayIntersectsTriangle(cl_float4 p, cl_float4 d, cl_float4 v0, cl_float4 v1, cl_float4 v2);
		void sdf(const cl_uint3 *c_triangles,
					const cl_float4 *c_vertices,
					const cl_uint *c_origins,
					const cl_float4 *c_rays,
					const cl_uint *c_targets,
					const cl_uint4 c_params,
					float *c_outputs,
					const cl_uint gid
				);
	};
}