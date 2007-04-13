//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
//////////////////////////////////////////////////////////////////////
// BCGPShapeBar.cpp: implementation of the CBCGPCShapeToolbarButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <math.h>
#include "BCGPShapeBar.h"
#include "BCGPShapeMenuButton.h"
#include "BCGPShapeButton.h"
#include "objects/Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int SEPARATOR_SIZE = 2;

IMPLEMENT_SERIAL(CBCGPCShapeToolbarButton, CBCGPToolbarButton, 1)

CBCGPCShapeToolbarButton::CBCGPCShapeToolbarButton(
			int type/*=0*/,
			int index/*=0*/, 
			BOOL bIsAutomatic /*= FALSE*/, 
			LPCTSTR lpszColorName /*= NULL*/, 
			BOOL bHighlight /*= FALSE*/)
{
	m_Type = type;
	m_nShape = index;
	m_bHighlight = bHighlight;
	m_strText = (lpszColorName == NULL) ? _T("") : lpszColorName;
	m_bIsAutomatic = bIsAutomatic;
	m_pParentBar = NULL;
	m_bLocked = TRUE;
}

void CBCGPCShapeToolbarButton::OnChangeParentWnd (CWnd* pWndParent)
{
	CBCGPToolbarButton::OnChangeParentWnd (pWndParent);
	m_pParentBar = DYNAMIC_DOWNCAST (CBCGPShapeBar, pWndParent);
}

BOOL CBCGPCShapeToolbarButton::OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* pTI)
{
	if (m_nStyle & TBBS_DISABLED)
	{
		return FALSE;
	}

	if (!CBCGPToolBar::GetShowTooltips () || pTI == NULL)
	{
		return FALSE;
	}

	CString str = m_strText;
	//if (!m_bIsAutomatic && !m_bIsOther && !m_bIsLabel)
	{
			str = CShape::GetShapeDef(m_Type,m_nShape);
	}

	pTI->lpszText = (LPTSTR) ::calloc ((str.GetLength () + 1), sizeof (TCHAR));
	_tcscpy (pTI->lpszText, str);

	return TRUE;
}

void CBCGPCShapeToolbarButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* /*pImages*/,
								BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
								BOOL bDrawBorder, BOOL /*bGrayDisabledButtons*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	BOOL bDisabled = (m_nStyle & TBBS_DISABLED);

	UINT nStyle = m_nStyle;

	if (m_bHighlight)
	{
		m_nStyle |= TBBS_CHECKED;
	}

	//----------------------
	// Fill button interior:
	//----------------------
	FillInterior (pDC, rect, bHighlight);

	//--------------------
	// Draw button border:
	//--------------------
	if (!bDisabled && HaveHotBorder () && bDrawBorder && !bCustomizeMode)
	{
		if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (
				pDC, this, rect, CBCGPVisualManager::ButtonsIsPressed);
		}
		else if (bHighlight &&
			!(m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
		{
			CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (
				pDC, this, rect, CBCGPVisualManager::ButtonsIsHighlighted);
		}
	}

	CRect rectFrame = rect;
	int cxBorder2=1,cyBorder2=1;
	rectFrame.DeflateRect (cxBorder2, cyBorder2);

	//if (m_bIsOther || m_bIsAutomatic || m_bIsLabel)
	{
		CRect rectText = rect;

		//if (m_bIsAutomatic && m_Color != (COLORREF)-1)
		{
			CRect rectColor = rectFrame;
			rectColor.DeflateRect (cxBorder2 + 1, cyBorder2 + 1);
			rectColor.right = rectColor.left + rectColor.Height ();

		/*	if (!bDisabled)
			{
				CBrush br (PALETTERGB(GetRValue (m_Color),
									GetGValue(m_Color), 
									GetBValue(m_Color)));
				CPen pen (PS_SOLID, 1, globalData.clrBarShadow);

				CBrush* pOldBrush = pDC->SelectObject (&br);
				CPen* pOldPen = pDC->SelectObject (&pen);
		
				pDC->Rectangle (rectColor);

				pDC->SelectObject (pOldPen);
				pDC->SelectObject (pOldBrush);
			}
			else
			{
				pDC->Draw3dRect (rectColor, globalData.clrBarHilite,
											globalData.clrBarShadow);
				rectColor.OffsetRect (1, 1);
				pDC->Draw3dRect (rectColor, globalData.clrBarShadow,
											globalData.clrBarHilite);
			}*/


			rectText.left = rectColor.right + cxBorder2;
			CRect shaperect = rectFrame;
			shaperect.left = rectText.left;
		shaperect.DeflateRect (cxBorder2, cyBorder2);
				CShape::DrawShapes(m_Type,pDC,shaperect,m_nShape,TRUE);
		}

		//------------
		// Draw label:
		//------------
		pDC->SetTextColor ((m_nStyle & TBBS_DISABLED) /*&& !m_bIsLabel */? 
			globalData.clrGrayedText :
			globalData.clrBarText);

		UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		//if (!m_bIsLabel)
		{
			nFormat |= DT_CENTER;
		}

		CFont* pCurrFont = NULL;
		if (!bHorz)
		{
			pCurrFont = pDC->SelectObject (&globalData.fontRegular);
			ASSERT (pCurrFont != NULL);
		}

		pDC->DrawText (m_strText, &rectText, nFormat);

		if (pCurrFont != NULL)
		{
			pDC->SelectObject (pCurrFont);
		}
	}


	//------------
	// Draw frame:
	//------------
	if (m_bIsAutomatic)
	{
		if (!bDisabled)
		{
			pDC->Draw3dRect (rectFrame, globalData.clrBarShadow,
										globalData.clrBarShadow);
		}
		else
		{
			pDC->Draw3dRect (rectFrame, globalData.clrBarHilite,
										globalData.clrBarShadow);
			rectFrame.OffsetRect (1, 1);
			pDC->Draw3dRect (rectFrame, globalData.clrBarShadow,
										globalData.clrBarHilite);
		}
	}


	m_nStyle = nStyle;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeBar

IMPLEMENT_SERIAL(CBCGPShapeBar, CBCGPPopupMenuBar, 1)

CBCGPShapeBar::CBCGPShapeBar()
{
    m_nNumColumns = 0;
	m_nNumRowsHorz = 0;
	m_nNumColumnsVert = 0;
	m_BoxSize = CSize (0, 0);
	m_nRowHeight = 0;
	m_nShapeSelected = -1;
	m_nCommandID = 0;
	m_bIsTearOff = TRUE;
	m_bLocked = TRUE;
	m_bIsEnabled = TRUE;
	m_pParentBtn = NULL;
	//m_pWndPropList = NULL;
	m_nHorzOffset = m_nVertOffset = 0;
	m_bInternal = FALSE;
	m_nVertMargin = 4;
	m_nHorzMargin = 4;
}
//**************************************************************************************
CBCGPShapeBar::CBCGPShapeBar (int type,
					 int nShape,
					 int nColumns,
				int	nRowsDockHorz,
				int	nColDockVert,
				UINT nCommandID,
				CBCGPShapeButton* pParentBtn) :
	m_nShapeSelected (nShape),
	m_nNumColumns (nColumns),
	m_nNumRowsHorz (nRowsDockHorz),
	m_nNumColumnsVert (nColDockVert),
	m_bIsTearOff (FALSE),
	m_nCommandID (nCommandID),
	m_pParentBtn (pParentBtn),
	m_Type(type)
{
	//m_pWndPropList = NULL;

	m_bLocked = TRUE;
	m_bIsEnabled = TRUE;

	m_nHorzOffset = m_nVertOffset = 0;
	m_bInternal = FALSE;
	m_nVertMargin = 4;
	m_nHorzMargin = 4;
}
//**************************************************************************************
CBCGPShapeBar::CBCGPShapeBar (CBCGPShapeBar& src, UINT uiCommandID) :
		m_nShapeSelected (src.m_nShapeSelected),
		m_nNumColumns (src.m_nNumColumns),
		m_nNumRowsHorz (src.m_nNumRowsHorz),
		m_nNumColumnsVert (src.m_nNumColumnsVert),
		m_bIsTearOff (TRUE),
		m_nCommandID (uiCommandID),
		m_Type(src.m_Type)

{
	m_bLocked = TRUE;
	m_bIsEnabled = TRUE;
	m_pParentBtn = NULL;
	//m_pWndPropList = NULL;
	m_nHorzOffset = m_nVertOffset = 0;
	m_bInternal = FALSE;
	m_nVertMargin = 4;
	m_nHorzMargin = 4;
}
//**************************************************************************************
CBCGPShapeBar::~CBCGPShapeBar()
{
}
//*************************************************************************************
void CBCGPShapeBar::AdjustLocations ()
{
	if (GetSafeHwnd () == NULL || !::IsWindow (m_hWnd))
	{
		return;
	}

	ASSERT_VALID(this);

	CRect rectClient;	// Client area rectangle
	GetClientRect (&rectClient);

	rectClient.DeflateRect (m_nHorzMargin + m_nHorzOffset, m_nVertMargin + m_nVertOffset);

	int x = rectClient.left;
	int y = rectClient.top;
	int i = 0;

	BOOL bPrevSeparator = FALSE;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; i ++)
	{
		CRect rectButton (0, 0, 0, 0);

		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			if (bPrevSeparator)
			{
				rectButton.SetRectEmpty ();
			}
			else
			{
				if (x > rectClient.left)
				{
					// Next line
					x = rectClient.left;
					y += m_BoxSize.cy + m_nVertMargin;
				}

				rectButton = CRect (
					CPoint (x, y),
					CSize (rectClient.Width (), SEPARATOR_SIZE));

				y += SEPARATOR_SIZE + 2;
				x = rectClient.left;
			}

			bPrevSeparator = TRUE;
		}
		else
		{
			CBCGPCShapeToolbarButton* pColorButton = DYNAMIC_DOWNCAST (CBCGPCShapeToolbarButton, pButton);
			if (pColorButton == NULL)
			{
				continue;
			}

			ASSERT_VALID (pColorButton);

			//if (pColorButton->m_bIsAutomatic || pColorButton->m_bIsOther ||
			//	pColorButton->m_bIsLabel)
			{
				if (x > rectClient.left)
				{
					// Next line
					x = rectClient.left;
					y += m_BoxSize.cy + m_nVertMargin;
				}


				{
					rectButton = CRect (
						CPoint (x, y),
						CSize (rectClient.Width (), (m_nRowHeight-5) /*- m_nVertMargin / 2*/));
					y += (m_nRowHeight-5) /*- m_nVertMargin / 2*/;
					x = rectClient.left;
				}

								bPrevSeparator = FALSE;
			}
			
		}

		pButton->SetRect (rectButton);
	}

	UpdateTooltips ();
}
//***************************************************************************************
CSize CBCGPShapeBar::CalcSize (BOOL bVertDock)
{
	CRect wndrect;
	if (m_pParentBtn)
		m_pParentBtn->GetWindowRect(wndrect);
	else
		wndrect = CRect(0,0,80,20);
	CSize sizeGrid = GetColorGridSize (bVertDock);

	return CSize(
		wndrect.Width(),//(sizeGrid.cx-2) * m_BoxSize.cx + 2 * m_nVertMargin, 
		/*sizeGrid.cy * m_BoxSize.cy +*/ GetExtraHeight (sizeGrid.cx) + 2 * m_nVertMargin);
}
//*************************************************************************************
CSize CBCGPShapeBar::GetColorGridSize (BOOL bVertDock) const
//
// Calculate number of columns and rows in the color grid
//
{
	int nNumColumns = 0;
	int nNumRows = 0;

	CShape temp(m_Type);
	
	int nColors = temp.GetShapeSize();

	if (!m_bIsTearOff || IsFloating () || bVertDock || m_nNumRowsHorz <= 0)
	{
		nNumColumns = !m_bIsTearOff || IsFloating () || m_nNumColumnsVert <= 0 ?
			m_nNumColumns : m_nNumColumnsVert;
nNumColumns=1;

		if (nNumColumns <= 0)
		{
			nNumColumns = (int) (sqrt ((double) nColors)) + 1;
		}

		nNumRows = nColors / nNumColumns;
		if ((nColors % nNumColumns) != 0)
		{
			nNumRows ++;
		}
	}
	else	// Horz dock
	{
		nNumRows = m_nNumRowsHorz;
		nNumColumns = nColors / nNumRows;

		if ((nColors % nNumRows) != 0)
		{
			nNumColumns ++;
		}
	}
	nNumRows=1;
	nNumColumns=nColors;

	return CSize (nNumColumns, nNumRows);
}
//**************************************************************************************
int CBCGPShapeBar::GetExtraHeight (int nNumColumns) const
//
// Calculate additional height required by the misc. elements such
// as "Other" button, document colors, e.t.c
//
{
	int nExtraHeight = 0;

		nExtraHeight += (m_nRowHeight-5)*(nNumColumns);
//		nExtraHeight += m_nVertMargin;

/*	if (!m_strOtherColor.IsEmpty ())
	{
		nExtraHeight += m_nRowHeight;
	}

	if (!m_strDocColors.IsEmpty () && !m_lstDocColors.IsEmpty () &&
		(m_bShowDocColorsWhenDocked || IsFloating ()))
	{
		int nDocColorRows = (int) m_lstDocColors.GetCount () / nNumColumns;
		if ((m_lstDocColors.GetCount () % nNumColumns) != 0)
		{
			nDocColorRows++;
		}

		nExtraHeight += m_nRowHeight + nDocColorRows * m_BoxSize.cy + 2 * SEPARATOR_SIZE + m_nVertMargin;
	}*/

	return nExtraHeight;
}

BEGIN_MESSAGE_MAP(CBCGPShapeBar, CBCGPPopupMenuBar)
	//{{AFX_MSG_MAP(CBCGPShapeBar)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeBar message handlers


//**************************************************************************************
void CBCGPShapeBar::ContextToSize (BOOL bSquareButtons, BOOL bCenterButtons)
{
	ASSERT (GetSafeHwnd () != NULL);

	CRect rectClient;
	GetClientRect (rectClient);

	// First, adjust height:
	int nCurrHeight = CalcSize (TRUE).cy;
	int yDelta = nCurrHeight < rectClient.Height () ? 1 : -1;

	while ((nCurrHeight = CalcSize (TRUE).cy) != rectClient.Height ())
	{
		if (yDelta < 0)
		{
			if (nCurrHeight < rectClient.Height ())
			{
				break;
			}
		}
		else if (nCurrHeight > rectClient.Height ())
		{
			m_BoxSize.cy -= yDelta;
			m_nRowHeight = m_BoxSize.cy * 3 / 2;
			break;
		}

		m_BoxSize.cy += yDelta;
		m_nRowHeight = m_BoxSize.cy * 3 / 2;
	}

	// Now, adjust width:
	int nCurrWidth = CalcSize (TRUE).cx;
	int xDelta = nCurrWidth < rectClient.Width () ? 1 : -1;

	while ((nCurrWidth = CalcSize (TRUE).cx) != rectClient.Width ())
	{
		if (xDelta < 0)
		{
			if (nCurrWidth < rectClient.Width ())
			{
				break;
			}
		}
		else if (nCurrWidth > rectClient.Width ())
		{
			m_BoxSize.cy -= xDelta;
			break;
		}

		m_BoxSize.cx += xDelta;
	}

	m_BoxSize.cx--;
	m_BoxSize.cy--;

	if (bSquareButtons)
	{
		m_BoxSize.cx = m_BoxSize.cy = min (m_BoxSize.cx, m_BoxSize.cy);
		m_nRowHeight = m_BoxSize.cy * 3 / 2;
	}

	if (bCenterButtons)
	{
		// Finaly, calculate offset to center buttons area:
		CSize size = CalcSize (TRUE);

		m_nHorzOffset = (rectClient.Width () - size.cx) / 2;
		m_nVertOffset = (rectClient.Height () - size.cy) / 2;
	}
	else
	{
		m_nHorzOffset = m_nVertOffset = 0;
	}

	AdjustLocations ();
}
//**************************************************************************************
void CBCGPShapeBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (m_bIsTearOff)
	{
		CBCGPToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
	else
	{
		CBCGPPopupMenuBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//**************************************************************************************
void CBCGPShapeBar::OnNcPaint() 
{
	if (m_bIsTearOff)
	{
		CBCGPToolBar::OnNcPaint();
	}
	else
	{
		CBCGPPopupMenuBar::OnNcPaint();
	}
}
//**************************************************************************************
int CBCGPShapeBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPPopupMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_BoxSize = GetMenuImageSize ();
	m_BoxSize.cx ++;
	m_BoxSize.cy ++;

	m_bLeaveFocus = FALSE;
	m_nRowHeight = m_BoxSize.cy * 3 / 2;
	Rebuild ();

	if (m_pParentBtn != NULL)
	{
		SetCapture ();
		m_pParentBtn->m_bCaptured = FALSE;
	}
	/*else if (m_pWndPropList != NULL)
	{
		SetCapture ();
	}*/

	return 0;
}
//*************************************************************************************
void CBCGPShapeBar::Rebuild ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	RemoveAllButtons ();

	BOOL bAlreadySelected = FALSE;

	CShape temp(m_Type);
	int nColors = temp.GetShapeSize();
	for (int i = 0; i <nColors; i ++)
	{
		InsertButton (new CBCGPCShapeToolbarButton (m_Type,i, FALSE, NULL, m_nShapeSelected == i));

		if (!bAlreadySelected)
		{
			bAlreadySelected = (m_nShapeSelected == i);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
/// CBCGPShapeCCmdUI
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapeCCmdUI : public CCmdUI
{
public:
	CBCGPShapeCCmdUI();

public: // re-implementations only
	virtual void Enable(BOOL bOn);
	BOOL m_bEnabled;
};

CBCGPShapeCCmdUI::CBCGPShapeCCmdUI()
{
	m_bEnabled = TRUE;  // assume it is enabled
}
//*************************************************************************************
void CBCGPShapeCCmdUI::Enable(BOOL bOn)
{
	m_bEnabled = bOn;
	m_bEnableChanged = TRUE;
}
//*************************************************************************************
void CBCGPShapeBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	if (m_nCommandID == 0 || m_nCommandID == (UINT)-1)
	{
		CBCGPPopupMenuBar::OnUpdateCmdUI (pTarget, bDisableIfNoHndler);
		return;
	}

	CBCGPShapeCCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = m_nCommandID;

	BOOL bIsEnabled = FALSE;
	if (pTarget->OnCmdMsg (m_nCommandID, CN_UPDATE_COMMAND_UI, &state, NULL))
	{
		bIsEnabled = state.m_bEnabled;
	}
	else if (bDisableIfNoHndler && !state.m_bEnableChanged)
	{
		AFX_CMDHANDLERINFO info;
		info.pTarget = NULL;

		bIsEnabled = pTarget->OnCmdMsg (m_nCommandID, CN_COMMAND, &state, &info);
	}

	if (bIsEnabled != m_bIsEnabled)
	{
		m_bIsEnabled = bIsEnabled;

		for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPCShapeToolbarButton* pColorButton = DYNAMIC_DOWNCAST (CBCGPCShapeToolbarButton, m_Buttons.GetNext (pos));
			if (pColorButton != NULL)
			{
				pColorButton->m_nStyle &= ~TBBS_DISABLED;
				if (!bIsEnabled)
				{
					pColorButton->m_nStyle |= TBBS_DISABLED;
				}
			}
		}

		Invalidate ();
		UpdateWindow ();
	}

	CBCGPPopupMenuBar::OnUpdateCmdUI (pTarget, bDisableIfNoHndler);
}
//*************************************************************************************
void CBCGPShapeBar::DoPaint (CDC* pDC)
{
	CBCGPPopupMenuBar::DoPaint (pDC);
}

//*************************************************************************************
BOOL CBCGPShapeBar::OnSendCommand (const CBCGPToolbarButton* pButton)
{
	if (m_pParentBtn != NULL /*|| m_pWndPropList != NULL*/)
	{
		ReleaseCapture ();
	}

 int shape = (COLORREF) -1;

	CBCGPShapeMenuButton* pShapeMenuButton = NULL;

	CBCGPPopupMenu* pParentMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, GetParent ());
	if (pParentMenu != NULL)
	{
		pShapeMenuButton = DYNAMIC_DOWNCAST (CBCGPShapeMenuButton, pParentMenu->GetParentButton ());
	}
	CBCGPCShapeToolbarButton* pShapeButton = DYNAMIC_DOWNCAST (CBCGPCShapeToolbarButton, pButton);
	if (pShapeButton == NULL)
	{
		ASSERT (FALSE);
	}
	if (pShapeButton->m_bIsAutomatic)
	{
		shape = (COLORREF) -1;
	}
	else
	{
		shape = pShapeButton->m_nShape;
	}

	if (pShapeMenuButton != NULL)
	{
		pShapeMenuButton->SetShape(m_Type,shape);
		InvokeMenuCommand (pShapeMenuButton->m_nID, pShapeMenuButton);
	}
	else if (m_pParentBtn != NULL)
	{
		m_pParentBtn->UpdateShape (shape);
		GetParent ()->SendMessage (WM_CLOSE);
	}
	/*else if (m_pWndPropList != NULL)
	{
		m_pWndPropList->UpdateShape (color);
		GetParent ()->SendMessage (WM_CLOSE);
	}*/
	else
	{
		ASSERT (m_nCommandID != 0);

		SetShape (shape);

	/*	CObList listButtons;
		if (CBCGPToolBar::GetCommandButtons (m_nCommandID, listButtons) > 0)
		{
			for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
			{
				CBCGPColorMenuButton* pButton = 
					DYNAMIC_DOWNCAST (CBCGPColorMenuButton, listButtons.GetNext (pos));
				if (pButton != NULL)
				{
					pButton->SetColor (color, FALSE);
				}
			}
		}

		CBCGPColorMenuButton::SetColorByCmdID (m_nCommandID, color);*/
		GetOwner()->SendMessage (WM_COMMAND, m_nCommandID);    // send command

		if (BCGCBProGetTopLevelFrame (this) != NULL)
		{
			BCGCBProGetTopLevelFrame (this)->SetFocus ();
		}
	}

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPShapeBar::Create(
			CWnd*		pParentWnd,
			DWORD		dwStyle,
			UINT		nID,
			CPalette*	pPalette/* = NULL*/,
			int			nColumns/* = 0*/,
			int			nRowsDockHorz/* = 0*/,
			int			nColDockVert/* = 0*/)
{
/*	if (m_colors.GetSize () != NULL)
	{
		return CBCGPPopupMenuBar::Create (pParentWnd, dwStyle, nID);
	}*/

	m_nNumColumns = nColumns;
	m_nNumColumnsVert = nColDockVert;
	m_nNumRowsHorz = nRowsDockHorz;

	//InitColors (pPalette, m_colors);
	return CBCGPPopupMenuBar::Create (pParentWnd, dwStyle, nID);
}
//************************************************************************************
BOOL CBCGPShapeBar::CreateControl (
				CWnd*			pParentWnd,
				const CRect&	rect,
				UINT			nID,
				int				nColumns,
				CPalette*		pPalette/* = NULL*/
				)
{
	ASSERT_VALID (pParentWnd);

	EnableLargeIcons (FALSE);

	if (nColumns <= 0)
	{
		const int nColorsCount = (pPalette == NULL) ? 20 : pPalette->GetEntryCount ();
		ASSERT (nColorsCount > 0);

		// Optimal fill
		for (nColumns = nColorsCount; nColumns > 0; nColumns--)
		{
			int nCellSize = (rect.Width () - 2 * m_nHorzMargin) / nColumns;
			if (nCellSize == 0)
			{
				continue;
			}

			int nRows = nColorsCount / nColumns;
			if (nRows * nCellSize > rect.Height () - 2 * m_nVertMargin)
			{
				nColumns++;
				break;
			}
		}

		if (nColumns <= 0)
		{
			nColumns = -1;
		}
	}

	if (!Create (pParentWnd, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, nID, pPalette, nColumns))
	{
		return FALSE;
	}

	SetBarStyle (
		GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	CRect rectWnd = rect;
	MoveWindow (rectWnd);
	ContextToSize ();

	SetWindowPos (&wndTop, -1, -1, -1, -1,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	SetOwner (pParentWnd);
	SetCommandID (nID);

	// All commands will be routed via this dialog, not via the parent frame:
	SetRouteCommandsViaFrame (FALSE);
	return TRUE;
}
//*************************************************************************************
void CBCGPShapeBar::Serialize (CArchive& ar)
{
	CBCGPPopupMenuBar::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_nNumColumns;
		ar >> m_nNumRowsHorz;
		ar >> m_nNumColumnsVert;
		ar >> m_bIsTearOff;
		ar >> m_nCommandID;


		Rebuild ();
		AdjustLocations ();
	}
	else
	{
		ar << m_nNumColumns;
		ar << m_nNumRowsHorz;
		ar << m_nNumColumnsVert;
		ar << m_bIsTearOff;
		ar << m_nCommandID;

	}
}
//*************************************************************************************
void CBCGPShapeBar::ShowCommandMessageString (UINT /*uiCmdId*/)
{
	GetOwner()->SendMessage (WM_SETMESSAGESTRING,
		m_nCommandID == (UINT) -1 ? AFX_IDS_IDLEMESSAGE : (WPARAM) m_nCommandID);
}
//*************************************************************************************

//*************************************************************************************
void CBCGPShapeBar::SetShape (int color)
{
	if (m_nShapeSelected == color)
	{
		return;
	}

	m_nShapeSelected = color;

	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	BOOL bWasOtherColor = FALSE;

	m_iHighlighted = -1;
	int iButton = 0;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; iButton++)
	{
		CRect rectButton;

		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			continue;
		}

		CBCGPCShapeToolbarButton* pColorButton = DYNAMIC_DOWNCAST (CBCGPCShapeToolbarButton, pButton);
		if (pColorButton == NULL)
		{
			continue;
		}

		ASSERT_VALID (pColorButton);


		if (pColorButton->m_bHighlight)
		{
			pColorButton->m_bHighlight = FALSE;
			InvalidateRect (pColorButton->Rect ());
		}

		if (pColorButton->m_bIsAutomatic && color == (COLORREF)-1)
		{
			pColorButton->m_bHighlight = TRUE;
			m_iHighlighted = iButton;
			InvalidateRect (pColorButton->Rect ());
		}
		else if (pColorButton->m_nShape == color)
		{
			pColorButton->m_bHighlight = TRUE;
			m_iHighlighted = iButton;
			InvalidateRect (pColorButton->Rect ());
		}
	}

		Invalidate ();
	UpdateWindow ();
}
//***************************************************************************************
void CBCGPShapeBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!IsCustomizeMode () || m_bInternal)
	{
		CBCGPToolBar::OnMouseMove(nFlags, point);
	}
}
//***************************************************************************************
void CBCGPShapeBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (IsCustomizeMode () && !m_bInternal)
	{
		return;
	}

	if (HitTest (point) == -1)
	{
		CBCGPToolBar::OnLButtonDown(nFlags, point);
	}
}
//***************************************************************************************
void CBCGPShapeBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!IsCustomizeMode () || m_bInternal)
	{
		int iHit = HitTest (point);
		if (iHit >= 0)
		{
			m_iButtonCapture = iHit;
		}

		CBCGPToolBar::OnLButtonUp(nFlags, point);
	}
}
//***************************************************************************************
void CBCGPShapeBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!IsCustomizeMode () || m_bInternal)
	{
		CBCGPToolBar::OnLButtonDblClk(nFlags, point);
	}
}
//***************************************************************************************
BOOL CBCGPShapeBar::PreTranslateMessage(MSG* pMsg)
{
	if ((m_pParentBtn != NULL /*|| m_pWndPropList != NULL*/) && !m_bInCommand)
	{
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
				CRect rect;
				GetClientRect (rect);

				CPoint pt (BCG_GET_X_LPARAM (pMsg->lParam), BCG_GET_Y_LPARAM (pMsg->lParam));
				if (!rect.PtInRect (pt))
				{
					GetParent ()->SendMessage (WM_CLOSE);
					return TRUE;
				}
			}
			break;

		case WM_SYSKEYDOWN:
		case WM_CONTEXTMENU:
			GetParent ()->SendMessage (WM_CLOSE);
			return TRUE;

		case WM_KEYDOWN:
			if (pMsg->wParam == VK_ESCAPE)
			{
				GetParent ()->SendMessage (WM_CLOSE);
				return TRUE;
			}
		}
	}

	return CBCGPPopupMenuBar::PreTranslateMessage(pMsg);
}
//*************************************************************************************
void CBCGPShapeBar::OnDestroy() 
{
	if (m_pParentBtn != NULL)
	{
		m_pParentBtn->m_pPopup = NULL;
		m_pParentBtn->SetFocus ();
	}
	/*else if (m_pWndPropList != NULL)
	{
		m_pWndPropList->CloseColorPopup ();
		m_pWndPropList->SetFocus ();
	}*/

	CBCGPPopupMenuBar::OnDestroy();
}
//****************************************************************************************
BOOL CBCGPShapeBar::OnKey(UINT nChar)
{
	POSITION posSel = 
		(m_iHighlighted < 0) ? NULL : m_Buttons.FindIndex (m_iHighlighted);
	CBCGPToolbarButton* pSelButton = 
		(posSel == NULL) ? NULL : (CBCGPToolbarButton*) m_Buttons.GetAt (posSel);

	switch (nChar)
	{
	case VK_RETURN:
		if (pSelButton != NULL)
		{
			GetOwner()->SendMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
			OnSendCommand (pSelButton);
			return TRUE;
		}
		break;
	}

	return CBCGPPopupMenuBar::OnKey (nChar);
}
//****************************************************************************************
afx_msg LRESULT CBCGPShapeBar::OnMouseLeave(WPARAM wp,LPARAM lp)
{
	if (m_pParentBtn == NULL /*&& m_pWndPropList == NULL*/)
	{
		return CBCGPToolBar::OnMouseLeave (wp, lp);
	}

	if (m_hookMouseHelp != NULL || 
		(m_bMenuMode && !IsCustomizeMode () && GetDroppedDownMenu () != NULL))
	{
		return 0;
	}

	m_bTracked = FALSE;
	m_ptLastMouse = CPoint (-1, -1);

	if (m_iHighlighted >= 0)
	{
		int iButton = m_iHighlighted;
		m_iHighlighted = -1;

		OnChangeHot (m_iHighlighted);

		InvalidateButton (iButton);
		UpdateWindow(); // immediate feedback

		GetOwner()->SendMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	}

	return 0;
}

//****************************************************************************************
void CBCGPShapeBar::SetVertMargin (int nVertMargin)
{
	ASSERT_VALID (this);

	m_nVertMargin = nVertMargin;
	AdjustLayout ();
}
//*****************************************************************************************
void CBCGPShapeBar::SetHorzMargin (int nHorzMargin)
{
	ASSERT_VALID (this);

	m_nHorzMargin = nHorzMargin;
	AdjustLayout ();
}