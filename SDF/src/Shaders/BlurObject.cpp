#include "stdafx.h"
#include "BlurObject.h"


namespace ShaderManipulation
{
	BlurObject::BlurObject()
	{
		GaussianBlurShader = new ShaderObject( "Shaders/blur_vert.txt", "Shaders/blur_frag.txt" );
		SampledBlurShader = new ShaderObject( "Shaders/blurSampled_vert.txt", "Shaders/blurSampled_frag.txt" );
	}


	void BlurObject::Apply( float Size, Vector4 Channels, Vector4 Direction, GLuint Source, GLuint Destination )
	{
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		glOrtho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glDisable( GL_DEPTH_TEST );
		glDepthMask( GL_FALSE );

		glEnable(GL_TEXTURE_2D);
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, Source );


		GaussianBlurShader->Use();
		GaussianBlurShader->setUniformValue( "SourceTexture", 0 );
		GaussianBlurShader->setUniformValue( "BlurSize", Size );
		GaussianBlurShader->setUniformValue( "BlurDirection", Direction.X, Direction.Y );
		GaussianBlurShader->setUniformValue( "Channels", Channels );

		GLuint* buffers = new GLuint[1];
		buffers[0] = Destination;
		glDrawBuffers( 1, buffers );
		delete [] buffers;

		glBegin(GL_QUADS);
		glTexCoord2f (0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex2f(1.0, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f (0.0, 1.0);
		glVertex2f(0.0, 1.0);
		glEnd();
		
		GaussianBlurShader->Stop();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();	
		glPopAttrib();
	}

	void BlurObject::Apply( float Size, Vector4 Channels, GLuint Source, GLuint Destination )
	{
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		glOrtho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glDisable( GL_DEPTH_TEST );
		glDepthMask( GL_FALSE );

		glEnable(GL_TEXTURE_2D);
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, Source );


		SampledBlurShader->Use();
		SampledBlurShader->setUniformValue( "SourceTexture", 0 );
		SampledBlurShader->setUniformValue( "BlurSize", Size );
		SampledBlurShader->setUniformValue( "Channels", Channels );

		GLuint* buffers = new GLuint[1];
		buffers[0] = Destination;
		glDrawBuffers( 1, buffers );
		delete [] buffers;

		glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(1.0, 0.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(0.0, 1.0);
		glEnd();

		SampledBlurShader->Stop();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();	
		glPopAttrib();
	}

}