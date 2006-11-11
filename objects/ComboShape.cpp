// ComboShape.cpp : implementation file
//

#include "stdafx.h"
#include "../calques3d.h"
#include "ComboShape.h"
#include "Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CBCGShapeProp::CBCGShapeProp(const CString& strName, int index,int size,CShape::TShapeType type,LPCTSTR lpszDescr):
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

CComboBox* CBCGShapeProp::CreateCombo (CWnd* pWndParent, CRect rect)
{
	ASSERT_VALID (this);

	rect.bottom = rect.top + 400;

	CComboBox* pWndCombo = new CComboShape(m_type);
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST |CBS_HASSTRINGS|CBS_OWNERDRAWFIXED| WS_VSCROLL, 
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}


void CBCGShapeProp::OnDrawValue (CDC* pDC, CRect rect)
{
	CShape::DrawShapes(m_type,pDC,rect,(long)GetValue());
}

CWnd* CBCGShapeProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CEdit* pWndEdit = new CEdit;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_READONLY;
	pWndEdit->Create (dwStyle, CRect(0,0,0,0), m_pWndList, BCGPROPLIST_ID_INPLACE);
	bDefaultFormat = TRUE;
	return pWndEdit;
}


/////////////////////////////////////////////////////////////////////////////
// CComboShape

CComboShape::CComboShape()
{
	m_type = CShape::PointShape;
}

CComboShape::CComboShape(CShape::TShapeType type)
{
	m_type = type;
}

CComboShape::~CComboShape()
{
}


BEGIN_MESSAGE_MAP(CComboShape, CComboBox)
	//{{AFX_MSG_MAP(CComboShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboShape message handlers

void CComboShape::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);
    
    CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect   rect    = lpDrawItemStruct->rcItem;
    UINT    state   = lpDrawItemStruct->itemState;

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) )
		pDC->FillRect(rect, &globalData.brHilite);
	else
		pDC->FillRect(rect, &globalData.brWindow);

	CShape::DrawShapes(m_type,pDC,rect,lpDrawItemStruct->itemID);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGObjectProp

CBCGObjectProp::CBCGObjectProp(const CString& strName, const CString& strHelp,int ndx,DWORD dwData):
	CBCGPProp(strName, _T(""),(LPCTSTR)strHelp,dwData)
{
	m_pImgList = NULL;
	m_bGroup = TRUE;
	m_nIcon = ndx;
}

void CBCGObjectProp::OnDrawExpandBox (CDC* pDC, CRect rectExpand)
{
	CRect rect = rectExpand;

	rect.DeflateRect(0,1,0,0);

	CWnd* pWndParent = m_pWndList->GetParent ();
	BOOL m_bbControlBarColors = (pWndParent == NULL ||
								!pWndParent->IsKindOf (RUNTIME_CLASS (CDialog)));

	{
		CRect rectFill = rect;

		CWnd* pWnd = m_pWndList->GetFocus();

		if (!IsSelected ())
		{
			pDC->FillRect (rectFill, &globalData.brWindow);
		}
		else
		{
			if (pWnd!=m_pWndList)
			{
				pDC->FillRect (rectFill, 
					m_bbControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
			}
			else
			{
				pDC->FillRect (rectFill, &globalData.brHilite);
			}
		}
	}

	if (m_pImgList)
		m_pImgList->Draw(pDC,m_nIcon,CPoint(rectExpand.TopLeft())+CSize(1,1),ILD_TRANSPARENT);

}

void CBCGObjectProp::OnDrawName (CDC* pDC, CRect rect)
{
	COLORREF clrTextOld = (COLORREF)-1;

	CWnd* pWndParent = m_pWndList->GetParent ();
	BOOL m_bbControlBarColors = pWndParent == NULL ||
		!pWndParent->IsKindOf (RUNTIME_CLASS (CDialog));

/*	CWnd* pWnd = m_pWndList->GetFocus();
		CRect rectFill = rect;
		rectFill.top++;

		if (pWnd!=m_pWndList)
		{
			clrTextOld = pDC->SetTextColor (globalData.clrBtnText);
			pDC->FillRect (rectFill, 
				m_bbControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (globalData.clrTextHilite);
			pDC->FillRect (rectFill, &globalData.brHilite);
		}*/

//	if (IsSelected () && (!m_pWndList->IsVSDotNetLook()))
	{
		CRect rectFill = rect;
		rectFill.top++;

		CWnd* pWnd = m_pWndList->GetFocus();

		if (!IsSelected ())
		{
			//clrTextOld = pDC->SetTextColor (globalData.clrBtnText);
			pDC->FillRect (rectFill, 
				m_bbControlBarColors ? &globalData.brWindow : &globalData.brWindow);
				//m_bbControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
		}
		else
		{
			if (pWnd!=m_pWndList)
			{
				clrTextOld = pDC->SetTextColor (globalData.clrBtnText);
				pDC->FillRect (rectFill, 
					m_bbControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
			}
			else
			{
				clrTextOld = pDC->SetTextColor (globalData.clrTextHilite);
				pDC->FillRect (rectFill, &globalData.brHilite);
			}
		}
	}

	CFont* pFont = CFont::FromHandle ((HFONT) ::GetStockObject (DEFAULT_GUI_FONT));
	CFont* hOldFont = pDC->SelectObject(pFont);

		rect.DeflateRect (TEXT_MARGIN, 0);
	int nTextHeight = pDC->DrawText (m_strName, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	pDC->SelectObject(hOldFont);

	m_bNameIsTrancated = pDC->GetTextExtent (m_strName).cx > rect.Width ();

/*	if (IsSelected () && m_pWndList->IsVSDotNetLook() )
	{
		CRect rectFocus = rect;
		rectFocus.top = rectFocus.CenterPoint ().y - nTextHeight / 2;
		rectFocus.bottom = rectFocus.top + nTextHeight;
		rectFocus.right = 
			min (rect.right, rectFocus.left + pDC->GetTextExtent (m_strName).cx);
		rectFocus.InflateRect (2, 0);

		COLORREF clrShadow = m_bbControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;

		pDC->Draw3dRect (rectFocus, clrShadow, clrShadow);
	}*/

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}
}

BOOL CBCGObjectProp::OnDblClick (CPoint point)
{
	AfxMessageBox("fdfdfddf dd f");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl

CPropSliderCtrl::CPropSliderCtrl(CBCGSliderProp* pProp, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush (m_clrBack);
	m_pProp = pProp;
}

CPropSliderCtrl::~CPropSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPropSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CPropSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl message handlers

HBRUSH CPropSliderCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor (m_clrBack);
	return m_brBackground;
}
//************************************************************************************
void CPropSliderCtrl::HScroll (UINT nSBCode, UINT nPos)
{
	ASSERT_VALID (m_pProp);

	m_pProp->OnUpdateValue ();
	m_pProp->Redraw ();
}

////////////////////////////////////////////////////////////////////////////////
// CSliderProp class

CBCGSliderProp::CBCGSliderProp(const CString& strName, long nValue, 
							 LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, nValue, lpszDescr, dwData)
{
}
//************************************************************************************
CWnd* CBCGSliderProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropSliderCtrl* pWndSlider = new CPropSliderCtrl (this, globalData.clrWindow);

	rectEdit.left += rectEdit.Height () + 5;

	pWndSlider->Create (WS_VISIBLE | WS_CHILD, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	pWndSlider->SetPos ((long) m_varValue);

	bDefaultFormat = TRUE;
	return pWndSlider;
}
//******************************************************************************************
BOOL CBCGSliderProp::OnUpdateValue ()
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