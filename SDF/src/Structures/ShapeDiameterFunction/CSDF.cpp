// CSDF.cpp : subor pre pracu so SDF funkciou
#include "stdafx.h"
#include "CSDF.h"

namespace SDFStructures
{
	// konstruktor
	CSDF::CSDF()
	{
		value = 0.0;
		smoothed = 0.0;
		normalized1 = 0.0;
		normalized2 = 0.0;
	}

	//destruktor
	CSDF::~CSDF()
	{
	}

	/// The SDF at a point is defined
	/// as the weighted average of all rays lengths which
	/// fall within one standard deviation from the median of
	/// all lengths. The weights used are the inverse of the angle
	/// between the ray to the center of the cone.
	void CSDF::ComputeValue(const std::vector<float> values, const std::vector<float> inverse_Yangles)
	{
		unsigned int size = values.size();
		if(size == 0)
			return;

		if(size == 1)
		{
			value = values[0];
			return;
		}

		float median = CalcMedian(values, size);
		float deviation = CalcStandardDeviation(values, size);

		float sum_values = 0.0;
		float sum_weights = 0.0;
		for(unsigned int i = 0; i < size; i++)
		{
			if(abs(values[i] - median) < deviation)
			{
				sum_values += values[i] * inverse_Yangles[i];
				sum_weights += inverse_Yangles[i];
			}
		}
		value = sum_values / sum_weights;
	}

	float CSDF::CalcMedian(const std::vector<float> values, unsigned int size)
	{
		float median = 0.0;

		// create a copy of our values
		std::vector<float> tmp(values);

		sort(tmp.begin(), tmp.end());

		if (size  % 2 == 0)
		{
			median = (tmp[size / 2 - 1] + tmp[size / 2]) / 2;
		}
		else 
		{
			median = tmp[size / 2];
		}

		return median;
	}

	float CSDF::CalcMean(const std::vector<float> values, unsigned int size)
	{
		float mean = 0.0;

		for(unsigned int i = 0; i < size; i++)
		{
			mean += values[i];
		}
		return (mean / float(size));
	}

	float CSDF::CalcStandardDeviation(const std::vector<float> values, unsigned int size)
	{
		float deviation = 0.0;

		float mean = CalcMean(values, size);

		// create a copy of our values
		std::vector<float> tmp(values);
		for(unsigned int i = 0; i < size; i++)
		{
			tmp[i] = (tmp[i] - mean) * (tmp[i] - mean);
		}
		deviation = sqrt(CalcMean(tmp, size));

		return deviation;
	}

	void CSDF::Normalize1(float min, float max, float alfa)
	{
		normalized1 = log(((value - min) / (max - min)) * alfa + 1.0f) / log(alfa + 1.0f);
	}

	void CSDF::Normalize2(float min, float max, float alfa)
	{
		normalized2 = log(((smoothed - min) / (max - min)) * alfa + 1.0f) / log(alfa + 1.0f);
	}

	void CSDF::Smooth(const std::vector<float> values, const std::vector<float> weights)
	{
		unsigned int size = values.size();
		if(size == 0)
			return;

		float sum_values = 0.0;
		float sum_weights = 0.0;
		for(unsigned int i = 0; i < size; i++)
		{
			sum_values += values[i] * weights[i];
			sum_weights += weights[i];
		}
		smoothed = sum_values / sum_weights;
	}
}