// BCGPShapeMenuButton.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "BCGPShapeMenuButton.h"
#include "BCGPShapePopup.h"
#include "objects/Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMap<UINT,UINT,CPoint, CPoint> CBCGPShapeMenuButton::m_ShapesByID;

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeMenuButton

IMPLEMENT_SERIAL(CBCGPShapeMenuButton, CBCGPToolbarMenuButton, VERSIONABLE_SCHEMA | 1)


CBCGPShapeMenuButton::CBCGPShapeMenuButton()
{
	SetShape(0, 0);
}

CBCGPShapeMenuButton::CBCGPShapeMenuButton(UINT uiCmdID,int type,int shape):
	CBCGPToolbarMenuButton (uiCmdID, NULL,
		CImageHash::GetImageOfCommand (uiCmdID, FALSE), NULL)
{
	SetShape(type, shape);

	CString strName;
	strName.LoadString(uiCmdID);
	int nfind = strName.Find('\n',0);
	if (nfind!=-1) strName.Delete(0,nfind+1);
	m_strText = strName;

}

CBCGPShapeMenuButton::~CBCGPShapeMenuButton()
{
}

//**************************************************************************************
CBCGPPopupMenu* CBCGPShapeMenuButton::CreatePopupMenu ()
{
	return new CBCGPShapePopup (NULL,m_nShapeType,m_nShape,2);
}

void CBCGPShapeMenuButton::SetShape(int type, int index/* automatic*/)
{
	m_ShapesByID.SetAt (m_nID, CPoint(type,index));
	m_nShape = index;
	m_nShapeType= type;
		if (m_pWndParent->GetSafeHwnd () != NULL)
	{
		m_pWndParent->InvalidateRect (m_rect);
	}

}

int CBCGPShapeMenuButton::GetShape () const
{
	return m_nShape;
}

//*****************************************************************************************
SIZE CBCGPShapeMenuButton::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	CSize cs = CBCGPToolbarMenuButton::OnCalculateSize (pDC, sizeDefault, bHorz);
	return cs+CSize(cs.cx,0);
}

CPoint CBCGPShapeMenuButton::GetShapeByCmdID (UINT uiCmdID)
{
	CPoint shape(-1,-1);
	m_ShapesByID.Lookup (uiCmdID, shape);

	return shape;
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeMenuButton message handlers

//**********************************************************************************************
int CBCGPShapeMenuButton::OnDrawOnCustomizeList (CDC* pDC, const CRect& rect, BOOL bSelected)
{
	int nID = m_nID;
	m_nID = 0;	// Force draw right arrow

	CRect rectColor = rect;
	rectColor.DeflateRect (1, 0);

	int iRes = CBCGPToolbarMenuButton::OnDrawOnCustomizeList (pDC, rect, bSelected);

	m_nID = nID;
	
	return iRes;
}

void CBCGPShapeMenuButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
			BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
			BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	if (m_bMenuMode)
	{
		DrawMenuItem (pDC, rect, pImages, bCustomizeMode, bHighlight, bGrayDisabledButtons);
		return;
	}

	bool bIsChecked = false;

	if (m_bMenuPaletteMode)
	{
		m_nStyle &= ~TBBS_CHECKED;
	}

	//----------------------
	// Fill button interior:
	//----------------------
	FillInterior (pDC, rect, bHighlight || IsDroppedDown ());

	CSize sizeImage = CMenuImages::Size ();
	if (CBCGPToolBar::IsLargeIcons ())
	{
		sizeImage.cx *= 2;
		sizeImage.cy *= 2;
	}

	CRect rectInternal = rect;
	CSize sizeExtra = m_bExtraSize ? 
		CBCGPVisualManager::GetInstance ()->GetButtonExtraBorder () : CSize (0, 0);

	if (sizeExtra != CSize (0, 0))
	{
		rectInternal.DeflateRect (sizeExtra.cx / 2 + 1, sizeExtra.cy / 2 + 1);
	}

	CRect rectParent = rect;
	CRect rectArrow = rectInternal;

	const int nMargin = CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
	const int xMargin = bHorz ? nMargin : 0;
	const int yMargin = bHorz ? 0 : nMargin;

	rectParent.DeflateRect (xMargin, yMargin);

	if (m_bDrawDownArrow)
	{
		if (bHorz)
		{
			rectParent.right -= sizeImage.cx + 2;
			rectArrow.left = rectParent.right - 1;
			rectArrow.OffsetRect (-sizeExtra.cx / 2, -sizeExtra.cy / 2);
		}
		else
		{
			rectParent.bottom -= sizeImage.cy + 2;
			rectArrow.top = rectParent.bottom - 1;
		}
	}

	UINT uiStyle = m_nStyle;

	if (CBCGPVisualManager::GetInstance ()->IsMenuFlatLook ())
	{
		m_nStyle &= ~(TBBS_PRESSED | TBBS_CHECKED);
	}
	else
	{
		if (m_bClickedOnMenu && m_nID != 0 && m_nID != (UINT) -1 && !m_bMenuOnly) 
		{
			m_nStyle &= ~TBBS_PRESSED;
		}
		else if (m_pPopupMenu != NULL)
		{
			m_nStyle |= TBBS_PRESSED;
		}
	}

	BOOL bDisableFill = m_bDisableFill;
	m_bDisableFill = TRUE;

	BOOL bDisabled = (bCustomizeMode && !IsEditable ()) ||
		(!bCustomizeMode && (m_nStyle & TBBS_DISABLED));

	if (bCustomizeMode)
		CBCGPToolbarButton::OnDraw (pDC, rectParent, pImages, bHorz, 
			bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
	else
	{
		rectParent.DeflateRect (4, 3,4,3);
		CShape::DrawShapes(m_nShapeType,pDC,rectParent,m_nShape,!bDisabled);
	}

	m_bDisableFill = bDisableFill;

	if (m_bDrawDownArrow)
	{
		if ((m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) &&
			!CBCGPVisualManager::GetInstance ()->IsMenuFlatLook ())
		{
			rectArrow.OffsetRect (1, 1);
		}

		if ((bHighlight || (m_nStyle & TBBS_PRESSED) ||
			m_pPopupMenu != NULL) &&
			m_nID != 0 && m_nID != (UINT) -1 && !m_bMenuOnly)	//JRG
		{
			//----------------
			// Draw separator:
			//----------------
			CRect rectSeparator = rectArrow;

			if (bHorz)
			{
				rectSeparator.right = rectSeparator.left + 2;
			}
			else
			{
				rectSeparator.bottom = rectSeparator.top + 2;
			}

			CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

			if (bHighlight || (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)))
			{
				//-----------------------
				// Pressed in or checked:
				//-----------------------
				state = CBCGPVisualManager::ButtonsIsPressed;
			}

			CBCGPVisualManager::GetInstance ()->OnDrawButtonSeparator (pDC,
											this, rectSeparator, state, bHorz);
		}

		int iImage;
		if (bHorz && !m_bMenuOnly)
		{
			iImage = CMenuImages::IdArowDown;
		}
		else
		{
			iImage = CMenuImages::IdArowRight;
		}

		if (m_pPopupMenu != NULL &&
			(m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) == 0 &&
			!CBCGPVisualManager::GetInstance ()->IsMenuFlatLook ())
		{
			rectArrow.OffsetRect (1, 1);
		}

		CPoint pointImage (
			rectArrow.left + (rectArrow.Width () - sizeImage.cx) / 2,
			rectArrow.top + (rectArrow.Height () - sizeImage.cy) / 2);

		CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, pointImage,
							bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack,
							sizeImage);
	}

	m_nStyle = uiStyle;

	if (!bCustomizeMode)
	{
		if ((m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) ||
			m_pPopupMenu != NULL)
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			if (!CBCGPVisualManager::GetInstance ()->IsMenuFlatLook () &&
				m_bClickedOnMenu && m_nID != 0 && m_nID != (UINT) -1 && !m_bMenuOnly) //JRG
			{
				CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
					this, rectParent, CBCGPVisualManager::ButtonsIsHighlighted);

				rectArrow.right --;
				rectArrow.bottom --;

				CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
					this, rectArrow, CBCGPVisualManager::ButtonsIsPressed);
			}
			else
			{
				CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
					this, rect, CBCGPVisualManager::ButtonsIsPressed);
			}
		}
		else if (bHighlight && !(m_nStyle & TBBS_DISABLED) &&
			!(m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
		{
			CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
				this, rect, CBCGPVisualManager::ButtonsIsHighlighted);
		}
	}

	if (bIsChecked)
	{
		m_nStyle |= TBBS_CHECKED;
	}
}

//****************************************************************************************
void CBCGPShapeMenuButton::OnChangeParentWnd (CWnd* pWndParent)
{
	CBCGPToolbarButton::OnChangeParentWnd (pWndParent);

	if (pWndParent != NULL)
	{
		if (pWndParent->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
		{
			m_bText = TRUE;
		}

		if (pWndParent->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
		{
			m_bMenuMode = TRUE;
			m_bText = TRUE;
		}
		else
		{
			m_bMenuMode = FALSE;
		}
	}

	m_bDrawDownArrow = TRUE;
	m_pWndParent = pWndParent;
}
