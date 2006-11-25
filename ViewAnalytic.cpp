// ViewAnalytic.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "Calques3DDoc.h"

#include "Prefs\Prefs.h"

#include "objects\Text3D.h"

#include "ViewAnalytic.h"

#include "MemDC.h"

#include "tasks\SuppressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewAnalytic

IMPLEMENT_DYNCREATE(CViewAnalytic, CScrollView)

CViewAnalytic::CViewAnalytic()
{
	m_pSelObject = NULL;
	m_bSelection = FALSE;
	m_rSelect.SetRectEmpty();
	m_rViewSize.SetRect(0,0,800,600);
}

CViewAnalytic::~CViewAnalytic()
{
	m_pSelObject = NULL;
}


BEGIN_MESSAGE_MAP(CViewAnalytic, CScrollView)
	//{{AFX_MSG_MAP(CViewAnalytic)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_MATHPAD_ADDCOMMENT, OnUpdateAddComment)
	ON_UPDATE_COMMAND_UI(ID_MATHPAD_ADDEQUATION, OnUpdateAddComment)
	ON_COMMAND(ID_MATHPAD_ADDCOMMENT, OnAddComment)
	ON_COMMAND(ID_MATHPAD_ADDEQUATION, OnAddEquation)
	ON_COMMAND(ID_HISTORY_PROPERTY, OnProperty)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_PROPERTY, OnUpdateProperty)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAnalytic drawing

void CViewAnalytic::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	myToolTip.Create(this);
	myToolTip.AddTool(this);
	myToolTip.SetDelayTime(TTDT_INITIAL,500);
	myToolTip.SetDelayTime(TTDT_RESHOW,100);
	
	CSize sizeTotal = m_rViewSize.Size();
	// TODO: calculate the total size of this view
	//sizeTotal.cx = sizeTotal.cy = 100;
	
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CViewAnalytic::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CViewAnalytic::OnDrawAnalytic(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	if (!pDoc) return;

	int nb = GetDocument()->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (!DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
		pObj->CalculVisuel(NULL);
		pObj->DrawMathPad(pDC);
	}

/*f (m_pSelObject)
	{
		CRect focRect = m_pSelObject->rActZone;
		if (focRect != CRect(-1,-1,-1,-1))
		{
			focRect.InflateRect(1,1);
			pDC->DrawFocusRect(focRect);
		}
	}*/

	if (m_bSelection)
	{
		LOGBRUSH logB = {BS_NULL,0,0};
		CBrush mbr;
		mbr.CreateBrushIndirect(&logB);
		CPen mpn(PS_DOT,1,RGB(0,0,0));

		CBrush *oldB = pDC->SelectObject(&mbr);
		CPen *oldP = pDC->SelectObject(&mpn);

		pDC->Rectangle(m_rSelect);

		pDC->SelectObject(oldB);
		pDC->SelectObject(oldP);
	}
}


void CViewAnalytic::OnDraw(CDC* rDC)
{
	CMemDC memDC(rDC);
	CDC* pDC = &memDC;
	//CDC* pDC = rDC;

	CRect  ClipRect;
	CBrush br(::GetSysColor(COLOR_WINDOW));
	pDC->GetClipBox(ClipRect);
	pDC->FillRect(ClipRect,&br);

	CPen pNew(PS_SOLID,1,RGB(220,220,220));

	CPen *pOld = pDC->SelectObject(&pNew);

	int nMaxW = max(m_rViewSize.Width(),ClipRect.Width());
	int nMaxH = max(m_rViewSize.Height(),ClipRect.Height());

	int nbS = 25;
	int nbX = 1 + nMaxW / nbS;
	int nbY = 1 + nMaxH / nbS;
	for (int i=0;i<nbX;i++)
	//	for (int j=0;j<nbY;j++)
		{
			pDC->MoveTo(i*nbS,0);
			pDC->LineTo(i*nbS,nMaxH);
		}
	for (i=0;i<nbY;i++)
	//	for (int j=0;j<nbY;j++)
		{
			pDC->MoveTo(0,i*nbS);
			pDC->LineTo(nMaxW,i*nbS);
		}

	pDC->SelectObject(pOld);
	OnDrawAnalytic(pDC);
}


BOOL CViewAnalytic::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return DoPreparePrinting(pInfo);
	//return CScrollView::OnPreparePrinting(pInfo);
}

void CViewAnalytic::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}


void CViewAnalytic::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnPrint(pDC, pInfo);
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
	OnDrawAnalytic(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CViewAnalytic diagnostics

#ifdef _DEBUG
void CViewAnalytic::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewAnalytic::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCalques3DDoc* CViewAnalytic::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
	return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewAnalytic message handlers

void CViewAnalytic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	BOOL bNotEmpty = (BOOL) m_cSelObjectSet.GetSize();

	CCalques3DDoc* pDoc = GetDocument();
	int nb = pDoc->m_cObjectSet.GetSize();
	if (m_pSelObject)
		m_pSelObject->SetSelected(FALSE);
	m_pSelObject = NULL;
	m_cSelObjectSet.RemoveAll();
	for (int i=0;i<nb;i++)
	{
		CObject3D *poo = pDoc->m_cObjectSet.GetAt(i);
		if (!poo) continue;
		if (!DYNAMIC_DOWNCAST(CEquation3D,poo)) continue;
		m_pSelObject = (CEquation3D*)poo->HitTest(point,TText3DClass);
		if (m_pSelObject)
		{
			m_cSelObjectSet.Add(m_pSelObject);
			break;
		}
	}
	if (m_pSelObject)
	{
		m_oldP = point;

		CEquation3D *pEqu = DYNAMIC_DOWNCAST(CEquation3D,m_pSelObject);
		m_pSelObject->SetSelected();
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,m_pSelObject);
		//Invalidate();
		//UpdateWindow();
		//SetCapture();
	}
	else
	{
		/*if (!bNotEmpty) */GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,NULL);
		//m_bSelection = TRUE;
		m_ptSelStart = point;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CViewAnalytic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bSelection)
	{
		m_bSelection = FALSE;
		Invalidate();
		UpdateWindow();
	}
	else if (m_pSelObject)
	{
		m_rViewSize.NormalizeRect();
		CRect objrect = m_pSelObject->rActZone;
		objrect.NormalizeRect();
		objrect |= m_rViewSize;

		if (m_rViewSize != objrect)
		{
			m_rViewSize == objrect;
			CSize sizeTotal = m_rViewSize.Size();
			// TODO: calculate the total size of this view
			//sizeTotal.cx = sizeTotal.cy = 100;
			SetScrollSizes(MM_TEXT, sizeTotal);
		}
	}
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CViewAnalytic::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	CSize ms = point - m_oldP;
	if (m_bSelection)
	{
		m_rSelect = CRect(m_ptSelStart,point);
		m_rSelect.NormalizeRect();
		Invalidate();
		UpdateWindow();
	}
	else if (m_pSelObject && (nFlags & MK_LBUTTON))
	{
		CRect mrect;
		//InvalidateRect(m_Obj->rGraphRect);
		m_pSelObject->rActZone.OffsetRect(ms.cx,ms.cy);
		//InvalidateRect(m_Obj->rGraphRect);
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
			if (!DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;

			pObjTT = pObj->HitTest(point,TText3DClass);
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

LRESULT CViewAnalytic::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!lp || !wp)
	{
		myToolTip.Activate(FALSE);
		return 0L;
	}

	CObject3D *mSet = (CObject3D*)wp;

	CString mstr = mSet->GetObjectHelp();

	myToolTip.UpdateTipText(mstr,this);
	if (!myToolTip.IsWindowVisible())
	{
		CRect mmmm;
		mmmm.SetRectEmpty();
		myToolTip.GetMargin(mmmm);
		myToolTip.Activate(TRUE);
	}
	return 0L;
}


void CViewAnalytic::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	if (m_pSelObject)
	{
		CObject3D *pObj = m_pSelObject->HitTest(point,TText3DClass);
		if (pObj && pObj==m_pSelObject)
		{
			OnProperty();
		}
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CViewAnalytic::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	BOOL bRedraw = TRUE;

	switch (lHint)
	{
	case WM_UPDATEOBJ_ADD:	// Object Added
		break;
	case WM_UPDATEOBJ_SEL:	// Object Selected
		break;
	case WM_UPDATEOBJ_MOV:	// Object Moved
	case WM_UPDATEOBJ_DEL:	// Object Deleted
		break;
	case WM_UPDATEOBJ_MOD:	// Object Modified
	default:
		bRedraw = FALSE;
		break;
	}
	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}
	
	
}

void CViewAnalytic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE)
	{
		TPref::TMathPad.ShowFraction = !TPref::TMathPad.ShowFraction;
		Invalidate();
		UpdateWindow();
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CViewAnalytic::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CMetaFileDC * m_pMetaDC = new CMetaFileDC();
	m_pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,"whatever");

	CClientDC clientDC(this) ; 
	m_pMetaDC->m_hAttribDC = clientDC.m_hDC; 

	//m_pMetaDC->SetMapMode(MM_TEXT   );
	//m_pMetaDC->SetViewportExt(m_pMetaDC->GetDeviceCaps(LOGPIXELSX),
	//	m_pMetaDC->GetDeviceCaps(LOGPIXELSY));
	//m_pMetaDC->SetWindowExt(100, -100);
	//draw meta file
	OnDrawAnalytic(m_pMetaDC);

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

void CViewAnalytic::OnEditCut() 
{
	// TODO: Add your command handler code here
	int nbO = m_cSelObjectSet.GetSize();
	if (!nbO) return;

	CObject3D* pObj = m_cSelObjectSet.GetAt(0);
	if (!pObj) return;
	
	CxObject3DSet mySet;
	pObj->GetDependList(&mySet);

	CSuppressDlg myDlg;
	myDlg.m_pList = &mySet;

	int rep = myDlg.DoModal();
	if (rep == IDOK)
	{
		pObj->SetSelected(FALSE);
		m_pSelObject = NULL;
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,NULL);
		BOOL bRes = GetDocument()->RemoveObject(pObj);
		if (bRes)
		{
			Invalidate();
			UpdateWindow();
		}
	}
}

void CViewAnalytic::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bOn = (BOOL) m_cSelObjectSet.GetSize();
	pCmdUI->Enable(bOn);
}

void CViewAnalytic::OnUpdateProperty(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bOn = (BOOL) m_cSelObjectSet.GetSize();
	pCmdUI->Enable(bOn);
}

void CViewAnalytic::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->m_nDocUndoState);
}

void CViewAnalytic::OnUpdateAddComment(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}


void CViewAnalytic::OnEditUndo() 
{
	// TODO: Add your command handler code here
	GetDocument()->OnDoUndo(this);
}

void CViewAnalytic::OnAddComment() 
{
	// TODO: Add your command handler code here
	CComment3D* pObj = new CComment3D(_T("ddd"));
	pObj->m_strComment = _T("new comment ...");

	if (GetDocument()->AddObject(pObj))
	{
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pObj);
		Invalidate();
		UpdateWindow();
	}
	
}

void CViewAnalytic::OnAddEquation() 
{
	// TODO: Add your command handler code here
	CMathOp3D* pObj = new CMathOp3D(_T("ddd"));
	pObj->m_strComment = _T("1+1");

	if (GetDocument()->AddObject(pObj))
	{
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pObj);
		Invalidate();
		UpdateWindow();
	}
	
}

void CViewAnalytic::OnProperty() 
{
	// TODO: Add your command handler code here
	if (!m_pSelObject) return;

	if (m_pSelObject->SetProperties(&(GetDocument()->m_cObjectSet)))
	{
		Invalidate();
		UpdateWindow();
	}
}

BOOL CViewAnalytic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||
        pMsg->message== WM_LBUTTONUP ||
        pMsg->message== WM_MOUSEMOVE)
		myToolTip.RelayEvent(pMsg);
	
	return CScrollView::PreTranslateMessage(pMsg);
}
