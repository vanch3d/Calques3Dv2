// ViewUniv.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Calques3D.h"
#include "Calques3DDoc.h"

#include "objects\Vector4.h"
#include "Prefs\Prefs.h"

#include "ViewUniv.h"
#include "ZoomDlg.h"
#include "MemDC.h"

#include "objects\Point3D.h"
#include "objects\Droite3D.h"
#include "objects\Sphere3D.h"
#include "objects\Plan3D.h"
#include "objects\Text3D.h"
#include "BCGPShapeMenuButton.h"

#include "objects\Macro3D.h"
#include "MacroInfoDlg.h"

#include "Tasks\Task.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewUniv

IMPLEMENT_DYNCREATE(CViewUniv, CView)

BEGIN_MESSAGE_MAP(CViewUniv, CView)
    //{{AFX_MSG_MAP(CViewUniv)
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_WM_CREATE()
    ON_WM_CONTEXTMENU()
    ON_WM_MDIACTIVATE()
    ON_WM_SETFOCUS()
    ON_WM_TIMER()

    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
    ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
    ON_COMMAND(ID_MACROS_SAVE, OnMacrosSave)
    ON_COMMAND(ID_MACROS_INFO, OnMacrosInfo)

    ON_UPDATE_COMMAND_UI(ID_MACROS_INFO, OnUpdateMacrosInfo)

    ON_COMMAND(ID_VISUALISATION_SHOWMARKS, OnToggleMarks)
    ON_COMMAND(ID_VISUALISATION_POINTSOFVIEW, OnChangePointsOfView)

    ON_UPDATE_COMMAND_UI(ID_VISUALISATION_SHOWMARKS, OnUpdateToggleMarks)
    ON_UPDATE_COMMAND_UI(ID_VISUALISATION_POINTSOFVIEW,OnUpdateReferential)
    //}}AFX_MSG_MAP
    // Messages added manually because the Class Wizard is crap!
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

    ON_UPDATE_COMMAND_UI_RANGE(ID_FORMAT_OBJCOLOR, ID_FORMAT_UNDERLINE, OnUpdateFormat)
    ON_COMMAND_RANGE(ID_FORMAT_OBJCOLOR, ID_FORMAT_UNDERLINE, OnFormat)
	ON_CBN_SELENDOK(ID_FORMAT_TXTFONT, OnFormat)
	ON_CBN_SELENDOK(ID_FORMAT_TXTSIZE, OnFormat)

	ON_COMMAND_RANGE(ID_VISUALISATION_ZOOM,ID_VISUALISATION_ZOOM_ADJUST, OnChangeZoom)
    ON_COMMAND_RANGE(ID_VISUALISATION_ZOOM_150,ID_VISUALISATION_ZOOM_150, OnChangeZoom)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_ZOOM, ID_VISUALISATION_ZOOM_ADJUST, OnUpdateZoom)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_ZOOM_150, ID_VISUALISATION_ZOOM_150, OnUpdateZoom)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_POV_HSLIDER, ID_VISUALISATION_POV_VSLIDER, OnUpdateSliders)

    ON_COMMAND_RANGE(ID_VISUALISATION_PERSPECTIVE_CAVALIERE,ID_VISUALISATION_PERSPECTIVE_OBLIQUE,OnChangePerspective)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_PERSPECTIVE_CAVALIERE,ID_VISUALISATION_PERSPECTIVE_OBLIQUE,OnUpdatePerspective)

    ON_COMMAND_RANGE(ID_VISUALISATION_REFERENTIAL,ID_VISUALISATION_REFERENTIAL_WALLS,OnChangeReferential)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_REFERENTIAL,ID_VISUALISATION_REFERENTIAL_WALLS,OnUpdateReferential)

    ON_COMMAND_RANGE(ID_VISUALISATION_DEFAULTTASK,ID_VISUALISATION_APPLYSTYLE, OnStartTask)
    ON_COMMAND_RANGE(ID_VISUALISATION_PROJECTION_FRONT,ID_VISUALISATION_PROJECTION_KEEPIT, OnStartTask)
    ON_COMMAND_RANGE(ID_VISUALISATION_STYLE_COPY,ID_VISUALISATION_STYLE_MARK, OnStartTask)
    ON_COMMAND_RANGE(ID_OBJECT_POINT,ID_OBJECT_POLYGON, OnStartTask)
    ON_COMMAND_RANGE(ID_CONSTRUCTION_POINTON,ID_CONSTRUCTION_DIVSEGMENT, OnStartTask)
    ON_COMMAND_RANGE(ID_CONSTRUCTION_INTERSECTION_LINECIRCLE,ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE, OnStartTask)
    ON_COMMAND_RANGE(ID_EXPLORATION_MOVE,ID_EXPLORATION_DISCOVERY, OnStartTask)
    ON_COMMAND_RANGE(ID_EXPLORATION_EXTRACTION_CALQUE1,ID_EXPLORATION_EXTRACTION_ADDIN, OnStartTask)
    ON_COMMAND_RANGE(ID_EXPLORATION_MOVE_MAGNETISM,ID_EXPLORATION_MOVE_TRACE, OnStartTask)
    ON_COMMAND_RANGE(ID_EXPLORATION_VERIFY_PAR,ID_EXPLORATION_VERIFY_PTALIGN, OnStartTask)
    ON_COMMAND_RANGE(ID_EXPLORATION_MEASURE_DISTANCE,ID_EXPLORATION_MEASURE_LENGTH, OnStartTask)

    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_DEFAULTTASK,ID_VISUALISATION_APPLYSTYLE, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_PROJECTION_FRONT,ID_VISUALISATION_PROJECTION_KEEPIT, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VISUALISATION_STYLE_COPY,ID_VISUALISATION_STYLE_MARK, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_OBJECT_POINT,ID_OBJECT_POLYGON, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_CONSTRUCTION_POINTON,ID_CONSTRUCTION_DIVSEGMENT, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_CONSTRUCTION_INTERSECTION_LINECIRCLE,ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_MOVE,ID_EXPLORATION_DISCOVERY, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_EXTRACTION_CALQUE1,ID_EXPLORATION_EXTRACTION_ADDIN, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_MOVE_MAGNETISM,ID_EXPLORATION_MOVE_TRACE, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_VERIFY_PAR,ID_EXPLORATION_VERIFY_PTALIGN, OnUpdateTasks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_MEASURE_DISTANCE,ID_EXPLORATION_MEASURE_LENGTH, OnUpdateTasks)

    ON_COMMAND_RANGE(ID_MACROS_LOAD,ID_MACROS_LOAD, OnStartTask)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MACROS_SAVE, ID_MACROS_LOAD, OnUpdateTasks)

    ON_COMMAND_RANGE(ID_MACROS_USER1, ID_MACROS_USER15, OnStartTask)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MACROS_USER1, ID_MACROS_USER15, OnUpdateTasks)

    //ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
    ON_MESSAGE(WM_SHOWOBJ_TOOLTIPS, OnUpdateObjTooltip)
    ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
    ON_MESSAGE(WM_UPDATEZOOM_MANUAL, OnChangeZoom)
    ON_COMMAND(ID_VISUALISATION_POV_HSLIDER, OnRotateHor)
    ON_COMMAND(ID_VISUALISATION_POV_VSLIDER, OnRotateVer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewUniv::CViewUniv()
{
    m_pVisParam = NULL;
    m_pCurrentTask = NULL;
}

CViewUniv::~CViewUniv()
{
    if (m_pCurrentTask) delete m_pCurrentTask;
    if (m_pVisParam) delete m_pVisParam;
    m_pVisParam = NULL;
    m_pCurrentTask = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CViewUniv drawing
/////////////////////////////////////////////////////////////////////////////

BOOL CViewUniv::OnEraseBkgnd(CDC* pDC)
{
    //return CView::OnEraseBkgnd(pDC);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
/// Method CViewUniv::OnDraw
///
/// Note that the order of drawing the various component of the universe is important, as there is no
/// implementation of a facet-ordering algorithm. Scene rendering is done in a ad-hoc fashion by:
/// - first drawing the System of Reference
/// - second calculating the positions and drawing the polygons (if any)
/// - third drawing the remaining objects
/// - and finally drawing any visual feedbacks coming from the current task
//////////////////////////////////////////////////////////////////////
void CViewUniv::OnDraw(CDC* rDC)
{
    CDocument* pDoc = GetDocument();

    CMemDC memDC(rDC);
    CDC* pDC = &memDC;
    //CDC* pDC = rDC;

    CRect  ClipRect;
    CBrush br(::GetSysColor(COLOR_WINDOW));
    pDC->GetClipBox(ClipRect);
    pDC->FillRect(ClipRect,&br);

    int oldM = pDC->SetBkMode(TRANSPARENT);
    if (GetVisualParam())
    {
        // Draw System of Reference
        //GetVisualParam()->Draw(pDC);

        // Calculate Visual Representation of objects
        int nb = GetDocument()->m_cObjectSet.GetSize();
        for (int i=0;i<nb;i++)
        {
            CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
            if (!pObj) continue;
            pObj->CalculVisuel(GetVisualParam());
        }

		// Treats polygons only if adequate referential 
		int nRep = GetVisualParam()->nVisuKind;
		if (nRep==CVisualParam::VisuNone || nRep==CVisualParam::VisuRep)
		{
			// Calculate Position of the Polygons
			int nbPol = GetDocument()->m_cPolygonSet.GetSize();
			CxObject3DSet mset;
			for (i=0;i<nbPol;i++)
			{
				CObject3D* pObj = GetDocument()->m_cPolygonSet.GetAt(i);
				if (!pObj) continue;
				CPolygon3D *pPol = DYNAMIC_DOWNCAST(CPolygon3D,pObj);
				if (!pPol) continue;

				BOOL bFound=FALSE;
				for (int k=0;k<mset.GetSize();k++)
				{
					CObject3D* pObj2 = mset.GetAt(k);
					CPolygon3D *pPol2 = DYNAMIC_DOWNCAST(CPolygon3D,pObj2);
					if (pPol->m_nZFact < pPol2->m_nZFact)
					{

						mset.InsertAt(k,pObj);
						bFound=TRUE;
						break;
					}
				}
				if (!bFound) mset.Add(pObj);
			}
			// Draw the Polygons
			for (i=0;i<mset.GetSize();i++)
			{
				CObject3D* pObj = mset.GetAt(i);
				if (!pObj) continue;
				pObj->Draw(pDC,GetVisualParam());
			}
		}

        // Draw System of Reference
        GetVisualParam()->Draw(pDC);

        // Draw the Objects
        for (i=0;i<nb;i++)
        {
            CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
            if (!pObj) continue;
            //if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
            if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
                pObj->Draw(pDC,GetVisualParam(),1);
            else
                pObj->Draw(pDC,GetVisualParam());
        }
    }

    // Draw the task feedback
    if (IsWindowActivated() && m_pCurrentTask)
    {
        m_pCurrentTask->DrawFeedBack(pDC);
    }
    pDC->SetBkMode(oldM);
}

/////////////////////////////////////////////////////////////////////////////
// CViewUniv diagnostics

#ifdef _DEBUG
void CViewUniv::AssertValid() const
{
    CView::AssertValid();
}

void CViewUniv::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CCalques3DDoc* CViewUniv::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
    return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Getters/Setters for local variables

CObject3D* CViewUniv::GetSelectedObject()
{
    return pSelectObj;
}


CVisualParam* CViewUniv::GetVisualParam()
{
    return m_pVisParam;
}

void CViewUniv::SetVisualParam(int VP)
{
    if (!GetVisualParam()) return;
    CVisualParam *pNewParam = NULL;
    switch (VP){
    case CVisualParam::VisuNone :
        pNewParam = new CVisuNone(*GetVisualParam());
        break;
    case CVisualParam::VisuRep :
        pNewParam = new CVisuRep(*GetVisualParam());
        break;
    case CVisualParam::VisuPlane :
        pNewParam = new CVisuPlane(*GetVisualParam());
        break;
    case CVisualParam::VisuClois :
    default:
        pNewParam = new CVisuCloison(*GetVisualParam());
        break;
    }
    if (pNewParam)
    {
        delete m_pVisParam;
        m_pVisParam = pNewParam;
        if (GetSafeHwnd())
        {
            Invalidate();
            UpdateWindow();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CViewUniv message handlers

void CViewUniv::OnInitialUpdate()
{
    CView::OnInitialUpdate();
    m_wndToolTip.Create(this);

    m_pVisParam = new CVisuCloison();
    SetVisualParam(TPref::TUniv.nDefRep);
    m_pCurrentTask = new CDefTask(this,ID_VISUALISATION_DEFAULTTASK);
    pSelectObj = NULL;
    if (GetVisualParam())
    {
        GetVisualParam()->SetProjParam();
        CRect mrect;
        GetClientRect(&mrect);
        GetVisualParam()->ptRepCoord = mrect.CenterPoint();
        //GetVisualParam()->ptRepCoord = CPoint(150,150);
        m_wndToolTip.AddTool(this);
        m_wndToolTip.SetDelayTime(TTDT_INITIAL,100);
        m_wndToolTip.SetDelayTime(TTDT_RESHOW,100);
        m_wndToolTip.Activate(FALSE);

        int r = (int)(GetVisualParam()->ProjParam.phi - 180 -TPref::TUniv.sDefParam.phi);
        r = (r < 0) ? r + 360 : ((r > 360) ? r-360 : r);
        r /= 5;

        //**** wndVSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_VSLIDER,r);
        r = (int)(180 - GetVisualParam()->ProjParam.theta + TPref::TUniv.sDefParam.theta);
        r = (r < 0) ?  r+ 360 : ((r > 360) ? r-360 : r);
        r /= 5;
        //**** wndHSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_HSLIDER,r);
    }
}

void CViewUniv::OnSize(UINT nType, int cx, int cy)
{
    CRect mrect;
    GetClientRect(&mrect);
    CView::OnSize(nType, cx, cy);
	CBCGPMDIChildWnd *pChild = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd,	GetParent());

	pChild->RecalcLayout();

    // TODO: Add your message handler code here
    if (GetVisualParam())
    {
        //CRect mrect;
        GetClientRect(&mrect);
        GetVisualParam()->ptRepCoord = mrect.CenterPoint();
        GetVisualParam()->nWndWidth = cx;
        GetVisualParam()->nWndHeight = cy;
        Invalidate();
        UpdateWindow();
    }
}


int CViewUniv::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
    return CView::OnToolHitTest(point, pTI);
}


BOOL CViewUniv::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID2 =pNMHDR->idFrom;

    return FALSE;
}

BOOL CViewUniv::PreTranslateMessage(MSG* pMsg)
{
	if (IsWindowActivated())
	    if(pMsg->message== WM_LBUTTONDOWN ||
		    pMsg->message== WM_LBUTTONUP ||
			pMsg->message== WM_MOUSEMOVE)
				m_wndToolTip.RelayEvent(pMsg);


    return CView::PreTranslateMessage(pMsg);
}


void CViewUniv::SetFixedProjection(BOOL bFixed)
{
    if (GetVisualParam()) GetVisualParam()->bFixed = bFixed;
    if (GetVisualParam()) GetVisualParam()->bKeepProj = bFixed;

    //**** wndHSliderBar.m_wndSlider.EnableWindow(!bFixed);
    //**** wndHSliderBar.m_wndSlider.ShowWindow(!bFixed);
    //**** wndHSliderBar.m_wndSlider.Invalidate();
    //**** wndHSliderBar.m_wndSlider.UpdateWindow();
    //**** wndVSliderBar.m_wndSlider.EnableWindow(!bFixed);
    //**** wndVSliderBar.m_wndSlider.ShowWindow(!bFixed);
    //**** wndVSliderBar.m_wndSlider.Invalidate();
    //**** wndVSliderBar.m_wndSlider.UpdateWindow();
}

void CViewUniv::OnUpdateSliders(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!GetVisualParam()->bFixed);
}

void CViewUniv::OnChangeReferential(UINT nID)
{
    // TODO: Add your command handler code here
    //SProjection myProj = GetVisualParam()->GetProjParam();
    BOOL    bFix =  (m_pCurrentTask && m_pCurrentTask->m_bIsRepereFixed);
    if (bFix) return;

    switch (nID){
    case ID_VISUALISATION_REFERENTIAL:
        // Decide what to do when the icon of the menu button is clicked.
        break;
    case ID_VISUALISATION_REFERENTIAL_AXES:
        SetVisualParam(CVisualParam::VisuRep);
        break;
    case ID_VISUALISATION_REFERENTIAL_FLOOR:
        SetVisualParam(CVisualParam::VisuPlane);
        break;
    case ID_VISUALISATION_REFERENTIAL_WALLS:
        SetVisualParam(CVisualParam::VisuClois);
        break;
    case ID_VISUALISATION_REFERENTIAL_NONE:
    default:
        SetVisualParam(CVisualParam::VisuNone);
        break;
    }
}

void CViewUniv::OnUpdateReferential(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    BOOL bCheck = FALSE;
    BOOL bFix =     (GetVisualParam() && (GetVisualParam()->bFixed || GetVisualParam()->bKeepProj));
    BOOL bFix2 = (m_pCurrentTask && m_pCurrentTask->m_bIsRepereFixed);
    int nRep = pCmdUI->m_nID -ID_VISUALISATION_REFERENTIAL_NONE;
    switch (pCmdUI->m_nID){
    case ID_VISUALISATION_REFERENTIAL_AXES:
    case ID_VISUALISATION_REFERENTIAL_FLOOR:
    case ID_VISUALISATION_REFERENTIAL_WALLS:
    case ID_VISUALISATION_REFERENTIAL_NONE:
        if (GetVisualParam())
            bCheck = (GetVisualParam()->nVisuKind == nRep);
        pCmdUI->Enable(!bFix2);
        pCmdUI->SetCheck(bCheck);
        break;
    case ID_VISUALISATION_POINTSOFVIEW:
    case ID_VISUALISATION_REFERENTIAL:
        pCmdUI->Enable(!bFix);
        break;
    default:
        break;
    }
}

void CViewUniv::OnUpdateZoom(CCmdUI* pCmdUI)
{
    BOOL bCheck = FALSE;
    switch (pCmdUI->m_nID)
    {
    case ID_VISUALISATION_ZOOM_150:
        bCheck = (GetVisualParam()->nZoom == 1.50);
        break;
    case ID_VISUALISATION_ZOOM_100:
        bCheck = (GetVisualParam()->nZoom == 1.);
        break;
    case ID_VISUALISATION_ZOOM_75:
        bCheck = (GetVisualParam()->nZoom == .75);
        break;
    case ID_VISUALISATION_ZOOM_50:
        bCheck = (GetVisualParam()->nZoom == .50);
        break;
    case ID_VISUALISATION_ZOOM_25:
        bCheck = (GetVisualParam()->nZoom == .25);
        break;
    case ID_VISUALISATION_ZOOM_ADJUST:
        break;
    default:
        return;
    }
    pCmdUI->SetCheck(bCheck);
    pCmdUI->Enable(TRUE);

}

LRESULT CViewUniv::OnChangeZoom(WPARAM rr,LPARAM)
{
	GetVisualParam()->nZoom = rr / 100.;
	Invalidate();
	UpdateWindow();
	return 0L;
}

void CViewUniv::OnChangeZoom(UINT nID)
{
    if (!GetVisualParam()) return;
    BOOL bRedraw = FALSE;
    switch (nID)
    {
		
    case ID_VISUALISATION_ZOOM_150:
        if (GetVisualParam()->nZoom != 1.50)
        {
            GetVisualParam()->nZoom = 1.50;
            bRedraw = TRUE;
        }
        break;
    case ID_VISUALISATION_ZOOM_100:
        if (GetVisualParam()->nZoom != 1.)
        {
            GetVisualParam()->nZoom = 1.0;
            bRedraw = TRUE;
        }
        break;
    case ID_VISUALISATION_ZOOM_75:
        if (GetVisualParam()->nZoom != 0.75)
        {
            GetVisualParam()->nZoom = 0.75;
            bRedraw = TRUE;
        }
        break;
    case ID_VISUALISATION_ZOOM_50:
        if (GetVisualParam()->nZoom != .5)
        {
            GetVisualParam()->nZoom = .5;
            bRedraw = TRUE;
        }
        break;
    case ID_VISUALISATION_ZOOM_25:
        if (GetVisualParam()->nZoom != .25)
        {
            GetVisualParam()->nZoom = .25;
            bRedraw = TRUE;
        }
        break;
    case ID_VISUALISATION_ZOOM_ADJUST:
        {
            CZoomDlg dlg(this);
            dlg.m_nZoom = (int)((GetVisualParam()->nZoom * 100 - 25)/5);
            if (dlg.DoModal())
            {
                GetVisualParam()->nZoom = (25+5*dlg.m_nZoom)/100.;
                bRedraw = TRUE;
            }

        }
        return;
    default:
        break;
    }
    if (bRedraw)
    {
        Invalidate();
        UpdateWindow();
    }
}


void CViewUniv::OnChangePerspective(UINT nID)
{
    // TODO: Add your command handler code here
    if (!GetVisualParam()) return;
    int nRep = nID - ID_VISUALISATION_PERSPECTIVE_CAVALIERE;
    GetVisualParam()->bParProj = !nRep;
    Invalidate();
    UpdateWindow();
}

void CViewUniv::OnUpdatePerspective(CCmdUI* pCmdUI)
{
    BOOL bCheck = FALSE;
    int nRep = pCmdUI->m_nID - ID_VISUALISATION_PERSPECTIVE_CAVALIERE;
    switch (pCmdUI->m_nID){
    case ID_VISUALISATION_PERSPECTIVE_CAVALIERE:
    case ID_VISUALISATION_PERSPECTIVE_OBLIQUE:
        if (GetVisualParam())
            bCheck = (GetVisualParam()->bParProj != nRep);
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(bCheck);
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CViewUniv Command Handlers

void CViewUniv::OnStartTask(UINT nID)
{
    // TODO: Add your command handler code here
    if (m_pCurrentTask->OnDoTasksOption(nID))
        return;
    if (m_pCurrentTask->m_nTaskID == nID)
    {
        //MessageBox("dfdfsfdsfdsa");
        return;
    }
    CTask *pNewTask = NULL;
    switch (nID) {
    case ID_VISUALISATION_DEFAULTTASK:
        pNewTask = new CDefTask(this,nID);
        break;
    case ID_VISUALISATION_PROJECTION:
        pNewTask = new CProjectionTask(this,nID);
        break;
    case ID_EXPLORATION_MOVE:
        pNewTask = new CMoveObjectTask(this,nID);
        break;
    case ID_EXPLORATION_VERIFY:
        pNewTask = new CVerify3DTask(this,nID);
        break;
    case ID_OBJECT_POINT:
        pNewTask = new CPoint3DTask(this,nID);
        break;
    case ID_OBJECT_LINE:
        pNewTask = new CDroite3DTask(this,nID);
        break;
    case ID_OBJECT_DELETE:
        pNewTask = new CDeleteObjectTask(this,nID);
        break;
    case ID_OBJECT_REDEFINE:
        pNewTask = new CRedefine3DTask(this,nID);
        break;
    case ID_OBJECT_SEGMENT:
    case ID_OBJECT_RAY:
        pNewTask = new CSegment3DTask(this,nID);
        break;
    case ID_OBJECT_CIRCLE:
    case ID_OBJECT_ARCCIRCLE:
        pNewTask = new CCercle3DTask(this,nID);
        break;
    case ID_OBJECT_PLANE:
        pNewTask = new CPlan3DTask(this,nID);
        break;
    case ID_OBJECT_POLYGON:
        pNewTask = new CPolygonTask(this,nID);
        break;
    case ID_OBJECT_CUBE:
        pNewTask = new CCube3DTask(this,nID);
        break;
    case ID_OBJECT_SPHERE:
        pNewTask = new CSphere3DTask(this,nID);
        break;
    case ID_VISUALISATION_CENTERON:
        pNewTask = new CCenterOn3DTask(this,nID);
        break;
    case ID_OBJECT_CYLINDRE:
        pNewTask = new CCylindreTask(this,nID);
        break;
    case ID_CONSTRUCTION_POINTON_CYLINDER:
    case ID_CONSTRUCTION_POINTON_LINE:
    case ID_CONSTRUCTION_POINTON_CIRCLE:
    case ID_CONSTRUCTION_POINTON_PLANE:
    case ID_CONSTRUCTION_POINTON_SPHERE:
        pNewTask = new CPointSur3DTask(this,nID);
        break;
    case ID_CONSTRUCTION_INTERSECTION_LINELINE:
    case ID_CONSTRUCTION_INTERSECTION_LINEPLANE:
    case ID_CONSTRUCTION_INTERSECTION_LINESPHERE:
    case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
    case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
    case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
    case ID_CONSTRUCTION_INTERSECTION_LINECIRCLE:
	case ID_CONSTRUCTION_INTERSECTION_PLANECIRCLE:
	case ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE:
    case ID_CONSTRUCTION_CROSSPRODUCT:
        pNewTask = new CInter3DTask(this,nID);
        break;
    case ID_CONSTRUCTION_MIDPOINT:
    case ID_CONSTRUCTION_DIVSEGMENT:
        pNewTask = new CMilieu3DTask(this,nID);
        break;
    case ID_CONSTRUCTION_CENTER:
        pNewTask = new CCenterTask(this,nID);
        break;
    case ID_CONSTRUCTION_PARALLELLINE:
        pNewTask = new CParallele3DTask(this,nID);
        break;
    case ID_CONSTRUCTION_NORMALLINE:
    case ID_CONSTRUCTION_PERPENDICULARPLANE:
    case ID_CONSTRUCTION_PERPENDICULARLINE:
        pNewTask = new CPerpendic3DTask(this,nID);
        break;
    case ID_EXPLORATION_EXTRACTION:
        pNewTask = new CExtractCalcTask(this,nID);
        break;
    case ID_CONSTRUCTION_SYMMETRICALPOINT:
        pNewTask = new CSymetricTask(this,nID);
        break;
    case ID_EXPLORATION_ANIMATION:
        pNewTask = new CAnimation3DTask(this,nID);
        break;
    case ID_CONSTRUCTION_TRANSLATION:
        pNewTask = new CTranslatTask(this,nID);
        break;
    case ID_EXPLORATION_LABEL:
    case ID_EXPLORATION_EQUATION:
        pNewTask = new CLabelTask(this,nID);
        break;
    case ID_EXPLORATION_MEASURE:
        pNewTask = new CMeasureTask(this,nID);
        break;
    case ID_VISUALISATION_APPLYSTYLE:
        pNewTask = new CCopyShapeTask(this,nID);
        break;
    case ID_EXPLORATION_LOCUS:
        pNewTask = new CLocus3DTask(this,nID);
        break;
    case ID_MACROS_USER1:
    case ID_MACROS_USER2:
    case ID_MACROS_USER3:
    case ID_MACROS_USER4:
    case ID_MACROS_USER5:
    case ID_MACROS_USER6:
    case ID_MACROS_USER7:
    case ID_MACROS_USER8:
    case ID_MACROS_USER9:
    case ID_MACROS_USER10:
    case ID_MACROS_USER11:
    case ID_MACROS_USER12:
    case ID_MACROS_USER13:
    case ID_MACROS_USER14:
    case ID_MACROS_USER15:
    case ID_MACROS_LOAD:
        pNewTask = new CGreffe3DTask(this,nID);
        break;
    default:
        break;
    }
    if (pNewTask)
    {
        delete m_pCurrentTask;
        m_pCurrentTask = pNewTask;
    }
}


void CViewUniv::OnUpdateTasks(CCmdUI* pCmdUI)
{
    if (m_pCurrentTask->OnUpdateTasksOption(pCmdUI)) return;

    BOOL bCheck = ((m_pCurrentTask) && (m_pCurrentTask->m_nTaskID == pCmdUI->m_nID));
    BOOL bEnab = GetDocument()->IsTaskAvailable(pCmdUI->m_nID);

    pCmdUI->Enable(bEnab);
    pCmdUI->SetCheck(bCheck);
}

/////////////////////////////////////////////////////////////////////////////
// CViewUniv Action Handlers
void CViewUniv::OnRotateHor()
{
	int idx = m_wndHorizBar.CommandToIndex(ID_VISUALISATION_POV_HSLIDER);
	CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST(CBCGPSliderButton,m_wndHorizBar.GetButton(idx));
	if (pSlider != NULL)
	{
		int nScrollPos = pSlider->GetPos (ID_VISUALISATION_POV_HSLIDER);

		nScrollPos *=5;
		int r = (int)(180 - nScrollPos + TPref::TUniv.sDefParam.theta);
		r = (r < 0) ?  r+ 360 : ((r > 360) ? r-360 : r);

		SProjection MySProj = GetVisualParam()->GetProjParam();
		MySProj.theta = r;
		GetVisualParam()->SetProjParam(MySProj);
		//GetVisualParam()->SetProjParam(r,ID_THETA);
		SetFocus();
		Invalidate();
		UpdateWindow();
	}
}

void CViewUniv::OnChangePointsOfView()
{
    // TODO: Add your command handler code here
    GetVisualParam()->SetProjParam();

    int r = (int)(GetVisualParam()->ProjParam.phi - 180 -TPref::TUniv.sDefParam.phi);
    r = (r < 0) ? r + 360 : ((r > 360) ? r-360 : r);
    r /= 5;
    //**** wndVSliderBar.SetSliderPos(r);
	SetSliderPosition(ID_VISUALISATION_POV_VSLIDER,r);
    r = (int)(180 - GetVisualParam()->ProjParam.theta + TPref::TUniv.sDefParam.theta);
    r = (r < 0) ?  r+ 360 : ((r > 360) ? r-360 : r);
    r /= 5;
    //**** wndHSliderBar.SetSliderPos(r);
	SetSliderPosition(ID_VISUALISATION_POV_HSLIDER,r);
    Invalidate();
    UpdateWindow();
}

void CViewUniv::OnRotateVer()
{
	int idx = m_wndVertBar.CommandToIndex(ID_VISUALISATION_POV_VSLIDER);
	CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST(CBCGPSliderButton,m_wndVertBar.GetButton(idx));
	if (pSlider != NULL)
	{
		int nScrollPos = pSlider->GetPos (ID_VISUALISATION_POV_HSLIDER);
		nScrollPos *=5;
		int r = (int)(180 + nScrollPos + TPref::TUniv.sDefParam.phi);
		r = (r < 0) ? r + 360 : ((r > 360) ? r-360 : r);
		SProjection MySProj = GetVisualParam()->GetProjParam();
		MySProj.phi = r;
		GetVisualParam()->SetProjParam(MySProj);
  		SetFocus();
		Invalidate();
		UpdateWindow();

	}
}

void CViewUniv::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
    if (!GetVisualParam()) return;
    if (GetVisualParam()->bFixed) return;

	    BOOL bRedraw = TRUE;

    switch (nChar){
    case VK_NUMPAD8 :
    case VK_UP:
        GetVisualParam()->AddProjParam(-5,CVisualParam::ID_PHI);
        break;
    case VK_NUMPAD2 :
    case VK_DOWN:
        GetVisualParam()->AddProjParam(+5,CVisualParam::ID_PHI);
        break;
    case VK_NUMPAD4 :
    case VK_LEFT:
        GetVisualParam()->AddProjParam(+5,CVisualParam::ID_THETA);
        break;
    case VK_NUMPAD6 :
    case VK_RIGHT:
        GetVisualParam()->AddProjParam(-5,CVisualParam::ID_THETA);
        break;
    case VK_ADD :
        if (GetVisualParam()->bParProj)
            bRedraw = FALSE;
        else
        GetVisualParam()->AddProjParam(+50,CVisualParam::ID_RHO);
        break;
    case VK_SUBTRACT :
        if (GetVisualParam()->bParProj)
            bRedraw = FALSE;
        else
            GetVisualParam()->AddProjParam(-50,CVisualParam::ID_RHO);
        break;
    case VK_SPACE :
        GetVisualParam()->bParProj = !(GetVisualParam()->bParProj);
        break;
    case VK_NUMPAD5 :
        OnChangePointsOfView();
    default:
        bRedraw = FALSE;
        break;
    }
    if (bRedraw)
    {
        int r = (int)(GetVisualParam()->ProjParam.phi - 180 -TPref::TUniv.sDefParam.phi);
        r = (r < 0) ? r + 360 : ((r > 360) ? r-360 : r);
        r /= 5;

        //**** wndVSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_VSLIDER,r);

        r = (int)(180 - GetVisualParam()->ProjParam.theta + TPref::TUniv.sDefParam.theta);
        r = (r < 0) ?  r+ 360 : ((r > 360) ? r-360 : r);
        r /= 5;
        //**** wndHSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_HSLIDER,r);
        OnUpdateObjTooltip(NULL);
        Invalidate();
        UpdateWindow();
    }
}

void CViewUniv::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentTask)
    {
        m_pCurrentTask->OnMouseL(nFlags,point);
    }
    CView::OnLButtonDown(nFlags, point);
}

void CViewUniv::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentTask)
    {
        m_pCurrentTask->OnMouseLUp(nFlags,point);
    }
    CView::OnLButtonUp(nFlags, point);
}

void CViewUniv::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentTask)
    {
        m_pCurrentTask->OnMouseLDC(nFlags,point);
    }
    GetDocument()->SaveWindowPos();
    CView::OnLButtonDblClk(nFlags, point);
}

void CViewUniv::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CView::OnRButtonDown(nFlags, point);
}

void CViewUniv::OnContextMenu(CWnd* pWnd, CPoint point)
{
    // TODO: Add your message handler code here
    if (m_pCurrentTask)
    {
        m_pCurrentTask->OnMouseR(0,point);
    }
}

BOOL CViewUniv::IsWindowActivated()
{
    BOOL bIsActivated = FALSE;

    CFrameWnd *pFnd = GetParentFrame();
    CBCGPMDIFrameWnd *pMWnd= DYNAMIC_DOWNCAST(CBCGPMDIFrameWnd,AfxGetMainWnd());
    if (!pMWnd) return bIsActivated;

    CWnd *pWnd = pMWnd->MDIGetActive();

    return (pWnd==pFnd);
}


void CViewUniv::OnMouseMove(UINT nFlags, CPoint point)
{
    if (IsWindowActivated() && m_pCurrentTask)
    {
        m_pCurrentTask->OnMouseMove(nFlags,point);
//      pCurrentTask->GetContextualHelp();


/*      BOOL bFound= FALSE;
        CString mstr = _T("");

        int nb = GetDocument()->m_cObjectSet.GetSize();
        for (int i=0;i<nb;i++)
        {
            CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
            if (!pObj) continue;
            //pObj->CalculVisuel(GetVisualParam());
            //pObj->Draw(pDC,GetVisualParam());
            CRgn *mmm = pObj->InvalideRect();
            if (mmm && mmm->PtInRegion(point))
            {
                bFound = TRUE;
                CString mstr2;
                mstr2.Format("TRUC DE MERDE %d",i);
                if (mstr != _T(""))
                    mstr += _T("\n");
                mstr += mstr2;
            }
            delete mmm;
        }
        if (bFound)
        {
            myToolTip.UpdateTipText(mstr,this);
            if (!myToolTip.IsWindowVisible())
            {
                myToolTip.Activate(TRUE);
            }
            //myToolTip.Update();
        }
        else
        {
            myToolTip.Activate(FALSE);
            //myToolTip.Update();
        }*/
    }

    CView::OnMouseMove(nFlags, point);
}

LRESULT CViewUniv::OnUpdateObjTooltip(WPARAM pObjSet, LPARAM bShowToolTip)
{
    if (!bShowToolTip|| pObjSet==NULL)
    {
        m_wndToolTip.Activate(FALSE);
        if (m_pCurrentTask) m_pCurrentTask->GetContextualHelp();
        return 0L;
    }

    //CStringArray *mSet = (CStringArray*)pObjSet;
    CxObject3DSet *mSet = (CxObject3DSet*)pObjSet;
    int nb = mSet->GetSize();

    if (!nb)
    {
        m_wndToolTip.Activate(FALSE);
        if (m_pCurrentTask) m_pCurrentTask->GetContextualHelp();
        return 0L;
    }

    CString strToolTip(_T(""));
    CString strTaskTip(_T(""));
    if (nb != 1)
    {
        strToolTip.LoadString(IDS_AMBIGUITY);
        strTaskTip.LoadString(ERR_AMBIGUITY);
    }
    else
    {
        //strToolTip = mSet->GetAt(0);
        strToolTip = mSet->GetAt(0)->GetObjectHelp();
        strTaskTip = mSet->GetAt(0)->GetObjectDef();
    }

    CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, theApp.m_pMainWnd);
    if (pMainFrame)
    {
        //CWnd* pMessageBar = GetParentFrame()->GetMessageBar();
        //if (pMessageBar) pMessageBar->SetWindowText(strTaskTip);
		pMainFrame->SendMessage(WM_SHOWOBJ_DESCRIPTION,(WPARAM)&strTaskTip,(LPARAM)FALSE);

    }
    m_wndToolTip.UpdateTipText(strToolTip,this);
    if (!m_wndToolTip.IsWindowVisible())
    {
        //CRect mmmm;
        //mmmm.SetRectEmpty();
        //m_wndToolTip.GetMargin(mmmm);
        m_wndToolTip.Activate(TRUE);
    }
    return 0L;
}

void CViewUniv::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // TODO: Add your specialized code here and/or call the base class
    BOOL bRedraw = TRUE;
    switch (lHint)
    {
    case WM_UPDATEOBJ_SEL:  // Object Selected
        {
            CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
            if (pSelectObj) pSelectObj->SetSelected(FALSE);
            pSelectObj = pObj;
            if (pSelectObj)
            {
                pSelectObj->SetSelected(TRUE);
                //CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
                //if (pWnd)
                //    pWnd->GetPropertyBar()->AddProperties(pSelectObj);
            }
            else
            {
                //CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
                //if (pWnd)
                ///   pWnd->GetPropertyBar()->AddProperties(NULL);
            }
        }
        break;
    case WM_UPDATEOBJ_ADD:  // Object Added
    case WM_UPDATEOBJ_MOD:  // Object Modified
    case WM_UPDATEOBJ_DEL:  // Object Deleted
    case WM_UPDATEOBJ_EXT:  // Object extracted
        break;
    case WM_UPDATEOBJ_MOV:  // Object Moved
        if (m_pCurrentTask && m_pCurrentTask->GetTaskResID()==CTX_PROJECTION &&
            GetVisualParam() && GetVisualParam()->bKeepProj)
        {
            m_pCurrentTask->CreateObject3D();
        }
        break;
    default:
        bRedraw = FALSE;
        break;
    }
    if (bRedraw && GetSafeHwnd())
    {
        Invalidate();
        UpdateWindow();
    }
}

BOOL CViewUniv::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

    // TODO: Add your message handler code here and/or call default
    BOOL bRet = FALSE;
    HCURSOR hCur;
    if (m_pCurrentTask)
    {
        UINT curID = m_pCurrentTask->GetTaskCursor();
        if (curID != -1)
            hCur = AfxGetApp()->LoadCursor(curID);
        else
            hCur = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
    }
    else
        hCur = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
    SetCursor(hCur);
    return TRUE;

    return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CViewUniv::OnToggleMarks()
{
    // TODO: Add your command handler code here
    if (GetVisualParam())
        GetVisualParam()->bDrawMark = !(GetVisualParam()->bDrawMark);
    Invalidate();
    UpdateWindow();
}

void CViewUniv::OnUpdateToggleMarks(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetCheck(GetVisualParam() && GetVisualParam()->bDrawMark);
}

int CViewUniv::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

	CBCGPMDIChildWnd *pChild = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd,	GetParent());

	if (!m_wndHorizBar.CreateEx(pChild,CBRS_ALIGN_TOP,AFX_IDW_TOOLBAR+7) || 
		!m_wndHorizBar.LoadToolBar(IDR_UNIVERSEH_TB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndVertBar.CreateEx(pChild,CBRS_ALIGN_LEFT,AFX_IDW_TOOLBAR+8) || 
		!m_wndVertBar.LoadToolBar(IDR_UNIVERSEV_TB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CString strHor,strVer;
	strHor.LoadString(IDR_UNIVERSEH_TB);
	strVer.LoadString(IDR_UNIVERSEV_TB);
	m_wndVertBar.SetWindowText (strVer);
	m_wndVertBar.SetPermament();
	m_wndHorizBar.SetWindowText (strHor);
	m_wndHorizBar.SetPermament();

	pChild->AddControlBar(&m_wndHorizBar);
	pChild->AddControlBar(&m_wndVertBar);

	CMenu menu;
	menu.LoadMenu (IDR_POPUP_POINTOFVIEW);

	CMenu* pPopupMenu = menu.GetSubMenu (0);
	VERIFY (pPopupMenu != NULL);

	m_wndHorizBar.ReplaceButton (ID_VISUALISATION_POINTSOFVIEW, 
		CBCGPToolbarMenuButton(ID_VISUALISATION_POINTSOFVIEW, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_VISUALISATION_POINTSOFVIEW, FALSE)));

	CBCGPSliderButton btnHSlider (ID_VISUALISATION_POV_HSLIDER);
	btnHSlider.SetRange (0, 72);
	btnHSlider.SetValue (31);

	m_wndHorizBar.ReplaceButton (ID_VISUALISATION_POV_HSLIDER,btnHSlider);

	CBCGPSliderButton btnVSlider (ID_VISUALISATION_POV_VSLIDER);
	btnVSlider.SetRange (0, 72);
	btnVSlider.SetValue (31);
	m_wndVertBar.ReplaceButton (ID_VISUALISATION_POV_VSLIDER,btnVSlider);

    //wndHSliderBar.SetSliderProperty(ID_VISUALISATION_HORSLIDER);
    //if (!wndHSliderBar.CreateEx(GetParent(),TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE |CBRS_TOP |
    //                                CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED,
    //                                CRect(0,0,0,0),
    //                                AFX_IDW_TOOLBAR+15) /*||
    //    !m_wHSliderBar.LoadToolBar(IDR_UNIVERSEH_TB)*/)
    //{
    //    TRACE0("Failed to create Horizontal Slider\n");
    //    return -1;      // fail to create
    //}
    //wndHSliderBar.SetSliderRange(0,72);
    //wndHSliderBar.SetSliderPos(180);
	//pChild->AddControlBar(&wndHSliderBar);

  //  wndVSliderBar.SetSliderProperty(ID_VISUALISATION_VERSLIDER,TBS_VERT);
  //  if (!wndVSliderBar.CreateEx(GetParent(),TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE |CBRS_LEFT |
  //                                  CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED,
  //                                  CRect(0,0,0,0),
  //                                  AFX_IDW_TOOLBAR+16)// ||
  //  /*  !m_wVSliderBar.LoadToolBar(IDR_UNIVERSEV_TB)*/)
  //  {
  //      TRACE0("Failed to create Horizontal Slider\n");
  //      return -1;      // fail to create
  //  }
  //  wndVSliderBar.SetSliderRange(0,72);
  //  wndVSliderBar.SetSliderPos(180);
//	pChild->AddControlBar(&wndVSliderBar);

    return 0;
}

void CViewUniv::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    CView::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

    // TODO: Add your message handler code here

}

void CViewUniv::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
 //   CFrameWnd *pFrmd = GetParentFrame();
   // if (GetSafeHwnd())
   // {
   //     Invalidate();
   //     UpdateWindow();
   // }


}

void CViewUniv::OnUpdateVisualisationReferential(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    int r=0;
    pCmdUI->Enable(GetVisualParam()->nVisuKind);
}

void CViewUniv::OnEditUndo()
{
    // TODO: Add your command handler code here
    GetDocument()->OnDoUndo(this);
}

void CViewUniv::OnEditRedo()
{
    // TODO: Add your command handler code here
    GetDocument()->OnDoRedo(this);
}

void CViewUniv::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->m_nDocUndoState);

}

LRESULT CViewUniv::OnToolbarReset(WPARAM wp,LPARAM)
{
    // TODO: reset toolbar with id = (UINT) wp to its initial state:
    //
    // if (uiToolBarId == IDR_MAINFRAME)
    // {
    //      do something with m_wndToolBar
    // }

    UINT uiToolBarId = (UINT) wp;
    switch (uiToolBarId){
    case IDR_UNIVERSEH_TB:
        {
            int r=0;
        }
        break;
    }
    return 0;
}

void CViewUniv::OnFilePrintPreview()
{
    BCGPPrintPreview (this);
}

void CViewUniv::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    // TODO: Add your specialized code here and/or call the base class
    // Get a DC for the current window (will be a screen DC for print previewing)
    CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if (!pCurrentDC) return;

    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));
    CSize m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
    m_CharSize.cx /= 52;

    // Get the page sizes (physical and logical)
    CSize m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
    CSize m_LogicalPageSize;
    m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 3;
    m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 3;

    pDC->SetMapMode(MM_ISOTROPIC);
    //pDC->SetWindowExt(CSize(1200,600));//m_DocSize);
    pDC->SetWindowExt(m_LogicalPageSize);
    pDC->SetViewportExt(pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height());

    CPoint mp1 = pDC->GetViewportOrg();
    CPoint mp2 = pDC->GetWindowOrg();
    //COLORREF bcolor = ::GetSysColor(COLOR_WINDOW/*COLOR_3DFACE*/);
    COLORREF bcolor = RGB(255,255,255);
    CBrush FixedBack(bcolor);
    CRect ClipRect;
    if (pDC->GetClipBox(ClipRect) != ERROR)
    {
        ClipRect.InflateRect(1, 1); // avoid rounding to nothing
        pDC->FillRect(ClipRect,&FixedBack);
    }

    //PaintAnim(pDC);
    if (GetVisualParam())
    {
        mp2.Offset(GetVisualParam()->ptRepCoord); //358,242
        mp2.Offset(-m_LogicalPageSize.cx/2,-m_LogicalPageSize.cy/2);
        pDC->SetWindowOrg(mp2);
        //pDC->SetViewportOrg(mp1);
        GetVisualParam()->Draw(pDC);
        int nb = GetDocument()->m_cObjectSet.GetSize();
        for (int i=0;i<nb;i++)
        {
            CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
            if (!pObj) continue;
            if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
            pObj->CalculVisuel(GetVisualParam());
            pObj->Draw(pDC,GetVisualParam());
        }
    }
//  CView::OnPrint(pDC, pInfo);
}

BOOL CViewUniv::OnPreparePrinting(CPrintInfo* pInfo)
{
    // TODO: call DoPreparePrinting to invoke the Print dialog box
    return DoPreparePrinting(pInfo);
    //return CView::OnPreparePrinting(pInfo);
}

void CViewUniv::OnEditCopy()
{

    CMetaFileDC * m_pMetaDC = new CMetaFileDC();
    m_pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,"whatever");

    CClientDC clientDC(this) ;
    m_pMetaDC->m_hAttribDC = clientDC.m_hDC;

    //m_pMetaDC->SetMapMode(MM_TEXT   );
    //m_pMetaDC->SetViewportExt(m_pMetaDC->GetDeviceCaps(LOGPIXELSX),
    //  m_pMetaDC->GetDeviceCaps(LOGPIXELSY));
    //m_pMetaDC->SetWindowExt(100, -100);
    //draw meta file
    OnDraw(m_pMetaDC);

    //close meta file dc and prepare for clipboard;
    HENHMETAFILE hMF = m_pMetaDC->CloseEnhanced();

    //copy to clipboard
    OpenClipboard();
    EmptyClipboard();
    ::SetClipboardData(CF_ENHMETAFILE,hMF);
    CloseClipboard();
    //DeleteMetaFile(hMF);
    delete m_pMetaDC;
/*  }
    else
    {
    CBitmap         bitmap;
    CClientDC       dc(this);
    CDC             memDC;

    BeginWaitCursor();

    memDC.CreateCompatibleDC(&dc);

    CRect m_rectArea;
    GetClientRect(&m_rectArea);

    bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(),m_rectArea.Height() );

    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    OnDraw(&memDC);

    OpenClipboard() ;
    EmptyClipboard() ;
    SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
    CloseClipboard () ;

    bitmap.Detach();

    EndWaitCursor();
    }

    rr = !rr;*/
}

void CViewUniv::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    CView::OnWindowPosChanging(lpwndpos);

    // TODO: Add your message handler code here

}
void CViewUniv::OnUpdateMacrosInfo(CCmdUI* pCmdUI)
{
    CCalques3DMacroDoc *pMacroDoc = DYNAMIC_DOWNCAST(CCalques3DMacroDoc,GetDocument());
    pCmdUI->Enable((BOOL)pMacroDoc);
}


void CViewUniv::OnMacrosInfo()
{
    CCalques3DMacroDoc *pMacroDoc = DYNAMIC_DOWNCAST(CCalques3DMacroDoc,GetDocument());

    if (!pMacroDoc) return;

    CMacroInfoDlg mInfoDlg;
    mInfoDlg.m_strDef = pMacroDoc->strObjectDef;
    mInfoDlg.m_strHelp = pMacroDoc->strObjectHelp;
    mInfoDlg.m_strName = pMacroDoc->strObjectName;
    mInfoDlg.m_strPath = pMacroDoc->GetPathName();
    mInfoDlg.m_bModify = TRUE;
    int ret = mInfoDlg.DoModal();
    if (ret==IDOK)
    {
        pMacroDoc->strObjectDef = mInfoDlg.m_strDef;
        pMacroDoc->strObjectHelp = mInfoDlg.m_strHelp;
        pMacroDoc->strObjectName = mInfoDlg.m_strName;
    }

}

void CViewUniv::OnMacrosSave()
{
    // TODO: Add your command handler code here
    CMacroInfoDlg mInfoDlg;

    CMacro3D myMacro;
    myMacro.SetName(_T("%s"));
    mInfoDlg.m_strDef = myMacro.GetObjectDef();
    mInfoDlg.m_strHelp = myMacro.GetObjectHelp();
    //mInfoDlg.m_strName = myMacro.GetObjectName();

    int ret = mInfoDlg.DoModal();
    if (ret==IDOK)
    {
        CCalques3DMacroDoc mdoc;

        mdoc.strObjectDef = mInfoDlg.m_strDef;
        mdoc.strObjectHelp = mInfoDlg.m_strHelp;
        mdoc.strObjectName = mInfoDlg.m_strName;

        int nb = GetDocument()->m_cObjectSet.GetSize();
        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = GetDocument()->m_cObjectSet.GetAt(i);
            mdoc.m_cObjectSet.Add(pObj);
        }

        int nb1 = mdoc.m_cObjectSet.GetSize();
        for (i=0;i<nb1;i++)
        {
            CObject3D *pObj = mdoc.m_cObjectSet.GetAt(i);
        }
        mdoc.OnSaveDocument(mInfoDlg.m_strPath);
        mdoc.m_cObjectSet.RemoveAll();
    }
}

/*void CViewUniv::OnUpdateMacrosLoad(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TPref::bMacroLoading);
}*/

/*void CViewUniv::OnMacrosLoad()
{
    static char BASED_CODE szFilter[] = "Macro-construction (*.m3d)||";

    CFileDialog mdlg(TRUE,"*.m3d","*.m3d",
            OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
            szFilter,
            this);
    int ret = mdlg.DoModal();
    if (ret==IDOK)
    {
        //mdlg.m_ofn;
        CCalques3DMacroDoc mdoc;
        mdoc.OnOpenDocument(mdlg.m_ofn.lpstrFile);

        int nb = mdoc.m_cObjectSet.GetSize();
        CMacro3D * pMacro = new CMacro3D();

        pMacro->strObjectDef = mdoc.strObjectDef;
        pMacro->strObjectHelp = mdoc.strObjectHelp;
        //pMacro->strObjectDef = mdoc.strObjectDef;
        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = mdoc.m_cObjectSet.GetAt(i);
            pObj->bDrawInGraph = FALSE;
            pObj->SetAvailHisto();
            pMacro->m_cSubObjects.Add(pObj);
        }

        mdoc.m_cObjectSet.RemoveAll();
        int nb1 = mdoc.m_cObjectSet.GetSize();
        int nb2 = pMacro->m_cSubObjects.GetSize();

        for (i=0;i<nb2;i++)
        {
            CObject3D *pObj = pMacro->m_cSubObjects.GetAt(i);
        }

        //GetDocument()->m_cObjectSet.Add(pObj);
        BOOL bRes = GetDocument()->AddObject(pMacro);
        if (bRes)
        {
            GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pMacro);
            Invalidate();
            UpdateWindow();
        }
        //delete pMacro;
    }

}*/

/*void CViewUniv::OnMacros (UINT id)
{
    int nbMacros = TPref::TMacroList.GetCount();
    int nItem = id-ID_MACROS_USER1+1;
    if (nItem <=0 || nItem > nbMacros) return;

    int i=0;
    CMacroUserTool* pTool = NULL;
    for(POSITION pos = TPref::TMacroList.GetHeadPosition(); pos != NULL; )
    {
        CMacroUserTool *pT = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
        i++;
        if (pT && (i==nItem))
        {
            pTool = pT;
            break;
        }
    }
    if (!pTool) return;
    if (pTool->m_strFile.IsEmpty()) return;

    CCalques3DMacroDoc mdoc;
    mdoc.OnOpenDocument(pTool->m_strFile);
    int nb = mdoc.m_cObjectSet.GetSize();
    CMacro3D * pMacro = new CMacro3D();

    pMacro->strObjectDef = mdoc.strObjectDef;
    pMacro->strObjectHelp = mdoc.strObjectHelp;
    //pMacro->strObjectDef = mdoc.strObjectDef;
    for (i=0;i<nb;i++)
    {
        CObject3D *pObj = mdoc.m_cObjectSet.GetAt(i);
        pObj->bDrawInGraph = FALSE;
        pObj->SetAvailHisto();
        pMacro->m_cSubObjects.Add(pObj);
    }
    mdoc.m_cObjectSet.RemoveAll();
    int nb1 = mdoc.m_cObjectSet.GetSize();
    int nb2 = pMacro->m_cSubObjects.GetSize();
    for (i=0;i<nb2;i++)
    {
        CObject3D *pObj = pMacro->m_cSubObjects.GetAt(i);
    }

    //GetDocument()->m_cObjectSet.Add(pObj);
    BOOL bRes = GetDocument()->AddObject(pMacro);
    if (bRes)
    {
        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pMacro);
        Invalidate();
        UpdateWindow();
    }
    //delete pMacro;
}*/

//void CViewUniv::OnUpdateMacros (CCmdUI* pCmdUI)
//{
    ///ASSERT(m_arrNames != NULL);

/*  CMenu* pMenu = pCmdUI->m_pMenu;
    int nb = TPref::TMacroList.GetCount();

    if (!pMenu) return;

    CString m_strOriginal;
    pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);


    for (int iMRU=0;iMRU<nb;iMRU++)
    {
        UINT mIdMacro = pCmdUI->m_nID + iMRU;
        m_strOriginal.Format(_T("fdsg df g %d"),mIdMacro);
        pCmdUI->m_pMenu->ModifyMenu(pCmdUI->m_nID + iMRU,MF_BYCOMMAND|MF_STRING,pCmdUI->m_nID + iMRU,m_strOriginal);
    }

    for (iMRU = nb; iMRU < 10; iMRU++)
        pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);
*/
/*  pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
            MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
            CString(buf) + strTemp);

*/
/*  if (m_strOriginal.IsEmpty() && pMenu != NULL)
        pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

    if (m_arrNames[0].IsEmpty())
    {
        // no MRU files
        if (!m_strOriginal.IsEmpty())
            pCmdUI->SetText(m_strOriginal);
        pCmdUI->Enable(FALSE);
        return;
    }

    if (pCmdUI->m_pMenu == NULL)
        return;

    for (int iMRU = 0; iMRU < m_nSize; iMRU++)
        pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

    TCHAR szCurDir[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurDir);
    int nCurDir = lstrlen(szCurDir);
    ASSERT(nCurDir >= 0);
    szCurDir[nCurDir] = '\\';
    szCurDir[++nCurDir] = '\0';

    CString strName;
    CString strTemp;
    for (iMRU = 0; iMRU < m_nSize; iMRU++)
    {
        if (!GetDisplayName(strName, iMRU, szCurDir, nCurDir))
            break;

        // double up any '&' characters so they are not underlined
        LPCTSTR lpszSrc = strName;
        LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
        while (*lpszSrc != 0)
        {
            if (*lpszSrc == '&')
                *lpszDest++ = '&';
            if (_istlead(*lpszSrc))
                *lpszDest++ = *lpszSrc++;
            *lpszDest++ = *lpszSrc++;
        }
        *lpszDest = 0;
        strTemp.ReleaseBuffer();

        // insert mnemonic + the file name
        TCHAR buf[10];
        wsprintf(buf, _T("&%d "), (iMRU+1+m_nStart) % 10);
        pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
            MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
            CString(buf) + strTemp);
    }

    // update end menu count
    pCmdUI->m_nIndex--; // point to last menu added
    pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

    pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
    */
//}

BOOL CViewUniv::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    int nDelat = zDelta / WHEEL_DELTA;
    BOOL bRedraw = TRUE;

    if (nFlags & MK_SHIFT)
        GetVisualParam()->AddProjParam(-15*nDelat,CVisualParam::ID_THETA);
    else
        GetVisualParam()->AddProjParam(-15*nDelat,CVisualParam::ID_PHI);
    if (bRedraw)
    {
        int r = (int)(GetVisualParam()->ProjParam.phi - 180 -TPref::TUniv.sDefParam.phi);
        r = (r < 0) ? r + 360 : ((r > 360) ? r-360 : r);
        r /= 5;

        //**** wndVSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_VSLIDER,r);
        r = (int)(180 - GetVisualParam()->ProjParam.theta + TPref::TUniv.sDefParam.theta);
        r = (r < 0) ?  r+ 360 : ((r > 360) ? r-360 : r);
        r /= 5;
        //**** wndHSliderBar.SetSliderPos(r);
		SetSliderPosition(ID_VISUALISATION_POV_HSLIDER,r);
        OnUpdateObjTooltip(NULL);
        Invalidate();
        UpdateWindow();
    }
    return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CViewUniv::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (IsWindowActivated() && m_pCurrentTask)
    {
        m_pCurrentTask->OnTimer(nIDEvent);
    }
    CView::OnTimer(nIDEvent);
}


void CViewUniv::SetSliderPosition(UINT sliderID,int pos)
{

	CBCGPSliderToolbar* pslider = (sliderID==ID_VISUALISATION_POV_HSLIDER)?&m_wndHorizBar:&m_wndVertBar;
	int idx = pslider->CommandToIndex(sliderID);
	CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST(CBCGPSliderButton,pslider->GetButton(idx));
		if (pSlider != NULL)
		{
			pSlider->SetValue(pos,FALSE);
		}

}


void CViewUniv::OnUpdateFormat(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
    switch (pCmdUI->m_nID)
    {
		case ID_FORMAT_TXTFONT:
		case ID_FORMAT_TXTCOLOR:
		case ID_FORMAT_TXTSIZE:
			{
				CObject3D *pObj= DYNAMIC_DOWNCAST(CLabel3D,pSelectObj);
				bEnable = (pObj!=NULL);
			}
			break;
		case ID_FORMAT_OBJCOLOR:
		case ID_FORMAT_OBJSHAPE:
			{
				CObject3D *pObj= DYNAMIC_DOWNCAST(CLabel3D,pSelectObj);
				bEnable = (pObj==NULL && pSelectObj!=NULL);
			}
			break;
			
	}
    pCmdUI->Enable(bEnable);

}

void CViewUniv::OnFormat()
{
	CLabel3D *pObj= DYNAMIC_DOWNCAST(CLabel3D,pSelectObj);
	if (pObj)
	{
		COLORREF clr = CBCGPColorMenuButton::GetColorByCmdID(ID_FORMAT_TXTCOLOR);
		pObj->mColorText = clr;

		int nb = CFormatToolBar::GetFontSizeByCmdID (ID_FORMAT_TXTSIZE);
		const CBCGPFontDesc* pfDesc = CFormatToolBar::GetFontByCmdID (ID_FORMAT_TXTFONT);
		if (pfDesc)
		{
			LOGFONT lf;
			pObj->mTextFont.GetLogFont(&lf);
			if (nb!=-1)
				lf.lfHeight = nb;
			lf.lfCharSet = pfDesc->m_nCharSet;
			lf.lfPitchAndFamily = pfDesc->m_nPitchAndFamily;
			_tcscpy (lf.lfFaceName, pfDesc->m_strName);
			pObj->SetFont(&lf);

		};

	}
	else if (pSelectObj)
	{
		CPoint shape = CBCGPShapeMenuButton::GetShapeByCmdID(ID_FORMAT_OBJSHAPE);
		COLORREF clr = CBCGPColorMenuButton::GetColorByCmdID(ID_FORMAT_OBJCOLOR);
		pSelectObj->SetStyle(shape.y);
		pSelectObj->SetColor(clr);
	}
	else
	{
		return;
	}
	SetFocus();
	GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_MOD,pSelectObj);
}

void CViewUniv::OnFormat(UINT m_nID)
{
	OnFormat();
}
