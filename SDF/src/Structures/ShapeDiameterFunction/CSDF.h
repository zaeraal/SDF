// CSDF.h : subor pre pracu so SDF funkciou
#pragma once

namespace SDFStructures
{
	public class CSDF
	{
	public:
		CSDF();
		~CSDF();

		void ComputeValue(double* hodnoty, unsigned int size);
		void Normalize(double min, double max, double alfa);


		double value;
		double normalized;
	};
}