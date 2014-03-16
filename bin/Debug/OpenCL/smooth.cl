// moja kernel funkcia
#define SQRT_THREE 1.7320508075f

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
__constant unsigned char c_pipc8LUT[] =
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

__kernel void smooth(__global const float4 *c_points,
					 __global const float4 *c_node_origins,
					 __global const uint *c_nodes,
					 __global const float *c_node_values,
					 __global const uint *c_node_counts,
					 __global float *c_results,
					 const uint n_points,
					 const float o_size
					)
{
	uint ref_point = get_global_id(0);								// kolkaty som point v poradi

	if(ref_point > n_points)										// thready navyse ignorujeme (nemali by byt)
		return;

	float4 pointik1 = c_points[ref_point];
	float value = pointik1.w;
	pointik1.w = 1.0f;

	float maxval = value * 0.5f;
	float distanc = 0;

	float weight = 0;
	float cube_radius = SQRT_THREE;
	float sum_values = 0.0f;
	float sum_weights = 0.0f;

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
			
			weight = (float)c_node_counts[tmp];
			cube_radius = o_size_array[idx] * SQRT_THREE;
			distanc = distance(pointik1, c_node_origins[tmp]) + cube_radius - maxval;
			if(distanc > 0.0001f)
			{
				float pct = (distanc / cube_radius) * 100.0f;		// kolko % je vonku
				if(pct < 0.0001f)									// nesmu byt priliz male!!
					pct = 0.0001f;
				pct = 1 / pct;										// kolko % je vnutry
				weight = weight * pct;
			}

			sum_values += c_node_values[tmp] * weight;
			sum_weights += weight;

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
			distanc = distance(pointik1, c_node_origins[node]) - maxval;
			cube_radius = o_size_array[idx+1] * SQRT_THREE;
			if(distanc <= (-cube_radius))
			{
				// copy pasta ak som v leafe, ale pre SYNA!!!
				weight = (float)c_node_counts[node];
				distanc = distanc + cube_radius;
				if(distanc > 0.0001f)
				{
					float pct = (distanc / cube_radius) * 100.0f;		// kolko % je vonku
					if(pct < 0.0001f)									// nesmu byt priliz male!!
						pct = 0.0001f;
					pct = 1 / pct;										// kolko % je vnutry
					weight = weight * pct;
				}

				sum_values += c_node_values[node] * weight;
				sum_weights += weight;

				sons = (tmp & 0xFF);
				sons = sons >> (id + 1);
				sons = sons << (id + 1);
				id = pipc8(sons);
				write(&stack, idx, tmp, id);
			}
			else if(distanc <= cube_radius)
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

	weight = 4.0f;
	sum_weights += weight;
	sum_values += value * weight;
	c_results[ref_point] = sum_values / sum_weights;
}