#pragma once
#include "glew.h"
#include "TextureObject.h"
#include <vector>
#include <map>

namespace ShaderManipulation
{
	enum FRAMEBUFFER_LAYER_TYPE { 
		FRAMEBUFFER_LAYER_TYPE_RGBA_32,
		FRAMEBUFFER_LAYER_TYPE_RGB_32,
		FRAMEBUFFER_LAYER_TYPE_RGBA_16,
		FRAMEBUFFER_LAYER_TYPE_RGB_16,
		FRAMEBUFFER_LAYER_TYPE_RGBA_8,
		FRAMEBUFFER_LAYER_TYPE_RGB_8,
		FRAMEBUFFER_LAYER_TYPE_DEPTH_32,
		FRAMEBUFFER_LAYER_TYPE_DEPTH_24,
		FRAMEBUFFER_LAYER_TYPE_DEPTH_16
	};

	class FrameBufferLayer
	{
		GLuint TextureHandle;
		TextureObject* Texture;
		FRAMEBUFFER_LAYER_TYPE LayerType;
		int Width, Height;
		bool OK;
		void DetermineFormatType( GLuint& InternalFormat, GLuint& Format, GLuint& Type );
	public:
		FrameBufferLayer( FRAMEBUFFER_LAYER_TYPE layerType, int width = 512, int height = 512 );
		~FrameBufferLayer();
		GLuint getHandle();
		TextureObject* getTexture();
		void Resize( int width, int height );
		void setType( FRAMEBUFFER_LAYER_TYPE layerType );
		FRAMEBUFFER_LAYER_TYPE getType();
		int getWidth();
		int getHeight();
		bool isOK();
	};

#define FBLayerMapType std::map< int, FrameBufferLayer* >

	class FrameBufferObject
	{
		GLuint Handle;

		std::map< int, FrameBufferLayer* > Layer;
		std::map< int, FrameBufferLayer* > DepthLayer;

		std::map< int, int > AssignedLayers;		// targetPosition -> layerIndex
		int AssignedDepth;

		int Width, Height;
		bool OK;
	public:
		GLuint Target( int Index );
		GLint MaxAttachments();
		void DrawBuffers( int attachmentIndex0, int attachmentIndex1 = -1, int attachmentIndex2 = -1, int attachmentIndex3 = -1, int attachmentIndex4 = -1, int attachmentIndex5 = -1, int attachmentIndex6 = -1, int attachmentIndex7 = -1 );

		FrameBufferObject( int width, int height );
		void DefaultAttach( FRAMEBUFFER_LAYER_TYPE Type = FRAMEBUFFER_LAYER_TYPE_RGBA_16 );
		void Resize( int width, int height );
		void RenderHere();
		void StopRenderingToFBO();

		bool Valid();
		int getWidth();
		int getHeight();

		bool validLayerIndex( int index );
		bool validDepthLayerIndex( int index );
		bool validTargetIndex( int index );

		bool SetLayer( int index, FRAMEBUFFER_LAYER_TYPE Type );
		bool SetDepthLayer( int index, FRAMEBUFFER_LAYER_TYPE Type );
		bool RemoveLayer( int index );
		bool RemoveDepthLayer( int index );
		int ClearAllLayers();

		//assigning layer to render to
		void AssignLayer( int index, int targetPosition );
		// index - identitier of the layer; can be any number
		// targetPosition - index of color attachment slot; usually 0..7
		void AssignDepthLayer( int index );
		// index - identitier of the depth layer; can be any number

		GLuint LayerHandle( int index );
		GLuint DepthHandle( int index );
		TextureObject* LayerTexture( int index );
		TextureObject* DepthTexture( int index );

		void BindDepth( int DepthIndex, int TextureUnit );

		void BindLayer( int LayerIndex, int TextureUnit );

		void FrameBufferObject::ReadBuffer(int layerIndex);
	};
}