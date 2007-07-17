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
//	n_bShowColors = TRUE;
	//{{AFX_DATA_INIT(CPrefObjShape)
	//m_bGrayHidden = (TPref::GrayedHidden);
	//m_idxFree = TPref::TPoint.nPtFree;
	//m_idxCstr = TPref::TPoint.nPtConst;
	//m_idxDep = TPref::TPoint.nPtDep;
	//m_idxRetro = 2;
	//m_nPtSize = TPref::TPoint.nSize;
	//}}AFX_DATA_INIT

//	for (int i=0;i<10;i++)
//	{
//		m_pwndLink[i] = NULL;
//	}
}


void CPrefObjShape::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefObjShape)
//	DDX_Control(pDX, IDC_SHAPE_VIS, m_cRetro);
//	DDX_Control(pDX, IDC_SHAPE_FREE, m_cFree);
//	DDX_Control(pDX, IDC_SHAPE_CSTR, m_cCstr);
//	DDX_Control(pDX, IDC_SHAPE_CALC, m_cDep);
//	DDX_Control(pDX, IDC_SHAPE_SPINSIZE, m_cSpinSize);
//	DDX_Check(pDX, IDC_SHAPE_HIDDEN, m_bGrayHidden);
//	DDX_CBIndex(pDX, IDC_SHAPE_FREE, m_idxFree);
//	DDX_CBIndex(pDX, IDC_SHAPE_CSTR, m_idxCstr);
//	DDX_CBIndex(pDX, IDC_SHAPE_CALC, m_idxDep);
//	DDX_CBIndex(pDX, IDC_SHAPE_VIS, m_idxRetro);
//	DDX_Text(pDX, IDC_SHAPE_POINTSIZE, m_nPtSize);
//	DDV_MinMaxInt(pDX, m_nPtSize, 1, 10);
	//}}AFX_DATA_MAP
	//DDX_GridControl(pDX, IDC_SHAPE_DEFCOLOR, m_cColorlist);
	DDX_Control(pDX, IDC_SHAPE_APP, m_wndPropListLocation);

	if (pDX->m_bSaveAndValidate)
	{
/*		TPref::GrayedHidden = m_bGrayHidden;
		for (int j=0;j<8;j++)
		{
			TPref::custColors[j] = m_cColorlist.GetItemBkColour(j+1,1);
			if (!TPref::GrayedHidden)
			{
				TPref::custColors[j+8] = m_cColorlist.GetItemBkColour(j+1,2);
			}
			TPref::custColorsString[j] = m_cColorlist.GetItemText(j+1,0);
		}*/

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
//	ON_BN_CLICKED(IDC_SHAPE_HIDDEN, OnGrayHidden)
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
//	ON_WM_CREATE()
	//}}AFX_MSG_MAP
//    ON_NOTIFY(NM_DBLCLK , IDC_SHAPE_DEFCOLOR, OnGridClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefObjShape message handlers

/*void CPrefObjShape::SetColors(BOOL bShow)
{
	n_bShowColors = bShow		;
}*/


/*void CPrefObjShape::FillColorList()
{
	const char *strDefaultTitle[]={
				"Name",
				"Appar.   ",
				"Non-app."
		};
	CBCGPPropertyPage::OnInitDialog();

	m_cColorlist.SetTextBkColor(RGB(255,255,255));
	m_cColorlist.EnableSelection(FALSE);
	m_cColorlist.SetRowResize(FALSE);
	m_cColorlist.SetColumnResize(FALSE);
	//m_cColorlist.SetEditable(false);
	//m_cColorlist.SetListMode(true);
	//m_cColorlist.EnableDragAndDrop(FALSE);
	//m_cColorlist.SetSingleRowSelection(true);
	//m_cColorlist.SetImageList(&m_ImageList);

	int NBCOLUMNS = 3;

	m_cColorlist.SetColumnCount(NBCOLUMNS);
	//m_cColorlist.SetFixedColumnCount(1);
	m_cColorlist.SetRowCount(8+1);
	m_cColorlist.SetFixedRowCount(1);

	for (int i=0;i<NBCOLUMNS;i++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		
		Item.row = 0;
		Item.col = i;
		Item.nFormat = DT_CENTER |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText.Format(_T("%s"),strDefaultTitle[i]);
		m_cColorlist.SetItem(&Item);
	}

	DWORD dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	for (int row = 0; row < 8; row++)
	{
		GV_ITEM Item,Item2;

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row+1;

		Item.strText.Format(_T("%s"),TPref::custColorsString[row]);
		Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
		Item.col = 0;

		Item2.mask = GVIF_BKCLR|GVIF_STATE;
		Item2.row = row+1;
		Item2.col = 1;
		int nformat = m_cColorlist.GetDefaultCell(FALSE,FALSE)->GetState();
		Item2.nState = nformat | GVIS_READONLY;
		Item2.crBkClr = TPref::custColors[row];

		m_cColorlist.SetItem(&Item);
		m_cColorlist.SetItem(&Item2);
	}
	m_bGrayHidden=!m_bGrayHidden;
	OnGrayHidden();
	m_cColorlist.AutoSizeColumns();
	//m_cColorlist.ExpandColumnsToFit();
	//m_cColorlist.EnableScrollBars(SB_HORZ, FALSE);

    CRect rect;
    m_cColorlist.GetClientRect(rect);
	int nsb = m_cColorlist.GetColumnWidth(2);
	nsb*=2;
	m_cColorlist.SetColumnWidth(0,rect.Width()-nsb+3);
}*/


void CPrefObjShape::FillLine()
{
	CString strName;
	strName.LoadString(PREF_NAME_LINE);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);

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

void CPrefObjShape::FillPoint()
{

	CString strName;
	strName.LoadString(PREF_NAME_PT);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);

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
	CBCGPProp* pGroup1 = new CBCGPProp (strName);

	strName.LoadString(PREF_NAME_LOCSIZE);
	strDef.LoadString(PREF_DEF_LOCSIZE);
	CBCGPProp* pProp = new CBCGPProp (strName, (_variant_t) (short)TPref::TLocus.nDelta,strDef);

	pProp->SetData((DWORD)intLocus[0]);
	pGroup1->AddSubItem (pProp);

	m_wndProp.AddProperty (pGroup1);

/*	CBCGProp* pGroup2 = new CBCGProp (_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
	font->GetLogFont (&lf);

	pGroup2->AddSubItem (new CBCGFontProp (_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the dialog")));
	pGroup2->AddSubItem (new CBCGProp (_T("Use System Font"), (_variant_t) true, _T("Specifies that the dialog uses MS Shell Dlg font")));

	m_wndProp.AddProperty (pGroup2);

	CBCGProp* pGroup3 = new CBCGProp (_T("Misc"));
	CBCGProp* pProp  = new CBCGProp (_T("(Name)"), _T("IDD_ABOUT_BOX (dialog)"));
	pProp->Enable (FALSE);
	pGroup3->AddSubItem (pProp);

	CBCGColorProp* pColorProp = new CBCGColorProp ("Window Color", RGB (192, 192, 192), NULL, "Specifies the default dialog color");
	pColorProp->EnableOtherButton ("Other...");
	pColorProp->EnableAutomaticButton ("Default", ::GetSysColor (COLOR_3DFACE));
	pGroup3->AddSubItem (pColorProp);

	static char BASED_CODE szFilter[] = "Icon Files (*.ico)|*.ico|All Files (*.*)|*.*||";
	pGroup3->AddSubItem (new CBCGFileProp ("Icon", TRUE, "", "ico", 0, szFilter, "Specifies the dialog icon"));

	pGroup3->AddSubItem (new CBCGFileProp ("Folder", "c:\\"));

	m_wndProp.AddProperty (pGroup3);*/
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
	CString strName;
	strName.LoadString(PREF_NAME_SPH);
	CBCGPProp* pGroup1 = new CBCGPProp (strName);

//	pGroup1->AddSubItem (new CBCGProp (_T("3D Look"), (_variant_t) false,
//		_T("Specifies the dialog's font will be nonbold and controls will have a 3D border")));

/*	CString strSp[2]={
		_T("Equator"),
		_T("Great Circles")
	};
	CString strDSp[2]={
		_T("Display the equator of the spheres"),
		_T("Display the two great circles of the spheres")
	};*/
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

/*	CWnd *pWnd = GetDlgItem(IDC_SHAPE_APP);
	CRect mrect(5,90,220,200);
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		ScreenToClient(mrect);
		mrect.top +=5;
		pWnd->ShowWindow(SW_HIDE);
	}
	BOOL bRes = m_wndProp.Create(WS_CHILD |WS_BORDER|WS_TABSTOP| WS_VISIBLE,mrect,this,IDC_SHAPE_APP);
	m_wndProp.EnableDesciptionArea(TRUE);
	m_wndProp.EnableHeaderCtrl(FALSE);
	m_wndProp.SetVSDotNetLook(FALSE);*/

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
	FillLocus();


	//FillColorList();

/*	if (n_bShowColors==TRUE)
	{
		m_wndPropListLocation.ShowWindow (SW_HIDE);
		m_wndProp.ShowWindow (SW_HIDE);
	}
	else
	{
		CWnd *pWndOK = GetDlgItem (IDC_SHAPE_DEFCOLOR);
		if (pWndOK != NULL)
			pWndOK->ShowWindow (SW_HIDE);
		pWndOK = GetDlgItem (IDC_SHAPE_DEFC);
		if (pWndOK != NULL)
			pWndOK->ShowWindow (SW_HIDE);
		pWndOK = GetDlgItem (IDC_SHAPE_HIDDEN);
		if (pWndOK != NULL)
			pWndOK->ShowWindow (SW_HIDE);
	}*/




/*	////////////////////
	CPoint3D Obj(CPoint(0,0));
	for (int i=0;i<Obj.pObjectShape.nShapeNb;i++)
	{
		CString defsh = Obj.pObjectShape.GetShapeDef(i);
		m_cRetro.AddString(defsh);
		m_cFree.AddString(defsh);
		m_cCstr.AddString(defsh);
		m_cDep.AddString(defsh);
	}
	// TODO: Add extra initialization here
	m_cSpinSize.SetRange(1,10);*/
//	UpdateData (FALSE);

	
/*	CRect mrect(4,4,187,362);
	CRect mrect2(4,4,187,362);
	CWnd *pWnd = GetDlgItem(IDC_TREE1);
	if (pWnd)
	{
		pWnd->GetClientRect(mrect2);
		pWnd->GetWindowRect(mrect);
		mrect2.bottom+=20;
		mrect2.right+=5;
	}
	// TODO: Add your specialized creation code here
	mrect2.OffsetRect(10,25);
	m_wndRollupCtrl.MoveWindow(mrect2);	*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*void CPrefObjShape::OnGrayHidden() 
{
	// TODO: Add your control notification handler code here
	m_bGrayHidden = ! m_bGrayHidden;
	for (int i=0;i<8;i++)
	{
		GV_ITEM Item;

		Item.mask = GVIF_BKCLR|GVIF_STATE;
		Item.row = i+1;
		Item.col = 2;
		int nformat = m_cColorlist.GetDefaultCell(FALSE,FALSE)->GetState();
		Item.nState = nformat | GVIS_READONLY;
		if (m_bGrayHidden)
			Item.crBkClr = RGB(192,192,192);
		else
			Item.crBkClr = TPref::custColors[i+8];
		m_cColorlist.SetItem(&Item);
	}
	m_cColorlist.Invalidate();
	m_cColorlist.UpdateWindow();
}

void CPrefObjShape::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	if (nCol)
	{
		if (nCol==2 && m_bGrayHidden) return;
		COLORREF oldClr = m_cColorlist.GetItemBkColour(nRow,nCol);

		//CColorDialog dlg(oldClr,CC_RGBINIT);
		CBCGPColorDialog dlg(oldClr);
		if (dlg.DoModal() == IDOK)
		{
			//m_cColorlist.SetItemBkColour(nRow,nCol,dlg.m_cc.rgbResult);
			m_cColorlist.SetItemBkColour(nRow,nCol,dlg.GetColor());
			m_cColorlist.RedrawCell(nRow,nCol);
		}
	}
}*/

/*void CPrefObjShape::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CBCGPPropertyPage::OnOK();
}

void CPrefObjShape::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	CBCGPPropertyPage::OnCancel();
}*/

/*void CPrefObjShape::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CBCGPPropertyPage::OnClose();
}*/

void CPrefObjShape::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here	
}

/*int CPrefObjShape::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

		// TODO: Add your specialized creation code here



	return 0;
}*/
