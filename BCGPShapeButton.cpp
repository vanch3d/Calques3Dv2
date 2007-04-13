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
// BCGPShapeButton.cpp: implementation of the CBCGPShapeButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPShapeButton.h"
#include "BCGPShapeBar.h"
#include "BCGPShapePopup.h"
#include "objects/Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int nImageHorzMargin = 8;

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeButton

IMPLEMENT_DYNAMIC(CBCGPShapeButton, CBCGPButton)

CBCGPShapeButton::CBCGPShapeButton()
{
	//m_Color = RGB (0, 0, 0);
	//m_ColorAutomatic = (COLORREF)-1;
	m_nColumns = -1;
	m_pPopup = NULL;
	//m_bAltColorDlg = TRUE;
	//m_pPalette = NULL;
	m_bEnabledInCustomizeMode = FALSE;
	m_bAutoSetFocus = TRUE;
}

CBCGPShapeButton::~CBCGPShapeButton()
{
/*	if (m_pPalette != NULL)
	{
		delete m_pPalette;
	}*/
}


BEGIN_MESSAGE_MAP(CBCGPShapeButton, CBCGPButton)
	//{{AFX_MSG_MAP(CBCGPShapeButton)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeButton message handlers

CSize CBCGPShapeButton::SizeToContent (BOOL bCalcOnly)
{
	CSize size = CBCGPButton::SizeToContent (FALSE);
	size.cx += CMenuImages::Size ().cx;

	if (!bCalcOnly)
	{
		SetWindowPos (NULL, -1, -1, size.cx, size.cy,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	return size;
}
//*****************************************************************************************
void CBCGPShapeButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	if (!IsDrawXPTheme ())
	{
		CBCGPButton::OnFillBackground (pDC, rectClient);
		return;
	}

	ASSERT_VALID (pDC);
	pDC->FillRect (rectClient, &globalData.brWindow);
}
//*****************************************************************************************
void CBCGPShapeButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	ASSERT_VALID (pDC);

	/*if (m_pPalette == NULL)
	{
		RebuildPalette (NULL);
	}*/

	//CPalette* pCurPalette = pDC->SelectPalette (m_pPalette, FALSE);
	//pDC->RealizePalette();

	CSize sizeArrow = CMenuImages::Size ();

	CRect rectColor = rect;
	rectColor.right -= sizeArrow.cx + nImageHorzMargin;

	CRect rectArrow = rect;
	rectArrow.left = rectColor.right;

	COLORREF color = RGB(255,0,0);
/*	if (color == (COLORREF) -1)	// Automatic
	{
		//---------------------------
		// Draw automatic text label:
		//---------------------------
		color = m_ColorAutomatic;
		
		if (!m_strAutoColorText.IsEmpty ())
		{
			rectColor.right = rectColor.left + rectColor.Height ();

			CRect rectText = rect;
			rectText.left = rectColor.right;
			rectText.right = rectArrow.left;

			CFont* pOldFont = SelectFont (pDC);
			ASSERT(pOldFont != NULL);

			pDC->SetBkMode (TRANSPARENT);
			pDC->SetTextColor (globalData.clrBtnText);

			pDC->DrawText (m_strAutoColorText, rectText,
							DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

			pDC->SelectObject (pOldFont);
		}
	}*/

	//----------------
	// Draw color box:
	//----------------
	rectColor.DeflateRect (2, 2);
	pDC->Draw3dRect (rectColor, globalData.clrBtnHilite, globalData.clrBtnHilite);
	rectColor.DeflateRect (1, 1);
	pDC->Draw3dRect (rectColor, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
	rectColor.DeflateRect (1, -1);
	//rectColor.DeflateRect (1, 1);

	if (color != (COLORREF)-1 && (uiState & ODS_DISABLED) == 0)
	{
		/*if (globalData.m_nBitsPerPixel == 8) // 256 colors
		{
			ASSERT_VALID (m_pPalette);
			color =  PALETTEINDEX (m_pPalette->GetNearestPaletteIndex (color));
		}*/

		CBrush br (color);
		//pDC->FillRect (rectColor, &br);
		CShape::DrawShapes(m_nShapeType,pDC,rectColor,m_nShape,TRUE);

	}

	//----------------------
	// Draw drop-down arrow:
	//----------------------
	CRect rectArrowWinXP = rectArrow;
	rectArrowWinXP.DeflateRect (2, 2);

	if (!m_bWinXPTheme || !CBCGPVisualManager::GetInstance ()->DrawComboDropButtonWinXP (
									pDC, rectArrowWinXP,
									(uiState & ODS_DISABLED), m_bPushed,
									m_bHighlighted))
	{
		pDC->FillRect (rectArrow, &globalData.brBtnFace);

		CPoint pointImage (
			rectArrow.left + (rectArrow.Width () - sizeArrow.cx) / 2,
			rectArrow.top + (rectArrow.Height () - sizeArrow.cy) / 2);

		CMenuImages::Draw (pDC, CMenuImages::IdArowDownLarge, pointImage,
			(uiState & ODS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);

		pDC->Draw3dRect (rectArrow, globalData.clrBtnLight, globalData.clrBtnDkShadow);
		rectArrow.DeflateRect (1, 1);
		pDC->Draw3dRect (rectArrow, globalData.clrBtnHilite, globalData.clrBtnShadow);
	}

	//if (pCurPalette != NULL)
	//{
	//	pDC->SelectPalette (pCurPalette, FALSE);
	//}
}
//*****************************************************************************************
void CBCGPShapeButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT /*uiState*/)
{
	ASSERT_VALID (pDC);
	ASSERT (m_nFlatStyle != BUTTONSTYLE_NOBORDERS);	// Always has borders

	if (!m_bWinXPTheme || !CBCGPVisualManager::GetInstance ()->DrawComboBorderWinXP (
			pDC, rectClient, !IsWindowEnabled (), FALSE, TRUE))
	{
		pDC->Draw3dRect (rectClient,
					globalData.clrBtnDkShadow, globalData.clrBtnHilite);

		rectClient.DeflateRect (1, 1);

		if (m_nFlatStyle == BUTTONSTYLE_3D || m_bHighlighted)
		{
			pDC->Draw3dRect (rectClient,
						globalData.clrBtnShadow, globalData.clrBtnLight);
		}
	}
}
//*****************************************************************************************
void CBCGPShapeButton::OnDrawFocusRect (CDC* pDC, const CRect& rectClient)
{
	CSize sizeArrow = CMenuImages::Size ();

	CRect rectColor = rectClient;
	rectColor.right -= sizeArrow.cx + nImageHorzMargin;

	CBCGPButton::OnDrawFocusRect (pDC, rectColor);
}
//*****************************************************************************************
void CBCGPShapeButton::OnShowColorPopup () 
{
	if (m_pPopup != NULL)
	{
		m_pPopup->SendMessage (WM_CLOSE);
		m_pPopup = NULL;
		return;
	}

	m_pPopup = new CBCGPShapePopup (this,m_nShapeType,m_nShape,m_nColumns);
	m_pPopup->m_bEnabledInCustomizeMode = m_bEnabledInCustomizeMode;

	CRect rectWindow;
	GetWindowRect (rectWindow);

	if (!m_pPopup->Create (this, rectWindow.left, rectWindow.bottom, NULL,
		m_bEnabledInCustomizeMode))
	{
		ASSERT (FALSE);
		m_pPopup = NULL;

		TRACE(_T ("Color menu can't be used in the customization mode. You need to set CBCGPShapeButton::m_bEnabledInCustomizeMode\n"));
	}
	else
	{
		if (m_bEnabledInCustomizeMode)
		{
			CBCGPShapeBar* pColorBar = DYNAMIC_DOWNCAST (
				CBCGPShapeBar, m_pPopup->GetMenuBar());

			if (pColorBar != NULL)
			{
				ASSERT_VALID (pColorBar);
				pColorBar->m_bInternal = TRUE;
			}
		}

		CRect rect;
		m_pPopup->GetWindowRect (&rect);
		m_pPopup->UpdateShadow (&rect);

		if (m_bAutoSetFocus)
		{
			m_pPopup->GetMenuBar()->SetFocus ();
		}
	}

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}
}
//*****************************************************************************************
void CBCGPShapeButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE || nChar == VK_DOWN)
	{
		OnShowColorPopup ();
		return;
	}
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
//*****************************************************************************************
void CBCGPShapeButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
	SetFocus ();
	OnShowColorPopup ();
}
//****************************************************************************************
void CBCGPShapeButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	FlatStyle nFlatStyle = m_nFlatStyle;
	if (IsDrawXPTheme ())
	{
		m_nFlatStyle = BUTTONSTYLE_SEMIFLAT;
	}

	CBCGPButton::OnMouseMove(nFlags, point);
	m_nFlatStyle = nFlatStyle;
}
//*****************************************************************************************
UINT CBCGPShapeButton::OnGetDlgCode() 
{
	return DLGC_WANTARROWS;
}
//*****************************************************************************************
/*void CBCGPShapeButton::EnableAutomaticButton (LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable)
{
	m_strAutoColorText = (bEnable && lpszLabel == NULL) ? _T("") : lpszLabel;
	m_ColorAutomatic = colorAutomatic;
}
//*****************************************************************************************
void CBCGPShapeButton::EnableOtherButton (LPCTSTR lpszLabel, BOOL bAltColorDlg, BOOL bEnable)
{
	m_strOtherText = (bEnable && lpszLabel == NULL) ? _T("") : lpszLabel;
	m_bAltColorDlg = bAltColorDlg;
}
//*****************************************************************************************
void CBCGPShapeButton::SetDocumentColors (LPCTSTR lpszLabel, CList<COLORREF,COLORREF>& lstColors)
{
	m_lstDocColors.RemoveAll ();
	m_strDocColorsText = (lpszLabel == NULL) ? _T("") : lpszLabel;

	if (!m_strDocColorsText.IsEmpty ())
	{
		m_lstDocColors.AddTail (&lstColors);
	}
}*/
//*****************************************************************************************
/*void CBCGPShapeButton::SetPalette (CPalette* pPalette)
{
	if (m_Colors.GetSize () != 0)
	{
		m_Colors.SetSize (0);
		m_Colors.FreeExtra ();
	}

	CBCGPShapeBar::InitColors (pPalette, m_Colors);
	RebuildPalette (pPalette);
}*/
//*****************************************************************************************
void CBCGPShapeButton::SetShape(int type, int index/* automatic*/)
//void CBCGPShapeButton::SetColor (COLORREF color /* -1 - automatic*/)
{
	m_nShape = index;
	m_nShapeType= type;
	if (GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*****************************************************************************************
void CBCGPShapeButton::UpdateShape (int color)
{
	SetShape(m_nShapeType,color);

		//-------------------------------------------------------
	// Trigger mouse up event (to button click notification):
	//-------------------------------------------------------
	CWnd* pParent = GetParent ();
	if (pParent != NULL)
	{
		pParent->SendMessage (	WM_COMMAND,
								MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
								(LPARAM) m_hWnd);
	}
}

int CBCGPShapeButton::GetShape () const
{
	return m_nShape;
}

//****************************************************************************************
/*void CBCGPShapeButton::RebuildPalette (CPalette* pPal)
{
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
	}

	m_pPalette = new CPalette ();

	// Create palette:
	CClientDC dc (this);

	if (pPal == NULL)
	{
		int nColors = 256;	// Use 256 first entries
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		::GetSystemPaletteEntries (dc.GetSafeHdc (), 0, nColors, pLP->palPalEntry);

		pLP->palVersion = 0x300;
		pLP->palNumEntries = (USHORT) nColors;

		m_pPalette->CreatePalette (pLP);

		delete[] pLP;
	}
	else
	{
		ASSERT_VALID (pPal);
		int nColors = pPal->GetEntryCount ();
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pPal->GetPaletteEntries (0, nColors, pLP->palPalEntry);

		pLP->palVersion = 0x300;
		pLP->palNumEntries = (USHORT) nColors;

		m_pPalette->CreatePalette (pLP);

		delete[] pLP;
	}
}*/
//*****************************************************************************************
void CBCGPShapeButton::OnSysColorChange() 
{
	CBCGPButton::OnSysColorChange();
	//RebuildPalette (NULL);

	Invalidate ();
	UpdateWindow ();
}
//*****************************************************************************************
BOOL CBCGPShapeButton::IsDrawXPTheme () const
{
	return m_bWinXPTheme &&
		CBCGPVisualManager::GetInstance ()->IsWinXPThemeSupported ();
}

