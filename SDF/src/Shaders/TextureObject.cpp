#include "stdafx.h"
#include "TextureObject.h"

namespace ShaderManipulation
{
	GLuint TextureObject::DefaultSettings_MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	GLuint TextureObject::DefaultSettings_MagFilter = GL_LINEAR;
	float TextureObject::DefaultSettings_AnisotropicFilter = 16.0;
	GLuint TextureObject::DefaultSettings_RepeatS = GL_CLAMP_TO_EDGE;
	GLuint TextureObject::DefaultSettings_RepeatT = GL_CLAMP_TO_EDGE;


	TextureObject::TextureObject()
	{
		Handle = 0;
		this->FileName = "NoName";
		Width = 0;
		Height = 0;
		Alpha_flag = false;
		HDR_flag = false;
		Ready = false;
		Data = 0;

		Settings_MagFilter = DefaultSettings_MagFilter;
		Settings_MinFilter = DefaultSettings_MinFilter;
		Settings_RepeatS = DefaultSettings_RepeatS;
		Settings_RepeatT = DefaultSettings_RepeatT;
		Settings_AnisotropicFilter = DefaultSettings_AnisotropicFilter;
	}


	void TextureObject::ChangeDefaultSettings_MinFilter( GLuint value ) { DefaultSettings_MinFilter = value; }
	void TextureObject::ChangeDefaultSettings_MagFilter( GLuint value ) { DefaultSettings_MagFilter = value; }
	void TextureObject::ChangeDefaultSettings_AnisotropicFilter( float value )
	{
		GLfloat max_ani = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_ani);
		DefaultSettings_AnisotropicFilter = clamp( value, 1.0, max_ani );
	}
	void TextureObject::ChangeDefaultSettings_RepeatS( GLuint value ) { DefaultSettings_RepeatS = value; }
	void TextureObject::ChangeDefaultSettings_RepeatT( GLuint value ) { DefaultSettings_RepeatT = value; }

	void TextureObject::setParameter_MinFilter( GLuint value )
	{
		Settings_MinFilter = value;
		if ( Handle == 0 ) return;
		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Settings_MinFilter );
	}
	void TextureObject::setParameter_MagFilter( GLuint value )
	{
		Settings_MagFilter = value;
		if ( Handle == 0 ) return;
		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Settings_MagFilter );
	}
	float TextureObject::clamp(float value, float minv, float maxv)
	{
		if(value < minv)
			value = minv;
		else if(value > maxv)
			value = maxv;
		return value;
	}
	void TextureObject::setParameter_AnisotropicFilter( float value )
	{
		GLfloat max_ani = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_ani);
		Settings_AnisotropicFilter = clamp( value, 1.0, max_ani );
		if ( Handle == 0 ) return;
		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Settings_AnisotropicFilter );
	}
	void TextureObject::setParameter_RepeatS( GLuint value )
	{
		Settings_RepeatS = value;
		if ( Handle == 0 ) return;
		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DefaultSettings_RepeatS );
	}
	void TextureObject::setParameter_RepeatT( GLuint value )
	{
		Settings_RepeatT = value;
		if ( Handle == 0 ) return;
		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DefaultSettings_RepeatT );
	}


	GLuint TextureObject::getHandle()
	{
		if ( !Ready ) CreateTexture();
		return Handle;
	}
	std::string TextureObject::getFileName()
	{
		return FileName;
	}
	int TextureObject::getWidth()
	{
		return Width;
	}
	int TextureObject::getHeight()
	{
		return Height;
	}
	bool TextureObject::hasAlpha()
	{
		return Alpha_flag;
	}
	bool TextureObject::isHDR()
	{
		return HDR_flag;
	}

	void TextureObject::Bind( int position )
	{
		glActiveTexture( GL_TEXTURE0 + position );
		if ( !Ready ) 
		{
			CreateTexture();
		}
		glBindTexture( GL_TEXTURE_2D, Handle );
	}


	void TextureObject::CreateTexture( TextureDataObject* D )
	{
		Data = D;
		CreateTexture();
	}

	void TextureObject::CreateTexture()
	{
		if ( Data == 0 ) return;

		Width = Data->Width;
		Height = Data->Height;
		HDR_flag = ( Data->PixelDataFormat == GL_FLOAT );
		Alpha_flag = ( Data->Channels == GL_RGBA );
		FileName = Data->FileName;

		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glEnable( GL_TEXTURE_2D );

		glGenTextures( 1, &Handle );

		glBindTexture( GL_TEXTURE_2D, Handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Settings_MagFilter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Settings_MinFilter );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Settings_AnisotropicFilter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DefaultSettings_RepeatS );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DefaultSettings_RepeatT );

		glTexImage2D( GL_TEXTURE_2D, 0, Data->internalFormat, Width, Height, 0, Data->Channels, Data->PixelDataFormat, Data->Data );

		glGenerateMipmap( GL_TEXTURE_2D );	//create even for not mipmapped textures so that settings can be changed later
		glPopAttrib();

		if(glGetError() != 0)
		{
			//std::cout<<"  >> Problematic texture: '"<<Data->FileName<<"'\n";
			Ready = false;
		} else {
			Ready = true;
		}
		delete Data;
		Data = 0;
	}

	void TextureObject::DeleteTexture()
	{
		glBindTexture( GL_TEXTURE_2D, Handle );
		glDeleteTextures( 1, &Handle );
		Handle = 0;
		Ready = false;
		Width = 0;
		Height = 0;
		Alpha_flag = false;
		HDR_flag = false;
		FileName = "";
	}
}