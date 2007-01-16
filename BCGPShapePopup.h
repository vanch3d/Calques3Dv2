//////////////////////////////////////////////////////////////////////
/// @file BCGPShapePopup.h
/// @brief interface for the CBCGPShapePopup class
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
#define AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "BCGPPopupMenu.h"
#include "BCGPShapeBar.h"

class CBCGPControlBar;
class CBCGPPropList;

/////////////////////////////////////////////////////////////////////////////
/// CBCGPShapePopup
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapePopup : public CBCGPPopupMenu
{
	friend class CBCGPShapeButton;
	//friend class CBCGPColorProp;
	//friend class CBCGPColorMenuButton;

	DECLARE_DYNAMIC(CBCGPShapePopup)

// Construction
public:
    CBCGPShapePopup (CBCGPShapeButton* pParentBtn,int type,int nShape,int nColumns)
		: m_wndColorBar (type, nShape, nColumns,-1, -1,(UINT)-1,pParentBtn)
	{
		m_bEnabledInCustomizeMode = FALSE;
	}

	virtual ~CBCGPShapePopup();

// Attributes
protected:
	CBCGPShapeBar	m_wndColorBar;
	BOOL			m_bEnabledInCustomizeMode;

protected:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBCGPShapePopup)
	//}}AFX_VIRTUAL

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndColorBar;
	}

	virtual CBCGPControlBar* CreateTearOffBar (CFrameWnd* pWndMain, UINT uiID, LPCTSTR lpszName);

    // Generated message map functions
protected:
    //{{AFX_MSG(CBCGPShapePopup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
