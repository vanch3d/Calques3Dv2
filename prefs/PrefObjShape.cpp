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
// PrefObjShape.cpp: implementation of the CPrefObjShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefObjShape.h"

#include "Prefs.h"
#include "..\Objects\Point3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrefObjShape dialog
IMPLEMENT_DYNCREATE(CPrefObjShape, CBCGPPropertyPage)


CPrefObjShape::CPrefObjShape()
	: CBCGPPropertyPage(CPrefObjShape::IDD)
{
	//{{AFX_DATA_INIT(CPrefObjShape)
	//}}AFX_DATA_INIT
}


void CPrefObjShape::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefObjShape)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SHAPE_APP, m_wndPropListLocation);

	if (pDX->m_bSaveAndValidate)
	{
		int nb = m_wndProp.GetPropertyCount();
		for (int i=0;i<nb;i++)
		{
			CBCGPProp* pProp = m_wndProp.GetProperty(i);
			if (!pProp) continue;

			if (pProp->IsGroup())
			{
				int nb2= pProp->GetSubItemsCount();
				for (int j=0;j<nb2;j++)
				{
					CBCGPProp* pProp2 = pProp->GetSubItem(j);
					if (!pProp2) continue;

					CBCGPColorProp* pColorProp = DYNAMIC_DOWNCAST (CBCGPColorProp, pProp2);
					_variant_t tvar = pProp2->GetValue();

					if (pColorProp)
					{
						COLORREF *ppclr = (COLORREF*)pProp2->GetData();
						if (ppclr)
							*ppclr = pColorProp->GetColor();
					}
					else if (tvar.vt==VT_I2)
					{
						int bb = (short)tvar;
						int *ff = (int*)pProp2->GetData();
						if (ff) 
							*ff = bb;
					}
					else if (tvar.vt==VT_BOOL)
					{
						bool bb = (bool)tvar;
						BOOL *ff = (BOOL*)pProp2->GetData();
						if (ff) 
							*ff = bb;
					}
					else if (tvar.vt==VT_BSTR)
					{
						CString strVal = (LPCTSTR)(_bstr_t)tvar;
						CPoint3D Obj(CPoint(0,0));
						for (int ki=0;ki<Obj.pObjectShape.nShapeNb;ki++)
						{
							CString defsh = Obj.pObjectShape.GetShapeDef(ki);
							if (defsh==strVal)
							{
								int *ff = (int*)pProp2->GetData();
								//*(ddddd[j])=ki;
								if (ff) *ff = ki;
								break;
							}

						}

					}
				}
			}

		}
	}
}


BEGIN_MESSAGE_MAP(CPrefObjShape, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefObjShape)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefObjShape message handlers

void CPrefObjShape::FillLine()
{
	CString strName,strDef;
	strName.LoadString(PREF_NAME_LINE);
	strDef.LoadString(PREF_DEF_LINE);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);
	pGroup1->SetDescription(strDef);

	{
		CString strName,strDef,strBtn;
		strName.LoadString(PREF_NAME_COLOR);
		strDef.LoadString(PREF_DEF_COLOR);
		CBCGPColorProp* pColorProp = new CBCGPColorProp (strName, TPref::TLine.clrDefault, NULL, strDef);
		pColorProp->SetData((DWORD)&TPref::TLine.clrDefault);
		pGroup1->AddSubItem (pColorProp);
	}

	m_wndProp.AddProperty (pGroup1);
}

void CPrefObjShape::FillCone()
{
	CString strName,strDef;
	strName.LoadString(PREF_NAME_CONE);
	strDef.LoadString(PREF_DEF_CONE);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);
	pGroup1->SetDescription(strDef);

	{
		CString strName,strDef,strBtn;
		strName.LoadString(PREF_NAME_COLOR);
		strDef.LoadString(PREF_DEF_COLOR);
		CBCGPColorProp* pColorProp = new CBCGPColorProp (strName, TPref::TCone.clrDefault, NULL, strDef);
		pColorProp->SetData((DWORD)&TPref::TCone.clrDefault);
		pGroup1->AddSubItem (pColorProp);
	}
	{
		CBCGPProp* pProp = NULL;
		CString strName,strDef;
		strName.LoadString(PREF_NAME_CONEDBL);
		strDef.LoadString(PREF_DEF_CONEDBL);
		pGroup1->AddSubItem (pProp = new CBCGPProp (strName, (_variant_t)(bool)(TPref::TCone.bDoubleCone==TRUE), strDef));
		pProp->SetData((DWORD)&TPref::TCone.bDoubleCone);
	}

	m_wndProp.AddProperty (pGroup1);
}

void CPrefObjShape::FillPoint()
{

	CString strName,strDef;
	strName.LoadString(PREF_NAME_PT);
	strDef.LoadString(PREF_DEF_PT);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);
	pGroup1->SetDescription(strDef);

	int *intPt[5]={	&TPref::TPoint.nPtFree,&TPref::TPoint.nPtConst,&TPref::TPoint.nPtDep,&TPref::TPoint.nPtFree,
					&TPref::TPoint.nSize};

	/// Point default shape
	for (int j=0;j<4;j++)
	{
		CString strName,strDef;
		strName.LoadString(PREF_NAME_PTFREE+j);
		strDef.LoadString(PREF_DEF_PTFREE+j);
		CBCGPProp* pProp = new CBCGPProp (strName, _T(""),strDef);

		CPoint3D Obj(CPoint(0,0));
		for (int i=0;i<Obj.pObjectShape.nShapeNb;i++)
		{
			CString defsh = Obj.pObjectShape.GetShapeDef(i);
			pProp->AddOption (defsh);
		}
		if (j==3) pProp->Enable (FALSE);
		pProp->AllowEdit (FALSE);
		pProp->SetValue((LPCTSTR)Obj.pObjectShape.GetShapeDef(*intPt[j]));
		pProp->SetData((DWORD)intPt[j]);
		pGroup1->AddSubItem (pProp);
	}

	/// Point size
	{
		CString strName,strDef;
		strName.LoadString(PREF_NAME_PTSIZE);
		strDef.LoadString(PREF_DEF_PTSIZE);
		CBCGPProp* pProp = new CBCGPProp (strName, (_variant_t) (short)TPref::TPoint.nSize,strDef);

		pProp->SetData((DWORD)intPt[4]);
		pProp->Enable (FALSE);
		pProp->AllowEdit(FALSE);
		pGroup1->AddSubItem (pProp);
	}

	/// Point color
	{
		CString strName,strDef,strBtn;
		strName.LoadString(PREF_NAME_COLOR);
		strDef.LoadString(PREF_DEF_COLOR);
		CBCGPColorProp* pColorProp = new CBCGPColorProp (strName, TPref::TPoint.clrDefault, NULL, strDef);
		//pColorProp->EnableOtherButton ("Other...");
		//pColorProp->EnableAutomaticButton ("Default", ::GetSysColor (COLOR_3DFACE));
		pColorProp->SetData((DWORD)&TPref::TPoint.clrDefault);
		//CColorPopup *pPop = pColorProp->GetColorPopup();
		//pPop->m_bEnabledInCustomizeMode = TRUE
		//CBCGColorMenuButton::m_bEnabledInCustomizeMode = TRUE;

		//strBtn.LoadString(PREF_NAME_CLROTHER);
		//pColorProp->EnableOtherButton (strBtn);
		//strBtn.LoadString(PREF_NAME_CLRAUTO);
		//pColorProp->EnableAutomaticButton (strBtn, ::GetSysColor (COLOR_3DFACE));
		pGroup1->AddSubItem (pColorProp);
	}

	m_wndProp.AddProperty (pGroup1);
}

void CPrefObjShape::FillLocus()
{
	int *intLocus[1]={	&TPref::TLocus.nDelta};

	CString strName,strDef;
	strName.LoadString(PREF_NAME_LOCUS);
	strDef.LoadString(PREF_DEF_LOCUS);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);
	pGroup1->SetDescription(strDef);

	strName.LoadString(PREF_NAME_LOCSIZE);
	strDef.LoadString(PREF_DEF_LOCSIZE);
	CBCGPProp* pProp = new CBCGPProp (strName, (_variant_t) (short)TPref::TLocus.nDelta,strDef);

	pProp->SetData((DWORD)intLocus[0]);
	pGroup1->AddSubItem (pProp);

	m_wndProp.AddProperty (pGroup1);

	{
		CString strName,strDef,strBtn;
		strName.LoadString(PREF_NAME_COLOR);
		strDef.LoadString(PREF_DEF_COLOR);
		CBCGPColorProp* pColorProp = new CBCGPColorProp (strName, TPref::TLocus.clrDefault, NULL, strDef);
		pColorProp->SetData((DWORD)&TPref::TLocus.clrDefault);
		pGroup1->AddSubItem (pColorProp);
	}

}

void CPrefObjShape::FillSphere()
{
	CString strName,strDef;
	strName.LoadString(PREF_NAME_SPH);
	strDef.LoadString(PREF_DEF_SPH);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);
	pGroup1->SetDescription(strDef);

	BOOL *optS[2]={&TPref::TSphere.bEquator,&TPref::TSphere.bGCircle};

	for (int i=0;i<2;i++)
	{
		CBCGPProp* pProp = NULL;
		CString strName,strDef;
		strName.LoadString(PREF_NAME_SPHEQ+i);
		strDef.LoadString(PREF_DEF_SPHEQ+i);
		pGroup1->AddSubItem (pProp = new CBCGPProp (strName, (_variant_t)(bool)(*optS[i]==TRUE), strDef));
		pProp->SetData((DWORD)optS[i]);
	}

/*	{
		CString strName,strDef,strBtn;
		strName.LoadString(PREF_NAME_COLOR);
		strDef.LoadString(PREF_DEF_COLOR);
		CBCGColorProp* pColorProp = new CBCGColorProp (strName, TPref::TLine.clrDef, NULL, strDef);
		pColorProp->SetData((DWORD)&TPref::TLine.clrDef);
		pGroup1->AddSubItem (pColorProp);
	}*/

	m_wndProp.AddProperty (pGroup1);
}


BOOL CPrefObjShape::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	CRect rectPropList;
	m_wndPropListLocation.GetClientRect (&rectPropList);
	m_wndPropListLocation.MapWindowPoints (this, &rectPropList);

	m_wndProp.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)IDC_SHAPE_APP);
	m_wndProp.EnableDesciptionArea(TRUE);
	m_wndProp.EnableHeaderCtrl(FALSE);
	m_wndProp.SetVSDotNetLook(TRUE);
	//m_wndProp.SetAlphabeticMode(TRUE);

	FillPoint();
	FillLine();
	FillSphere();
	FillCone();
	FillLocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefObjShape::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here	
}
