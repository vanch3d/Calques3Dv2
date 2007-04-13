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
// ObjectPropPage.cpp: implementation of the CObjectPropPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Calques3DDoc.h"

#include "..\Prefs\Prefs.h"

#include "..\MathParser\MathParser.h"


#include "Object3D.h"
#include "ObjectPropPage.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectPropPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CObjectPropPage, CPropertyPage)


CObjectPropPage::CObjectPropPage()
	: CPropertyPage(CObjectPropPage::IDD)
{

	//{{AFX_DATA_INIT(CObjectPropPage)
	m_bHidden = 2;
	m_bMarked = FALSE;
	m_nLocus = 20;
	m_strName = _T("");
//	m_nShape = -1;
	//}}AFX_DATA_INIT
	m_clrObj = m_clrDefObj = RGB(0,0,0);
	m_bChanged = FALSE;
	m_bIsLocus = FALSE;
	m_strDefName =  _T("");
}


void CObjectPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	if (pDX->m_bSaveAndValidate)
	{
		CString mstr;
		DDX_Text(pDX, IDC_OBJ_NAME, mstr);
		m_bChanged = (mstr == _T("")) || (mstr == m_strDefName);
	}
	//{{AFX_DATA_MAP(CObjectPropPage)
	DDX_Control(pDX, IDC_OBJ_HIDDEN, m_cHidden);
	DDX_Control(pDX, IDC_OBJ_COLOR, m_cColor);
	DDX_Control(pDX, IDC_OBJ_SHAPE, m_cShape);
	DDX_Check(pDX, IDC_OBJ_HIDDEN, m_bHidden);
	DDX_Check(pDX, IDC_OBJ_MARK, m_bMarked);
	DDX_Text(pDX, IDC_OBJ_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 50);
	DDX_Control(pDX, IDC_OBJ_LOCUSSPIN, m_wndLocusSpin);
	DDX_Control(pDX, IDC_OBJ_LOCUS, m_wndLocus);
	DDX_Control(pDX, IDC_OBJ_LOCUSN, m_wndLocusTitle);
	DDX_Text(pDX, IDC_OBJ_LOCUS, m_nLocus);
	DDV_MinMaxInt(pDX, m_nLocus, 20, 100);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		if (m_bChanged)
			m_strName = _T("");
	}
}


BEGIN_MESSAGE_MAP(CObjectPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CObjectPropPage)
	ON_EN_CHANGE(IDC_OBJ_NAME, OnChangeObjName)
	ON_BN_CLICKED(IDC_OBJ_COLOR, OnSetObjColor)
	ON_BN_CLICKED(IDC_OBJ_HIDDEN, OnHiddenObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CObjectPropPage message handlers

void CObjectPropPage::OnHiddenObject() 
{
	// TODO: Add your control notification handler code here

	int nb = m_cHidden.GetCheck();
	if (nb)
		m_cHidden.SetCheck(0);
	else
		m_cHidden.SetCheck(1);
}

BOOL CObjectPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_wndLocus.EnableWindow(m_bIsLocus);
	m_wndLocus.ShowWindow(m_bIsLocus);
	m_wndLocusSpin.EnableWindow(m_bIsLocus);
	m_wndLocusSpin.ShowWindow(m_bIsLocus);
	m_wndLocusTitle.EnableWindow(m_bIsLocus);
	m_wndLocusTitle.ShowWindow(m_bIsLocus);
	m_wndLocusSpin.SetRange(20,100);


	int m_nNumColours = 8;
	if (m_palColorPicker.GetSafeHandle () == NULL)
	{
		// Create the palette
		struct 
		{
			LOGPALETTE    LogPalette;
			PALETTEENTRY  PalEntry[8];
		}pal;

		LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
		pLogPalette->palVersion    = 0x300;
		pLogPalette->palNumEntries = (WORD) m_nNumColours; 

		for (int i = 0; i < m_nNumColours; i++)
		{
			pLogPalette->palPalEntry[i].peRed   = GetRValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peGreen = GetGValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peBlue  = GetBValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peFlags = 0;
		}

		m_palColorPicker.CreatePalette (pLogPalette);
	}

	// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorMenuButton::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}


	for (i = 0; i < m_nNumColours; i++)
	{
		m_cColor.SetColorName(TPref::custColors[i], TPref::custColorsString[i]);
	}

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	strDoc.LoadString(PREF_NAME_CLRDOC);
	m_cColor.SetPalette(&m_palColorPicker);
	m_cColor.EnableAutomaticButton (strDef, m_clrDefObj);
	m_cColor.EnableOtherButton (strOther);
	m_cColor.SetColor (m_clrObj);
	m_cColor.SetColumnsNumber (8);
	m_cColor.SetDocumentColors(strDoc,TPref::docColors);


	UINT nBStyle = m_cHidden.GetButtonStyle();
	nBStyle |= BS_3STATE;
	m_cHidden.SetButtonStyle(BS_3STATE/*BS_AUTO3STATE*/);
	m_cHidden.SetCheck(m_bHidden);

	UpdateData (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectPropPage::OnSetObjColor() 
{
	// TODO: Add your control notification handler code here
	m_clrObj = m_cColor.GetColor();	

	if (m_clrObj==-1) m_clrObj = m_clrDefObj;
		
	BOOL bFound=FALSE;
	for (int i=0;i<8;i++)
	{
		if (TPref::custColors[i]==m_clrObj)
		{
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		POSITION pos = TPref::docColors.GetHeadPosition();
		bFound = FALSE;
		while (pos)
		{
			COLORREF rcol = TPref::docColors.GetNext(pos);
			if (rcol==m_clrObj)
			{
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
		{
			CString strDoc;
			strDoc.LoadString(PREF_NAME_CLRDOC);

			TPref::docColors.AddTail(m_clrObj);
			m_cColor.SetDocumentColors(strDoc,TPref::docColors);
		}
	}
}

void CObjectPropPage::OnChangeObjName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int w = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CObjectInfoPage dialog
/////////////////////////////////////////////////////////////////////////////


CObjectInfoPage::CObjectInfoPage()
	: CPropertyPage(CObjectInfoPage::IDD)
{
	//{{AFX_DATA_INIT(CObjectInfoPage)
	m_strObjDef = _T("");
	m_strObjHelp = _T("");
	//}}AFX_DATA_INIT
	m_bModif = 0;
}


void CObjectInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectInfoPage)
	DDX_Control(pDX, IDC_OBJ_HELP, m_cHelp);
	DDX_Control(pDX, IDC_OBJ_DEF, m_cDef);
	DDX_Text(pDX, IDC_OBJ_DEF, m_strObjDef);
	DDX_Text(pDX, IDC_OBJ_HELP, m_strObjHelp);
	//}}AFX_DATA_MAP
}

BOOL CObjectInfoPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cHelp.SetReadOnly(!m_bModif);
	m_cDef.SetReadOnly(!m_bModif);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CObjectInfoPage, CPropertyPage)
	//{{AFX_MSG_MAP(CObjectInfoPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectInfoPage message handlers

/////////////////////////////////////////////////////////////////////////////
// CObjectDepPage dialog
/////////////////////////////////////////////////////////////////////////////

CObjectDepPage::CObjectDepPage()
	: CPropertyPage(CObjectDepPage::IDD)
{
	//{{AFX_DATA_INIT(CObjectDepPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pList = NULL;
}

CObjectDepPage::CObjectDepPage(UINT nID)
	: CPropertyPage(nID)
{
	m_pList = NULL;
}




void CObjectDepPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDepPage)
	DDX_Control(pDX, IDC_OBJ_DEPLIST, m_cDepList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectDepPage, CPropertyPage)
	//{{AFX_MSG_MAP(CObjectDepPage)
	ON_BN_CLICKED(IDC_SWITCH_MODE, OnSwitchMode)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectDepPage message handlers


BOOL CObjectDepPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	int nb = m_ImageList.Create(IDB_OBJECTS_ICONS,16,0,RGB(192,192,192));
	m_cDepList.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_cDepList.SetImageList(&m_ImageList,LVSIL_SMALL);
	
	// TODO: Add extra initialization here
	if (m_pList)
	{
		int nb = m_pList->GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pDep = m_pList->GetAt(i);
			if (!pDep) continue;
			//if (!pDep->bDrawInGraph) continue;

			CString mstr = pDep->GetObjectHelp();

			LVITEM pItem;
			pItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
			pItem.pszText = mstr.GetBuffer(mstr.GetLength());
			pItem.lParam = (LPARAM) pDep;
			pItem.iItem = i;
			pItem.iSubItem = 0;
			pItem.iImage = pDep->GetNameID() - IDS_NAME_OBJECT;
			m_cDepList.InsertItem(&pItem);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectDepPage::OnSwitchMode() 
{
	DWORD dwStyle = m_cDepList.GetStyle();


	if ((dwStyle & LVS_TYPEMASK) == LVS_REPORT)
	{
		OnViewIcon();
	}
	else if ((dwStyle & LVS_TYPEMASK) == LVS_ICON)
	{
		OnViewList();
	}

	return;

	if ((dwStyle & LVS_TYPEMASK) == LVS_LIST)
	{
		dwStyle &= ~LVS_LIST;
		dwStyle |= LVS_ICON;
	}
	else if ((dwStyle & LVS_TYPEMASK) == LVS_ICON)
	{
		dwStyle &= ~LVS_ICON;
		dwStyle |= LVS_LIST;//LVS_SMALLICON;
	}
/*	else if ((dwStyle & LVS_TYPEMASK) == LVS_SMALLICON)
	{
		dwStyle &= ~LVS_SMALLICON;
		dwStyle |= LVS_REPORT;
	}
	else if ((dwStyle & LVS_TYPEMASK) == LVS_REPORT)
	{
		dwStyle &= ~LVS_REPORT;
		dwStyle |= LVS_LIST;
	}*/
	// TODO: Add your control notification handler code here
		//dwStyle |= /*WS_CHILD | WS_VISIBLE| */LVS_LIST | /*LVS_AUTOARRANGE | /*LVS_ALIGNTOP | /*LVS_ALIGNLEFT | */
		//		LVS_SINGLESEL | LVS_SHOWSELALWAYS ;
	//	dwStyle |= TVS_HASLINES | TVS_LINESATROOT |  TVS_HASBUTTONS ;
	::SetWindowLong(m_cDepList.m_hWnd,GWL_STYLE,dwStyle);
		//DWORD dwExtStyle = mListCtrl.GetExStyle();
		//dwExtStyle |= LVS_EX_INFOTIP   ;
	m_cDepList.Invalidate();
}

void CObjectDepPage::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE)
		OnSwitchMode();
	
	CPropertyPage::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CObjectDepPage::OnViewIcon() 
{
	// TODO: Add your control notification handler code here
	m_cDepList.DeleteAllItems();

	LONG nlong = ::GetWindowLong(m_cDepList,GWL_STYLE);
	nlong &= ~LVS_REPORT;
	nlong |= LVS_ICON;
	::SetWindowLong(m_cDepList,GWL_STYLE,nlong);

	if (m_pList)
	{
		int nb = m_pList->GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pDep = m_pList->GetAt(i);
			if (!pDep) continue;
			//if (!pDep->bDrawInGraph) continue;

			CString mstr = pDep->GetObjectHelp();

			LVITEM pItem;
			pItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
			pItem.pszText = mstr.GetBuffer(mstr.GetLength());
			pItem.lParam = (LPARAM) pDep;
			pItem.iItem = i;
			pItem.iSubItem = 0;
			pItem.iImage = pDep->GetNameID() - IDS_NAME_OBJECT;
			m_cDepList.InsertItem(&pItem);
		}
	}
}

void CObjectDepPage::OnViewList() 
{
	// TODO: Add your control notification handler code here
	m_cDepList.DeleteAllItems();
	m_cDepList.DeleteColumn(0);
//	m_cDepList.DeleteColumn(0);

	LONG nlong = ::GetWindowLong(m_cDepList,GWL_STYLE);
	nlong &= ~LVS_ICON;
	//nlong &= ~(LVS_SORTASCENDING | LVS_SORTDESCENDING);
	//nlong &= ~LVS_NOSORTHEADER ;
	nlong |= LVS_REPORT ;//| LVS_SORTASCENDING;
	::SetWindowLong(m_cDepList,GWL_STYLE,nlong);

	CRect rect;
	
	m_cDepList.GetWindowRect(&rect);
//	m_cDepList.InsertColumn(0, _T("Name"), LVCFMT_LEFT,
//		-20+rect.Width() * 1/3, 0);
//	m_cDepList.InsertColumn(1, _T("Description"), LVCFMT_LEFT,
//		-1+rect.Width() * 2/3, 1);
	m_cDepList.InsertColumn(1, _T("Description"), LVCFMT_LEFT,
		-20+rect.Width(), 0);

	//ModifyHeaderItems();
	if (m_pList)
	{
		int nb = m_pList->GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pDep = m_pList->GetAt(i);
			if (!pDep) continue;
			//if (!pDep->bDrawInGraph) continue;

			CString mstr = pDep->GetObjectName();
			mstr = pDep->GetObjectDef();

			LVITEM pItem;
			pItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
			pItem.pszText = mstr.GetBuffer(mstr.GetLength());
			pItem.lParam = (LPARAM) pDep;
			pItem.iItem = i;
			pItem.iSubItem = 0;
			pItem.iImage = pDep->GetNameID() - IDS_NAME_OBJECT;
			m_cDepList.InsertItem(&pItem);

			/*mstr = pDep->GetObjectDef();

			LVITEM pItem2;
			pItem2.mask = LVIF_TEXT;
			pItem2.pszText = mstr.GetBuffer(mstr.GetLength());
			pItem2.iItem = i;
			pItem2.iSubItem = 1;
			m_cDepList.SetItem(&pItem2);*/
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CObjectCpntPage dialog
/////////////////////////////////////////////////////////////////////////////

CObjectCpntPage::CObjectCpntPage()
	: CObjectDepPage(CObjectCpntPage::IDD)
{
	//{{AFX_DATA_INIT(CObjectCpntPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CObjectCpntPage, CObjectDepPage)
	//{{AFX_MSG_MAP(CObjectCpntPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectTextPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CObjectTextPage, CPropertyPage)


CObjectTextPage::CObjectTextPage()
	: CPropertyPage(CObjectTextPage::IDD)
{
	m_bMod = FALSE;
	m_nSelD = m_nSelF = 0;
	//m_pValue = 1.0;
	//{{AFX_DATA_INIT(CObjectTextPage)
	m_strText = _T("");
	m_strEqu = _T("");
	m_strVarName = _T("");
	m_bMarked = FALSE;
	//}}AFX_DATA_INIT
}


void CObjectTextPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectTextPage)
//	DDX_Control(pDX, IDC_TXT_PALETTE, m_wndPaletteBarLocation);
	DDX_Control(pDX, IDC_TXT_EQUATION, m_cEquation);
	DDX_Text(pDX, IDC_TXT_EDIT, m_strText);
	DDX_Text(pDX, IDC_TXT_EQUATION, m_strEqu);
	DDX_Control(pDX, IDC_TXT_VAR, m_cInsertVar);
	DDX_Text(pDX, IDC_TXT_VARNAME, m_strVarName);
	DDX_Check(pDX, IDC_OBJ_MARK, m_bMarked);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate && m_bMod==TMathOp3DClass)
	{	
		CMathParser mp;
		double result;
		CStrMap varmap;

		double varvalues[] = { 5, 6.1,0.1 };

		for (int i=0;i<m_pSource.GetSize();i++)
		{
			CObject3D*pObj = m_pSource.GetAt(i);
			if (!pObj) continue;

			if (pObj->isA() == TEquation3DClass) continue;

			if (pObj->strObjectName.IsEmpty()) continue;
			CString str = pObj->strObjectName;
			str.MakeUpper();

			char tt[255];

			strcpy(tt,str.GetBuffer(str.GetLength()));
			varmap.AddString(tt, varvalues );
		}


		varmap.AddString("AAA", varvalues );
		//varmap.AddString("y", varvalues+1 );
		mp.Parameters=&varmap;
		//char *ErrMsg = mp.Parse( "AAA+y", &result );
		char *ErrMsg = mp.Parse( m_strEqu.GetBuffer(m_strEqu.GetLength()), &result );
		if (ErrMsg)
		{
			MessageBox(ErrMsg);
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(CObjectTextPage, CPropertyPage)
	//{{AFX_MSG_MAP(CObjectTextPage)
	ON_EN_CHANGE(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_ERRSPACE(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_HSCROLL(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_KILLFOCUS(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_MAXTEXT(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_SETFOCUS(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_UPDATE(IDC_TXT_EQUATION, OnChangeEquation)
	ON_EN_VSCROLL(IDC_TXT_EQUATION, OnChangeEquation)
	ON_BN_CLICKED(IDC_TXT_FONT, OnSetFont)
	ON_BN_CLICKED(IDC_TXT_VAR, OnVarMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CObjectTextPage::OnInitDialog() 
{
	BOOL bRet = CPropertyPage::OnInitDialog();

	CWnd *pWnd = GetDlgItem(IDC_TXT_EDIT);
	if (pWnd)
	{
		pWnd->EnableWindow(m_bMod==TCComment3DClass);
		pWnd->ShowWindow(m_bMod==TCComment3DClass ? SW_SHOW : SW_HIDE);
	}
	pWnd = GetDlgItem(IDC_TXT_EQUATION);
	if (pWnd)
	{
		pWnd->EnableWindow(m_bMod==TMathOp3DClass);
		pWnd->ShowWindow(m_bMod==TMathOp3DClass ? SW_SHOW : SW_HIDE);
	}
	pWnd = GetDlgItem(IDC_OBJ_MARK);
	if (pWnd)
	{
		BOOL bRet = (m_bMod==TAngle3DClass || m_bMod==TDistance3DClass);
		pWnd->EnableWindow(bRet);
		pWnd->ShowWindow(bRet ? SW_SHOW : SW_HIDE);
	}


	m_menu.CreatePopupMenu();
	//m_menu.LoadMenu (IDR_POPUP_DEFAULT);
	//int nb = m_menu.InsertMenu(-1,MF_BYPOSITION|MF_STRING,IDS_HELP_CYLIN,_T("fdfdfdfd"));

	int nb=0;
	for (int i=0;i<m_pSource.GetSize();i++)
	{
		CObject3D*pObj = m_pSource.GetAt(i);
		if (!pObj) continue;

		if (pObj->strObjectName.IsEmpty()) continue;
	
		if (pObj->isA() == TEquation3DClass) continue;

		CString mstr,mstr2;
		mstr.LoadString(pObj->GetNameID());
		mstr2.Format("%s%d",mstr,pObj->nObjectId);
		mstr2 = pObj->GetObjectHelp();
		m_menu.AppendMenu(MF_STRING,200+i,mstr2);

		nb++;

		//m_pVarMap.AddString("aaa", &m_pValue );

	}
	m_cInsertVar.m_bRightArrow = TRUE;
	m_cInsertVar.m_hMenu = m_menu.GetSafeHmenu ();
	m_cInsertVar.m_bOSMenu = TRUE;
	m_cInsertVar.m_bStayPressed =TRUE;

	pWnd = GetDlgItem(IDC_TXT_VAR);
	if (pWnd)
	{
		pWnd->EnableWindow(m_bMod==TMathOp3DClass && nb);
		pWnd->ShowWindow((m_bMod==TMathOp3DClass) ? SW_SHOW : SW_HIDE);
	}


	return bRet;
}


void CObjectTextPage::OnSetFont() 
{
	LOGFONT pFont = m_lFont;
	CFontDialog mdlg(&pFont,CF_BOTH|CF_FORCEFONTEXIST|CF_NOSCRIPTSEL/*|CF_EFFECTS*/);
	mdlg.m_cf.rgbColors = m_clrFont;

	if (mdlg.DoModal())
	{
		m_lFont = *(mdlg.m_cf.lpLogFont);
	}
}

void CObjectTextPage::OnChangeEquation() 
{
	m_cEquation.GetSel(m_nSelD,m_nSelF);
}

void CObjectTextPage::OnVarMenu() 
{
	CString strItem;
	
	int a = m_cInsertVar.m_nMenuResult-200;

	CObject3D*pObj = m_pSource.GetAt(a);
	if (!pObj) return;

	CString mstr,mstr2;
	mstr.LoadString(pObj->GetNameID());
	mstr2.Format("%s",pObj->strObjectName);

	m_cEquation.SetSel(m_nSelD,m_nSelF);
	m_cEquation.ReplaceSel(mstr2,TRUE);
}
