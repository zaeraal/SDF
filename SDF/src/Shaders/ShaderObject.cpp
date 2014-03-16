#include "stdafx.h"
#include "ShaderObject.h"
#include <iostream>
#include <fstream>

namespace ShaderManipulation
{
	ShaderObject::ShaderObject()
	{
		Handle = glCreateProgramObjectARB();

		Compiled = false;
	}

	ShaderObject::ShaderObject( std::string VertexShaderFileName, std::string PixelShaderFileName )
	{
		Handle = glCreateProgramObjectARB();
		Compiled = false;

		ShaderCode VertexShader;
		ShaderCode PixelShader;

		VertexShader.CreateFromFile( VertexShaderFileName, GL_VERTEX_SHADER_ARB );
		PixelShader.CreateFromFile( PixelShaderFileName, GL_FRAGMENT_SHADER_ARB );

		AttachShader( VertexShader );
		AttachShader( PixelShader );
		LinkProgram();
	}


	void ShaderObject::AttachShader( ShaderCode Shader )
	{
		glAttachObjectARB( Handle, Shader.Handle );
		CodeFragments.push_back( Shader );
	}

	bool ShaderObject::LinkProgram()
	{
		glLinkProgramARB( Handle );
		GLint compiledStatus;
		glGetProgramiv( Handle, GL_LINK_STATUS, &compiledStatus );
		Compiled = ( compiledStatus == GL_TRUE );
		if ( Compiled ) 
		{
			glBindAttribLocation( Handle, VERTEX_ATTRIBUTE_TEXTURE_COORDINATES, "Attrib_TexCoords" );
			glBindAttribLocation( Handle, VERTEX_ATTRIBUTE_NORMAL, "Attrib_Normal" );
			glBindAttribLocation( Handle, VERTEX_ATTRIBUTE_TANGENT, "Attrib_Tangent" );
			glBindAttribLocation( Handle, VERTEX_ATTRIBUTE_COLOR, "Attrib_Color" );
			glBindAttribLocation( Handle, VERTEX_ATTRIBUTE_POSITION, "Attrib_Position" );
		} else {
			int infoLogLen = 0;
			int charsWritten = 0;
			GLchar *infoLog;

			glGetProgramiv( Handle, GL_INFO_LOG_LENGTH, &infoLogLen );
			std::string LogMessage;

			if (infoLogLen > 0)
			{
				infoLog = new GLchar[infoLogLen];
				glGetProgramInfoLog ( Handle, infoLogLen, &charsWritten, infoLog );
				LogMessage = std::string( infoLog );
				delete [] infoLog;
			}

			std::cout<<"Problem with compiling shader (Handle="<<Handle<<"):\n"<<LogMessage<<std::endl;
		}
		return Compiled;
	}

	void ShaderObject::Use()
	{
		if ( Compiled )
		{
			glUseProgramObjectARB( Handle );
		}
	}

	void ShaderObject::Stop()
	{
		glUseProgramObjectARB( 0 );
	}

	GLuint ShaderObject::getUniformLocation( std::string VariableName )
	{
		GLuint returnValue = glGetUniformLocation( Handle, VariableName.c_str() );
		return returnValue;
	}

	void ShaderObject::setUniformValue( std::string VariableName, float Value )
	{
		glUniform1f( getUniformLocation( VariableName ), Value );
	}
	void ShaderObject::setUniformValue( std::string VariableName, int Value )
	{
		glUniform1i( getUniformLocation( VariableName ), Value );
	}
	void ShaderObject::setUniformValue( std::string VariableName, float Value1, float Value2 )
	{
		glUniform2f( getUniformLocation( VariableName ), Value1, Value2 );
	}
	void ShaderObject::setUniformValue( std::string VariableName, float Value1, float Value2, float Value3 )
	{
		glUniform3f( getUniformLocation( VariableName ), Value1, Value2, Value3 );
	}
	void ShaderObject::setUniformValue( std::string VariableName, Vector4 Value )
	{
		glUniform4f( getUniformLocation( VariableName ), Value.X, Value.Y, Value.Z, Value.W );
	}












	ShaderCode::ShaderCode()
	{
		Handle = 0;
		Valid = false;
	}

	void ShaderCode::CreateFromString( std::string Source, GLuint target )
	{
		CompileSource( Source, target );
	}

	void ShaderCode::CreateFromFile( std::string FileName, GLuint target )
	{
		std::cout<<"Loading from '"<<FileName<<"'.\n";
		std::ifstream File ( FileName );
		if ( !File.is_open())
		{
			std::cout<<"File '"<<FileName<<"' not found."<<std::endl;
			return;
		}

		std::string Source;
		std::string line;
		while ( File.good() )
		{
			getline( File, line );
			Source.append( line );
			Source.append( "\n" );
		}
		CompileSource( Source, target );
	}
	void ShaderCode::CompileSource( std::string Source, GLuint target )
	{
		if ( target != GL_VERTEX_SHADER_ARB  &&  target != GL_FRAGMENT_SHADER_ARB )
		{
			return;
		}


		GLuint shaderID = glCreateShaderObjectARB( target );	

		const char *ShaderCStr = Source.c_str();
		int flen = strlen( ShaderCStr );
		glShaderSource( shaderID, 1, (const GLchar **)&ShaderCStr, &flen );

		glCompileShader( shaderID );

		int infoLogLen = 0;
		int charsWritten = 0;
		GLchar *infoLog;

		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLen );
		std::string LogMessage;


		if (infoLogLen > 0)
		{
			infoLog = new GLchar[infoLogLen];
			glGetShaderInfoLog( shaderID, infoLogLen, &charsWritten, infoLog );
			LogMessage = std::string( infoLog );
			delete [] infoLog;
		}


		GLint Compiled;
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &Compiled);

		if ( Compiled != GL_TRUE )
		{
			std::cout<<"Error compiling shader:\n"<<LogMessage<<std::endl;
			glDeleteShader( shaderID );
			Handle = 0;
			Valid = false;
		}

		Target = target;
		Handle = shaderID;
		Valid = true;
	}
}