#if !defined(AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_)
#define AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPShapeMenuButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeMenuButton window

class CBCGPShapeMenuButton : public CBCGPToolbarMenuButton
{
	DECLARE_SERIAL(CBCGPShapeMenuButton)
// Construction
public:
	CBCGPShapeMenuButton();
	CBCGPShapeMenuButton(UINT uiCmdID,int type,int shape);

// Attributes
public:
	int		m_nShapeType;				///<
	int		m_nShape;					///<

	static CMap<UINT,UINT,CPoint, CPoint> m_ShapesByID;
	static CPoint GetShapeByCmdID (UINT uiCmdID);

// Operations
public:
	int GetShape() const;
	void SetShape(int type, int index =-1/* automatic*/);

// Implementation
public:
	virtual ~CBCGPShapeMenuButton();
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);

protected:
		virtual CBCGPPopupMenu* CreatePopupMenu ();
	virtual int OnDrawOnCustomizeList (CDC* pDC, const CRect& rect, BOOL bSelected);
	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_)
