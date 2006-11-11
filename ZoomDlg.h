#if !defined(AFX_ZOOMDLG_H__92530682_94AF_11D5_A2F4_B80C2A89502F__INCLUDED_)
#define AFX_ZOOMDLG_H__92530682_94AF_11D5_A2F4_B80C2A89502F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog
class CViewUniv;

/////////////////////////////////////////////////////////////////////////////
/// CZoomDlg
///
/////////////////////////////////////////////////////////////////////////////
class CZoomDlg : public CDialog
{
// Construction
public:
	CFont m_font;
	CZoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZoomDlg)
	enum { IDD = IDD_ZOOM_DLG };
	CStatic	m_cZoomValue;
	CSliderCtrl	m_cZoom;
	int		m_nZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnOK();
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CZoomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedCaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDLG_H__92530682_94AF_11D5_A2F4_B80C2A89502F__INCLUDED_)
