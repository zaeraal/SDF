// moja kernel funkcia
#define FLOAT_MAX  99999.0

__kernel void proces(__global float *c_outputs,
		__global float *c_results,
		__global float *weights,
		const uint n_triangles
		)
	{
		uint ref_triangle = get_global_id(0);						// kolkaty som luc v poradi
	
		if(ref_triangle >= n_triangles)								// thready navyse ignorujeme
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