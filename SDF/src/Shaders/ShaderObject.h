#pragma once
#include <iostream>
#include <string>
#include <list>
#include "glew.h"
#include "Vector4.h"

namespace ShaderManipulation
{
	using namespace MathStructures;

	const GLenum FRAGMENT_SHADER = GL_FRAGMENT_SHADER_ARB;
	const GLenum VERTEX_SHADER = GL_VERTEX_SHADER_ARB;

	const GLuint VERTEX_ATTRIBUTE_POSITION = 0;
	const GLuint VERTEX_ATTRIBUTE_COLOR = 1;
	const GLuint VERTEX_ATTRIBUTE_NORMAL = 2;
	const GLuint VERTEX_ATTRIBUTE_TEXTURE_COORDINATES = 4;
	const GLuint VERTEX_ATTRIBUTE_TANGENT = 3;

	class ShaderCode
	{
		GLuint Handle;
		GLuint Target;
		bool Valid;
		void CompileSource( std::string Source, GLuint target );
	public:
		ShaderCode();
		void CreateFromFile( std::string FileName, GLuint target );
		void CreateFromString( std::string Source, GLuint target );

		friend class ShaderObject;
	};


	class ShaderObject
	{
		GLuint Handle;
		bool Compiled;
		std::list<ShaderCode> CodeFragments;
	public:
		ShaderObject();
		ShaderObject( std::string VertexShaderFileName, std::string PixelShaderFileName );
		void AttachShader( ShaderCode Shader );
		bool LinkProgram();
		void Use();
		void Stop();
		void setUniformValue( std::string VariableName, float Value );
		void setUniformValue( std::string VariableName, int Value );
		void setUniformValue( std::string VariableName, float Value1, float Value2 );
		void setUniformValue( std::string VariableName, float Value1, float Value2, float Value3 );
		void setUniformValue( std::string VariableName, Vector4 Value );
		GLuint getUniformLocation( std::string VariableName );
	};
}