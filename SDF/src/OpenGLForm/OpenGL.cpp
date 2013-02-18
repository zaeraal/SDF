// OpenGL.cpp : subor pre OpenGL manipulaciu a vykreslenie
#include "stdafx.h"
#include "OpenGL.h"

namespace OpenGLForm
{
	// inicializacia OpenGL, vytvorenie handle pre okno a zavolanie vsetkeho potrebneho
	COpenGL::COpenGL(System::Windows::Forms::Panel ^ parentForm, Controller::ModelController* MController)
	{
		Width = parentForm->Size.Width;
		Height = parentForm->Size.Height;
		radius = 1.0f;
		c_Zoom = 45.0f;
		c_X = 0.0f;
		c_Y = 0.0f;
		c_Z = 0.0f;
		c_h = 45.0f;
		c_w = 0.0f;
		control = MController;
		CreateParams^ cp = gcnew CreateParams;

		// Set the position on the form
		cp->X = 0;
		cp->Y = 0;
		cp->Height = Height;
		cp->Width = Width;

		// Specify the form as the parent.
		cp->Parent = parentForm->Handle;

		// Create as a child of the specified parent and make OpenGL compliant (no clipping)
		cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		// Create the actual window
		this->CreateHandle(cp);
		m_hDC = GetDC((HWND)this->Handle.ToPointer());

		assert(m_hDC);

		MySetPixelFormat(m_hDC);
		ReSizeGLScene(Width, Height);
		InitGL();
	}

	// zavolane pri zmene velkosti okna (tej formy)
	System::Void COpenGL::ResizeOpengl(GLsizei width, GLsizei height)
	{
		// resize the window
		HWND hWnd = (HWND)this->Handle.ToPointer();
		SetWindowPos(      
			hWnd, 
			HWND_TOP, 
			0, 
			0, 
			width, 
			height,
			NULL
		);
		ReSizeGLScene(width, height);
	}

	// funkcia ktora spracuvava eventy z windowsu (kliknutie mysov, klavesnica...)
	// pouzivam radsej glut funkcie
	void COpenGL::WndProc(System::Windows::Forms::Message %m)  
	{      
		if(m.Msg == WM_MOUSEMOVE)
		{
			//control->logInfo("zavolany WM_MOUSEMOVE");
			int tmpx = LOWORD(m.LParam.ToInt32());
			int tmpy = HIWORD(m.LParam.ToInt32());

			// ak taha pravym tlacitkom
			if(m.WParam.ToInt32() & MK_RBUTTON)
			{
				SetRotation(c_w-(360.0/(double)Width)*(double)(tmpx-Last_X), c_h-(180.0/(double)Height)*(double)(tmpy-Last_Y));
			}
			SetLastMouse(tmpx, tmpy);
				 
		}
		// ak klikol lavym tlacitkom - oznac nejaky facet (alebo ziaden)
		if(m.Msg == WM_LBUTTONDOWN)
		{
			//control->logInfo("zavolany WM_LBUTTONDOWN");
			if(control->loaded == true)
			{
				int tmpx = LOWORD(m.LParam.ToInt32());
				int tmpy = HIWORD(m.LParam.ToInt32());

				control->setDrawMode(4);
				Render();
				control->ProcessPick(tmpx, tmpy);
				control->setDrawMode(1);
			}
		}
		if(m.Msg == WM_MOUSEWHEEL)
        {
            if ( (int)m.WParam >0)
            {
                // Zoom in
				//control->logDebug("zavolany WM_MOUSEWHEEL in");
				c_Zoom -= 3;
				if(c_Zoom < 5)
					c_Zoom = 5;
            }
			else
            {
                // Zoom out
				//control->logDebug("zavolany WM_MOUSEWHEEL out");
				c_Zoom += 3;
				if(c_Zoom > 90)
					c_Zoom = 90;
            }
			return;
        }
		else
			this->DefWndProc(m);
	}

	// vykreslenie celej sceny
	System::Void COpenGL::Render(System::Void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
		glLoadIdentity();									// Reset the current modelview matrix

		ZoomAndRotate();									// otoc scenu tak jak ma byt

		if(control->loaded == false)
		{
			// nieco default
			glRotatef(90,-1.0f,0.0f,0.0f);

			// debug kod ak chcem sledovat vektor
			/*double x = 1.0;
			double y = -1.0;
			double z = 0.5;

			double r = sqrt(x*x + y*y + z*z);
			double wx = atan2(z, x);
			double wy = acos(y / r);

			double CameraX = wx * (180.0/M_PI);
			double CameraY = wy * (180.0/M_PI);;

			// uhol na radian
			GetSphereCoordinates(CameraX, CameraY);

			GLdouble X = sin(CameraY) * sin(CameraX);
			GLdouble Y = cos(CameraY);
			GLdouble Z = sin(CameraY) * cos(CameraX);

			// gluLookAt (kde som ja, kam pozeram, kde je UP vector)
			gluLookAt (X, Y, Z, 0, 0, 0, 0.0, 1.0, 0.0);*/

			glColor3f(1.0f,1.0f,1.0f);							// biela farba
			GLUquadricObj *p = gluNewQuadric();
			gluQuadricDrawStyle(p, GLU_LINE);
			gluSphere(p, 1.0f, 20, 20);
			//gluCylinder(p, 0.5, 0.1, 1.0, 20, 20);
			//delete p;
		}
		else
			control->DrawModel();
	}

	// nacitaj si suradnice objektu
	void COpenGL::ReloadBoundary()
	{
		double rad = 0.0;
		double xx = 0.0;
		double yy = 0.0;
		double zz = 0.0;
		control->GetBoundary(rad, xx, yy, zz);
		radius = (GLfloat)rad;
		c_X = (GLfloat)xx;
		c_Y = (GLfloat)yy;
		c_Z = (GLfloat)zz;

		// reset camera;
		c_Zoom = 45.0f;
		c_h = 45.0f;
		c_w = 0.0f;
	}

	// Prerobi uhle na radiany
	void COpenGL::GetSphereCoordinates(double &x, double &y)
	{
		x = x * (M_PI / 180.0f);
		y = y * (M_PI / 180.0f);
	}

	// otoci ma smerom kde vykreslujem tu panoramu
	void COpenGL::ZoomAndRotate()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// zoom
		gluPerspective(c_Zoom, (float)Width/(float)Height, radius / 10.0f, radius * 10.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// rotacia Z, Y, X
		double CameraX = c_w;					// je to (0) - (360)
		double CameraY = c_h;					// je to (0) - (180)
		//control->logDebug(str_format("X: %f, Y: %f", CameraFi, CameraTheta));			// vypis na overenie pokial treba

		// uhol na radian
		GetSphereCoordinates(CameraX, CameraY);

		GLdouble X = c_X + sin(CameraY) * sin(CameraX) * radius * 5.0f;
        GLdouble Y = c_Y + cos(CameraY) * radius * 5.0f;
        GLdouble Z = c_Z + sin(CameraY) * cos(CameraX) *radius * 5.0f;

		// gluLookAt (kde som ja, kam pozeram, kde je UP vector)
		gluLookAt (X, Y, Z, c_X, c_Y, c_Z, 0.0, 1.0, 0.0);

		//GLfloat LightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		//glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	}

	// nastav predosly pohyb mysky
	void COpenGL::SetLastMouse(int x, int y)
	{
		Last_X = x;
		Last_Y = y;
	}

	// nastav spravnu rotaciu
	void COpenGL::SetRotation(double x, double y)
	{
		if(x > 360.0f) x -= 360.0f;
		if(x < 0.0f) x += 360.0f;

		if(y > 179.5f) y = 179.5f;
		if(y < 0.5f) y = 0.5f;

		c_w = (GLfloat)x;
		c_h = (GLfloat)y;
	}

	// malo by zmazat vsetky objekty ktore ostali vpameti OpenGL
	COpenGL::~COpenGL(System::Void)
	{
		this->DestroyHandle();
	}

	// nastavi vlastnosti OpenGL oknu
	GLint COpenGL::MySetPixelFormat(HDC hdc)
	{
		PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
			1,                                // version number 
			PFD_DRAW_TO_WINDOW |              // support window 
			PFD_SUPPORT_OPENGL |              // support OpenGL 
			PFD_DOUBLEBUFFER,                 // double buffered 
			PFD_TYPE_RGBA,                    // RGBA type 
			24,                               // 24-bit color depth 
			0, 0, 0, 0, 0, 0,                 // color bits ignored 
			0,                                // no alpha buffer 
			0,                                // shift bit ignored 
			0,                                // no accumulation buffer 
			0, 0, 0, 0,                       // accum bits ignored 
			32,                               // 32-bit z-buffer     
			0,                                // no stencil buffer 
			0,                                // no auxiliary buffer 
			PFD_MAIN_PLANE,                   // main layer 
			0,                                // reserved 
			0, 0, 0                           // layer masks ignored 
		}; 
			
		GLint  iPixelFormat; 
		 
		// get the device context's best, available pixel format match 
		if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
		{
			MessageBox::Show("ChoosePixelFormat Failed");
			control->logInfo("ChoosePixelFormat Failed");
			return 0;
		}
			 
		// make that match the device context's current pixel format 
		if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
		{
			MessageBox::Show("SetPixelFormat Failed");
			control->logInfo("SetPixelFormat Failed");
			return 0;
		}

		if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
		{
			MessageBox::Show("wglCreateContext Failed");
			control->logInfo("wglCreateContext Failed");
			return 0;
		}
			
		if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
		{
			MessageBox::Show("wglMakeCurrent Failed");
			control->logInfo("wglCreateContext Failed");
			return 0;
		}


		return 1;
	}

	// nastavi vlastnosti pre scenu
	GLvoid COpenGL::InitGL()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);							// Enable smooth shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Background
		glClearDepth(1.0f);									// Depth buffer setup
		glEnable(GL_DEPTH_TEST);							// Enables depth testing
		glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
		//glEnable (GL_BLEND);								// Enables Aplpha Blending
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Set Aplpha Function
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations

		// lighting

		// light settings
		GLfloat LightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightPosition[] = { 0.0f, 1.0f, 1.0f, 0.0f };

		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		glEnable(GL_LIGHT0);

		glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
		glEnable(GL_COLOR_MATERIAL);
	}

	// zmenila sa velkost, tak resizuj objekty
	GLvoid COpenGL::ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		Width = width;
		Height = height;

		glViewport(0,0,width,height);						// Reset The Current Viewport

		ZoomAndRotate();
		control->logDebug(str_format("ReSizeGLScene width: %d, height: %d",width,height));
	}	
}