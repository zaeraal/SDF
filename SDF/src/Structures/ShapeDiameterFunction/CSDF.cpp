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
	void CSDF::ComputeValue(const std::vector<double> values, const std::vector<double> inverse_Yangles)
	{
		unsigned int size = values.size();
		if(size == 0)
			return;

		if(size == 1)
		{
			value = values[0];
			return;
		}

		double median = CalcMedian(values, size);
		double deviation = CalcStandardDeviation(values, size);

		double sum_values = 0.0;
		double sum_weights = 0.0;
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

	double CSDF::CalcMedian(const std::vector<double> values, unsigned int size)
	{
		double median = 0.0;

		// create a copy of our values
		std::vector<double> tmp(values);

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

	double CSDF::CalcMean(const std::vector<double> values, unsigned int size)
	{
		double mean = 0.0;

		for(unsigned int i = 0; i < size; i++)
		{
			mean += values[i];
		}
		return (mean / double(size));
	}

	double CSDF::CalcStandardDeviation(const std::vector<double> values, unsigned int size)
	{
		double deviation = 0.0;

		double mean = CalcMean(values, size);

		// create a copy of our values
		std::vector<double> tmp(values);
		for(unsigned int i = 0; i < size; i++)
		{
			tmp[i] = (tmp[i] - mean) * (tmp[i] - mean);
		}
		deviation = sqrt(CalcMean(tmp, size));

		return deviation;
	}

	void CSDF::Normalize1(double min, double max, double alfa)
	{
		normalized1 = log(((value - min) / (max - min)) * alfa + 1.0) / log(alfa + 1.0);
	}

	void CSDF::Normalize2(double min, double max, double alfa)
	{
		normalized2 = log(((smoothed - min) / (max - min)) * alfa + 1.0) / log(alfa + 1.0);
	}

	void CSDF::Smooth(const std::vector<double> values, const std::vector<double> weights)
	{
		unsigned int size = values.size();
		if(size == 0)
			return;

		double sum_values = 0.0;
		double sum_weights = 0.0;
		for(unsigned int i = 0; i < size; i++)
		{
			sum_values += values[i] * weights[i];
			sum_weights += weights[i];
		}
		smoothed = sum_values / sum_weights;
	}
}