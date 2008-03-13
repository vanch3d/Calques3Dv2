// TestGLView.cpp : implementation of the CTestGLView class
//

#include "stdafx.h"
#include "BallController.h"
//#include "TestGL.h"

#include "..\Calques3D.h"
#include "..\Calques3DDoc.h"
#include "TestGLView.h"
//#include "OGLInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Vertices DataSets (convex polygons to tessellate)
// NOTE: the polygons will be automatically closed
		GLdouble star[5][3]={ 
		{0.2, 0.0, 0.0},
		{0.5, 0.9, 0.0},
		{0.8, 0.0, 0.0},
		{0.0, 0.6, 0.0},
		{1.0, 0.6, 0.0},
		};
		GLdouble Quad[4][3]={ 
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{1.0, 1.0, 0.0},
		{0.0, 1.0, 0.0},
		};
		GLdouble Triang[3][3]={ 
		{0.3, 0.3, 0.0},
		{0.7, 0.3, 0.0},
		{0.5, 0.7, 0.0},
		};
		GLdouble Triang2[3][3]={ 
		{0.5, 0.5, 0.0},
		{1.5, 1.0, 0.0},
		{1.5, 0.5, 0.0},
		};

/////////////////////////////////////////////////////////////////////////////
// CTestGLView

IMPLEMENT_DYNCREATE(CTestGLView, CGLEnabledView)

BEGIN_MESSAGE_MAP(CTestGLView, CGLEnabledView)
	//{{AFX_MSG_MAP(CTestGLView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_COMMAND(ID_HELP_OGLINFO, OnHelpOglinfo)
//	ON_COMMAND(ID_TESSELATOR_FILL, OnTesselatorFill)
//	ON_UPDATE_COMMAND_UI(ID_TESSELATOR_FILL, OnUpdateTesselatorFill)
//	ON_COMMAND(ID_TESSELATOR_NEG, OnTesselatorNeg)
//	ON_UPDATE_COMMAND_UI(ID_TESSELATOR_NEG, OnUpdateTesselatorNeg)
//	ON_COMMAND(ID_TESSELATOR_NONZERO, OnTesselatorNonzero)
//	ON_UPDATE_COMMAND_UI(ID_TESSELATOR_NONZERO, OnUpdateTesselatorNonzero)
//	ON_COMMAND(ID_TESSELATOR_ODDRULE, OnTesselatorOddrule)
//	ON_UPDATE_COMMAND_UI(ID_TESSELATOR_ODDRULE, OnUpdateTesselatorOddrule)
//	ON_COMMAND(ID_TESSELATOR_POS, OnTesselatorPos)
//	ON_UPDATE_COMMAND_UI(ID_TESSELATOR_POS, OnUpdateTesselatorPos)
// 	ON_COMMAND(ID_CHANGE_SCENE, OnChangeScene)
// 	ON_COMMAND(ID_QUADRIC_FILL, OnQuadricFill)
// 	ON_COMMAND(ID_QUADRIC_FLAT, OnQuadricFlat)
// 	ON_COMMAND(ID_QUADRIC_INSIDE, OnQuadricInside)
// 	ON_COMMAND(ID_QUADRIC_LINE, OnQuadricLine)
// 	ON_COMMAND(ID_QUADRIC_NONE, OnQuadricNone)
// 	ON_COMMAND(ID_QUADRIC_OUTSIDE, OnQuadricOutside)
// 	ON_COMMAND(ID_QUADRIC_POINT, OnQuadricPoint)
// 	ON_COMMAND(ID_QUADRIC_SILHOUETTE, OnQuadricSilhouette)
// 	ON_COMMAND(ID_QUADRIC_SMOOTH, OnQuadricSmooth)
// 	ON_COMMAND(ID_ENVIR_FLAT, OnEnvirFlat)
// 	ON_COMMAND(ID_ENVIR_LIGHTING, OnEnvirLighting)
// 	ON_COMMAND(ID_ENVIR_SMOOTH, OnEnvirSmooth)
// 	ON_UPDATE_COMMAND_UI(ID_ENVIR_LIGHTING, OnUpdateEnvirLighting)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_FILL, OnUpdateQuadricFill)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_FLAT, OnUpdateQuadricFlat)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_INSIDE, OnUpdateQuadricInside)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_LINE, OnUpdateQuadricLine)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_NONE, OnUpdateQuadricNone)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_OUTSIDE, OnUpdateQuadricOutside)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_POINT, OnUpdateQuadricPoint)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_SILHOUETTE, OnUpdateQuadricSilhouette)
// 	ON_UPDATE_COMMAND_UI(ID_QUADRIC_SMOOTH, OnUpdateQuadricSmooth)
// 	ON_UPDATE_COMMAND_UI(ID_ENVIR_FLAT, OnUpdateEnvirFlat)
// 	ON_UPDATE_COMMAND_UI(ID_ENVIR_SMOOTH, OnUpdateEnvirSmooth)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGLView construction/destruction

CTestGLView::CTestGLView():
TessWindRule(GLU_TESS_WINDING_ODD), TessFilling(TRUE),
sceneselect(0),	quadricNormals(GLU_SMOOTH),
quadricDwStyle(GLU_FILL), quadricOrientation(GLU_OUTSIDE),
trackball(.8,unitquaternion(DegToRad(45),Y_AXIS)*unitquaternion(DegToRad(-30),X_AXIS))
{
	trackball.SetColor(RGB(130,80,30));
	trackball.SetDrawConstraints();
}

CTestGLView::~CTestGLView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTestGLView diagnostics

#ifdef _DEBUG
void CTestGLView::AssertValid() const
{
	CGLEnabledView::AssertValid();
}

void CTestGLView::Dump(CDumpContext& dc) const
{
	CGLEnabledView::Dump(dc);
}

CCalques3DDoc* CTestGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
	return (CCalques3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestGLView Overridables from CGLEnabledView

void CTestGLView::VideoMode(ColorsNumber& c, ZAccuracy& z, BOOL& dbuf)
{
// ask for 65355 colors...
	c=THOUSANDS;
// ...16 bit Z-buffer...
	z=NORMAL;
// ...double-buffering
	dbuf=TRUE;
}

void CTestGLView::OnCreateGL()
{
// perform hidden line/surface removal (enable Z-Buffering)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
// set background color to black
	glClearColor(0.f,0.f,0.f,1.0f );
// set clear Z-Buffer value
	glClearDepth(1.0f);
// create and enable a white directional light
	float  color[]={1.f,1.f,1.f,1.f};// RGBA color spec
	glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
	float  ambient[]={.3f,.3f,.3f,1.f};// RGBA color spec
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	float pos[]={1.f,1.f,1.f,0.f};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	glEnable(GL_LIGHT0);
// prepare a bunch of line segments (carthesian axes arrows)
	StartStockDListDef();
	glBegin(GL_LINES);
		// light red x axis arrow
		glColor3f(1.f,0.5f,.5f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		// light green y axis arrow
		glColor3f(.5f,1.f,0.5f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		// light blue z axis arrow
		glColor3f(.5f,.5f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
	glEnd();
	glBegin(GL_LINES);
		// x letter & arrowhead
		glColor3f(1.f,0.5f,.5f);
		glVertex3f(1.1f,0.1f,0.0f);
		glVertex3f(1.3f,-0.1f,0.0f);
		glVertex3f(1.3f,0.1f,0.0f);
		glVertex3f(1.1f,-0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,-0.1f,0.0f);
		// y letter & arrowhead
		glColor3f(.5f,1.f,0.5f);
		glVertex3f(-0.1f,1.3f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.1f,1.3f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.f,1.1f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.1f,0.9f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(-0.1f,0.9f,0.0f);
		// z letter & arrowhead
		glColor3f(.5f,.5f,1.f);
		glVertex3f(0.0f,-0.1f,1.3f);
		glVertex3f(0.0f,0.1f,1.3f);
		glVertex3f(0.0f,0.1f,1.3f);
		glVertex3f(0.0f,-0.1f,1.1f);
		glVertex3f(0.0f,-0.1f,1.1f);
		glVertex3f(0.0f,0.1f,1.1f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.1f,0.9f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,-0.1f,0.9f);
	glEnd();
	EndStockListDef();
// prepare a single display list (yet another cube)
	anothercube.StartDef();// <- do not execute list immediately
		glBegin(GL_QUADS);
			glColor3f(0.4f,.2f,.7f);
			glVertex3f(0.1f,0.1f,.1f);
			glVertex3f(0.6f,.1f,.1f);
			glVertex3f(0.6f,0.6f,.1f);
			glVertex3f(0.1f,0.6f,.1f);
			glColor3f(.8f,1.f,.2f);
			glVertex3f(0.1f,0.1f,.6f);
			glVertex3f(0.6f,.1f,.6f);
			glVertex3f(0.6f,0.6f,.6f);
			glVertex3f(0.1f,0.6f,.6f);
		glEnd();
		glBegin(GL_QUAD_STRIP);
			glColor3f(0.4f,.2f,.7f);
			glVertex3f(0.1f,0.1f,.1f);
			glVertex3f(0.1f,0.1f,.6f);
			glColor3f(.8f,1.f,.2f);
			glVertex3f(0.6f,.1f,.1f);
			glVertex3f(0.6f,.1f,.6f);
			glColor3f(0.4f,.2f,.7f);
			glVertex3f(0.6f,0.6f,.1f);
			glVertex3f(0.6f,0.6f,.6f);
			glColor3f(.8f,1.f,.2f);
			glVertex3f(0.1f,0.6f,.1f);
			glVertex3f(0.1f,0.6f,.6f);
			glColor3f(0.4f,.2f,.7f);
			glVertex3f(0.1f,0.1f,.1f);
			glVertex3f(0.1f,0.1f,.6f);
		glEnd();
	anothercube.EndDef();
// prepare tesselator demo objects
	BuildTessDispList();
// prepare quadric disp list
	BuildQuadrDispList();
// prepare charset for text
	PrepareCharset2D(_T("Arial"),20);
}

void CTestGLView::BuildTessDispList()
{
// create a display list for the convex polygons
		CGLTesselator t;
		tessPolygon.StartDef();
// set winding rule and filling
		t.SetWindingRule(TessWindRule);
		t.SetFilling(TessFilling);
// first polygon: a quadrilateral with a triangular hole
		t.StartDef();
// define exterior
		t.AddVertexArray(Quad,4);
// define hole
		t.ContourSeparator();
		t.AddVertexArray(Triang,3);
		t.EndDef();
// move "the 3d cursor" right
		glTranslatef(1.f,0.f,0.f);
// second polygon: a star (defined vertex per vertex)
		t.StartDef();
		for (int c=0;c<5;c++)
		{
			t.AddVertex(star[c]);
		};
		t.EndDef();
// move "the 3d cursor" right again
		glTranslatef(1.f,0.f,0.f);
// third polygon: a quadrilateral and an intersecting triangle
// NOTE: the intersecting triangle has vertices in CCW order
//       this has effect on positive and negative winding rule
		t.StartDef();
		t.AddVertexArray(Quad,4);
		t.ContourSeparator();
		t.AddVertexArray(Triang2,3);
		t.EndDef();
// close disp list definition
		tessPolygon.EndDef();
}

void CTestGLView::BuildQuadrDispList()
{
	const int SECTIONS=16;
	const double RADIUS=.5;
// construct a quadric object
	CGLQuadric q(quadricDwStyle,quadricNormals,quadricOrientation);
// open disp list definition
	quadric.StartDef();
// draw an azure sphere
	glColor3f(.2f,.5f,.8f);
	q.DrawSphere(RADIUS,SECTIONS,SECTIONS);
	glTranslatef(1.2f, 0.0f, 0.0f);
// draw a maroon cylinder
	glColor3f(.8f,.5f,.2f);
	q.DrawCylinder(RADIUS,RADIUS,1.0,SECTIONS,2);
	glTranslatef(-2.4f, 0.0f, 0.0f);
// draw a green disk
	glColor3f(.5f,.8f,.2f);
	q.DrawDisk(RADIUS-.3,RADIUS,SECTIONS,2);
// close disp list definition
	quadric.EndDef();
}

void CTestGLView::OnDrawGL()
{
// the couple glPush - glPop is necessary because the entire
// scene is rotated of the given angle (which is absolute) at every redraw
	glPushMatrix();
// apply trackball rotation
	trackball.IssueGLrotation();
// "save" the current transformation matrix for subsequent superimpose of axes
	glPushMatrix();
// this should be self explanatory
	const float radius=0.34f;
	switch(sceneselect)
	{
	case 0:
		// draw directly an RGB cube with numbered vertices
		{
	int nb = GetDocument()->m_cObjectSet.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
        if (!pObj) continue;
	//glLoadIdentity();
//glTranslatef(-01.5f,0.0f,-6.0);			// Move Left And Into The Screen
		pObj->Draw3DRendering(0);
	}

		}
	/*	glBegin(GL_QUAD_STRIP);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.3f, 0.3f, 0.3f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-0.3f, -0.3f, 0.3f);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.3f, 0.3f, 0.3f);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.3f, -0.3f, 0.3f);
			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(0.3f, 0.3f, -0.3f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.3f, -0.3f, -0.3f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.3f, 0.3f, -0.3f);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.3f, -0.3f,  -0.3f);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.3f, 0.3f, 0.3f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-0.3f, -0.3f, 0.3f);
		glEnd();
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.3f, 0.3f, 0.3f);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.3f, 0.3f, 0.3f);
			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(0.3f, 0.3f, -0.3f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.3f, 0.3f, -0.3f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-0.3f, -0.3f, 0.3f);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.3f, -0.3f, -0.3f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.3f, -0.3f, -0.3f);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.3f, -0.3f, 0.3f);
		glEnd();
		glColor3f(1.f,1.f,1.f);
		glRasterPos3f(-radius, radius, radius);
		Text2D("1");
		glRasterPos3f(-radius, -radius, radius);
		Text2D("2");
		glRasterPos3f(radius, radius, radius);
		Text2D("3");
		glRasterPos3f(radius, -radius, radius);
		Text2D("4");
		glRasterPos3f(radius, radius, -radius);
		Text2D("5");
		glRasterPos3f(radius, -radius, -radius);
		Text2D("6");
		glRasterPos3f(-radius, radius, -radius);
		Text2D("7");
		glRasterPos3f(-radius, -radius,  -radius);
		Text2D("8");*/
		break;
	case 1: // tesselators scene
// move left to almost center the tesselators display list
		glTranslatef(-1.5f,0.f,0.1f);
// choose the color and draw
		glColor3f(1.f,1.f,1.f);
		tessPolygon.Draw();
		break;
	case 2: // quadric scene
		quadric.Draw();
		break;
	case 3:	// stylized aero vehicle
		glTranslatef(0.01f,0.f,0.01f);
		glColor3f(0.0f, 0.4f, 0.8f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.f, 0.f, 0.001f);
			glVertex3f(0.f, -0.5f, 1.f);
			glVertex3f(0.f, 1.f, 0.001f);
		glEnd();
		glColor3f(0.0f, 0.3f, 0.7f);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(1.f, -0.5f, 0.f);
			glVertex3f(0.f, 0.f, 0.2f);
			glVertex3f(0.f, 2.f, 0.f);
			glVertex3f(-1.f, -0.5f, 0.f);
		glEnd();
// execute previously prepared displ. list 2 times
// Note:All translations are concatenated (imagine to deal with a "3D cursor")
		glTranslatef(-1.35f,-0.2f,-0.2f);
		anothercube.Draw();
		glTranslatef(2.f,0.f,0.f);
		anothercube.Draw();
		break;
	};
	trackball.DrawBall();
//"superimpose" the RGB-carthesian-axes in a small area near the bottom-left corner of the view
	glViewport(0,0,m_ClientRect.right/6,m_ClientRect.bottom/6);
	glPopMatrix();
	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		DrawStockDispLists();
	glPopAttrib();
	glViewport(0,0,m_ClientRect.right,m_ClientRect.bottom);

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// CTestGLView message handlers

void CTestGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(nFlags & MK_CONTROL) trackball.UseConstraints(CAMERA_AXES);
	else if(nFlags & MK_SHIFT) trackball.UseConstraints(BODY_AXES);
		 else trackball.UseConstraints(NO_AXES);
// remember where we clicked
	MouseDownPoint=point;
	trackball.MouseDown(point);
// capture mouse movements even outside window borders
	SetCapture();
// redraw the view
		Invalidate(TRUE);
}

void CTestGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
// forget where we clicked
	MouseDownPoint=CPoint(0,0);
	trackball.MouseUp(point);
// release mouse capture
	ReleaseCapture();
// redraw the view
	Invalidate(TRUE);
}

void CTestGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	trackball.MouseMove(point);
// check if we have captured the mouse
	if (GetCapture()==this)
	{
// increment the object rotation angles
//		X_Angle+=double(point.y-MouseDownPoint.y)/3.6;
//		Y_Angle+=double(point.x-MouseDownPoint.x)/3.6;
// redraw the view
		Invalidate(TRUE);
// remember the mouse point
//		MouseDownPoint=point;
	};
}

void CTestGLView::OnHelpOglinfo() 
{
/*
// istantiate dialog
	COGLInfoDlg dlg;
// retrieve infos
	GLInfoStruct info;
	info=GetInformation();
// prepare dialog
	dlg.m_accel=info.acceleration;
	dlg.m_extensions=info.glextensions;
	dlg.m_rend=info.renderer;
	dlg.m_vendor=info.vendor;
	dlg.m_version=info.glversion;
	dlg.m_gluext=info.gluextensions;
	dlg.m_gluver=info.gluversion;
	dlg.m_detailstext.Format("Color buffer: %i bit red, %i bit green, %i bit blue, %i bit alpha.\nDepth buffer: %i bit.\nStencil buffer: %i bit.",info.red_bits,info.green_bits,info.blue_bits,info.alpha_bits,info.depth_bits,info.stencil_bits);
// convert the spaces in newlines (in extensions fields) 
	int pos=0;
	while ( (pos=dlg.m_extensions.Find(" "))!=-1 )
	{
		// substitute space with LF
		dlg.m_extensions.SetAt(pos,'\n');
		// insert a CR before LF
		dlg.m_extensions.Insert(pos,'\r');
	}
	pos=0;
	while ( (pos=dlg.m_gluext.Find(" "))!=-1 )
	{
		// substitute space with LF
		dlg.m_gluext.SetAt(pos,'\n');
		// insert a CR before LF
		dlg.m_gluext.Insert(pos,'\r');
	}
// show dialog
	dlg.DoModal();	
*/
}

void CTestGLView::OnTesselatorFill() 
{
// toggle filling
	TessFilling^=1;
// rebuild display list
	BeginGLCommands();
	BuildTessDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateTesselatorFill(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==1);
	pCmdUI->SetCheck(TessFilling);
}

void CTestGLView::OnTesselatorNeg() 
{
// set specified rule
	TessWindRule=GLU_TESS_WINDING_NEGATIVE;
// rebuild display list
	BeginGLCommands();
	BuildTessDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateTesselatorNeg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==1);
	pCmdUI->SetCheck(TessWindRule==GLU_TESS_WINDING_NEGATIVE);
}

void CTestGLView::OnTesselatorNonzero() 
{
// set specified rule
	TessWindRule=GLU_TESS_WINDING_NONZERO;
// rebuild display list
	BeginGLCommands();
	BuildTessDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateTesselatorNonzero(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==1);
	pCmdUI->SetCheck(TessWindRule==GLU_TESS_WINDING_NONZERO);
}

void CTestGLView::OnTesselatorOddrule() 
{
// set specified rule
	TessWindRule=GLU_TESS_WINDING_ODD;
// rebuild display list
	BeginGLCommands();
	BuildTessDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateTesselatorOddrule(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==1);
	pCmdUI->SetCheck(TessWindRule==GLU_TESS_WINDING_ODD);
}

void CTestGLView::OnTesselatorPos() 
{
// set specified rule
	TessWindRule=GLU_TESS_WINDING_POSITIVE;
// rebuild display list
	BeginGLCommands();
	BuildTessDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateTesselatorPos(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==1);
	pCmdUI->SetCheck(TessWindRule==GLU_TESS_WINDING_POSITIVE);
}

void CTestGLView::OnChangeScene() 
{
// toggle scene
	++sceneselect%=4;
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricFill() 
{
// set specified parameter
	quadricDwStyle=GLU_FILL;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricFlat() 
{
// set specified parameter
	quadricNormals=GLU_FLAT;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricInside() 
{
// set specified parameter
	quadricOrientation=GLU_INSIDE;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricLine() 
{
// set specified parameter
	quadricDwStyle=GLU_LINE;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricNone() 
{
// set specified parameter
	quadricNormals=GLU_NONE;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricOutside() 
{
// set specified parameter
	quadricOrientation=GLU_OUTSIDE;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricPoint() 
{
// set specified parameter
	quadricDwStyle=GLU_POINT;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricSilhouette() 
{
// set specified parameter
	quadricDwStyle=GLU_SILHOUETTE;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnQuadricSmooth() 
{
// set specified parameter
	quadricNormals=GLU_SMOOTH;
// rebuild display list
	BeginGLCommands();
	BuildQuadrDispList();
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnUpdateQuadricFill(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricDwStyle==GLU_FILL);
}

void CTestGLView::OnUpdateQuadricFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricNormals==GLU_FLAT);
}

void CTestGLView::OnUpdateQuadricInside(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricOrientation==GLU_INSIDE);
}

void CTestGLView::OnUpdateQuadricLine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricDwStyle==GLU_LINE);
}

void CTestGLView::OnUpdateQuadricNone(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricNormals==GLU_NONE);
}

void CTestGLView::OnUpdateQuadricOutside(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricOrientation==GLU_OUTSIDE);
}

void CTestGLView::OnUpdateQuadricPoint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricDwStyle==GLU_POINT);
}

void CTestGLView::OnUpdateQuadricSilhouette(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricDwStyle==GLU_SILHOUETTE);
}

void CTestGLView::OnUpdateQuadricSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sceneselect==2);
	pCmdUI->SetCheck(quadricNormals==GLU_SMOOTH);
}

void CTestGLView::OnEnvirFlat() 
{
	BeginGLCommands();
// set flat shading
	glShadeModel(GL_FLAT);
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnEnvirLighting() 
{
	BeginGLCommands();
// toggle lighting
	if(glIsEnabled(GL_LIGHTING)) glDisable(GL_LIGHTING);
	else glEnable(GL_LIGHTING);
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

void CTestGLView::OnEnvirSmooth() 
{
	BeginGLCommands();
// set smooth shading
	glShadeModel(GL_SMOOTH);
	EndGLCommands();
// redraw
	Invalidate(TRUE);
}

/*************
NOTE: the funtions below retrieve OGL state to give some visual cue to menu items
	  but this is extremely slow (and it's been done for simplicity).
	  You should mantain local variables by which update the state of
	  menus,buttons etc.
*************/
void CTestGLView::OnUpdateEnvirLighting(CCmdUI* pCmdUI) 
{
	BeginGLCommands();
// update menu check
	pCmdUI->SetCheck(glIsEnabled(GL_LIGHTING));
	EndGLCommands();
}

void CTestGLView::OnUpdateEnvirFlat(CCmdUI* pCmdUI) 
{
	BeginGLCommands();
// check shading mode
	GLint val;
	glGetIntegerv(GL_SHADE_MODEL,&val);
// set check accordingly
	pCmdUI->SetCheck(val==GL_FLAT);
	EndGLCommands();
}

void CTestGLView::OnUpdateEnvirSmooth(CCmdUI* pCmdUI) 
{
	BeginGLCommands();
// check shading mode
	GLint val;
	glGetIntegerv(GL_SHADE_MODEL,&val);
// set check accordingly
	pCmdUI->SetCheck(val==GL_SMOOTH);
	EndGLCommands();
}

void CTestGLView::OnSizeGL(int cx, int cy) 
{
//	afxDump<<"client area: "<<cx<<" x "<<cy<<"\n";
	trackball.ClientAreaResize(CRect(0,0,cx,cy));
	CGLEnabledView::OnSizeGL(cx,cy);
}

void CTestGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_SPACE) OnEnvirLighting();//trackball.ToggleMethod();
	trackball.Key(nChar);
	CGLEnabledView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(TRUE);
}
