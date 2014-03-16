#pragma once
#include "glew.h"
#include "TextureDataObject.h"

namespace ShaderManipulation
{
	class TextureObject
	{
		GLuint Handle;
		int Width, Height;
		bool Alpha_flag;
		bool HDR_flag;
		std::string FileName;
		bool Ready;

		TextureDataObject* Data;
		void CreateTexture();

		static GLuint DefaultSettings_MinFilter;
		static GLuint DefaultSettings_MagFilter;
		static float DefaultSettings_AnisotropicFilter;
		static GLuint DefaultSettings_RepeatS;
		static GLuint DefaultSettings_RepeatT;

		GLuint Settings_MinFilter;
		GLuint Settings_MagFilter;
		float Settings_AnisotropicFilter;
		GLuint Settings_RepeatS;
		GLuint Settings_RepeatT;
	public:
		TextureObject();
		void Bind( int position );
		void CreateTexture( TextureDataObject* D );
		void DeleteTexture();	// Note: This is not destructor of the object, the method
		//		 will actually delete the texture data from the GPU.

		GLuint getHandle();
		std::string getFileName();
		int getWidth();
		int getHeight();
		bool hasAlpha();
		bool isHDR();

		void setParameter_MinFilter( GLuint value );
		void setParameter_MagFilter( GLuint value );
		void setParameter_AnisotropicFilter( float value );
		void setParameter_RepeatS( GLuint value );
		void setParameter_RepeatT( GLuint value );
		static float clamp(float value, float minv, float maxv);

		static void ChangeDefaultSettings_MinFilter( GLuint value );
		static void ChangeDefaultSettings_MagFilter( GLuint value );
		static void ChangeDefaultSettings_AnisotropicFilter( float value );
		static void ChangeDefaultSettings_RepeatS( GLuint value );
		static void ChangeDefaultSettings_RepeatT( GLuint value );

		friend class FrameBufferLayer;
	};
}