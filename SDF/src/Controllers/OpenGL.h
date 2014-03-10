// OpenGL.h : subor pre OpenGL manipulaciu a vykreslenie
#pragma once
#include "StringHelper.h"
#include "Model.h"

using namespace System::Windows::Forms;

namespace OpenGLForm
{
	using namespace ModelController;

	public ref class COpenGL: public System::Windows::Forms::NativeWindow
	{
	public:
		COpenGL(System::Windows::Forms::Panel ^ parentForm, CModel* MController);
		System::Void ResizeOpengl(GLsizei width, GLsizei height);
		GLvoid ReloadViewport();
		virtual void WndProc(System::Windows::Forms::Message %m) override;
		virtual System::Void Render(System::Void);
		System::Void SwapOpenGLBuffers(System::Void) { SwapBuffers(m_hDC); }
		void ReloadBoundary();
		GLfloat Look_X, Look_Y, Look_Z;
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
		CModel* control;					// pre logovanie a nacitanie modelu
		void GetSphereCoordinates(float &xx, float &yy);
		void ZoomAndRotate();
		void SetLastMouse(int x, int y);
		void SetRotation(float x, float y);
	protected:
		~COpenGL(System::Void);
		GLint MySetPixelFormat(HDC hdc);
		GLvoid InitGL();
		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	};
}