#include "stdafx.h"
#include "glew.h"
#include "FrameBufferObject.h"

namespace ShaderManipulation
{
	FrameBufferLayer::FrameBufferLayer( FRAMEBUFFER_LAYER_TYPE layerType, int width, int height )
	{
		OK = false;
		Width = width;
		Height = height;
		LayerType = layerType;

		glGenTextures( 1, &TextureHandle );
		glBindTexture( GL_TEXTURE_2D, TextureHandle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0 );

		GLuint InternalFormat, Format, Type;
		DetermineFormatType( InternalFormat, Format, Type );

		glTexImage2D( GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, Type, NULL );

		if(glGetError() == 0) OK = true; else OK = false;

		Texture = new TextureObject();
			Texture->Handle = TextureHandle;
			Texture->Width = Width;
			Texture->Height = Height;
			Texture->Alpha_flag = ( Format == GL_RGBA );
			Texture->HDR_flag = ( Type == GL_FLOAT );
			Texture->FileName = "";
			Texture->Ready = OK;
			Texture->Data = 0;

		//std::cout<<"  > Created texture with valid status: "<<toString(OK)<<std::endl;
	}

	FrameBufferLayer::~FrameBufferLayer()
	{
		delete Texture;
		glBindTexture( GL_TEXTURE_2D, TextureHandle );
		glDeleteTextures( 1, &TextureHandle );
		OK = false;
	}

	void FrameBufferLayer::DetermineFormatType( GLuint& InternalFormat, GLuint& Format, GLuint& Type )
	{
		//defaults to FRAMEBUFFER_LAYER_TYPE_RGB_8
		InternalFormat = GL_RGB;
		Format = GL_RGB;
		Type = GL_UNSIGNED_BYTE;

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_RGBA_32 )
		{
			InternalFormat = GL_RGBA32F;
			Format = GL_RGBA;
			Type = GL_FLOAT;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_RGB_32 )
		{
			InternalFormat = GL_RGB32F;
			Format = GL_RGB;
			Type = GL_FLOAT;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_RGBA_16 )
		{
			InternalFormat = GL_RGBA16F;
			Format = GL_RGBA;
			Type = GL_FLOAT;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_RGB_16 )
		{
			InternalFormat = GL_RGB16F;
			Format = GL_RGB;
			Type = GL_FLOAT;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_RGBA_8 )
		{
			InternalFormat = GL_RGBA;
			Format = GL_RGBA;
			Type = GL_UNSIGNED_BYTE;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_DEPTH_32 )
		{
			InternalFormat = GL_DEPTH_COMPONENT32;
			Format = GL_DEPTH_COMPONENT;
			Type = GL_UNSIGNED_BYTE;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_DEPTH_24 )
		{
			InternalFormat = GL_DEPTH_COMPONENT24;
			Format = GL_DEPTH_COMPONENT;
			Type = GL_UNSIGNED_BYTE;
		}

		if ( LayerType == FRAMEBUFFER_LAYER_TYPE_DEPTH_16 )
		{
			InternalFormat = GL_DEPTH_COMPONENT16;
			Format = GL_DEPTH_COMPONENT;
			Type = GL_UNSIGNED_BYTE;
		}
	}
	void FrameBufferLayer::Resize( int width, int height )
	{
		if ( ( Width == width ) & ( Height == height ) ) return;
		Width = width;
		Height = height;
		Texture->Width = Width;
		Texture->Height = Height;
		if ( !OK ) return;

		glBindTexture( GL_TEXTURE_2D, TextureHandle );
		GLuint InternalFormat, Format, Type;
		DetermineFormatType( InternalFormat, Format, Type );

		glTexImage2D( GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, Type, NULL );
		if(glGetError() == 0) OK = true; else OK = false;

		Texture->Ready = OK;

		if ( !OK ) assert(false); //"Error resizing the framebuffer layer!\n";
	}

	GLuint FrameBufferLayer::getHandle()
	{
		if ( !OK ) return 0;
		return TextureHandle;
	}
	TextureObject* FrameBufferLayer::getTexture()
	{
		return Texture;
	}

	void FrameBufferLayer::setType( FRAMEBUFFER_LAYER_TYPE layerType )
	{
		if ( LayerType == layerType ) return;
		LayerType = layerType;

		GLuint InternalFormat, Format, Type;
		DetermineFormatType( InternalFormat, Format, Type );
		Texture->Alpha_flag = ( Format == GL_RGBA );
		Texture->HDR_flag = ( Type == GL_FLOAT );
		if ( !OK ) return;

		glBindTexture( GL_TEXTURE_2D, TextureHandle );	

		glTexImage2D( GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, Type, NULL );
		if(glGetError() == 0) OK = true; else OK = false;
	
		Texture->Ready = OK;
	}
	FRAMEBUFFER_LAYER_TYPE FrameBufferLayer::getType()
	{
		return LayerType;
	}
	int FrameBufferLayer::getWidth()
	{
		return Width;
	}
	int FrameBufferLayer::getHeight()
	{
		return Height;
	}
	bool FrameBufferLayer::isOK()
	{
		return OK;
	}

	FrameBufferObject::FrameBufferObject( int width, int height )
	{
		Width = width;
		Height = height;
	
		Layer.clear();
		DepthLayer.clear();

		//create buffer object
		glGenFramebuffers( 1, &Handle );
	}

	void FrameBufferObject::DefaultAttach( FRAMEBUFFER_LAYER_TYPE Type )
	{
		SetDepthLayer( 0, FRAMEBUFFER_LAYER_TYPE_DEPTH_32 );
		AssignDepthLayer( 0 );

		if (	( Type != FRAMEBUFFER_LAYER_TYPE_RGB_8 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGB_16 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGB_32 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_8 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_16 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_32 )
			) Type = FRAMEBUFFER_LAYER_TYPE_RGBA_16;

		for ( int i = 0; i < 8; i++ )
		{
			SetLayer( i, Type );
			AssignLayer( i, i );
		}
	}


	GLuint FrameBufferObject::Target( int Index )
	{
		return GL_COLOR_ATTACHMENT0_EXT + Index;
	}

	GLint FrameBufferObject::MaxAttachments()
	{
		GLint Result = 0;
		glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &Result);
		return Result;
	}

	bool FrameBufferObject::Valid()
	{
		GLint currentFB = 0;
		glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &currentFB );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, Handle );

		GLenum state = glCheckFramebufferStatus( GL_FRAMEBUFFER_EXT );
	
		bool returnValue = ( state == GL_FRAMEBUFFER_COMPLETE_EXT );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, currentFB );

		for ( FBLayerMapType::iterator L = Layer.begin(); L != Layer.end(); L++ )
		{
			if ( L->second != 0 ) 
			{
				returnValue = returnValue & L->second->isOK();
			} else returnValue = false;
		}

		for ( FBLayerMapType::iterator DL = DepthLayer.begin(); DL != DepthLayer.end(); DL++ )
		{
			if ( DL->second != 0 ) 
			{
				returnValue = returnValue & DL->second->isOK();
			} else returnValue = false;
		}

		return returnValue;
	}

	bool FrameBufferObject::validLayerIndex( int index )
	{
		if( Layer.find( index ) == Layer.end() ) return false;
		return Layer[ index ] != 0;
	}

	bool FrameBufferObject::validDepthLayerIndex( int index )
	{
		if( DepthLayer.find( index ) == DepthLayer.end() ) return false;
		return DepthLayer[ index ] != 0;
	}

	bool FrameBufferObject::validTargetIndex( int index )
	{
		if ( index < 0 ) return false;
		return ( index < MaxAttachments() );
	}

	void FrameBufferObject::DrawBuffers( int attachmentIndex0, int attachmentIndex1, int attachmentIndex2, 
										 int attachmentIndex3, int attachmentIndex4, int attachmentIndex5, 
										 int attachmentIndex6, int attachmentIndex7 )
	{
		int count = 8;
		GLuint buffers[ 8 ];

		buffers[ 0 ] = Target( attachmentIndex0 );
		buffers[ 1 ] = Target( attachmentIndex1 );
		buffers[ 2 ] = Target( attachmentIndex2 );
		buffers[ 3 ] = Target( attachmentIndex3 );
		buffers[ 4 ] = Target( attachmentIndex4 );
		buffers[ 5 ] = Target( attachmentIndex5 );
		buffers[ 6 ] = Target( attachmentIndex6 );
		buffers[ 7 ] = Target( attachmentIndex7 );
	
		if ( !validTargetIndex( attachmentIndex7 ) ) count = 7;
		if ( !validTargetIndex( attachmentIndex6 ) ) count = 6;
		if ( !validTargetIndex( attachmentIndex5 ) ) count = 5;
		if ( !validTargetIndex( attachmentIndex4 ) ) count = 4;
		if ( !validTargetIndex( attachmentIndex3 ) ) count = 3;
		if ( !validTargetIndex( attachmentIndex2 ) ) count = 2;
		if ( !validTargetIndex( attachmentIndex1 ) ) count = 1;
		if ( !validTargetIndex( attachmentIndex0 ) ) count = 0;

		if ( count <= 0 )
		{
			buffers[0] = GL_BACK;
			count = 1;		
		}
		glDrawBuffers( count, buffers );
	}


	void FrameBufferObject::AssignLayer( int index, int targetPosition )
	{
		if ( !validLayerIndex( index ) ) return;
		if ( Layer[ index ] == 0 ) return;
		if ( !Layer[ index ]->isOK() ) return;
		if ( !validTargetIndex( targetPosition ) ) return;


		GLint currentFB = 0;
		glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &currentFB );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, Handle );

		glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, Target( targetPosition ), GL_TEXTURE_2D, Layer[ index ]->getHandle(), 0 );

		glBindFramebuffer( GL_FRAMEBUFFER_EXT, currentFB );

		AssignedLayers[ targetPosition ] = index;
	}


	void FrameBufferObject::AssignDepthLayer( int index )
	{
		if ( !validDepthLayerIndex( index ) ) return;
		if ( DepthLayer[ index ] == 0 ) return;
		if ( !( DepthLayer[ index ]->isOK() ) ) return;

		GLint currentFB = 0;
		glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &currentFB );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, Handle );
		glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, DepthLayer[ index ]->getHandle(), 0 );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, currentFB );

		AssignedDepth = index;
	}


	void FrameBufferObject::Resize( int width, int height )
	{
		if ( ( Width == width ) & ( Height == height ) )
		{
			return;
		}

		Width = width;
		Height = height;

		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glActiveTexture( GL_TEXTURE0 );
	
		//resize all color layers
		for ( FBLayerMapType::iterator L = Layer.begin(); L != Layer.end(); L++ )
		{
			L->second->Resize( Width, Height );
		}

		//resize all depth layers
		for ( FBLayerMapType::iterator DL = DepthLayer.begin(); DL != DepthLayer.end(); DL++ )
		{
			DL->second->Resize( Width, Height );
		}

		glPopAttrib();
	}


	int FrameBufferObject::getWidth()
	{
		return Width;
	}
	int FrameBufferObject::getHeight()
	{
		return Height;
	}

	void FrameBufferObject::RenderHere()
	{
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, Handle );
	}

	void FrameBufferObject::StopRenderingToFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0 );
	}


	GLuint FrameBufferObject::LayerHandle( int index )
	{
		if ( !validLayerIndex( index ) ) return 0;
		if ( Layer[ index ] == 0 ) return 0;
	
		return Layer[ index ]->getHandle();
	}
	GLuint FrameBufferObject::DepthHandle( int index )
	{
		if ( !validDepthLayerIndex( index ) ) return 0;
		if ( DepthLayer[ index ] == 0 ) return 0;
	
		return DepthLayer[ index ]->getHandle();
	}

	TextureObject* FrameBufferObject::LayerTexture( int index )
	{
		if ( !validLayerIndex( index ) ) return 0;
		if ( Layer[ index ] == 0 ) return 0;
	
		return Layer[ index ]->getTexture();
	}
	TextureObject* FrameBufferObject::DepthTexture( int index )
	{
		if ( !validDepthLayerIndex( index ) ) return 0;
		if ( DepthLayer[ index ] == 0 ) return 0;
	
		return DepthLayer[ index ]->getTexture();
	}


	void FrameBufferObject::BindDepth( int DepthIndex, int TextureUnit )
	{
		if ( !validDepthLayerIndex( DepthIndex ) ) return;
		if ( DepthLayer[ DepthIndex ] == 0 ) return;

		glActiveTexture( GL_TEXTURE0 + TextureUnit );
		glBindTexture( GL_TEXTURE_2D, DepthLayer[ DepthIndex ]->getHandle() );
	}

	void FrameBufferObject::BindLayer( int LayerIndex, int TextureUnit )
	{
		if ( !validLayerIndex( LayerIndex ) ) return;
		if ( Layer[ LayerIndex ] == 0 ) return;

		glActiveTexture( GL_TEXTURE0 + TextureUnit );
		glBindTexture( GL_TEXTURE_2D, Layer[ LayerIndex ]->getHandle() );
	}



	bool FrameBufferObject::SetLayer( int index, FRAMEBUFFER_LAYER_TYPE Type )
	{
		if (	( Type != FRAMEBUFFER_LAYER_TYPE_RGB_8 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGB_16 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGB_32 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_8 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_16 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_RGBA_32 )
			) return false;

		if ( validLayerIndex( index ) )
		{
			if ( Layer[ index ] == 0 )
			{
				Layer[ index ] = new FrameBufferLayer( Type, Width, Height );
			} else {
				Layer[ index ]->setType( Type );
			}
		} else {
			Layer[ index ] = new FrameBufferLayer( Type, Width, Height );
		}
		return true;
	}

	bool FrameBufferObject::SetDepthLayer( int index, FRAMEBUFFER_LAYER_TYPE Type )
	{
		if (	( Type != FRAMEBUFFER_LAYER_TYPE_DEPTH_16 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_DEPTH_24 ) &
				( Type != FRAMEBUFFER_LAYER_TYPE_DEPTH_32 )
			) return false;

		if ( validDepthLayerIndex( index ) )
		{
			if ( DepthLayer[ index ] == 0 )
			{
				DepthLayer[ index ] = new FrameBufferLayer( Type, Width, Height );
			} else {
				DepthLayer[ index ]->setType( Type );
			}
		} else {
			DepthLayer[ index ] = new FrameBufferLayer( Type, Width, Height );
		}
		return true;
	}

	bool FrameBufferObject::RemoveLayer( int index )
	{
		if ( !validLayerIndex( index ) ) return false;

		if ( Layer[ index ] != 0 ) Layer[ index ]->~FrameBufferLayer();
		Layer.erase( Layer.find( index ) );
		return true;
	}

	bool FrameBufferObject::RemoveDepthLayer( int index )
	{
		if ( !validDepthLayerIndex( index ) ) return false;

		if ( DepthLayer[ index ] != 0 ) DepthLayer[ index ]->~FrameBufferLayer();
		DepthLayer.erase( DepthLayer.find( index ) );
		return true;
	}

	int FrameBufferObject::ClearAllLayers()
	{
		int countRemoved = 0;
		for ( FBLayerMapType::iterator L = Layer.begin(); L != Layer.end(); L++ )
		{
			if ( RemoveLayer( L->first ) ) countRemoved++;
		}
		for ( FBLayerMapType::iterator DL = DepthLayer.begin(); DL != DepthLayer.end(); DL++ )
		{
			if ( RemoveDepthLayer( DL->first ) ) countRemoved++;
		}
		return countRemoved;
	}

	void FrameBufferObject::ReadBuffer(int layerIndex)
	{
		glReadBuffer( Target(layerIndex) );
	}
}