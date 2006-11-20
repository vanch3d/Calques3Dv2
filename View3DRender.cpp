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
/*		if (m_bResizing)
		{
			CRect rect;
			GetClientRect(&rect);

			glViewport(0,0, rect.Width(), rect.Height());
			gluOrtho2D (-10.0, 10.0, -10.0, 10.0);
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
	glMatrixMode(GL_MODELVIEW);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
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
	int nb = GetDocument()->m_cObjectSet.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
        if (!pObj) continue;
	glLoadIdentity();
//glTranslatef(-01.5f,0.0f,-6.0);			// Move Left And Into The Screen
		pObj->Draw3DRendering();
	}

	glLoadIdentity();
//glTranslatef(-01.5f,0.0f,-6.0);			// Move Left And Into The Screen
/*	glBegin(GL_TRIANGLES); // Drawing Using Triangles
glVertex3f( 0.0f, 1.0f, 0.0f); // Top
glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
glEnd(); // Finished Drawing The Triangle*/
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
	
	if (cy==0) cy =1;
	// TODO: Add your message handler code here
	CClientDC dc(this);

	m_wgl.Begin(&dc);
	glViewport(0,0, cx, cy);
	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)cx/(GLfloat)cy,0.1f,100.0f);
	gluLookAt(1,1,1,
			  1,0,0,
			  10,1,8);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	m_wgl.End();
}
