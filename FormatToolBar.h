#if !defined(AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_)
#define AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatToolBar.h : header file
//

class CObject3D;
/////////////////////////////////////////////////////////////////////////////
// CFormatToolBar window

class CFormatToolBar : public CBCGPToolBar
{
	DECLARE_SERIAL(CFormatToolBar)

// Construction
public:
	CFormatToolBar();

// Attributes
public:
	static CBCGPToolbarFontCombo*		CreateTextFontButton ();
	static CBCGPToolbarFontSizeCombo*	CreateTextFontSizeButton ();
	static CBCGPColorMenuButton*		CreateTextColorButton ();
	static CBCGPColorMenuButton*		CreateObjectColorButton ();
	static void OnUpdateCmdUI(CObject3D *pObj);

	static const CBCGPFontDesc* GetFontByCmdID (UINT uiCmdID);
	static int		GetFontSizeByCmdID (UINT uiCmdID);

private:
	static CPalette	m_palObjectColorPicker;	// Palette for color picker

// Operations
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormatToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormatToolBar();

	// Generated message map functions
protected:
	virtual void OnReset ();

	//{{AFX_MSG(CFormatToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_)