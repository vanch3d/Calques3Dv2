// PropertyBar.cpp: implementation of the CPropertyBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "MainFrm.h"
#include "PropertyBar.h"
#include "objects/ComboShape.h"
#include "objects/Object3D.h"
#include "objects/Text3D.h"
#include "objects/CompositeObj3D.h"

#include "Prefs\Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar

BEGIN_MESSAGE_MAP(CPropertyBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CPropertyBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_PROPERTY_SORTING, OnSorting)
	ON_COMMAND(ID_PROPERTY_EXPAND, OnExpand)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_SORTING, OnUpdateSorting)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_EXPAND, OnUpdateExpand)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar construction/destruction

CPropertyBar::CPropertyBar()
{
	m_pImgList.Create(IDB_OBJECTS_ICONS,16,0,RGB(192,192,192));
	m_pSelObj = NULL;
}

CPropertyBar::~CPropertyBar()
{
}

void CPropertyBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout (FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos (NULL,
							   rectClient.left, 
							   rectClient.top, 
							   rectClient.Width (),
							   cyTlb,
							   SWP_NOACTIVATE | SWP_NOZORDER);


	m_wndProp.SetWindowPos (NULL,
									  rectClient.left, 
									  rectClient.top + cyTlb, 
									  rectClient.Width (),
									  rectClient.Height () - cyTlb ,
									  SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertyBar::EnableBar(BOOL bOpen/*=TRUE*/)
{
	m_wndProp.EnableWindow(bOpen);
	int n = m_wndProp.GetPropertyCount ();
	for (int i=0;i<n;i++)
		m_wndProp.GetProperty(i)->Enable(bOpen);
	m_wndProp.SetCurSel(NULL,TRUE);
	m_wndProp.RemoveAll();
	m_wndProp.Invalidate(TRUE);
}

CBCGPProp* CPropertyBar::GetObjectAppearance(CObject3D* pObj)
{
	CString strRes,strDefRes;

	strRes.LoadString(PROP_APPEARANCE);
	strDefRes.LoadString(PROP_APPEARANCE_DESC);
	CBCGPProp* pGroup = new CBCGPProp (strRes);
	pGroup->SetDescription(strDefRes);

	strRes.LoadString(PROP_OBJNAME);
	strDefRes.LoadString(PROP_OBJNAME_DESC);
	CBCGPProp* pProp = new CBCGPProp (strRes, (LPCTSTR) pObj->GetObjectName(),strDefRes);
	pGroup->AddSubItem (pProp);
	pProp->SetData((DWORD)&pObj->strObjectName);

	if (m_palColorPicker.GetSafeHandle () == NULL)
	{	// Create the palette
		int m_nNumColours = 8;
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

	strRes.LoadString(PROP_OBJCOLOR);
	strDefRes.LoadString(PROP_OBJCOLOR_DESC);
	CBCGPColorProp* pColorProp = new CBCGPColorProp (strRes, 
						pObj->pObjectShape.GetObjectColor(), &m_palColorPicker, 
						strDefRes);
	pColorProp->AllowEdit(FALSE);
	pColorProp->SetData((DWORD)&pObj->pObjectShape.clrObject);

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	pColorProp->EnableAutomaticButton (strDef, pObj->GetDefaultColor());
	pColorProp->EnableOtherButton (strOther);

	pGroup->AddSubItem (pColorProp);

	strRes.LoadString(PROP_OBJSHAPE);
	strDefRes.LoadString(PROP_OBJSHAPE_DESC);
	int index = pObj->pObjectShape.nShapeId;
	pProp = new CBCGPShapeProp (strRes, index, 
					pObj->pObjectShape.GetShapeSize(), 
					pObj->pObjectShape.GetShapeType(),
					strDefRes);
	pProp->SetData((DWORD)&pObj->pObjectShape.nShapeId);
	pGroup->AddSubItem (pProp);

	strRes.LoadString(PROP_OBJVISIBLE);
	strDefRes.LoadString(PROP_OBJVISIBLE_DESC);
	pProp = new CBCGPCheckBoxProp (strRes, pObj->bVisible,strDefRes);
	pProp->SetData((DWORD)&pObj->bVisible);

	pProp->SetData(CObject3D::ATTRIB_VISIBLE);
	pGroup->AddSubItem (pProp);

	strRes.LoadString(PROP_OBJMARKED);
	strDefRes.LoadString(PROP_OBJMARKED_DESC);
	pProp = new CBCGPCheckBoxProp (strRes, pObj->bMarked,strDefRes);
	pProp->SetData((DWORD)&pObj->bMarked);
	pProp->SetData(CObject3D::ATTRIB_MARK);
	pGroup->AddSubItem (pProp);

	return pGroup;
}


CBCGPProp* CPropertyBar::GetTextAppearance(CText3D* pObj)
{
	CString strRes,strDefRes;

	strRes.LoadString(PROP_APPEARANCE);
	strDefRes.LoadString(PROP_APPEARANCE_DESC);
	CBCGPProp* pGroup = new CBCGPProp (strRes);
	pGroup->SetDescription(strDefRes);

	strRes.LoadString(PROP_OBJNAME);
	strDefRes.LoadString(PROP_OBJNAME_DESC);
	CBCGPProp* pProp = new CBCGPProp (strRes, (LPCTSTR) pObj->GetObjectName(),strDefRes);
	pGroup->AddSubItem (pProp);
	pProp->SetData((DWORD)&pObj->strObjectName);

	if (m_palColorPicker.GetSafeHandle () == NULL)
	{	// Create the palette
		int m_nNumColours = 8;
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

	strRes.LoadString(PROP_OBJCOLOR);
	strDefRes.LoadString(PROP_OBJCOLOR_DESC);
	CBCGPColorProp* pColorProp = new CBCGPColorProp (strRes, 
						pObj->mColorText, &m_palColorPicker, 
						strDefRes);
	pColorProp->AllowEdit(FALSE);
	pColorProp->SetData((DWORD)&pObj->mColorText);

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	pColorProp->EnableAutomaticButton (strDef, pObj->GetDefaultColor());
	pColorProp->EnableOtherButton (strOther);

	pGroup->AddSubItem (pColorProp);

	return pGroup;
}
void CPropertyBar::AddProperties(CObject3D* pObj)
{
	if (pObj == m_pSelObj)
		return;

	m_pSelObj = pObj;
	m_wndProp.RemoveAll();
	if (!pObj)
	{
		m_wndProp.Invalidate(TRUE);
		return;
	}

	CString strRes,strDesRes;

	CBCGPProp* pTopItem = NULL;
	strRes.LoadString(PROP_DEFINITION);
	strDesRes.LoadString(PROP_DEFINITION_DESC);
	CBCGPProp* pGroup0 = new CBCGPProp (strRes);
	pGroup0->SetDescription(strDesRes);

	CString str = pObj->GetObjectDef();
	CBCGPObjectProp* pProp = new CBCGPObjectProp(
									pObj->GetObjectHelp(),
									pObj->GetObjectDef(),
									pObj->GetNameID() - IDS_NAME_OBJECT,
									FALSE);
	pProp->SetImageList(&m_pImgList);
	pGroup0->AddSubItem (pProp);
	pTopItem = pProp;
	m_wndProp.AddProperty (pGroup0);

	// Set the appearance group
	CText3D *pTextObj = DYNAMIC_DOWNCAST (CText3D, pObj);
	if (pTextObj==NULL)
		m_wndProp.AddProperty (GetObjectAppearance(pObj));
	else
		m_wndProp.AddProperty (GetTextAppearance(pTextObj));

	if (pTopItem) m_wndProp.SetCurSel(pTopItem);
}


/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers

int CPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	m_wndProp.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP /*| WS_BORDER*/, rectDummy, this, 4);
	m_wndProp.EnableDesciptionArea(TRUE);
	m_wndProp.EnableHeaderCtrl(FALSE);
	m_wndProp.SetVSDotNetLook(TRUE);
	m_wndProp.SetGroupNameFullWidth (TRUE);

	// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorBar::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}

	m_wndToolBar.Create (this, dwDefaultToolbarStyle, IDR_PROPERTY_TB);
	m_wndToolBar.LoadToolBar (IDR_PROPERTY_TB);

	OnChangeVisualStyle ();
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);

	AdjustLayout ();

	return 0;
}

void CPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	AdjustLayout ();
}
void CPropertyBar::OnSorting() 
{
	m_wndProp.SetAlphabeticMode ();
}

void CPropertyBar::OnUpdateSorting(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndProp.IsAlphabeticMode ());
}

void CPropertyBar::OnExpand() 
{
	m_wndProp.SetAlphabeticMode (FALSE);
}

void CPropertyBar::OnUpdateExpand(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (!m_wndProp.IsAlphabeticMode ());
}

void CPropertyBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	
	m_wndProp.SetFocus ();
	
}


LRESULT CPropertyBar::OnPropertyChanged (WPARAM wParam,LPARAM lParam)
{
	if (!lParam) return 0;
	if (!m_pSelObj) return 0;

	CBCGPProp* pProp = (CBCGPProp*) lParam;

	CBCGPColorProp* pColorProp = DYNAMIC_DOWNCAST (CBCGPColorProp, pProp);
	_variant_t tvar = pProp->GetValue();

	if (pColorProp)	// Color Property
	{
		COLORREF *ppclr = (COLORREF*)pProp->GetData();
		if (ppclr)
			*ppclr = pColorProp->GetColor();
	}
	else if (tvar.vt==VT_I2 || tvar.vt==VT_I4) // int property
	{
						int bb = (short)tvar;
						int *ff = (int*)pProp->GetData();
						if (ff) 
							*ff = bb;
	}
	else if (tvar.vt==VT_BOOL)	// boolean property
	{
		bool bb = (bool)tvar;
// 		BOOL *ff = (BOOL*)pProp->GetData();
// 		if (ff) 
// 			*ff = bb;
		DWORD type = pProp->GetData();
		if (type==CObject3D::ATTRIB_VISIBLE)
			m_pSelObj->SetVisible(bb);
		else if (type==CObject3D::ATTRIB_MARK)
			m_pSelObj->bMarked = bb;


	}
	else if (tvar.vt==VT_BSTR)
	{
		CString strVal = (LPCTSTR)(_bstr_t)tvar;
								CString *ff = (CString*)pProp->GetData();
								//*(ddddd[j])=ki;
								if (ff) *ff = strVal;
	}

	CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
	if (!pWnd) return 0;

	CMDIChildWnd*pChild = pWnd->MDIGetActive();
	if (!pChild) return 0;
	
	CDocument* pDoc = pChild->GetActiveDocument();
    if (!pDoc) return 0; // only for views with document

	pDoc->UpdateAllViews(NULL,WM_UPDATEOBJ_MOD,m_pSelObj);

	return 0;
}

void CPropertyBar::OnChangeVisualStyle ()
{
//	m_wndToolBar.CleanUpLockedImages ();
//	m_wndToolBar.LoadBitmap (theApp.m_bHiColorIcons ? 
//		IDB_PROP24 : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);
}




BEGIN_MESSAGE_MAP(CDependentBar, CPropertyBar)
	//{{AFX_MSG_MAP(CDependentBar)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_SORTING, OnUpdateSorting)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_EXPAND, OnUpdateExpand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDependentBar::CDependentBar() : CPropertyBar()
{
}

CDependentBar::~CDependentBar()
{
}

int CDependentBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	m_wndProp.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP /*| WS_BORDER*/, rectDummy, this, 4);
	m_wndProp.EnableDesciptionArea(TRUE);
	m_wndProp.EnableHeaderCtrl(FALSE);
	m_wndProp.SetVSDotNetLook(TRUE);
	m_wndProp.SetGroupNameFullWidth (TRUE);
	m_wndProp.SetAlphabeticMode(FALSE);

	// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorBar::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}

	m_wndToolBar.Create (this, dwDefaultToolbarStyle, IDR_DEPENDENT_TB);
	m_wndToolBar.LoadToolBar (IDR_PROPERTY_TB);
	OnChangeVisualStyle ();
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);

	AdjustLayout ();

	return 0;
}

void CDependentBar::AddProperties(CObject3D* pObj)
{
	if (pObj == m_pSelObj)
		return;

	m_pSelObj = pObj;
	m_wndProp.RemoveAll();
	if (!pObj)
	{
		m_wndProp.Invalidate(TRUE);
		return;
	}

	CBCGPProp* pTopItem = NULL;
	// Set the definition group

	CString strRes,strDefRes;

	{
			strRes.LoadString(PROP_DEFINITION);
		strDefRes.LoadString(PROP_DEFINITION_DESC);
		CBCGPProp* pGroup0 = new CBCGPProp (strRes);
		pGroup0->SetDescription(strDefRes);

		CString str = pObj->GetObjectDef();
		CBCGPObjectProp* pProp = new CBCGPObjectProp(
										pObj->GetObjectHelp(),
										pObj->GetObjectDef(),
										pObj->GetNameID() - IDS_NAME_OBJECT,
										FALSE);
		pProp->SetImageList(&m_pImgList);
		pGroup0->AddSubItem (pProp);
		m_wndProp.AddProperty (pGroup0);
		if (!pTopItem) pTopItem = pProp;

	}
	// Set Parents
	{
		CxObject3DSet* pParent = pObj->GetParents();
		if (pParent) 
		{
			strRes.LoadString(PROP_PARENTS);
			strDefRes.LoadString(PROP_PARENTS_DESC);
			CBCGPProp* pGroup3 = new CBCGPProp (strRes);
			pGroup3->SetDescription(strDefRes);
			for (int j=0;j<pParent->GetSize();j++)
			{
				CObject3D *pPar = pParent->GetAt(j);
				if (!pPar) continue;
				CBCGPObjectProp* pProp2 = new CBCGPObjectProp(
											pPar->GetObjectHelp(),
											pPar->GetObjectDef(),
											pPar->GetNameID() - IDS_NAME_OBJECT,
											FALSE);
				pProp2->SetImageList(&m_pImgList);
				pGroup3->AddSubItem (pProp2);
				if (!pTopItem) pTopItem = pProp2;

			}
			delete pParent;
			m_wndProp.AddProperty (pGroup3);
		}
	}
	// Set Dependents
	{
		CxObject3DSet pList;
		pObj->GetDependList(&pList,FALSE);
		int nsize = pList.GetSize();
		if (nsize > 1)
		{
			strRes.LoadString(PROP_DEPENDENT);
			strDefRes.LoadString(PROP_DEPENDENT_DESC);
			CBCGPProp* pGroup3 = new CBCGPProp (strRes);
			pGroup3->SetDescription(strDefRes);
			for (int i=0;i<nsize;i++)
			{
				CObject3D* pDep = (CObject3D*)pList.GetAt(i);
				if (!pDep) continue;
				if (pDep==pObj) continue;
				CBCGPObjectProp* pProp = new CBCGPObjectProp(pDep->GetObjectHelp(),pDep->GetObjectDef(),pDep->GetNameID() - IDS_NAME_OBJECT,FALSE);
				pProp->SetImageList(&m_pImgList);
				pGroup3->AddSubItem (pProp);

				if (!pTopItem) pTopItem = pProp;

			}
			m_wndProp.AddProperty (pGroup3);
		}
	}
	// Set Composites
	if (pObj->MaskObject(TCompositeObject3DClass))
	{
		CCompositeObj3D *pComp = DYNAMIC_DOWNCAST( CCompositeObj3D, pObj);
		if (!pComp) return;
		int nsize = pComp->m_cSubObjects.GetSize();
		if (nsize > 0)
		{
			strRes.LoadString(PROP_COMPOSITE);
			strDefRes.LoadString(PROP_COMPOSITE_DESC);
			CBCGPProp* pGroup3 = new CBCGPProp (strRes);
			pGroup3->SetDescription(strDefRes);
			for (int i=0;i<nsize;i++)
			{
				CObject3D* pDep = (CObject3D*)pComp->m_cSubObjects.GetAt(i);
				if (!pDep) continue;
				if (pDep==pObj) continue;
				CBCGPObjectProp* pProp = new CBCGPObjectProp(pDep->GetObjectHelp(),pDep->GetObjectDef(),pDep->GetNameID() - IDS_NAME_OBJECT,FALSE);
				pProp->SetImageList(&m_pImgList);
				//pProp->AddSubItem  (AddAppearance(pDep));
				pGroup3->AddSubItem (pProp);

			}
			m_wndProp.AddProperty (pGroup3);
		}

		
	}
	if (pTopItem) m_wndProp.SetCurSel(pTopItem);
}

void CDependentBar::OnUpdateSorting(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndProp.IsAlphabeticMode ());
	pCmdUI->Enable(FALSE);
}

void CDependentBar::OnUpdateExpand(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (!m_wndProp.IsAlphabeticMode ());
	pCmdUI->Enable(FALSE);
}

