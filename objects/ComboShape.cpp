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
// ComboShape.cpp: implementation of the CBCGPShapeProp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../calques3d.h"
#include "ComboShape.h"
#include "Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//***************************************************************************
// CBCGPShapeProp
//***************************************************************************


/////////////////////////////////////////////////////////////////////////////
/// Constructor for the shape property widget
///	@param strName		The name of the property
///	@param index		The zero-based index of the selected shape
///	@param size			The number of shapes available
///	@param type			The type of the shape
///	@param lpszDescr	The description associated with the property widget
/////////////////////////////////////////////////////////////////////////////
CBCGPShapeProp::CBCGPShapeProp(const CString& strName, int index,int size,int type,LPCTSTR lpszDescr):
	CBCGPProp (strName, _variant_t((long)index), lpszDescr) 
{		
	m_type = type;
	for (int i=0;i<size;i++)
	{
		CString str;
		str.Format(_T("%d"),i);
		AddOption (str);
	}
	AllowEdit(FALSE);
};

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when it needs to create a combo box inside the property. 
///	@param pWndParent	The parent for the combo box 
///	@param rect			The bounding rectangle of the combo box
///	@return				A pointer to the newly created CComboBox object.
/////////////////////////////////////////////////////////////////////////////
CComboBox* CBCGPShapeProp::CreateCombo (CWnd* pWndParent, CRect rect)
{
	ASSERT_VALID (this);

	rect.bottom = rect.top + 400;

	CComboBox* pWndCombo = new CPropShapeCombo(m_type);
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST |CBS_HASSTRINGS|CBS_OWNERDRAWFIXED| WS_VSCROLL, 
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework to display the property value. 
///	@param pDC		A pointer to a device context.  
///	@param rect		The bounding rectangle of the property value.
/////////////////////////////////////////////////////////////////////////////
void CBCGPShapeProp::OnDrawValue (CDC* pDC, CRect rect)
{
	CShape::DrawShapes(m_type,pDC,rect,(long)GetValue(),TRUE);
}

/////////////////////////////////////////////////////////////////////////////
/// The function is called by the framework to create an in-place edit control for a property
///	@param rectEdit			The bounding rectangle of the edit control.  
///	@param bDefaultFormat	Specifies whether to use the default property format (TRUE) when 
///							setting the text to edit control, or not (FALSE).
///	@return					A valid pointer to edit control if success, or NULL if the property doesn't 
///							support in-place editing.
/////////////////////////////////////////////////////////////////////////////
CWnd* CBCGPShapeProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CEdit* pWndEdit = new CEdit;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_READONLY;
	pWndEdit->Create (dwStyle, CRect(0,0,0,0), m_pWndList, BCGPROPLIST_ID_INPLACE);
	bDefaultFormat = TRUE;
	return pWndEdit;
}


//***************************************************************************
// CComboShape
//***************************************************************************

BEGIN_MESSAGE_MAP(CPropShapeCombo, CComboBox)
	//{{AFX_MSG_MAP(CComboShape)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////
CPropShapeCombo::CPropShapeCombo()
{
	m_type = CShape::PointShape;
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////
CPropShapeCombo::CPropShapeCombo(int type)
{
	m_type = type;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when a visual aspect of an owner-draw combo box changes.
///	@param lpDrawItemStruct		A  pointer to a DRAWITEMSTRUCT structure that contains information 
///								about the type of drawing required.  
/////////////////////////////////////////////////////////////////////////////
void CPropShapeCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);
    
    CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect   rect    = lpDrawItemStruct->rcItem;
    UINT    state   = lpDrawItemStruct->itemState;

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) )
		pDC->FillRect(rect, &globalData.brHilite);
	else
		pDC->FillRect(rect, &globalData.brWindow);

	CShape::DrawShapes(m_type,pDC,rect,lpDrawItemStruct->itemID,TRUE);
}

//***************************************************************************
// CBCGPObjectProp
//***************************************************************************

/////////////////////////////////////////////////////////////////////////////
/// Constructor for the CObject3D property widget
///	@param strName		The name of the geometrical object
///	@param strHelp		The description of the geometrical object
///	@param ndx			The zero-based index of the object's icon
///	@param bHasList		TRUE if the object has parents, FALSE otherwise
///	@param dwData		The user defined data to be associated with the property. 
/////////////////////////////////////////////////////////////////////////////
CBCGPObjectProp::CBCGPObjectProp(const CString& strName, const CString& strHelp,
								 int ndx,BOOL bHasList,DWORD dwData):
	CBCGPProp(strName, _T(""),(LPCTSTR)strHelp,dwData)
{
	m_pImgList = NULL;
	m_bGroup = bHasList;
	m_nIcon = ndx;
	m_bIsValueList = bHasList;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework to display the property name.
///	@param pDC		A pointer to a device context.  
///	@param rect		The bounding rectangle of the property value.
/////////////////////////////////////////////////////////////////////////////
void CBCGPObjectProp::OnDrawName (CDC* pDC, CRect rect)
{
	IMAGEINFO pInfo;
	if (m_pImgList)
	{
		m_pImgList->GetImageInfo(m_nIcon,&pInfo);
		m_pImgList->Draw(pDC,m_nIcon,CPoint(rect.TopLeft())+CSize(1,1),ILD_TRANSPARENT);
		rect.left = rect.left + pInfo.rcImage.right-pInfo.rcImage.left+3;
	}
	CBCGPProp::OnDrawName (pDC, rect);
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user double clicks the property.
///	@param point	Specifies the point in the property list control client coordinates.  
///	@return			TRUE if the notification is processed by the property.
/////////////////////////////////////////////////////////////////////////////
BOOL CBCGPObjectProp::OnDblClick (CPoint point)
{
	//AfxMessageBox("fdfdfddf dd f");
	return TRUE;
}

//***************************************************************************
// CPropSliderCtrl
//***************************************************************************

BEGIN_MESSAGE_MAP(CPropSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CPropSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// Constructor for the CObject3D property widget
///	@param pProp		A pointer to the parent property
///	@param clrBack		The background colour
/////////////////////////////////////////////////////////////////////////////
CPropSliderCtrl::CPropSliderCtrl(CBCGPSliderProp* pProp, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush (m_clrBack);
	m_pProp = pProp;
}

HBRUSH CPropSliderCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor (m_clrBack);
	return m_brBackground;
}

void CPropSliderCtrl::HScroll (UINT nSBCode, UINT nPos)
{
	ASSERT_VALID (m_pProp);

	m_pProp->OnUpdateValue ();
	m_pProp->Redraw ();
}

//***************************************************************************
// CBCGPSliderProp
//***************************************************************************

/////////////////////////////////////////////////////////////////////////////
/// Constructor for the slider property widget
///	@param strName		The name of the geometrical object
///	@param strHelp		The description of the geometrical object
///	@param ndx			The zero-based index of the object's icon
///	@param bHasList		TRUE if the object has parents, FALSE otherwise
///	@param dwData		The user defined data to be associated with the property. 
/////////////////////////////////////////////////////////////////////////////
CBCGPSliderProp::CBCGPSliderProp(const CString& strName, long nValue, 
							 LPCTSTR lpszDescr, DWORD dwData,int min,int max)
	:	CBCGPProp (strName, nValue, lpszDescr, dwData)
{
	m_nMin =min;
	m_nMax =max;

}

CWnd* CBCGPSliderProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropSliderCtrl* pWndSlider = new CPropSliderCtrl (this, globalData.clrWindow);

	rectEdit.left += rectEdit.Height () + 5;
	rectEdit.bottom +=2;

	pWndSlider->Create (WS_VISIBLE | WS_CHILD| TBS_NOTICKS|TBS_BOTH, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	pWndSlider->SetPos ((long) m_varValue);
	pWndSlider->SetRange(m_nMin,m_nMax);

	bDefaultFormat = TRUE;
	return pWndSlider;
}

BOOL CBCGPSliderProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	long lCurrValue = m_varValue;

	CSliderCtrl* pSlider = (CSliderCtrl*) m_pWndInPlace;

	m_varValue = (long) pSlider->GetPos ();

	if (lCurrValue != (long) m_varValue)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}


//***************************************************************************
// CBCGPCheckBoxProp
//***************************************************************************

CBCGPCheckBoxProp::CBCGPCheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, _variant_t (bool (bCheck != 0)), lpszDescr, dwData)
{
	m_rectCheck.SetRectEmpty ();
}

void CBCGPCheckBoxProp::OnDrawName(CDC* pDC, CRect rect)
{
	CBCGPProp::OnDrawName (pDC, rect);
}

void CBCGPCheckBoxProp::OnDrawValue(CDC* pDC, CRect rect)
{
	m_rectCheck = rect;

	m_rectCheck.DeflateRect (1, 1);

	m_rectCheck.right = m_rectCheck.left + m_rectCheck.Height();

	rect.left = m_rectCheck.right + 1;

	CBCGPProp::OnDrawValue (pDC, rect);

	OnDrawCheckBox (pDC, m_rectCheck, bool (m_varValue));
}

CWnd* CBCGPCheckBoxProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CBCGPButton * pWndCheckBox = new CBCGPButton ();

	rectEdit.left = rectEdit.right ;

	pWndCheckBox->Create ("",WS_CHILD|BS_CHECKBOX, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
		pWndCheckBox->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
		pWndCheckBox->m_nAlignStyle = CBCGPButton::ALIGN_LEFT;
		pWndCheckBox->m_bDrawFocus = FALSE;
		pWndCheckBox->m_bResponseOnButtonDown = TRUE;
		pWndCheckBox->m_bTransparent = TRUE;
		pWndCheckBox->m_bDontUseWinXPTheme = TRUE;
			pWndCheckBox->SetCheck ((bool)m_varValue);

	bDefaultFormat = TRUE;
	return pWndCheckBox;
}

void CBCGPCheckBoxProp::OnClickName(CPoint point)
{
	CBCGPProp::OnClickName (point);
	if (m_bEnabled && m_rectCheck.PtInRect (point)) 
	{
		m_varValue = !bool(m_varValue);
		m_pWndList->InvalidateRect (GetRect());
	}
}

BOOL CBCGPCheckBoxProp::OnClickValue (UINT uiMsg, CPoint point)
{
	return CBCGPProp::OnClickValue (uiMsg, point);
}

BOOL CBCGPCheckBoxProp::OnDblClick (CPoint point)
{
	if (m_bEnabled && m_rectCheck.PtInRect (point)) 
	{
		//return TRUE;
	}

	m_varValue = !bool(m_varValue);
	m_pWndList->InvalidateRect (GetRect()	);
		m_pWndList->OnPropertyChanged (this);
	return TRUE;
}

void CBCGPCheckBoxProp::OnDrawCheckBox (CDC * pDC, CRect rect, BOOL bChecked)
{
	COLORREF clrTextOld = pDC->GetTextColor ();

	CBCGPVisualManager::GetInstance ()->OnDrawCheckBox (pDC, rect,
										 FALSE, bChecked, m_bEnabled);

	pDC->SetTextColor (clrTextOld);
}

BOOL CBCGPCheckBoxProp::PushChar (UINT nChar)
{
	if (nChar == VK_SPACE)
	{
		OnDblClick (CPoint (-1, -1));
	}

	return TRUE;
}