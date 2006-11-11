// PropertyBar.h: interface for the CPropertyBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_)
#define AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of CBCGPropList in order to correct a few bugs.
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPropListEx : public CBCGPPropList
{
protected:
	//{{AFX_MSG(CBCGPropListEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


class CObject3D;

/////////////////////////////////////////////////////////////////////////////
/// CPropertyBar
///
/////////////////////////////////////////////////////////////////////////////
class CPropertyBar : public CBCGPDockingControlBar
{
public:
	CPropertyBar();

// Attributes
private:
	CBCGPropListEx	m_wndProp;			///<
	CPalette		m_palColorPicker;	///<
	CImageList		m_pImgList;			///<
	CObject3D*		m_pSelObj;			///< Current selected object

// Operations
protected:
	CBCGPProp* AddAppearance(CObject3D* pObj);

public:
    void EnableBar(BOOL bOpen=TRUE);
	void AddProperties();
	void AddProperties(CObject3D* pObj);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertyBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_)
