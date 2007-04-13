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
// PrefMathPad.cpp: implementation of the CPrefMathPad class.
//
//////////////////////////////////////////////////////////////////////

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
IMPLEMENT_DYNCREATE(CPrefMathPad, CBCGPPropertyPage)


CPrefMathPad::CPrefMathPad()
	: CBCGPPropertyPage(CPrefMathPad::IDD)
{
	//{{AFX_DATA_INIT(CPrefMathPad)
	m_bFraction = TPref::TMathPad.ShowFraction;
	m_strScale = _T("");
	m_nUnitAngle = TPref::TMathPad.UnitAngular;
	m_nUnitLinear = TPref::TMathPad.UnitLinear;
	m_nPrecFraction = TPref::TMathPad.PrecFraction;
	m_nPrecReal = TPref::TMathPad.PrecReal;
	//}}AFX_DATA_INIT
	m_strScale.Format(_T("%.4g"),TPref::TMathPad.UnitScale);
}

void CPrefMathPad::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
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
		TPref::TMathPad.ShowFraction = m_bFraction;
		TPref::TMathPad.UnitScale = atof(m_strScale);
		TPref::TMathPad.UnitAngular = m_nUnitAngle;
		TPref::TMathPad.UnitLinear = m_nUnitLinear;
		TPref::TMathPad.PrecFraction = m_nPrecFraction;
		TPref::TMathPad.PrecReal = m_nPrecReal;
	}
}


BEGIN_MESSAGE_MAP(CPrefMathPad, CBCGPPropertyPage)
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
	CBCGPPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CPrefMathPad::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
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
	// send this notification unless you override the CBCGPPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CPrefMathPad::OnChangeMathpadPfr() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
