// moja kernel funkcia
#define FLOAT_MAX  99999.0

float rayIntersectsTriangle(float4 p, float4 d, float4 v0, float4 v1, float4 v2)
{
	float4 e1 = v1 - v0;
	float4 e2 = v2 - v0;
	float a,f,u,v;

	float4 h = cross(d,e2);
	a = dot(e1,h);

	if (a > -0.00001f && a < 0.00001f)
		return FLOAT_MAX;

	f = 1.0f / a;
	float4 s = p - v0;
	u = f * dot(s,h);

	if (u < 0.0f || u > 1.0f)
		return FLOAT_MAX;

	float4 q = cross(s,e1);
	v = f * dot(d,q);

	if (v < 0.0f || u + v > 1.0f)
		return FLOAT_MAX ;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float result = f * dot(e2,q);

	if (result > 0.00001f) // ray intersection (v smere luca)
		return result;

	else // this means that there is a line intersection
		 // but not a ray intersection
		 return FLOAT_MAX;
}

__kernel void sdf(__global const uint3 *c_triangles,
                  __global const float4 *c_vertices,
				  __global const uint *c_origins,
				  __global const float4 *c_rays,
				  __global const uint *c_targets,
				  const uint4 c_params,
				  __global float *c_outputs
                  )
{
    const uint gid = get_global_id(0);
	//const int lid = get_local_id(0);
	uint ref_ray = gid;								// zisti kolkaty som luc v poradi
	uint ref_triangle = ref_ray / c_params.y;		// na zaklade toho zisti ku ktoremu trojuholniku patrim
	
	if(ref_triangle >= c_params.w)					// thready navyse ignorujeme
		return;

	/*c_outputs[ref_ray] = ref_ray;
	return;*/

	float dist = FLOAT_MAX;
	float dist2 = FLOAT_MAX;
	float theta = 0.0f;
	uint i = 0;

	const float4 ray = c_rays[ref_ray];
	float4 v0 = c_vertices[c_triangles[c_origins[ref_triangle]].x];
	float4 v1 = c_vertices[c_triangles[c_origins[ref_triangle]].y];
	float4 v2 = c_vertices[c_triangles[c_origins[ref_triangle]].z];
	const float4 center = (v0 + v1 + v2) / 3.0f;
	const float4 normal = normalize((-1.0f)*cross(v1-v0, v2-v0));
	
	for(i = 0; i < c_params.z; i++)
	{
		ref_triangle = c_targets[(ref_ray * c_params.z) + i];
		if(ref_triangle == 0)						// uz nemame dalsie trojuholniky v zozname, TODO, dat na -1
			break;

		ref_triangle = ref_triangle - 1;			// naschval posunute aby som mohol pouzit 0 ako neexistujuci trojuholnik

		v0 = c_vertices[c_triangles[ref_triangle].x];
		v1 = c_vertices[c_triangles[ref_triangle].y];
		v2 = c_vertices[c_triangles[ref_triangle].z];

		dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2);
		if(dist2 < FLOAT_MAX)
		{
			theta = acos( dot(ray, normal) / length(ray) );
			theta = theta * (180.0f / M_PI_F);
			if((theta < 90.0f) && (dist2 < dist))
				dist = dist2;
		}
	}

	c_outputs[ref_ray] = dist;
}