#pragma once
#include "glew.h"
#include "ShaderObject.h"
#include "Vector4.h"

namespace ShaderManipulation
{
	using namespace MathStructures;

	class BlurObject
	{
		public:
		BlurObject();	

		ShaderObject* GaussianBlurShader;
		ShaderObject* SampledBlurShader;
	
		void Apply( float Size, Vector4 Channels, Vector4 Direction, GLuint Source, GLuint Destination );
		void Apply( float Size, Vector4 Channels, GLuint Source, GLuint Destination );
	};
}