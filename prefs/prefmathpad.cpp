// PrefMathPad.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefMathPad.h"

#include "Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefMathPad dialog
IMPLEMENT_DYNCREATE(CPrefMathPad, CPropertyPage)


CPrefMathPad::CPrefMathPad()
	: CPropertyPage(CPrefMathPad::IDD)
{
	//{{AFX_DATA_INIT(CPrefMathPad)
	m_bFraction = TPref::MathPad.ShowFraction;
	m_strScale = _T("");
	m_nUnitAngle = TPref::MathPad.UnitAngular;
	m_nUnitLinear = TPref::MathPad.UnitLinear;
	m_nPrecFraction = TPref::MathPad.PrecFraction;
	m_nPrecReal = TPref::MathPad.PrecReal;
	//}}AFX_DATA_INIT
	m_strScale.Format(_T("%.4g"),TPref::MathPad.UnitScale);
}

void CPrefMathPad::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefMathPad)
	DDX_Control(pDX, IDC_MATHPAD_DIST, m_cUnitLinear);
	DDX_Radio(pDX, IDC_MATHPAD_REAL, m_bFraction);
	DDX_Text(pDX, IDC_MATHPAD_SCALE, m_strScale);
	DDX_CBIndex(pDX, IDC_MATHPAD_ANGLE, m_nUnitAngle);
	DDX_CBIndex(pDX, IDC_MATHPAD_DIST, m_nUnitLinear);
	DDX_Text(pDX, IDC_MATHPAD_PFR, m_nPrecFraction);
	DDV_MinMaxInt(pDX, m_nPrecFraction, 1, 100);
	DDX_Text(pDX, IDC_MATHPAD_PRV, m_nPrecReal);
	DDV_MinMaxInt(pDX, m_nPrecReal, 0, 8);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		TPref::MathPad.ShowFraction = m_bFraction;
		TPref::MathPad.UnitScale = atof(m_strScale);
		TPref::MathPad.UnitAngular = m_nUnitAngle;
		TPref::MathPad.UnitLinear = m_nUnitLinear;
		TPref::MathPad.PrecFraction = m_nPrecFraction;
		TPref::MathPad.PrecReal = m_nPrecReal;
	}
}


BEGIN_MESSAGE_MAP(CPrefMathPad, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefMathPad)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MATHPAD_REAL, OnMathpadReal)
	ON_BN_CLICKED(IDC_MATHPAD_FRACTION, OnMathpadFraction)
	ON_EN_CHANGE(IDC_MATHPAD_PRV, OnChangeMathpadPrv)
	ON_EN_CHANGE(IDC_MATHPAD_PFR, OnChangeMathpadPfr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefMathPad message handlers

void CPrefMathPad::OnDestroy() 
{
	UpdateData();
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CPrefMathPad::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nb = 	TPref::strUnits.GetSize();

	m_cUnitLinear.ResetContent();
	for (int i=0;i<nb;i++)
	{
		m_cUnitLinear.AddString(TPref::strUnits.GetAt(i));
	}

	m_cUnitLinear.SetCurSel(m_nUnitLinear);

	if (m_bFraction)
		OnMathpadFraction();
	else
		OnMathpadReal();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefMathPad::OnMathpadReal() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_MATHPAD_PRV);
	if (pWnd) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_MATHPAD_PFR);
	if (pWnd) pWnd->EnableWindow(FALSE);
	
}

void CPrefMathPad::OnMathpadFraction() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_MATHPAD_PRV);
	if (pWnd) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_MATHPAD_PFR);
	if (pWnd) pWnd->EnableWindow(TRUE);
	
}

void CPrefMathPad::OnChangeMathpadPrv() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CPrefMathPad::OnChangeMathpadPfr() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
