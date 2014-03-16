// SDFOpenCL.h : subor pre OpenCL manipulaciu
#pragma once
#include "StringHelper.h"
#include "OpenCLDebug.h"

namespace OpenCLForm
{
	//using namespace std;
	using namespace OpenCLDebugger;

	public class COpenCL
	{
	public:
		COpenCL();
		~COpenCL();

		int InitOpenCL();
		int LoadKernel1(const char* cSourceFile);
		int LoadKernel2(const char* cSourceFile);
		int LoadKernel3(const char* cSourceFile);
		int BuildKernel1();
		int BuildKernel2();
		int BuildKernel3();
		int GetGPUVariables();
		int SetupMemory(unsigned int ss_triangles, unsigned int ss_vertices, unsigned int ss_origins, unsigned int ss_rays, unsigned int ss_targets, unsigned int ss_outputs);
		int SetupMemory2(unsigned int ss_triangles, unsigned int ss_nodes, unsigned int ss_node_tria, unsigned int ss_rays, unsigned int ss_outputs);
		int SetupMemory3(unsigned int ss_results, unsigned int ss_weights);
		int SetupMemory4(unsigned int ss_points, unsigned int ss_pnodes, unsigned int ss_pnode_origins, unsigned int ss_pnode_values, unsigned int ss_pnode_counts);
		int LaunchKernel(cl_uint3	*c_triangles, cl_float4	*c_vertices, cl_uint *c_origins, cl_float4 *c_rays, cl_uint *c_targets, cl_float *c_outputs, cl_uint4 c_params);
		int LaunchKernel2(cl_float4 *c_triangles, cl_uint *c_nodes, cl_uint *c_node_tria, cl_float4 o_min, cl_float4 o_max, cl_float bias, cl_float4 *c_rays, cl_uint n_rays, cl_uint n_triangles, cl_float *c_outputs);
		int LaunchKernel3(cl_float *c_outputs, cl_float *c_results, cl_float *c_weights, cl_uint n_triangles);
		int LaunchKernel4(cl_float4 *c_points, cl_float4 *c_pnode_origins, cl_uint *c_pnodes, cl_float *c_pnode_values, cl_uint *c_pnode_counts, cl_float *c_results, cl_uint n_points, cl_float o_size);
		void WaitForFinish();

		cl_uint		num_cores;				// pocet paralelnych jednotiek
		size_t		num_1D_work_items;		// celkove maximum, to co je v dimenziach sa nasobi
		cl_uint		num_dimensions;			// predpokladam ze 3
		size_t		num_ND_work_items[3];	// mali by byt 3 a znamenaju kolko maximalne moze mat kazda dimenzia zvlast
		cl_ulong	num_memsize;			// maximalna velkost pamete, v B

		size_t global;  // global domain size for our calculation
		size_t local;   // local domain size for our calculation

		size_t debug_len;
		char debug_buffer[32768];
		COpenCLDebug *debugger;
	private:
		cl_int err;     // error code returned from api calls
   
		cl_platform_id cpPlatform;	// OpenCL platform
		cl_device_id device_id;		// compute device id
		cl_context context;			// compute context
		cl_command_queue commands;	// compute command queue
		cl_program program1;			// compute program
		cl_program program2;			// compute program
		cl_program program3;			// compute program
		cl_kernel kernel1;			// compute kernel
		cl_kernel kernel2;			// compute kernel
		cl_kernel kernel3;			// compute kernel
		
		size_t szKernelLength;		// Byte size of kernel code
		char* cSourceCL;			// Buffer to hold source for compilation

		// velkosti pre dane pametove bloky
		unsigned int s_triangles;
		unsigned int s_vertices;
		unsigned int s_origins;
		unsigned int s_nodes;
		unsigned int s_node_tria;
		unsigned int s_rays;
		unsigned int s_targets;
		unsigned int s_outputs;
		unsigned int s_weights;
		unsigned int s_results;
		unsigned int s_points;
		unsigned int s_pnodes;
		unsigned int s_pnode_origins;
		unsigned int s_pnode_values;
		unsigned int s_pnode_counts;

		// OpenCL Buffery
		cl_mem b_triangles;
		cl_mem b_nodes;
		cl_mem b_node_tria;
		cl_mem b_vertices;
		cl_mem b_origins;
		cl_mem b_rays;
		cl_mem b_targets;
		cl_mem b_outputs;
		cl_mem b_results;
		cl_mem b_weights;
		cl_mem b_points;
		cl_mem b_pnodes;
		cl_mem b_pnode_origins;
		cl_mem b_pnode_values;
		cl_mem b_pnode_counts;

		int moznost;
	};
}