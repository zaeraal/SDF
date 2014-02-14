// moja kernel funkcia
#define FLOAT_MAX  99999.0
#define KERNEL_SIZE  4096

inline float rayIntersectsTriangle(const float4 p, const float4 d, const float4 v0, const float4 v1, const float4 v2, const float bias)
	{
		float4 e1 = v1 - v0;
		float4 e2 = v2 - v0;
		float a,f,u,v;

		float4 h = cross(d,e2);
		a = dot(e1,h);

		if (a > -bias && a < bias)
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
		f = f * dot(e2,q);

		if (f > bias) // ray intersection (v smere luca)
			return f;

		else // this means that there is a line intersection
				// but not a ray intersection
				return FLOAT_MAX;
	}

inline float4 Multiply(float4 a, float4 t, float4 n, float4 b)
{
	float4 r1; r1.x = t.x; r1.y = n.x; r1.z = b.x; r1.w = 0;
	float4 r2; r2.x = t.y; r2.y = n.y; r2.z = b.y; r2.w = 0;
	float4 r3; r3.x = t.z; r3.y = n.z; r3.z = b.z; r3.w = 0;

	float4 result;
	result.x = dot(a, r1);
	result.y = dot(a, r2);
	result.z = dot(a, r3);
	result.w = 0;

	return normalize(result);
}

inline uchar first_node(float4 t0, float4 tm)
{
	unsigned char answer = 0;   // initialize to 00000000
	// select the entry plane and set bits
	if(t0.x > t0.y)
	{
		if(t0.x > t0.z)					// PLANE YZ
		{
			if(tm.y < t0.x) answer|=2;    // set bit at position 1
			if(tm.z < t0.x) answer|=1;    // set bit at position 0
			return answer;
		}  
	} else
	{      
		if(t0.y > t0.z)					// PLANE XZ
		{
			if(tm.x < t0.y) answer|=4;    // set bit at position 2
			if(tm.z < t0.y) answer|=1;    // set bit at position 0
			return answer;
		}
	}
	// PLANE XY
	if(tm.x < t0.z) answer|=4;			// set bit at position 2
	if(tm.y < t0.z) answer|=2;			// set bit at position 1
	return answer;
}

inline uchar new_node(float txm, unsigned char x, float tym, unsigned char y, float tzm, unsigned char z)
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

inline bool CheckValid(unsigned char mask, unsigned char num)
{
	return((mask >> num) & 1);
}

typedef struct {
	float4       t0stack[STACK_SIZE];
	float4       t1stack[STACK_SIZE];
	int				cstack[STACK_SIZE];
	unsigned int	ostack[STACK_SIZE];
}CastStack;

inline inline uint read (CastStack *stack, int idx, float4 *t0, float4 *t1, int *cnode)
{
	(*t0) = (*stack).t0stack[idx];
	(*t1) = (*stack).t1stack[idx];
	(*cnode) = (*stack).cstack[idx];

	return (*stack).ostack[idx];
}

inline void write (CastStack *stack, int idx, float4 t0, float4 t1, int cnode, unsigned int node)
{
	(*stack).t0stack[idx] = t0;
	(*stack).t1stack[idx] = t1;
	(*stack).cstack[idx] = cnode;
	(*stack).ostack[idx] = node;
}

__constant unsigned char c_popc8LUT[] =
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
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 0,
};
inline unsigned char popc8(unsigned char mask)
{
	return c_popc8LUT[mask];
}

/*inline bool CanAdd(uint8 *short_shack, unsigned char *ss_idx, unsigned int triangle)
{
	if((*short_shack).x == triangle) return false;
	if((*short_shack).y == triangle) return false;
	if((*short_shack).z == triangle) return false;
	if((*short_shack).w == triangle) return false;
	if((*short_shack).s4 == triangle) return false;
	if((*short_shack).s5 == triangle) return false;
	if((*short_shack).s6 == triangle) return false;
	if((*short_shack).s7 == triangle) return false;

	(*ss_idx) = (*ss_idx) + 1; (*ss_idx) = (*ss_idx) % 8;
	switch((*ss_idx))
	{
		case 0: (*short_shack).x = triangle; break;
		case 1: (*short_shack).y = triangle; break;
		case 2: (*short_shack).z = triangle; break;
		case 3: (*short_shack).w = triangle; break;
		case 4: (*short_shack).s4 = triangle; break;
		case 5: (*short_shack).s5 = triangle; break;
		case 6: (*short_shack).s6 = triangle; break;
		case 7: (*short_shack).s7 = triangle; break;
		default: break;
		
	}
	return true;
}*/

__kernel void smooth(__global const float4 *c_triangles,
                  __global const uint *c_nodes,
				  __global const uint *c_node_tria,
				  __global const float4 *c_rays,
				  __global float *c_outputs,
					const float4 o_min,
					const float4 o_max,
					const float bias,
					const uint n_rays,
					const uint n_triangles,
					const uint poradie
                  )

{
	const uint ref_ray = get_global_id(0) + (poradie * KERNEL_SIZE);	// zisti kolkaty som luc v poradi
	const uint ref_triangle = (uint)(ref_ray / n_rays);					// na zaklade toho zisti ku ktoremu trojuholniku patrim
	
	if(ref_triangle > n_triangles)										// thready navyse ignorujeme
		return;

	/*c_outputs[ref_ray] = 10;
	return;*/

	// premenne zadefinovat dopredu
	const float4 v0 = c_triangles[ref_triangle * 3];
	const float4 v1 = c_triangles[ref_triangle * 3 + 1];
	const float4 v2 = c_triangles[ref_triangle * 3 + 2];
	const float4 center = (v0 + v1 + v2) / 3.0f;
	const float4 normal = normalize(cross(v1-v0, v2-v0) * (-1.0f));
	const float4 tangens = normalize(v0-v2);
	const float4 binormal = normalize(cross(tangens, normal));

	float4 ray = Multiply(c_rays[ref_ray % n_rays], tangens, normal, binormal);

	float dist = FLOAT_MAX;
	unsigned char idx = 0;

	// avoid division by zero
	if (fabs(ray.x) < bias) ray.x = ray.x < 0.0 ? -bias : bias;
	if (fabs(ray.y) < bias) ray.y = ray.y < 0.0 ? -bias : bias;
	if (fabs(ray.z) < bias) ray.z = ray.z < 0.0 ? -bias : bias;

	float invdirx = 1.0f / fabs(ray.x);
	float invdiry = 1.0f / fabs(ray.y);
	float invdirz = 1.0f / fabs(ray.z);

	float4 t0, t1;
	t0.w = 0; t1.w = 0;

	// fixes for rays with negative direction
	if(ray.x < 0.0)
	{
		t0.x = (o_max.x - center.x) * -invdirx;
		t1.x = (o_min.x - center.x) * -invdirx;
		idx |= 4 ; //bitwise OR (latest bits are XYZ)
	}
	else
	{
		t0.x = (o_min.x - center.x) * invdirx;
		t1.x = (o_max.x - center.x) * invdirx;
	}
	if(ray.y < 0.0)
	{
		t0.y = (o_max.y - center.y) * -invdiry;
		t1.y = (o_min.y - center.y) * -invdiry;
		idx |= 2 ;
	}
	else
	{
		t0.y = (o_min.y - center.y) * invdiry;
		t1.y = (o_max.y - center.y) * invdiry;
	}
	if(ray.z < 0.0)
	{
		t0.z = (o_max.z - center.z) * -invdirz;
		t1.z = (o_min.z - center.z) * -invdirz;
		idx |= 1 ;
	}
	else
	{
		t0.z = (o_min.z - center.z) * invdirz;
		t1.z = (o_max.z - center.z) * invdirz;
	}

	if( max(max(t0.x,t0.y),t0.z) < min(min(t1.x,t1.y),t1.z) )
	{ 
		float4 tm;
		int currNode = -1;
		CastStack stack;
		int scale = 0;
		unsigned int node = c_nodes[0];
		unsigned char sons = (node & 0xFF);
		unsigned int tindex;
		unsigned int size = 0;
		unsigned int i = 0;
		/*uint8 short_shack;
		short_shack.x = ref_triangle;
		short_shack.y = ref_triangle;
		short_shack.z = ref_triangle;
		short_shack.w = ref_triangle;
		short_shack.s4 = ref_triangle;
		short_shack.s5 = ref_triangle;
		short_shack.s6 = ref_triangle;
		short_shack.s7 = ref_triangle;*/
		unsigned char ss_idx = 0;
		float dist2 = FLOAT_MAX;
		float theta = 0.0f;
		float4 v0, v1, v2;
		float4 tnormal;
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
				if((t1.x < 0.0) || (t1.y < 0.0) || (t1.z < 0.0))
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
						//if(CanAdd(&short_shack, &ss_idx, c_node_tria[tindex+i]))
						{
							v0 = c_triangles[c_node_tria[tindex+i] * 3 + 0];
							v1 = c_triangles[c_node_tria[tindex+i] * 3 + 1];
							v2 = c_triangles[c_node_tria[tindex+i] * 3 + 2];

							dist2 = rayIntersectsTriangle(center, ray, v0, v1, v2, bias);
							if((dist2 < FLOAT_MAX) && (dist2 > bias))
							{
								tnormal = normalize(cross(v1-v0, v2-v0));
								theta = acos( dot(ray, tnormal) / length(ray) );
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
				tm = (t0+t1) * 0.5f;
				currNode = first_node(t0,tm);
			}
			else
				tm = (t0+t1) * 0.5f;

			switch (currNode)
			{
			case 0:
				if(CheckValid(sons, idx))
				{
					currNode = new_node(tm.x,4,tm.y,2,tm.z,1);
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
					currNode = new_node(tm.x,4,tm.y,2,tm.z,1);
					scale--;
				}
				break;
			case 1: 
				if(CheckValid(sons, 1^idx))
				{
					currNode = new_node(tm.x,5,tm.y,3,t1.z,8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.z = tm.z;
					t1.x = tm.x;
					t1.y = tm.y;
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
					currNode = new_node(tm.x,5,tm.y,3,t1.z,8);
					scale--;
				}
				break;
			case 2:
				if(CheckValid(sons, 2^idx))
				{
					currNode = new_node(tm.x,6,t1.y,8,tm.z,3);
					write(&stack, scale, t0, t1, currNode, node);
					t0.y = tm.y;
					t1.x = tm.x;
					t1.z = tm.z;
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
					currNode = new_node(tm.x,6,t1.y,8,tm.z,3);
					scale--;
				}
				break;
			case 3:
				if(CheckValid(sons, 3^idx))
				{
					currNode = new_node(tm.x,7,t1.y,8,t1.z,8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.y = tm.y;
					t0.z = tm.z;
					t1.x = tm.x;
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
					currNode = new_node(tm.x,7,t1.y,8,t1.z,8);
					scale--;
				}
				break;
			case 4:
				if(CheckValid(sons, 4^idx))
				{
					currNode = new_node(t1.x,8,tm.y,6,tm.z,5);
					write(&stack, scale, t0, t1, currNode, node);
					t0.x = tm.x;
					t1.y = tm.y;
					t1.z = tm.z;
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
					currNode = new_node(t1.x,8,tm.y,6,tm.z,5);
					scale--;
				}
				break;
			case 5:
				if(CheckValid(sons, 5^idx))
				{
					currNode = new_node(t1.x,8,tm.y,7,t1.z,8);
					write(&stack, scale, t0, t1, currNode, node);
					t0.x = tm.x;
					t0.z = tm.z;
					t1.y = tm.y;
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
					currNode = new_node(t1.x,8,tm.y,7,t1.z,8);
					scale--;
				}
				break;
			case 6:
				if(CheckValid(sons, 6^idx))
				{
					currNode = new_node(t1.x,8,t1.y,8,tm.z,7);
					write(&stack, scale, t0, t1, currNode, node);
					t0.x = tm.x;
					t0.y = tm.y;
					t1.z = tm.z;
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
					currNode = new_node(t1.x,8,t1.y,8,tm.z,7);
					scale--;
				}
				break;
			case 7:
				if(CheckValid(sons, 7^idx))
				{
					currNode = 8;
					write(&stack, scale, t0, t1, currNode, node);
					t0.x = tm.x;
					t0.y = tm.y;
					t0.z = tm.z;
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

	c_outputs[ref_ray] = dist;
}