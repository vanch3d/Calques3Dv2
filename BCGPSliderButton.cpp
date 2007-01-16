// BCGPSliderButton.cpp: implementation of the CBCGPSliderButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "BCGPSliderButton.h"
#include "MemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_SERIAL(CBCGPSliderButton, CBCGPToolbarButton, 1)

static const int iDefaultWidth = 150;
static const int iDefaultHeight = 24;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPSliderButton::CBCGPSliderButton() : CBCGPToolbarButton(),
	m_wndSlider (*this)
{
	m_dwStyle = WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS;
	m_iWidth = iDefaultWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
	m_pToolBar = NULL;
}
//**************************************************************************************
CBCGPSliderButton::CBCGPSliderButton (UINT uiId,
			int iImage,
			DWORD dwStyle,
			int iWidth) :
	CBCGPToolbarButton (uiId, iImage),
	m_wndSlider (*this)
{
	m_dwStyle = dwStyle | WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS;
	m_iWidth = (iWidth == 0) ? iDefaultWidth : iWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
	m_pToolBar = NULL;
}
//**************************************************************************************
CBCGPSliderButton::~CBCGPSliderButton()
{
	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.DestroyWindow ();
	}
}
//**************************************************************************************
SIZE CBCGPSliderButton::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	if (bHorz)
	{
		if (m_wndSlider.GetSafeHwnd () != NULL)
		{
			m_wndSlider.ModifyStyle (TBS_VERT, TBS_HORZ);
			//m_wndSlider.ModifyStyle (TBS_NOTICKS, TBS_AUTOTICKS);
		}

		return CSize (m_iWidth, iDefaultHeight);
	}
	else
	{
		if (m_wndSlider.GetSafeHwnd () != NULL)
		{
			m_wndSlider.ModifyStyle (TBS_HORZ, TBS_VERT);
			//m_wndSlider.ModifyStyle (TBS_NOTICKS, TBS_AUTOTICKS);
		}

		return CSize (iDefaultHeight, m_iWidth);
	}
}
//**************************************************************************************
void CBCGPSliderButton::Serialize(CArchive& ar)
{
	CBCGPToolbarButton::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_iWidth;
		ar >> m_dwStyle;

		int		nMin;
		int		nMax;
		int		nValue;

		ar >> nMin;
		ar >> nMax;
		ar >> nValue;

		SetRange (nMin, nMax);
		SetValue (nValue, FALSE);
	}
	else
	{
		ar << m_iWidth;
		ar << m_dwStyle;
		ar << m_nMin;
		ar << m_nMax;
		ar << m_nValue;
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnMove ()
{
	if (m_wndSlider.GetSafeHwnd () != NULL &&
		(m_wndSlider.GetStyle () & WS_VISIBLE))
	{
		m_wndSlider.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnSize (int iSize)
{
	m_iWidth = iSize;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_wndSlider.GetSafeHwnd () != NULL &&
		(m_wndSlider.GetStyle () & WS_VISIBLE))
	{
		m_wndSlider.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnChangeParentWnd (CWnd* pWndParent)
{
	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		CWnd* pWndParentCurr = m_wndSlider.GetParent ();
		ASSERT (pWndParentCurr != NULL);

		if (pWndParent != NULL &&
			pWndParentCurr->GetSafeHwnd () == pWndParent->GetSafeHwnd ())
		{
			return;
		}

		m_wndSlider.DestroyWindow ();
	}

	if (pWndParent == NULL || pWndParent->GetSafeHwnd () == NULL)
	{
		return;
	}

	if (!m_wndSlider.Create (m_dwStyle, m_rect, pWndParent, m_nID))
	{
		ASSERT (FALSE);
		return;
	}

	m_pToolBar = DYNAMIC_DOWNCAST(CBCGPToolBar,pWndParent);


	m_wndSlider.SetRange (m_nMin, m_nMax);
	m_wndSlider.SetPos (m_nValue);
	m_wndSlider.SetOwner (pWndParent);
}
//**************************************************************************************
void CBCGPSliderButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarButton::CopyFrom (s);

	const CBCGPSliderButton& src = (const CBCGPSliderButton&) s;

	m_dwStyle = src.m_dwStyle;
	m_iWidth = src.m_iWidth;
	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nValue = src.m_nValue;
}
//************************************************************************************
void CBCGPSliderButton::SetRange (int iMin, int iMax)
{
	m_nMin = iMin;
	m_nMax = iMax;

	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.SetRange (iMin, iMax, TRUE);
	}

	/*CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nMin = iMin;
				pSlider->m_nMax = iMax;

				if (pSlider->m_wndSlider.GetSafeHwnd () != NULL)
				{
					pSlider->m_wndSlider.SetRange (iMin, iMax, TRUE);
				}
			}
		}
	}*/
}
//************************************************************************************
void CBCGPSliderButton::SetValue (int iValue, BOOL bNotify)
{
	m_nValue = iValue;
	if (GetHwnd () != NULL)
	{
		m_wndSlider.SetPos (iValue);
	}

/*	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nValue = iValue;

				if (pSlider->GetHwnd () != NULL)
				{
					pSlider->m_wndSlider.SetPos (iValue);
				}
			}
		}
	}*/

	if (bNotify && m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.GetOwner ()->SendMessage (WM_COMMAND, m_nID);
	}
}
//************************************************************************************
int CBCGPSliderButton::GetPos (UINT uiCmd)
{
	if (GetHwnd () != NULL)
	{
		return m_wndSlider.GetPos ();
	}
	else
		return -1;

/*	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider->GetHwnd () != NULL)
			{
				return pSlider->m_wndSlider.GetPos ();
			}
		}
	}

	return -1;*/
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPSliderCtrl

CBCGPSliderCtrl::CBCGPSliderCtrl(CBCGPSliderButton& btnSlider) :
	m_btnSlider (btnSlider)
{
		m_Images.SetTransparentColor(RGB(255,0,255));
		m_Images.SetImageSize(CSize(21,21));
		m_Images.Load(IDB_SLIDERS);
}

CBCGPSliderCtrl::~CBCGPSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CBCGPSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CBCGPSliderCtrl)
	ON_WM_MOUSEMOVE()
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPSliderCtrl message handlers

void CBCGPSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos = GetPos ();

	CSliderCtrl::OnMouseMove(nFlags, point);

	if ((nFlags & MK_LBUTTON) && GetPos () != nPos)
	{
		m_btnSlider.SetValue (GetPos (), TRUE);
	}
}

void CBCGPSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nPos = GetPos ();

	CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (GetPos () != nPos)
	{
		m_btnSlider.SetValue (GetPos (), TRUE);
	}
}

void CBCGPSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CSliderCtrl::OnLButtonUp(nFlags, point);
	m_btnSlider.SetValue (GetPos (), TRUE);
}

BOOL CBCGPSliderCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	CSliderCtrl::PreCreateWindow(cs);
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.dwExStyle |= WS_EX_TRANSPARENT;

	return TRUE;
}


/*BOOL CBCGPSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}*/

void CBCGPSliderCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW lpCustDraw = (LPNMCUSTOMDRAW)pNMHDR;
	if (lpCustDraw->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;	// send messages for each piece-part
		return;
	}
	if((lpCustDraw->dwDrawStage == CDDS_ITEMPREPAINT) && (lpCustDraw->dwItemSpec != TBCD_THUMB))
	{
		*pResult = CDRF_DODEFAULT;
		return;
	}

	CRect clientRect,channelRect,thumbRect;
	CDC *pDC = CDC::FromHandle(lpCustDraw->hdc);
	CBCGPMemDC MemDC (*pDC, this);
	CDC* memDC = &MemDC.GetDC();

   	GetClientRect(&clientRect);
   	GetChannelRect(&channelRect);
	if (GetStyle( )& TBS_VERT)
	{
		int temp = channelRect.top;
		channelRect.top = channelRect.left;
		channelRect.left = temp;
		temp = channelRect.bottom;
		channelRect.bottom = channelRect.right;
		channelRect.right = temp;
		channelRect.DeflateRect(-1, 5);
	}
	else
		channelRect.DeflateRect(5, -1);
	
	CBCGPVisualManager::GetInstance ()->OnFillBarBackground (memDC, m_btnSlider.m_pToolBar,
		clientRect, clientRect);

   	memDC->FillSolidRect(&channelRect,GetSysColor(COLOR_WINDOW));
   	memDC->DrawEdge(&channelRect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);
	*pResult = CDRF_DODEFAULT;

}

/*void CBCGPSliderCtrl::OnPaint()
{
 	CPaintDC dc(this); // device context for painting
	CBCGPMemDC MemDC (dc, this);
	CDC* memDC = &MemDC.GetDC();
	///CDC* memDC = &dc;

	CRect clientRect,channelRect,thumbRect;

   	GetClientRect(&clientRect);
   	GetChannelRect(&channelRect);

	if (GetStyle( )& TBS_VERT)
	{
		int temp = channelRect.top;
		channelRect.top = channelRect.left;
		channelRect.left = temp;
		temp = channelRect.bottom;
		channelRect.bottom = channelRect.right;
		channelRect.right = temp;
		channelRect.DeflateRect(-1, 5);
	}
	else
		channelRect.DeflateRect(5, -1);

   	GetThumbRect(&thumbRect);

	CBCGPVisualManager::GetInstance ()->OnFillBarBackground (memDC, m_btnSlider.m_pToolBar,
		clientRect, clientRect);

   	memDC->FillSolidRect(&channelRect,GetSysColor(COLOR_WINDOW));
   	memDC->DrawEdge(&channelRect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);

 	BOOL enable = IsWindowEnabled();
 	BOOL hilite = (GetFocus()== this);

	//if (GetStyle( )& TBS_VERT)
	{
		COLORREF clrNormal = RGB(102,204,102),
				 clrHilite = RGB(255,204,51),
				 clrDisable = RGB(160,160,164);
		COLORREF clrBNormal = RGB(51,153,51),
				 clrBHilite = RGB(204,153,51),
				 clrBDisable = RGB(119,119,119);
		COLORREF clrBg = (enable)? ((hilite)? clrHilite : clrNormal) : clrDisable;
		COLORREF clrBBg = (enable)? ((hilite)? clrBHilite : clrBNormal) : clrBDisable;
		int y = thumbRect.Height()/2;
		memDC->FillSolidRect(thumbRect.left+1,thumbRect.top+1,thumbRect.Width()-2,thumbRect.Height()-2,clrBBg);
		memDC->FillSolidRect(thumbRect.left+1,thumbRect.top+1,thumbRect.Width()-3,thumbRect.Height()-3,RGB(255,255,255));
		memDC->FillSolidRect(thumbRect.left+1,thumbRect.top+1,4,thumbRect.Height()-3,clrBg);
		memDC->FillSolidRect(thumbRect.right-7,thumbRect.top+1,5,thumbRect.Height()-3,clrBg);

		memDC->FillSolidRect(thumbRect.left,thumbRect.top+1,1,thumbRect.Height()-2,RGB(192,192,192));
		memDC->FillSolidRect(thumbRect.left+1,thumbRect.top,thumbRect.Width()-2-2,1,RGB(192,192,192));
		memDC->FillSolidRect(thumbRect.right-3,thumbRect.top+1,1,thumbRect.Height()-2,RGB(134,134,134));
		memDC->FillSolidRect(thumbRect.left+1,thumbRect.bottom-1,thumbRect.Width()-2-2,1,RGB(134,134,134));


		//memDC->FillSolidRect(thumbRect.right-4,thumbRect.top+1,1,thumbRect.Height()-2,clrBBg);
		//memDC->FillSolidRect(thumbRect.left+2,thumbRect.bottom-1,thumbRect.Width()-2-2,1,clrBBg);

		//memDC->FillSolidRect(thumbRect.left+4,thumbRect.top,thumbRect.Width()-7-4,thumbRect.Height(),RGB(0,255,0));
	}

// 	CBCGPDrawState ds;
// 	m_Images.PrepareDrawImage (ds);
// 	BOOL enable = IsWindowEnabled();
// 	BOOL hilite = (GetFocus()== this);
// 	int index = (GetStyle( )& TBS_VERT)?2:3;
// 	if (!enable) index+=2;
// 	else if (hilite) index-=2;
// 
// 	if (GetStyle( )& TBS_VERT)
// 		m_Images.Draw (memDC, thumbRect.left, thumbRect.top-5, index);
// 	else
// 		m_Images.Draw (memDC, thumbRect.left-5, thumbRect.top, index);
// 	m_Images.EndDrawImage (ds);

}*/

CBCGPSliderToolbar::CBCGPSliderToolbar()
{
}

BOOL CBCGPSliderToolbar::CreateEx(CWnd* pParentWnd,UINT align,UINT nID)
{
	m_nAlign = align;
	BOOL ret =  CBCGPToolBar::CreateEx(pParentWnd,TBSTYLE_FLAT,
		WS_CHILD | WS_VISIBLE | align | 
		/*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(1, 1, 1, 1),
		nID);

	SetBarStyle(GetBarStyle() & ~CBRS_GRIPPER); 
	return ret;
}


BEGIN_MESSAGE_MAP(CBCGPSliderToolbar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CBCGPSliderToolbar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBCGPSliderToolbar::OnSize(UINT nType, int cx, int cy) 
{

	int idx = CommandToIndex((m_nAlign==CBRS_ALIGN_TOP)?ID_VISUALISATION_POV_HSLIDER:ID_VISUALISATION_POV_VSLIDER);
	CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST(CBCGPSliderButton,GetButton(idx));
		if (pSlider != NULL)
		{
			DWORD style = pSlider->m_wndSlider.GetStyle( );
			CRect rect = pSlider->GetInvalidateRect ();
			if (style & TBS_VERT)
				pSlider->OnSize(cy);
			else//if (style & TBS_HORZ)
				pSlider->OnSize(cx-rect.left);
		}

/*	CObList listButtons;
	CBCGPToolBar::GetCommandButtons ((m_nAlign==CBRS_ALIGN_TOP)?ID_VISUALISATION_POV_HSLIDER:ID_VISUALISATION_POV_VSLIDER, listButtons);
	for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton,listButtons.GetNext (pos));
		if (pSlider != NULL)
		{
			DWORD style = pSlider->m_wndSlider.GetStyle( );
			CRect rect = pSlider->GetInvalidateRect ();
			if (style & TBS_VERT)
				pSlider->OnSize(cy);
			else//if (style & TBS_HORZ)
				pSlider->OnSize(cx-rect.left);
		}
	}*/
		CBCGPToolBar::OnSize(nType, cx, cy);

}


