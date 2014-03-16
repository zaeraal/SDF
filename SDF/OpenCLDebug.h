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
		void ExecuteKernel2( size_t   /* global_work_size */, size_t   /* local_work_size */);
		void ExecuteKernel3( size_t   /* global_work_size */, size_t   /* local_work_size */);
		void ExecuteKernel4( size_t   /* global_work_size */, size_t   /* local_work_size */);
		unsigned int nodeCount;
		unsigned int triangleCount;
		unsigned int nn_triangles;
		unsigned int max_outputs;
	private:
		void** args;
		cl_uint arg_size;
		// pomocne OpenCL funkcie
		cl_float cl_distance(cl_float4 a, cl_float4 b);
		cl_float4 cl_minus(cl_float4 a, cl_float4 b);
		cl_float4 cl_plus(cl_float4 a, cl_float4 b);
		cl_float cl_dot(cl_float4 a, cl_float4 b);
		cl_float4 cl_cross(cl_float4 a, cl_float4 b);
		cl_float4 cl_normalize(cl_float4 a);
		cl_float cl_length(cl_float4 a);
		cl_float4 cl_div(cl_float4 a, cl_float b);
		cl_float4 cl_multiply(cl_float4 a, cl_float b);
		cl_float4 Multiply(cl_float4 a, cl_float4 t, cl_float4 n, cl_float4 b);
		//---------------------------------MOJE OPENCL FUNKCIE---------------------------------
		float rayIntersectsTriangle(const cl_float4 p, const cl_float4 d, const cl_float4 v0, const cl_float4 v1, const cl_float4 v2, const cl_float bias);
		unsigned char first_node(cl_float4 t0, cl_float4 tm);
		unsigned char new_node(float txm, unsigned char x, float tym, unsigned char y, float tzm, unsigned char z);
		bool CheckValid(unsigned char mask, unsigned char num);
		void proc_subtree(const cl_float4 *c_triangles,
			const cl_uint *c_nodes,
			const cl_uint *c_node_tria,
			const cl_float4 ray,
			const cl_float4 center,
			const cl_uint triangle_ref,
			const cl_float bias,
			cl_float *dist,
			unsigned char idx,
			cl_float4 t0,
			cl_float4 t1);
		void OctreeTraversal( const cl_float4 *c_triangles,
			const cl_uint *c_nodes,
			const cl_uint *c_node_tria,
			const cl_float4 o_min,
			const cl_float4 o_max,
			cl_float4 ray,
			const cl_float4 center,
			const cl_uint triangle_ref,
			const cl_float bias,
			cl_float *dist);
		void sdf(const cl_uint3 *c_triangles,
					const cl_float4 *c_vertices,
					const cl_uint *c_origins,
					const cl_float4 *c_rays,
					const cl_uint *c_targets,
					const cl_uint4 c_params,
					float *c_outputs,
					const cl_uint gid
				);
		void sdf2(const cl_float4 *c_triangles,
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
			);
		void sdf3(const cl_float4 *c_triangles,
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
			);

		void proces(float *c_outputs,
			float *c_results,
			float *weights,
			const cl_uint n_triangles,
			const cl_uint gid
			);
		void smooth(const cl_float4 *c_points,
			const cl_float4 *c_node_origins,
			const cl_uint *c_nodes,
			const cl_float *c_node_values,
			const cl_uint *c_node_counts,
			float *c_results,
			const cl_uint n_points,
			const float o_size,
			const cl_uint gid
			);
	};
}