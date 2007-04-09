// View3DRender.cpp : implementation file
//

#include "stdafx.h"

#include "calques3d.h"
#include "Calques3DDoc.h"

#include "View3DRender.h"
#include "Prefs\Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView3DRender

IMPLEMENT_DYNCREATE(CView3DRender, CGLEnabledView)


BEGIN_MESSAGE_MAP(CView3DRender, CGLEnabledView)
	//{{AFX_MSG_MAP(CView3DRender)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_RENDERER_PROPERTY, OnShowProperty)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_PROPERTY, OnUpdateProperty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CView3DRender::CView3DRender():
	//trackball(.8,unitquaternion(DegToRad(45),Y_AXIS)*unitquaternion(DegToRad(-30),X_AXIS))
	trackball(.8,unitquaternion(DegToRad(135),Z_AXIS)*unitquaternion(DegToRad(45),X_AXIS))
{
	trackball.SetColor(RGB(130,80,30));
	trackball.SetDrawConstraints();
	m_pDlg = NULL;
}

CView3DRender::~CView3DRender()
{
	if (m_pDlg) delete m_pDlg;
}

/////////////////////////////////////////////////////////////////////////////
// CView3DRender diagnostics

#ifdef _DEBUG
void CView3DRender::AssertValid() const
{
	CGLEnabledView::AssertValid();
}

void CView3DRender::Dump(CDumpContext& dc) const
{
	CGLEnabledView::Dump(dc);
}

CCalques3DDoc* CView3DRender::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
    return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3DRender message handlers
void CView3DRender::GetLightParams(int *pPos)
{
	for (int i=0; i<NBVIEWPARAMS; i++)
		pPos[i] = m_LightParam[i];
}

void CView3DRender::SetLightParam (short lp, int nPos)
{
	m_LightParam[lp] = nPos;
	SetLight();
	Invalidate(FALSE);
}

void CView3DRender::SetLight ()
{
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	
	//====== Both surface sides are considered when calculating
	//====== each pixel color with the lighting formula
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   //====== Light source position depends on the
   //       object sizes scaled to (0,100)
	float fPos[] =
	{
		(m_LightParam[0]-50),///100,
		(m_LightParam[1]-50),///100,
		(m_LightParam[2]-50),///100,
		1.f
	};
	glLightfv(GL_LIGHT0, GL_POSITION, fPos);

	//====== Ambient light intensity
	float f = m_LightParam[3]/100.f;
	float fAmbient[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbient);

	//====== Diffuse light intensity
	f = m_LightParam[4]/100.f;
	float fDiffuse[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuse);

	//====== Specular light intensity
	f = m_LightParam[5]/100.f;
	float fSpecular[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecular);

	//====== Surface material reflection properties for
	//       each light component
	f = m_LightParam[6]/100.f;
	float fAmbMat[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fAmbMat);

	f = m_LightParam[7]/100.f;
	float fDifMat[4] = { f, f, f, 1.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDifMat);

	f = m_LightParam[8]/100.f;
	float fSpecMat[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fSpecMat);

	//====== Material shininess
	float fShine = 128 * m_LightParam[9]/100.f;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, fShine);

	//====== Material light emission property
	f = m_LightParam[10]/100.f;
	float fEmission[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fEmission);

// 	glEnable(GL_LIGHTING);      // Lighting will be used
// 	//====== Only one (first) source of light
// 	glEnable(GL_LIGHT0);
// 	//====== Depth of the Z-buffer will be taken into account
// 	glEnable(GL_DEPTH_TEST);
// 	//====== Material colors will be taken into account
// 	glEnable(GL_COLOR_MATERIAL);

	 		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(m_LightParam[11]+20,m_dAspectRatio,1.0f, 100.0f);
				glTranslatef(0.0f,0.0f,-6.0f);
				glTranslatef(0.0f,-0.667f,0.0f);
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

}


void CView3DRender::OnCreateGL()
{
// perform hidden line/surface removal (enable Z-Buffering)
	glEnable(GL_DEPTH_TEST);
// set background color to black
	glClearColor(0.f,0.f,0.f,1.0f );
// set clear Z-Buffer value
	glClearDepth(1.0f);
// create and enable a white directional light
// 	float  color[]={1.f,1.f,1.f,1.f};// RGBA color spec
// 	glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
// 	float  ambient[]={.3f,.3f,.3f,1.f};// RGBA color spec
// 	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
// 	float pos[]={1.f,1.f,1.f,0.f};
// 	glLightfv(GL_LIGHT0,GL_POSITION,pos);
// 
// 	float model_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
// 	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
// 	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0); //0=infiniteViewpoint, 1=localViewpoint
// 	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
// 
// 	glEnable(GL_LIGHT0);
// 	glEnable(GL_LIGHTING);



	//====== Initial lighting params
   m_LightParam[0] = 50;   // X position
   m_LightParam[1] = 80;   // Y position
   m_LightParam[2] = 100;  // Z position
   m_LightParam[3] = 15;   // Ambient light
   m_LightParam[4] = 70;   // Diffuse light
   m_LightParam[5] = 100;  // Specular light
   m_LightParam[6] = 100;  // Ambient material
   m_LightParam[7] = 100;  // Diffuse material
   m_LightParam[8] = 40;   // Specular material
   m_LightParam[9] = 70;   // Shininess material
   m_LightParam[10] = 0;   // Emission material
   m_LightParam[11] = 50;   // Perspective Field of View
 
	//====== Now you can issue OpenGL commands (i.e. call gl*** functions)
	glEnable(GL_LIGHTING);		// Lighting will be used
	//====== Only one (first) source of light
	glEnable(GL_LIGHT0);
	//====== The depth of the Z-buffer will be taken into account
	glEnable(GL_DEPTH_TEST);
	//====== Material colors will be taken into account
	glEnable(GL_COLOR_MATERIAL);

	//SetLight();
		//glEnable(GL_POINT_SMOOTH);
		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		//glShadeModel(GL_SMOOTH);
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

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

}


void CView3DRender::OnDrawGL()
{
	SetLight();
// the couple glPush - glPop is necessary because the entire
// scene is rotated of the given angle (which is absolute) at every redraw
	glPushMatrix();
// apply trackball rotation
	trackball.IssueGLrotation();
// "save" the current transformation matrix for subsequent superimpose of axes
	glPushMatrix();
// this should be self explanatory
	const float radius=0.34f;

//	{
//	    float mat_ambient_color[] = {255/255., 255/255.,150/255. , 0.010f};
//		float no_shininess = 0.9f;
//		GLdouble size = (1/3.);
//
// 		glPushMatrix();
// 		glBegin(GL_QUADS);
// 			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
// 			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambient_color);
//			//glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
//			int max = TPref::TUniv.nRepPas;
// 			for (int i=-max;i<=max;i++)
// 				for (int j=-max;j<=max;j++)
// 				{
// 					glNormal3f(0.0f, 0.0f, 1.0f);   //N1
// 					glVertex3d( 0+i*size, 0+j*size, 0);   //V2
// 					glVertex3d( size+i*size,0+j*size,0);   //V1
// 					glVertex3d( size+i*size,size+j*size,0);   //V3
// 					glVertex3d( 0+i*size,size+j*size,0);   //V4
// 				}
// 		glEnd();
// 		glPopMatrix();
//	}

	int nb = GetDocument()->m_cObjectSet.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
        if (!pObj) continue;
		pObj->Draw3DRendering();
	}


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

void CView3DRender::OnInitialUpdate() 
{
	CGLEnabledView::OnInitialUpdate();
}

void CView3DRender::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CView3DRender::OnLButtonUp(UINT nFlags, CPoint point) 
{
// forget where we clicked
	MouseDownPoint=CPoint(0,0);
	trackball.MouseUp(point);
// release mouse capture
	ReleaseCapture();
// redraw the view
	Invalidate(TRUE);
}

void CView3DRender::OnMouseMove(UINT nFlags, CPoint point) 
{
	trackball.MouseMove(point);
	Invalidate(TRUE);
}

void CView3DRender::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CGLEnabledView::OnRButtonDown(nFlags, point);
	OnShowProperty();
}

void CView3DRender::OnShowProperty() 
{
	// TODO: Add your command handler code here
	if (!m_pDlg)
	{
		m_pDlg = new CRenderPropDlg(this);
		m_pDlg->Create(CRenderPropDlg::IDD);
		m_pDlg->CenterWindow();
		m_pDlg->ShowWindow(SW_SHOW);
		m_pDlg->SetActiveWindow();
	}
	else
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

void CView3DRender::OnUpdateProperty(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_pDlg!=NULL);
}

void CView3DRender::OnSizeGL(int cx, int cy) 
{
//	afxDump<<"client area: "<<cx<<" x "<<cy<<"\n";
	trackball.ClientAreaResize(CRect(0,0,cx,cy));
	//CGLEnabledView::OnSizeGL(cx,cy);
	// set correspondence between window and OGL viewport
		glViewport(0,0,cx,cy);

// update the camera
 		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(m_LightParam[11]+20,m_dAspectRatio,1.0f, 100.0f);
				glTranslatef(0.0f,0.0f,-6.0f);
				glTranslatef(0.0f,-0.667f,0.0f);
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

}

void CView3DRender::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_SPACE) trackball.ToggleMethod();
	trackball.Key(nChar);
	CGLEnabledView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(TRUE);
}


CRenderPropDlg::CRenderPropDlg(CView3DRender* p) : CDialog(CRenderPropDlg::IDD, p)
{
	m_pView = p;
	//{{AFX_DATA_INIT(CPropDlg)
	//}}AFX_DATA_INIT
}

void CRenderPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenderPropDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenderPropDlg, CDialog)
	//{{AFX_MSG_MAP(CRenderPropDlg)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRenderPropDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	HWND hwnd = pScrollBar->GetSafeHwnd();

	UINT nID;
	int num = GetSliderNum(hwnd, nID);	// Slider number
	int	delta, newPos;
	
	switch (nSBCode)
	{
	case SB_THUMBTRACK:		
	case SB_THUMBPOSITION:	m_Pos[num] = nPos;	break;
	case SB_LEFT:			delta = -100;		goto New_Pos;
	case SB_RIGHT:			delta = +100;		goto New_Pos;
	case SB_LINELEFT:		delta = -1;			goto New_Pos;
	case SB_LINERIGHT:		delta = +1;			goto New_Pos;
	case SB_PAGELEFT:		delta = -20;		goto New_Pos;
	case SB_PAGERIGHT:		delta = +20;		goto New_Pos;
	New_Pos:
		newPos = m_Pos[num] + delta;
		m_Pos[num] = newPos<0 ? 0 : newPos>100 ? 100 : newPos;
		break;
	case SB_ENDSCROLL:
	default:
		return;
	}
	
	char s[8];
	sprintf (s,"%d",m_Pos[num]);
	SetDlgItemText(nID, (LPCTSTR)s);

	m_pView->SetLightParam (num, m_Pos[num]);
}

int CRenderPropDlg::GetSliderNum(HWND hwnd, UINT& nID)
{
	switch (::GetDlgCtrlID(hwnd))
	{
	case IDC_RENDERER_LGTX:
		nID = IDC_RENDERER_LGTXTXT;
		return 0;
	case IDC_RENDERER_LGTY:
		nID = IDC_RENDERER_LGTYTXT;
		return 1;
	case IDC_RENDERER_LGTZ:
		nID = IDC_RENDERER_LGTZTXT;
		return 2;
	case IDC_RENDERER_AMBIENT:
		nID = IDC_RENDERER_AMBTXT;
		return 3;
	case IDC_RENDERER_DIFFUSE:
		nID = IDC_RENDERER_DIFFUSETXT;
		return 4;
	case IDC_RENDERER_SPECULAR:
		nID = IDC_RENDERER_SPECULARTXT;
		return 5;
	case IDC_RENDERER_AMBMAT:
		nID = IDC_RENDERER_AMBMATTXT;
		return 6;
	case IDC_RENDERER_DIFFMAT:
		nID = IDC_RENDERER_DIFFMATTXT;
		return 7;
	case IDC_RENDERER_SPECMAT:
		nID = IDC_RENDERER_SPECMATTXT;
		return 8;
	case IDC_RENDERER_SHINE:
		nID = IDC_RENDERER_SHINETXT;
		return 9;
	case IDC_RENDERER_EMISSION:
		nID = IDC_RENDERER_EMISSIONTXT;
		return 10;
	case IDC_RENDERER_FIELDVIEW:
		nID = IDC_RENDERER_FIELDVIEWTXT;
		return 11;
	}
	return 0;
}

BOOL CRenderPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pView->GetLightParams(m_Pos);
		
	UINT IDs[] =
	{
		IDC_RENDERER_LGTX,	IDC_RENDERER_LGTY,	IDC_RENDERER_LGTZ,
		IDC_RENDERER_AMBIENT,IDC_RENDERER_DIFFUSE,IDC_RENDERER_SPECULAR,
		IDC_RENDERER_AMBMAT,	IDC_RENDERER_DIFFMAT,IDC_RENDERER_SPECMAT,
		IDC_RENDERER_SHINE,	IDC_RENDERER_EMISSION, IDC_RENDERER_FIELDVIEW
	};

	for (int i=0; i<sizeof(IDs)/sizeof(IDs[0]); i++)
	{
		HWND hwnd = GetDlgItem(IDs[i])->GetSafeHwnd();
		UINT nID;
		int num = GetSliderNum(hwnd, nID);
		::SendMessage(hwnd, TBM_SETPOS, TRUE, (LPARAM)m_Pos[i]);
		char s[8];
		sprintf (s,"%d",m_Pos[i]);
		SetDlgItemText(nID, (LPCTSTR)s);
 	}

	return TRUE;
}

void CRenderPropDlg::OnClose() 
{
	m_pView->m_pDlg = 0;
	CDialog::OnClose();
	delete this;
}

void CView3DRender::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
}
