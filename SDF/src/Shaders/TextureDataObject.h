#pragma once
#include "glew.h"

namespace ShaderManipulation
{
	class TextureDataObject
	{
		int Width;
		int Height;
		GLuint PixelDataFormat;			// unsigned byte, float, ...
		GLuint Channels;				// rgb, rgba, ...
		GLuint internalFormat;			// 3,4..., or some special ie depth...
		BYTE* Data;
		std::string FileName;
	public:
		TextureDataObject();
		~TextureDataObject();

		friend class TextureObject;
	};

}