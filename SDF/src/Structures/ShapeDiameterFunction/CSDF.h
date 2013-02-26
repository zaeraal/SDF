// CSDF.h : subor pre pracu so SDF funkciou
#pragma once

namespace SDFStructures
{
	public class CSDF
	{
	public:
		CSDF();
		~CSDF();

		void ComputeValue(const std::vector<double> values, const std::vector<double> inverse_Yangles);
		double CalcMedian(const std::vector<double> values, unsigned int size);
		double CalcMean(const std::vector<double> values, unsigned int size);
		double CalcStandardDeviation(const std::vector<double> values, unsigned int size);


		void Normalize1(double min, double max, double alfa);
		void Normalize2(double min, double max, double alfa);


		double value;
		double normalized1;
		double normalized2;
	};
}