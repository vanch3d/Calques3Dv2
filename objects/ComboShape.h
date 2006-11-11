#if !defined(AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_)
#define AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboShape.h : header file
//

#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////
/// CComboShape
///
/////////////////////////////////////////////////////////////////////////////
class CComboShape : public CComboBox
{
// Construction
public:
	CComboShape();
	CComboShape(CShape::TShapeType type);

// Attributes
private:
	CShape::TShapeType	m_type;		///< Type of the shape widget

// Operations
public:
	void SetType(CShape::TShapeType type) {m_type = type;};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboShape)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboShape();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGShapeProp
///
/////////////////////////////////////////////////////////////////////////////
class CBCGShapeProp : public CBCGPProp
{
public:
	CBCGShapeProp(	const CString& strName, int index = 0, int size = 0,
					CShape::TShapeType type=CShape::PointShape,
					LPCTSTR lpszDescr = NULL);

// Attributes
private:
	CShape::TShapeType	m_type;		///< Type of the shape widget

// Overrides
public:
	virtual void OnDrawValue (CDC* pDC, CRect rect);
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, CRect rect);

};

/////////////////////////////////////////////////////////////////////////////
/// CBCGPointProp
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPointProp : public CBCGShapeProp
{
public:
	CBCGPointProp(const CString& strName, int index = 0, LPCTSTR lpszDescr = NULL):
		CBCGShapeProp (strName, index, 6, CShape::PointShape,lpszDescr) 
		{};
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGLineProp
///
/////////////////////////////////////////////////////////////////////////////
class CBCGLineProp : public CBCGShapeProp
{
public:
	CBCGLineProp(const CString& strName, int index = 0, LPCTSTR lpszDescr = NULL):
		CBCGShapeProp (strName, index, 8, CShape::LineShape,lpszDescr) 
		{		
		};
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGVolumeProp
///
/////////////////////////////////////////////////////////////////////////////
class CBCGVolumeProp : public CBCGShapeProp
{
public:
	CBCGVolumeProp(const CString& strName, int index = 0, LPCTSTR lpszDescr = NULL):
	  CBCGShapeProp (strName, index, 3, CShape::VolShape,lpszDescr) 
		{		
		};
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGObjectProp
///
/////////////////////////////////////////////////////////////////////////////
class CBCGObjectProp : public CBCGPProp
{
protected:
	CImageList*		m_pImgList;	///< A pointer to the image list containing the object icons
	int				m_nIcon;	///< Index of the icon to draw

public:
	CBCGObjectProp(const CString& strName, const CString& strHelp,int ndx,DWORD dwData = 0);
	CBCGObjectProp();


	virtual void OnDrawExpandBox (CDC* pDC, CRect rectExpand);
	virtual void OnDrawName (CDC* pDC, CRect rect);
	virtual BOOL OnEdit (LPPOINT lptClick) { return FALSE;};
	virtual BOOL OnSetCursor () const { return FALSE;};
	virtual BOOL OnDblClick (CPoint point);

	void SetImageList(CImageList* pImgList) { m_pImgList = pImgList;};
};


class CBCGSliderProp : public CBCGPProp
{
public:
	CBCGSliderProp(
		const CString& strName,
		long nValue,
		LPCTSTR lpszDescr = NULL,
		DWORD dwData = 0);

	virtual BOOL OnUpdateValue ();

protected:
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor () const	{	return FALSE; /* Use default */	}
};

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl window

class CPropSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CPropSliderCtrl(CBCGSliderProp* pProp, COLORREF clrBack);

// Attributes
protected:
	CBrush			m_brBackground;
	COLORREF		m_clrBack;
	CBCGSliderProp*	m_pProp;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void HScroll (UINT nSBCode, UINT nPos);

	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_)
