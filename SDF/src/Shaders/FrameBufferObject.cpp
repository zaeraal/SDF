#include "stdafx.h"
#include "glew.h"
#include "FrameBufferObject.h"

namespace ShaderManipulation
{
	FrameBufferObject::FrameBufferObject( int width, int height )
	{
		Width = width;
		Height = height;

		// Create layers
		for ( int layerIndex = 0; layerIndex < FRAMEBUFFER_LAYERS_COUNT; layerIndex++ )
		{
			glGenTextures( 1, &( Layers[ layerIndex ] ) );
			glBindTexture( GL_TEXTURE_2D, Layers[ layerIndex ] );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0 );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL );
		}

		//bind textures to the buffer
		glGenFramebuffers( 1, &Handle );
		glBindFramebuffer( GL_FRAMEBUFFER_EXT, Handle );

		for ( int layerIndex = 0; layerIndex < FRAMEBUFFER_LAYERS_COUNT; layerIndex++ )
		{
			glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + layerIndex, GL_TEXTURE_2D, Layers[ layerIndex ], 0 );
		}

		//check that everything is ok
		OK = (   glCheckFramebufferStatus( GL_FRAMEBUFFER_EXT ) == GL_FRAMEBUFFER_COMPLETE_EXT   );
		if ( ! OK )
		{
			//std::cout<<"Failed to create FrameBuffer\n";
			// failo nam to
			assert(false);
			assert(true);
		}

		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0 );
	}



	void FrameBufferObject::AssignLayerTexture( int layerIndex, GLuint position )
	{
		if ( layerIndex < 0 ){ layerIndex = 0; }
		if ( layerIndex >= FRAMEBUFFER_LAYERS_COUNT ){ layerIndex = FRAMEBUFFER_LAYERS_COUNT-1; }

		glBindFramebuffer(GL_FRAMEBUFFER_EXT, Handle );
		glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + position, GL_TEXTURE_2D, Layers[ layerIndex ], 0 );
	}

	void FrameBufferObject::RenderHere()
	{
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, Handle );
		GLuint* buffers = new GLuint[ FRAMEBUFFER_LAYERS_COUNT ];
		for ( int layerIndex = 0; layerIndex < FRAMEBUFFER_LAYERS_COUNT; layerIndex++ )
		{
			buffers[layerIndex] = GL_COLOR_ATTACHMENT0_EXT + layerIndex;
		}
		glDrawBuffers( FRAMEBUFFER_LAYERS_COUNT, buffers );
		delete [] buffers;
	}

	void FrameBufferObject::ReadBuffer(GLuint position)
	{
		GLenum buffer = GL_COLOR_ATTACHMENT0_EXT + position;
		glReadBuffer( buffer );
	}

	void FrameBufferObject::StopRenderingToFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0 );
	}


	GLuint FrameBufferObject::Layer( int index )
	{
		if ( index < 0 ){ index = 0; }
		if ( index >= FRAMEBUFFER_LAYERS_COUNT ){ index = FRAMEBUFFER_LAYERS_COUNT-1; }
		return Layers[index];
	}

	void FrameBufferObject::BindLayer( int LayerIndex, int TextureIndex )
	{
		if ( LayerIndex < 0 ) { LayerIndex = 0; }
		if ( LayerIndex >= FRAMEBUFFER_LAYERS_COUNT ) { LayerIndex = FRAMEBUFFER_LAYERS_COUNT-1; }

		glActiveTexture( GL_TEXTURE0 + TextureIndex );
		glBindTexture( GL_TEXTURE_2D, Layers[ LayerIndex ] );
	}
}