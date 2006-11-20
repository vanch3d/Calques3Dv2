// ViewGraph.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"

#include "Calques3DDoc.h"

#include "ViewGraph.h"

#include "MemDC.h"

#include "objects\CompositeObj3D.h"
#include "objects\Text3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewGraph

IMPLEMENT_DYNCREATE(CViewGraph, CScrollView)

CViewGraph::CViewGraph()
{
	m_nShowCmd = CObject3D::GRAPH_FULL;
}

CViewGraph::~CViewGraph()
{
	m_cRectPos.RemoveAll();
}


BEGIN_MESSAGE_MAP(CViewGraph, CScrollView)
	//{{AFX_MSG_MAP(CViewGraph)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_GRAPH_SHOWPARENTS,ID_GRAPH_SHOWBASEPT,OnGraphShowCmd)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GRAPH_SHOWPARENTS,ID_GRAPH_SHOWBASEPT,OnUpdateGraphShow)
	ON_COMMAND(ID_HISTORY_PROPERTY, OnObjectProperty)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_PROPERTY, OnUpdateObjectProperty)
	//ON_COMMAND_RANGE(ID_HISTORY_PROPERTY,ID_INFORMATION_PROPERTY, OnGraphShowCmd)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_HISTORY_PROPERTY,ID_INFORMATION_PROPERTY,OnUpdateGraphShow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewGraph drawing

void CViewGraph::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	m_wndToolTip.Create(this);
	m_wndToolTip.AddTool(this);
	m_wndToolTip.SetDelayTime(TTDT_INITIAL,500);
	m_wndToolTip.SetDelayTime(TTDT_RESHOW,100);


	m_Obj = NULL;
	m_ImageList.Create(IDB_OBJECTS_ICONS,16,0,RGB(192,192,192));

	// TODO: calculate the total size of this view
	CSize sTot(0,0);

	int nbf = m_font.CreatePointFont (80, _T ("MS Sans Serif"));

	CCalques3DDoc* pDoc = GetDocument();
	int nb = pDoc->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = pDoc->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;

		CPoint pt = AddObject(pObj);

		sTot.cx = max(sTot.cx,pt.x);
		sTot.cy = max(sTot.cy,pt.y);

	}

	sTot += CSize(75,75);
	SetScrollSizes(MM_TEXT, sTot);
}

void CViewGraph::OnDrawGraph(CDC* pDC)
{
	CCalques3DDoc* pDoc = GetDocument();
	// TODO: add draw code here

    //CFont* pOldFont = pDC->SelectObject (&m_font);
/*	if (m_Obj)
	{
		CRect focRect = m_Obj->rGraphRect;
		if (focRect != CRect(-1,-1,-1,-1))
		{
			focRect.InflateRect(-1,-1);
			CBrush mbr(RGB(192,192,192));
			CBrush *oldB = pDC->SelectObject(&mbr);
			pDC->Rectangle(focRect);
			pDC->SelectObject(oldB );
			//pDC->DrawFocusRect(focRect);
		}
	}*/

	int nb = pDoc->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = pDoc->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
		if (!pObj->bDrawInGraph) continue;

		pObj->DrawDepGraph(pDC,&m_ImageList);

	}
	int nbTrace = m_ObjList.GetSize();
	if (nbTrace)
	{
		CObject3D *pFirst = NULL;
		CObject3D *pPrev = NULL;
		for (int i=0;i<nbTrace;i++)
		{
			CObject3D *pTObj = m_ObjList.GetAt(i);
			if (!pTObj) continue;
			if (m_nShowCmd==CObject3D::GRAPH_BASE)
			{
				BOOL bDraw = pTObj->MaskObject(TPoint3DClass|TPointSurD3DClass|
						 TPointSurC3DClass|TPointSurP3DClass|
						 TPointSurS3DClass);
				pTObj->DrawDepGraph(pDC,&m_ImageList,m_nShowCmd,bDraw,FALSE);
				if (pPrev) 
				{
					CxObject3DSet *pParent = pTObj->GetParents();
					if (pParent) for (int j=0;j<pParent->GetSize();j++)
					{
						CObject3D *pPar = pParent->GetAt(j);
						if (pPar->pComposite==pPrev || pPrev==pPar)
							pTObj->DrawDepGraphLink(pDC,pPrev,pTObj,m_nShowCmd);
	
					}
					delete pParent;
				}
			}
			else if (!pTObj->bDrawInGraph) 
				continue;

			else if (m_nShowCmd==CObject3D::GRAPH_PARENT||m_nShowCmd==CObject3D::GRAPH_CHILDREN)
			{
				if (!pFirst) pFirst = pTObj;
				pTObj->DrawDepGraph(pDC,&m_ImageList,m_nShowCmd,TRUE,FALSE);
				if (pFirst != pTObj)
				{
					if (m_nShowCmd==CObject3D::GRAPH_PARENT)
						pTObj->DrawDepGraphLink(pDC,pTObj,pFirst,m_nShowCmd);
					else
						pTObj->DrawDepGraphLink(pDC,pFirst,pTObj,m_nShowCmd);
				}
			}
/*			else if (m_nShowCmd==2)
			{
				pTObj->DrawDepGraph(pDC,&m_ImageList,m_nShowCmd,TRUE,(i));
			}*/
			else
				pTObj->DrawDepGraph(pDC,&m_ImageList,m_nShowCmd);

			pPrev=pTObj;
		}

	/*	CRect focRect = m_ObjList.GetAt(0)->rGraphRect;
		if (focRect != CRect(-1,-1,-1,-1))
		{
			focRect.InflateRect(1,1);
			pDC->DrawFocusRect(focRect);
		}*/

	}
	if (m_Obj)
	{
		CRect focRect = m_Obj->rGraphRect;
		if (focRect != CRect(-1,-1,-1,-1))
		{
			focRect.InflateRect(1,1);
			pDC->DrawFocusRect(focRect);
		}
	}
	//pDC->SelectObject (pOldFont );
}

void CViewGraph::OnDraw(CDC* pDC)
{
	CMemDC memDC(pDC);

	CRect  ClipRect;
	CBrush br(::GetSysColor(COLOR_WINDOW));
	memDC.GetClipBox(ClipRect);
	memDC.FillRect(ClipRect,&br);

	//CFont mTextFont;
	//mTextFont.CreatePointFont (80, _T ("MS Sans Serif"));

	CFont *oldF = NULL;
	oldF  = memDC.SelectObject(&m_font);

	OnDrawGraph(memDC);

	if (oldF) memDC.SelectObject(oldF);
}

/////////////////////////////////////////////////////////////////////////////
// CViewGraph diagnostics

#ifdef _DEBUG
void CViewGraph::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewGraph::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCalques3DDoc* CViewGraph::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
	return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewGraph message handlers

void CViewGraph::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	CCalques3DDoc* pDoc = GetDocument();
	int nb = pDoc->m_cObjectSet.GetSize();
	m_Obj = NULL;
	for (int i=0;i<nb;i++)
	{
		CObject3D *poo = pDoc->m_cObjectSet.GetAt(i);
		if (!poo) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,poo)) continue;
		m_Obj = GetObjectAt(poo,point);
		if (m_Obj)
			break;
	}
	if (m_Obj)
	{
		m_oldP = point;
		//OnSelectObject(NULL);
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,m_Obj);
		Invalidate();
		UpdateWindow();
		SetCapture();
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CViewGraph::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	CSize ms = point - m_oldP;
//	ms.cx = (int)(ms.cx * 40) / 40;
//	ms.cy = (int)(ms.cy * 40) / 40;
	if (m_Obj && (nFlags & MK_LBUTTON))
	{
		CRect mrect,mrect2;

/*		mrect2 = CRect(point,point);
		mrect2.InflateRect(20,20);

		double nb = (mrect2.left / 20.) - (mrect2.left / 20);
		int add = 0;//(nb<0.5) ? 0 : 1;

		mrect2.left= (int)((mrect2.left / 20)+add) * 20;
		mrect2.top= (int)((mrect2.top / 20)+add) * 20;
		mrect2.right = mrect2.left+ 40;
		mrect2.bottom = mrect2.top+40;*/

		//InvalidateRect(m_Obj->rGraphRect);
		m_Obj->rGraphRect.OffsetRect(ms.cx,ms.cy);
//		m_Obj->rGraphRect = mrect2;
		//InvalidateRect(m_Obj->rGraphRect);

		if (nFlags & MK_SHIFT)
		for (int i=0;i<m_ObjList.GetSize();i++)
		{
			CObject3D* pObj= m_ObjList.GetAt(i);
			if (!pObj || (pObj == m_Obj)) continue;
			pObj->rGraphRect.OffsetRect(ms.cx,ms.cy);
		}
		Invalidate();
		UpdateWindow();
		m_oldP = point;
	}
	else
	{
		CObject *pObjTT = NULL;
		for (int i=0;i<GetDocument()->m_cObjectSet.GetSize();i++)
		{
			CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
			if (!pObj) continue;
			if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;

			pObjTT = GetObjectAt(pObj,point);
			if (pObjTT)	break;

		}
		if (pObjTT)
		{
			//m_pParent->SendMessage(WM_SHOWGRAPHTOOLTIPS,(WPARAM)NULL,(LPARAM)FALSE);
			OnUpdateObjTooltip((WPARAM)pObjTT,TRUE);
		}
		else
		{
			//m_pParent->SendMessage(WM_SHOWGRAPHTOOLTIPS,(WPARAM)(&m_cStrObjFound),(LPARAM)trouve);
			OnUpdateObjTooltip(NULL,FALSE);
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CViewGraph::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	if (m_Obj)
	{
		CSize ms = point - m_oldP;
		CRect mrect;
		m_Obj->rGraphRect.OffsetRect(ms.cx,0);
		ReleaseCapture();
		Invalidate();
		//m_Obj = NULL;
		CCalques3DDoc* pDoc = GetDocument();

//		CRect mrect;
		mrect.SetRectEmpty();

		int nb = pDoc->m_cObjectSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D* pObj = pDoc->m_cObjectSet.GetAt(i);
			if (!pObj) continue;
			CRect drect = pObj->rGraphRect;
			drect.NormalizeRect();
			mrect |= drect;
		}
		CSize mtot(mrect.right,mrect.bottom);
		mtot += CSize(20,20);
		SetScrollSizes(MM_TEXT, mtot);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

BOOL CViewGraph::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

CObject3D *CViewGraph::GetObjectAt(CObject3D *pObj,CPoint point)
{
	if (!pObj) return NULL;
	if (!pObj->bDrawInGraph) return NULL;
	//if (pObj->rGraphRect == CRect(-1,-1,-1,-1)) return NULL;

	if (pObj->rGraphRect.PtInRect(point))
	{
		return pObj;
	}
	CCompositeObj3D * pCObj = DYNAMIC_DOWNCAST(CCompositeObj3D,pObj);
	if (!pCObj) return NULL;

	int nb = pCObj->m_cSubObjects.GetSize();
	for (int i=pCObj->nStartShow;i<nb;i++)
	{
		CObject3D *co = pCObj->m_cSubObjects.GetAt(i);
		if (!co) continue;
		if (!co->bDrawInGraph) continue;
		if (co->rGraphRect == CRect(-1,-1,-1,-1)) continue;

		if (co->rGraphRect.PtInRect(point))
		{
			return co;
		}
	}
	return NULL;
}

void CViewGraph::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	CCalques3DDoc* pDoc = GetDocument();
	int nb = pDoc->m_cObjectSet.GetSize();

/*	m_Obj = NULL;
	for (int i=0;i<nb;i++)
	{
		CObject3D *poo = pDoc->m_cObjectSet.GetAt(i);
		if (!poo) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,poo)) continue;
		m_Obj = GetObjectAt(poo,point);
		if (m_Obj)
			break;
	}*/

	if (m_nShowCmd)
		OnSelectObject(m_Obj);
/*	else if (m_Obj)
	{
		if (m_Obj->bValidate)
			GetDocument()->ModifyPropObject(m_Obj);
		else
		{
			UINT rr = m_Obj->CalculConceptuel();
			m_Obj->HandleObjectError(rr,TRUE);
		}
	}*/
	GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,m_Obj);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CViewGraph::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	CCalques3DDoc* pDoc = GetDocument();
	int nb = pDoc->m_cObjectSet.GetSize();
	//m_Obj = NULL;
	CObject3D *pObj = NULL;
	for (int i=0;i<nb;i++)
	{
		CObject3D *poo = pDoc->m_cObjectSet.GetAt(i);
		if (!poo) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,poo)) continue;
		pObj = GetObjectAt(poo,point);
		if (pObj)
			break;
	}

	if (m_Obj && m_Obj==pObj)
		OnObjectProperty();

	CScrollView::OnRButtonDown(nFlags, point);
}

CPoint CViewGraph::AddObject(CObject3D *pObj)
{
	if (!pObj) return NULL;
	if (!pObj->bDrawInGraph) return NULL;

	if (pObj->rGraphRect == CRect(-1,-1,-1,-1))
	{
		CPoint& pt = CPoint(-30,60*pObj->nDepth-40);
		BOOL bRet = m_cRectPos.Lookup(pObj->nDepth,pt);

		pt.x += 50;
		m_cRectPos.SetAt(pObj->nDepth,pt);

		CRect mrect(pt,CSize(40,40));
		//CString mstr = pObj->GetObjectHelp();
		//pDC->DrawText(mstr,mrect,DT_END_ELLIPSIS|DT_CENTER|DT_WORDBREAK);
		pObj->rGraphRect = mrect;
		return pt;
	}
	
	return pObj->rGraphRect.TopLeft();
}

void CViewGraph::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRedraw = TRUE;
	CPoint pt=NULL;
	switch (lHint)
	{
	case WM_UPDATEOBJ_ADD:	// Object Added
		{
			CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
			pt = AddObject(pObj);
			CSize sTot = GetTotalSize();
			CRect mrect(0,0,0,0);
			mrect.InflateRect(0,0,sTot.cx,sTot.cy);
			mrect.NormalizeRect();
			CRect rrect = pObj->rGraphRect;
			rrect.NormalizeRect();
			mrect |= rrect;
			sTot = CSize(mrect.right,mrect.bottom);
			sTot += CSize(20,20);
			SetScrollSizes(MM_TEXT, sTot);
		}
		break;
	case WM_UPDATEOBJ_SEL:	// Object Selected
		{
			CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
			//OnSelectObject(pObj);
			CEquation3D *pEqu = DYNAMIC_DOWNCAST(CEquation3D,pObj);
			if (pEqu)
			{
				pObj = pEqu->pSource;
			}

			//m_Obj = pObj;
		}
		break;
	case WM_UPDATEOBJ_MOV:	// Object Moved
		break;
	case WM_UPDATEOBJ_DEL:	// Object Deleted
		{
			CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
			m_Obj = NULL;
			OnSelectObject(NULL);
		}
		break;
	case WM_UPDATEOBJ_MOD:	// Object Modified
	default:
		bRedraw = FALSE;
		break;
	}
	if (bRedraw)
	{
		//CSize sTot = GetTotalSize();
		//sTot.cx = max(sTot.cx,pt.x+75);
		//sTot.cy = max(sTot.cy,pt.y+75);
		//sTot += CSize(75,75);
		//SetScrollSizes(MM_TEXT, sTot);
		Invalidate();
		UpdateWindow();
	}

}

BOOL GetFreePoint(CObject3D* pObj,CxObject3DSet& pSet)
{
	if (pObj->MaskObject(TPoint3DClass|TPointSurD3DClass|
						 TPointSurC3DClass|TPointSurP3DClass|
						 TPointSurS3DClass))
	{
		pSet.Add(pObj);
		return TRUE;
	}
	else
	{
		CxObject3DSet* pParentSet = pObj->GetParents();
		if (pParentSet) for (int i=0;i<pParentSet->GetSize();i++)
		{
			CObject3D *pParent = pParentSet->GetAt(i);
			if (!pParent) continue;
			if (GetFreePoint(pParent,pSet))
			{
				if (pObj->pComposite)
					pSet.Add(pObj->pComposite);
				else
					pSet.Add(pObj);
				//return TRUE;
			}
		}
		delete pParentSet;
	}
	return TRUE;
}


// Response to a WM_UPDATEOBJ_SEL notification
BOOL CViewGraph::OnSelectObject(CObject3D *pObj)
{
	m_ObjList.RemoveAll();
	switch (m_nShowCmd){
	case CObject3D::GRAPH_NONE:
		break;
	case CObject3D::GRAPH_BASE:
		if (pObj)
		{
			GetFreePoint(pObj,m_ObjList);
			for (int i=0;i<m_ObjList.GetSize();i++)
			{
				CObject3D *pOobj = m_ObjList.GetAt(i);
				CString str = pOobj->GetObjectName();
			}
		}
		break;
	case CObject3D::GRAPH_PARENT:
		if (pObj)
		{
			m_ObjList.Add(pObj);
			CxObject3DSet* list = pObj->GetParents();
			if (list)
			{
				for (int i=0;i<list->GetSize();i++)
				{
					CObject3D *pOO = list->GetAt(i);
					if (pOO)
					{
						if (pOO->pComposite) 
							m_ObjList.Add(pOO->pComposite);
						else 
							m_ObjList.Add(pOO);
					}
				}
				delete list;
			}
		}
		break;
	case CObject3D::GRAPH_CHILDREN:
		if (pObj)
		{
			pObj->GetDependList(&m_ObjList,2);
			//m_ObjList.Add(pObj);
		}
		break;
	case CObject3D::GRAPH_FULL:
		if (pObj)
		{
			pObj->GetDependList(&m_ObjList);

		}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL CViewGraph::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	return DoPreparePrinting(pInfo);
	//return CScrollView::OnPreparePrinting(pInfo);
}

void CViewGraph::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

void CViewGraph::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
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
	m_LogicalPageSize.cx = 0.33 * ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 ;
	m_LogicalPageSize.cy = 0.33 * ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 ;

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

	CFont mTextFont;
	mTextFont.CreatePointFont (80, _T ("Arial"));

	CFont *oldF = NULL;
	oldF  = pDC->SelectObject(&mTextFont);

	OnDrawGraph(pDC);

	if (oldF) pDC->SelectObject(oldF);
	//CScrollView::OnPrint(pDC, pInfo);
}

void CViewGraph::OnEditCopy() 
{
	CMetaFileDC * m_pMetaDC = new CMetaFileDC();
	m_pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,"whatever");

	CClientDC clientDC(this) ; 
	m_pMetaDC->m_hAttribDC = clientDC.m_hDC; 

	//m_pMetaDC->SetMapMode(MM_TEXT   );
	//m_pMetaDC->SetViewportExt(m_pMetaDC->GetDeviceCaps(LOGPIXELSX),
	//	m_pMetaDC->GetDeviceCaps(LOGPIXELSY));
	//m_pMetaDC->SetWindowExt(100, -100);
	//draw meta file
	OnDrawGraph(m_pMetaDC);

	//close meta file dc and prepare for clipboard;
	HENHMETAFILE hMF = m_pMetaDC->CloseEnhanced();

	//copy to clipboard
	OpenClipboard();
	EmptyClipboard();
	::SetClipboardData(CF_ENHMETAFILE,hMF);
	CloseClipboard();
	//DeleteMetaFile(hMF);
	delete m_pMetaDC;
	
}

void CViewGraph::OnGraphShowCmd(UINT nCmd) 
{
	// TODO: Add your command handler code here
	BOOL bRedraw = TRUE;
	switch (nCmd){
//	case ID_HISTORY_PROPERTY:
//		m_nShowCmd = DIS_GRAPH_NONE;
//		break;
	case ID_GRAPH_SHOWPARENTS:
		m_nShowCmd = CObject3D::GRAPH_PARENT;
		break;
	case ID_GRAPH_SHOWBASEPT:
		m_nShowCmd = CObject3D::GRAPH_BASE;
		break;
	case ID_GRAPH_SHOWCHILDREN:
		m_nShowCmd = CObject3D::GRAPH_CHILDREN;
		break;
	case ID_GRAPH_SHOWDEPENDENTS:
		m_nShowCmd = CObject3D::GRAPH_FULL;
		break;
	default:
		bRedraw = FALSE;
		break;
	}
	if (bRedraw)
	{
		OnSelectObject(m_Obj);
		Invalidate();
		UpdateWindow();
	}
}

void CViewGraph::OnUpdateGraphShow (CCmdUI* pCmdUI)
{
	BOOL bCheck=FALSE;
	switch (pCmdUI->m_nID){
//	case ID_INFORMATION_PROPERTY:
//		bCheck = (m_nShowCmd == DIS_GRAPH_NONE);
//		break;
	case ID_GRAPH_SHOWPARENTS:
	case ID_GRAPH_SHOWCHILDREN:
	case ID_GRAPH_SHOWDEPENDENTS:
	case ID_GRAPH_SHOWBASEPT:
		bCheck = (m_nShowCmd == (pCmdUI->m_nID-ID_GRAPH_SHOWPARENTS+1));
		break;
	default:
		break;
	}
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(bCheck);
}

void CViewGraph::OnObjectProperty() 
{
	// TODO: Add your command handler code here
	if (!m_Obj) return;
	if (m_Obj->bValidate)
		GetDocument()->ModifyPropObject(m_Obj);
	else
	{
		UINT rr = m_Obj->CalculConceptuel();
		m_Obj->HandleObjectError(rr,TRUE);
	}
	
}

void CViewGraph::OnUpdateObjectProperty(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Obj != NULL);
}

LRESULT CViewGraph::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!lp || !wp)
	{
		m_wndToolTip.Activate(FALSE);
		return 0L;
	}

	CObject3D *mSet = (CObject3D*)wp;

	CString mstr = mSet->GetObjectHelp();

	m_wndToolTip.UpdateTipText(mstr,this);
	if (!m_wndToolTip.IsWindowVisible())
	{
		//CRect mmmm;
		//mmmm.SetRectEmpty();
		//m_wndToolTip.GetMargin(mmmm);
		m_wndToolTip.Activate(TRUE);
	}
	return 0L;
}

BOOL CViewGraph::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||
        pMsg->message== WM_LBUTTONUP ||
        pMsg->message== WM_MOUSEMOVE)
		m_wndToolTip.RelayEvent(pMsg);
	
	return CScrollView::PreTranslateMessage(pMsg);
}
