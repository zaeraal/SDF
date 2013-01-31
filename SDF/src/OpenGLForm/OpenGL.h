// OpenGL.h : subor pre OpenGL manipulaciu a vykreslenie
#pragma once

using namespace System::Windows::Forms;

namespace OpenGLForm
{
	public ref class COpenGL: public System::Windows::Forms::NativeWindow
	{
	public:
		COpenGL(System::Windows::Forms::Panel ^ parentForm, AssimpFileHandler::CAssimp* assimp);
		System::Void ResizeOpengl(GLsizei width, GLsizei height);
		virtual void WndProc(System::Windows::Forms::Message %m) override;
		virtual System::Void Render(System::Void);
		System::Void SwapOpenGLBuffers(System::Void) { SwapBuffers(m_hDC); }
	private:
		HDC m_hDC;
		HGLRC m_hglrc;
		GLint Last_X, Last_Y;
		GLint Width, Height;
		GLfloat radius;
		GLfloat c_X, c_Y, c_Z;
		// premenne na pohyb kamery
		GLfloat c_Zoom;
		GLfloat c_h, c_w;
		AssimpFileHandler::CAssimp* Assimp;			// pre logovanie a nacitanie modelu
		void GetSphereCoordinates(double &xx, double &yy);
		void ZoomAndRotate();
		void SetLastMouse(int x, int y);
		void SetRotation(double x, double y);
	protected:
		~COpenGL(System::Void);
		GLint MySetPixelFormat(HDC hdc);
		GLvoid InitGL();
		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	};
}