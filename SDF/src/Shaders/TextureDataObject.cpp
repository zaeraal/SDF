#include "stdafx.h"
#include "TextureDataObject.h"

namespace ShaderManipulation
{
	TextureDataObject::TextureDataObject()
	{
		Width = 0;
		Height = 0;
		Data = 0;
		PixelDataFormat = GL_UNSIGNED_BYTE;
		Channels = GL_RGBA;
		internalFormat = 4;
	}

	TextureDataObject::~TextureDataObject()
	{
		Width = 0;
		Height = 0;
		delete [] Data;
		PixelDataFormat = GL_UNSIGNED_BYTE;
		Channels = GL_RGBA;
		internalFormat = 4;
	}
}