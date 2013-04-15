// CSDF.h : subor pre pracu so SDF funkciou
#pragma once

namespace SDFStructures
{
	public class CSDF
	{
	public:
		CSDF();
		~CSDF();

		void ComputeValue(const std::vector<float> values, const std::vector<float> inverse_Yangles);
		float CalcMedian(const std::vector<float> values, unsigned int size);
		float CalcMean(const std::vector<float> values, unsigned int size);
		float CalcStandardDeviation(const std::vector<float> values, unsigned int size);
		void Smooth(const std::vector<float> values, const std::vector<float> weights);


		void Normalize1(float min, float max, float alfa);
		void Normalize2(float min, float max, float alfa);


		float value;
		float smoothed;
		float normalized1;
		float normalized2;
	};
}