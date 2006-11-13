// View3DRender.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "Calques3DDoc.h"
#include "View3DRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView3DRender

IMPLEMENT_DYNCREATE(CView3DRender, CView)

CView3DRender::CView3DRender()
: m_rtri(0), m_rquad(0), m_bResizing(true)

{

}

CView3DRender::~CView3DRender()
{
}


BEGIN_MESSAGE_MAP(CView3DRender, CView)
	//{{AFX_MSG_MAP(CView3DRender)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView3DRender drawing
BOOL CView3DRender::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CView3DRender::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	m_wgl.Begin(pDC);
		if (m_bResizing)
		{
			CRect rect;
			GetClientRect(&rect);

			glViewport(0,0, rect.Width(), rect.Height());
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(35,rect.Width()/(double)rect.Height(),1,100);
			gluLookAt(0,0,0,0,0,-10,0,1,0);
			glMatrixMode(GL_MODELVIEW);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35,0.5,1,1000);
	glMatrixMode(GL_MODELVIEW);
		DrawGLScene();
		
	m_wgl.End();
}

/////////////////////////////////////////////////////////////////////////////
// CView3DRender diagnostics

#ifdef _DEBUG
void CView3DRender::AssertValid() const
{
	CView::AssertValid();
}

void CView3DRender::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCalques3DDoc* CView3DRender::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
    return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3DRender message handlers

int CView3DRender::DrawGLScene()
{
	glLoadIdentity();						// Reset The View
	glTranslatef(-1.5f,0.0f,-6.0);			// Move Left And Into The Screen

	glRotatef(m_rtri,0.0f,1.0f,0.0f);		// Rotate The Pyramid On It's Y Axis

	glBegin(GL_TRIANGLES);					// Start Drawing The Pyramid

		glColor3f(1.0f,0.0f,0.0f);			// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);			// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);			// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Right Of Triangle (Front)


		glColor3f(1.0f,0.0f,0.0f);			// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);			// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);			// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Right Of Triangle (Right)


		glColor3f(1.0f,0.0f,0.0f);			// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);			// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);			// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);			// Right Of Triangle (Back)

		glColor3f(1.0f,0.0f,0.0f);			// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);			// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);			// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Right Of Triangle (Left)
	glEnd();						// Done Drawing The Pyramid

	glLoadIdentity();
	glTranslatef(1.5f,0.0f,-7.0f);				// Move Right And Into The Screen
	glRotatef(m_rquad,1.0f,1.0f,1.0f);			// Rotate The Cube On X, Y & Z

	glBegin(GL_QUADS);					// Start Drawing The Cube

		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.5f,0.0f);			// Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

		glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Back)

		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

   
		glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
	glEnd();						// Done Drawing The Quad

	m_rtri+=0.5f;						// Increase The Rotation Variable For The Triangle 
	m_rquad-=0.4f;						// Decrease The Rotation Variable For The Quad 
	return TRUE;						// Keep Going
}

void CView3DRender::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CClientDC dc(this);

	m_wgl.Begin(&dc);
		
		glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			// Black Background
		glClearDepth(1.0f);								// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	m_wgl.End();
	
}

void CView3DRender::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
		m_bResizing = true;

}
