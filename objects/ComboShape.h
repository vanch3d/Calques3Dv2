//////////////////////////////////////////////////////////////////////
/// @file ComboShape.h
/// @brief interface of various CProp widgets used in Calques 3D
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_)
#define AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////
/// CPropShapeCombo
///
/////////////////////////////////////////////////////////////////////////////
class CPropShapeCombo : public CComboBox
{
// Attributes
private:
	int	m_type;		///< Type of the shape widget

// Construction
public:
	CPropShapeCombo();
	CPropShapeCombo(int type);

// Operations
public:
	
	//////////////////////////////////////////////////////////////////////
	/// Set the type of shapes associated with the combobox.
	/// \param type	The type of the shape (see CShape::TShapeType)
	//////////////////////////////////////////////////////////////////////
	void SetType(int type) {m_type = type;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboShape)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// A property widget for object shapes.
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapeProp : public CBCGPProp
{
// Attributes
private:
	int	m_type;		///< Type of the shape widget

// Construction
public:
	CBCGPShapeProp(	const CString& strName, int index = 0, int size = 0,
					int type=CShape::PointShape,
					LPCTSTR lpszDescr = NULL);

// Overrides
public:
	virtual void OnDrawValue (CDC* pDC, CRect rect);
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, CRect rect);

};

/////////////////////////////////////////////////////////////////////////////
/// A property widget for CObject3D objects.
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPObjectProp : public CBCGPProp
{
// Attributes
protected:
	CImageList*		m_pImgList;	///< A pointer to the image list containing the object icons
	int				m_nIcon;	///< Index of the icon to draw

// Construction
public:
	CBCGPObjectProp(const CString& strName, const CString& strHelp,int ndx,BOOL bHasList,DWORD dwData = 0);
	//CBCGObjectProp();

// Overrides
public:
	virtual BOOL HasValueField () const
	{
		return FALSE;
	}
//	virtual void OnDrawExpandBox (CDC* pDC, CRect rectExpand);
	virtual void OnDrawName (CDC* pDC, CRect rect);
	virtual BOOL OnEdit (LPPOINT lptClick) { return FALSE;};
	virtual BOOL OnSetCursor () const { return FALSE;};
	virtual BOOL OnDblClick (CPoint point);

// Operations
public:
	void SetImageList(CImageList* pImgList) { m_pImgList = pImgList;};
};


/////////////////////////////////////////////////////////////////////////////
/// A property widget for integer values (with slider).
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPSliderProp : public CBCGPProp
{
// Construction
public:
	CBCGPSliderProp(
		const CString& strName,
		long nValue,
		LPCTSTR lpszDescr = NULL,
		DWORD dwData = 0);

// Overrides
public:
	virtual BOOL OnUpdateValue ();

protected:
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor () const	{	return FALSE; /* Use default */	}
};

/////////////////////////////////////////////////////////////////////////////
/// A property widget for boolean values (with check box).
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPCheckBoxProp : public CBCGPProp
{
// Attributes
protected:
	CRect	m_rectCheck;	///< The bounding rect of the check box

// Construction
public:
	CBCGPCheckBoxProp(
		const CString& strName,
		BOOL bCheck,
		LPCTSTR lpszDescr = NULL,
		DWORD dwData = 0);

// Overrides
protected:
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnEdit(LPPOINT lptClick)				{	return FALSE;	}
	virtual void OnDrawButton (CDC* pDC, CRect rectButton)	{}
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual BOOL HasButton () const						{	return FALSE;	}

	virtual BOOL PushChar (UINT nChar);
	virtual void OnDrawCheckBox (CDC * pDC, CRect rectCheck, BOOL bChecked);
	virtual void OnDrawName (CDC* pDC, CRect rect);
	virtual void OnClickName (CPoint point);
	virtual BOOL OnClickValue (UINT uiMsg, CPoint point);
	virtual BOOL OnDblClick (CPoint point);

};


/////////////////////////////////////////////////////////////////////////////
/// CPropSliderCtrl
///
/////////////////////////////////////////////////////////////////////////////
class CPropSliderCtrl : public CSliderCtrl
{
// Attributes
protected:
	CBrush				m_brBackground;		///< The background brush used for the slider
	COLORREF			m_clrBack;			///< The background color used for the slider
	CBCGPSliderProp*	m_pProp;			///< The property widget associated with this control

// Construction
public:
	CPropSliderCtrl(CBCGPSliderProp* pProp, COLORREF clrBack);

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void HScroll (UINT nSBCode, UINT nPos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOSHAPE_H__57010BB1_245D_430F_B746_9708A6E73E25__INCLUDED_)
