#include "Resource.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "OGLWindow.h"
#include "Matrix4x4.h"
#include "Vector4D.h"
#include <iostream>

// Initialising x, y, z axis to 1
float x_axis, y_axis, z_axis = 1;

OGLWindow::OGLWindow()
{

}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_cube;
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyOGLWindow()
{
	DestroyOGLContext();

	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{
	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}

	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	RECT clientrect;

	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"OGLWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	GetClientRect(m_hwnd, &clientrect);

	SetWindowPos(m_hwnd, NULL, 10, 10, (width << 1) - clientrect.right, (height << 1) - clientrect.bottom, SWP_HIDEWINDOW | SWP_NOREDRAW);

	m_width = width;
	m_height = height;

	if (!(m_hglrc = CreateOGLContext(m_hdc)))
		return FALSE;

	// Instantiate a Renderable as OGLCube
	// as new TriangleMesh
	m_cube = new TriangleMesh();
	m_cube->LoadMeshFromOBJFile("cube.obj");

	return TRUE;
}

void OGLWindow::SetVisible ( BOOL visible )
{
	ShowWindow ( m_hwnd, visible? SW_SHOW : SW_HIDE );
}

void OGLWindow::Render()
{
	double aspectratio = (double)m_width / (double)m_height;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//gluPerspective(90, aspectratio, 1, 200);
	glLoadIdentity();

	Matrix4x4 transformation;

	// Set the matrix 'transformation' as a rotation about the Z axis

	x_axis = x_axis + 0.0003;
	y_axis = y_axis + 0.0003;
	z_axis = z_axis + 0.0003;
	transformation.SetRotationZAxis(z_axis);

	// Transformation of obj using SetViewMatrix method from Matrix4x4.cpp
	// calculated for each of "camera position", "view" and 'upview"
	transformation.SetViewMatrix(Vector4D(-200, 0, 200),Vector4D(-200, 0, 200), Vector4D(-200, 0, 200));

	// View test only
	//gluLookAt(2, 1.5, 1.5, 0, 0, 0, 0, 0, 1);

	// Set the matrix 'transformation' as a more complex transformation by concatenating, e.g. Rz Rx Ry
	transformation.SetRotationXAxis(x_axis);
	//transformation.SetRotationYAxis(y_axis);
	//transformation.SetRotationZAxis(z_axis);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Render as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Render as filled
	m_cube->Render(&transformation);

	glFlush();

	SwapBuffers(m_hdc);
	return;
}

void OGLWindow::Resize( int width, int height )
{
	RECT clientrect;

	GetClientRect(m_hwnd, &clientrect);

	int left = width - clientrect.right;
	int top = height - clientrect.bottom;

	m_width = width - left;
	m_height = height - top;

	double aspectratio = (double)m_width / (double)m_height;

	glViewport(left >> 1, top >>1 , m_width, m_height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Change from orthographic project to perspective projection
	// use either glFrustum or gluPerspective
	//glFrustum( -0.5*width, 0.5*width, -0.5*height, 0.5*height, -1.0, 1.0);
	glOrtho(aspectratio*-2.0, aspectratio*2.0, -2.0, 2.0, -5.0, 5.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluPerspective(90, aspectratio, 1, 200);

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{

	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	return TRUE;
}