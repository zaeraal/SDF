#pragma once
#include "glew.h"
#include <vector>
#include <map>

namespace ShaderManipulation
{
	const int FRAMEBUFFER_LAYERS_COUNT = 2;

	class FrameBufferObject
	{
		GLuint Handle;
		GLuint Layers[ FRAMEBUFFER_LAYERS_COUNT ];
		int Width, Height;
		bool OK;

	public:
		FrameBufferObject( int width, int height );
		void RenderHere();
		void ReadBuffer(GLuint position);
		void StopRenderingToFBO();

		void AssignLayerTexture( int layerIndex, GLuint position );

		GLuint Layer( int index );

		void BindLayer( int LayerIndex, int TextureIndex );
	};

}