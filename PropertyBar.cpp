// PropertyBar.cpp: implementation of the CPropertyBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "PropertyBar.h"
#include "objects/ComboShape.h"
#include "objects/Object3D.h"
#include "objects/CompositeObj3D.h"

#include "Prefs\Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
// To hold the colours and their names
typedef struct {
    COLORREF crColour;
    TCHAR    *szName;
} ColourTableEntry;


#define MAX_COLOURS      100

static ColourTableEntry crColours[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark Blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Gray-80%")          },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark Red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark Yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-Gray")         },
    { RGB(0x66, 0x66, 0x66),    _T("Gray-50%")          },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light Orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea Green")         },
    { RGB(51, 204, 204),		_T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light Blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Gray-40%")          },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0xFF, 0x00),    _T("Bright Green")      },
    { RGB(0x00, 0xFF, 0xFF),    _T("Turquoise")         },
    { RGB(0, 204, 255),			_T("Sky Blue")          },
    { RGB(234, 128, 102),		_T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Gray-25%")          },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(255, 254, 153),		_T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light Yellow")      },
    { RGB(204, 255, 204),		_T("Light Green ")      },
    { RGB(204, 255, 255),		_T("Light Turquoise")	},
    { RGB(153, 204, 255),		_T("Pale Blue")         },
    { RGB(204, 153, 255),		_T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};
*/

BEGIN_MESSAGE_MAP(CBCGPropListEx, CBCGPPropList)
	//{{AFX_MSG_MAP(CBCGPropListEx)
	//ON_WM_PAINT()
	//ON_WM_MOUSEWHEEL()
	//ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//******************************************************************************************
BOOL CBCGPropListEx::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/) 
{
	if (m_nVertScrollTotal > 0)
	{
		const int nSteps = abs(zDelta) / WHEEL_DELTA;

		for (int i = 0; i < nSteps; i++)
		{
			OnVScroll (zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, NULL);
		}
	}

	return TRUE;
}

void CBCGPropListEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (globalData.m_nBitsPerPixel <= 8)
	{
		m_clrGray = m_bControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;
	}
	else
	{
		m_clrGray = CBCGPDrawManager::PixelAlpha (
			m_bControlBarColors ? globalData.clrBarFace : globalData.clrBtnFace, 94);
	}

	CRect rectClip;
	dc.GetClipBox (rectClip);

	CRect rectClient;
	GetClientRect (rectClient);

	CDC*		pDC = &dc;
	BOOL		bMemDC = FALSE;
	CDC			dcMem;
	CBitmap		bmp;
	CBitmap*	pOldBmp = NULL;

	if (dcMem.CreateCompatibleDC (&dc) &&
		bmp.CreateCompatibleBitmap (&dc, rectClient.Width (),
								  rectClient.Height ()))
	{
		//-------------------------------------------------------------
		// Off-screen DC successfully created. Better paint to it then!
		//-------------------------------------------------------------
		bMemDC = TRUE;
		pOldBmp = dcMem.SelectObject (&bmp);
		pDC = &dcMem;
	}

	if (IsWindowEnabled())
		pDC->FillRect (rectClient, &globalData.brWindow);
	else
		pDC->FillRect (rectClient, &globalData.brBtnFace);

	//HFONT hfontOld = SetFont (pDC);
		HFONT hfontOld = SetCurrFont (pDC);

	pDC->SetTextColor (globalData.clrWindowText);
	pDC->SetBkMode (TRANSPARENT);

	OnDrawList (pDC);
	
	if (m_bDescriptionArea)
	{
		CRect rectDescr = rectClient;
		rectDescr.top = m_rectList.bottom;
		rectDescr.DeflateRect (1, 1);

		OnDrawDescription (pDC, rectDescr);
	}

	::SelectObject (pDC->GetSafeHdc (), hfontOld);

	if (bMemDC)
	{
		//--------------------------------------
		// Copy the results to the on-screen DC:
		//-------------------------------------- 
		dc.BitBlt (rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(),
					   &dcMem, rectClip.left, rectClip.top, SRCCOPY);

		dcMem.SelectObject(pOldBmp);
	}
}

void CBCGPropListEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CBCGPPropList::OnLButtonDblClk(nFlags, point);

	if (m_pSel == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pSel);

	if (m_pSel->IsGroup ())
	{
		m_pSel->OnDblClick (point);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar

BEGIN_MESSAGE_MAP(CPropertyBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CPropertyBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
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

void CPropertyBar::EnableBar(BOOL bOpen/*=TRUE*/)
{
//	m_wndTree1.EnableWindow(bOpen);
//	m_wndTree2.EnableWindow(bOpen);
	m_wndProp.EnableWindow(bOpen);
	int n = m_wndProp.GetPropertyCount ();
	for (int i=0;i<n;i++)
		m_wndProp.GetProperty(i)->Enable(bOpen);
	m_wndProp.SetCurSel(NULL,TRUE);
m_wndProp.RemoveAll();
	//if (bOpen)

	//	AddProperties();
	m_wndProp.Invalidate(TRUE);

}

CBCGPProp* CPropertyBar::AddAppearance(CObject3D* pObj)
{
	CBCGPProp* pGroup = new CBCGPProp ("Appearance");

	CBCGPProp* pProp = new CBCGPProp ("Name", (LPCTSTR) pObj->GetObjectName(),_T(""));
	pGroup->AddSubItem (pProp);

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

	CBCGPColorProp* pColorProp = new CBCGPColorProp ("Colour", 
						pObj->pObjectShape.GetObjectColor(), &m_palColorPicker, 
						_T(""));
	pColorProp->AllowEdit(FALSE);

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	pColorProp->EnableAutomaticButton (strDef, pObj->GetDefaultColor());
	pColorProp->EnableOtherButton (strOther);

	pGroup->AddSubItem (pColorProp);

	if (pObj->MaskObject(TAllPointClass))
	{
		int index = pObj->pObjectShape.nShapeId;
		pProp = new CBCGPointProp("Shape",index,_T(""));
		pGroup->AddSubItem (pProp);
	}
	else if (pObj->MaskObject(TSphere3DClass))
	{
		int index = pObj->pObjectShape.nShapeId;
		pProp = new CBCGVolumeProp("Shape",index,_T(""));
		pGroup->AddSubItem (pProp);
	}
	else //if (pObj->MaskObject(TAllDroiteClass))
	{
		int index = pObj->pObjectShape.nShapeId;
		pProp = new CBCGLineProp("Shape",index,_T(""));
		pGroup->AddSubItem (pProp);
	}

	pProp = new CBCGPProp ("Hidden", (_variant_t) true,_T(""));
	pGroup->AddSubItem (pProp);

	pProp = new CBCGPProp ("Marked", (_variant_t) true,_T(""));
	pGroup->AddSubItem (pProp);

	pProp = new CBCGSliderProp ("Marked", 10,_T(""));
	pGroup->AddSubItem (pProp);
	

	//m_wndProp.AddProperty (pGroup1);
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

	CBCGPProp* pTopItem = NULL;
	// Set the definition group
	{
		CBCGPProp* pGroup0 = new CBCGPProp ("Definition");
	
		CString str = pObj->GetObjectDef();
		CBCGObjectProp* pProp = new CBCGObjectProp(pObj->GetObjectHelp(),pObj->GetObjectDef(),pObj->GetNameID() - IDS_NAME_OBJECT);
		pTopItem = pProp;
		pProp->Enable(FALSE);
		pProp->SetImageList(&m_pImgList);
		pGroup0->AddSubItem (pProp);
		m_wndProp.AddProperty (pGroup0);
	}

	// Set the appearance group
	m_wndProp.AddProperty (AddAppearance(pObj));

	// Set Dependents
	{
		CxObject3DSet pList;
		pObj->GetDependList(&pList,FALSE);
		int nsize = pList.GetSize();
		if (nsize > 1)
		{
			CBCGPProp* pGroup3 = new CBCGPProp ("Dependents");
			for (int i=0;i<nsize;i++)
			{
				CObject3D* pDep = (CObject3D*)pList.GetAt(i);
				if (!pDep) continue;
				if (pDep==pObj) continue;
				CBCGObjectProp* pProp = new CBCGObjectProp(pDep->GetObjectHelp(),pDep->GetObjectDef(),pDep->GetNameID() - IDS_NAME_OBJECT);
				pProp->SetImageList(&m_pImgList);
				pGroup3->AddSubItem (pProp);

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
			CBCGPProp* pGroup3 = new CBCGPProp ("Composites");
			for (int i=0;i<nsize;i++)
			{
				CObject3D* pDep = (CObject3D*)pComp->m_cSubObjects.GetAt(i);
				if (!pDep) continue;
				if (pDep==pObj) continue;
				CBCGObjectProp* pProp = new CBCGObjectProp(pDep->GetObjectHelp(),pDep->GetObjectDef(),pDep->GetNameID() - IDS_NAME_OBJECT);
				pProp->SetImageList(&m_pImgList);
				//pProp->AddSubItem  (AddAppearance(pDep));
				pGroup3->AddSubItem (pProp);

			}
			m_wndProp.AddProperty (pGroup3);
		}

		
	}
	if (pTopItem)
		m_wndProp.SetCurSel(pTopItem);
	//m_wndProp.Invalidate(TRUE);
}

void CPropertyBar::AddProperties()
{
	//CBCGProp* pGroup2 = new CBCGProp ("Object");

	CBCGPProp* pProp = new CBCGPProp ("Type", _T(""),"dfdff fdd fdfdf fddf");
	pProp->Enable(false);

	//pGroup2->AddSubItem (pProp);
	m_wndProp.AddProperty (pProp);
	
	pProp = new CBCGPProp ("Definition", _T(""),"dfdff fdd fdfdf fddf");
	pProp->Enable(false);

	m_wndProp.AddProperty (pProp);
	//pGroup2->AddSubItem (pProp);
	
	//m_wndProp.AddProperty (pGroup2);

	CBCGPProp* pGroup2 = new CBCGPProp ("dffdfdfd");
	CBCGPProp* pGroup3 = new CBCGPProp ("dffdfdfd");
	pProp = new CBCGPProp ("dddd", _T(""),"dfdff fdd fdfdf fddf");
	pGroup3->AddSubItem (pProp);
	pGroup2->AddSubItem (pGroup3);
	m_wndProp.AddProperty (pGroup2);

	CBCGPProp* pGroup1 = new CBCGPProp ("Appearance");

	pProp = new CBCGPProp ("Name", _T(""),"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);
	
	CBCGPColorProp* pColorProp = new CBCGPColorProp ("Colour", RGB(255,255,255), NULL, "dfdfd  sfddf gdsfg");
	pColorProp->AllowEdit(FALSE);
	pGroup1->AddSubItem (pColorProp);

	pProp = new CBCGPointProp ("Shape",0,"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);

	pProp = new CBCGLineProp("Line",0,"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);

	pProp = new CBCGVolumeProp("Volume",0,"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);

	pProp = new CBCGPProp ("Hidden", (_variant_t) true,"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);

	pProp = new CBCGPProp ("Marked", (_variant_t) true,"dfdff fdd fdfdf fddf");
	pGroup1->AddSubItem (pProp);

	pProp = new CBCGPProp ("dfdffdfd", _T(""),"dd");
			pProp->AddOption ("1");
			pProp->AddOption ("2");
			pProp->AddOption ("3");
			pProp->AddOption ("4");
			pProp->AddOption ("5");
	
	pGroup1->AddSubItem (pProp);

	m_wndProp.AddProperty (pGroup1);



}


/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers

int CPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
/*	if (!m_wndTabs.Create (CBCGTabWnd::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}

	m_wndTabs.SetImageList (IDB_WORKSPACE, 16, RGB (255, 0, 255));

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree1.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndTree2.Create (dwViewStyle, rectDummy, &m_wndTabs, 3))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}
*/

	//m_wndProp.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectDummy, &m_wndTabs, 4);
	m_wndProp.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP /*| WS_BORDER*/, rectDummy, this, 4);
	m_wndProp.EnableDesciptionArea(TRUE);
	m_wndProp.EnableHeaderCtrl(FALSE);
	m_wndProp.SetVSDotNetLook(TRUE);

/*
	// Setup trees content:
	HTREEITEM hRoot1 = m_wndTree1.InsertItem (_T("Root 1"));
	m_wndTree1.InsertItem (_T("Item 1"), hRoot1);
	m_wndTree1.InsertItem (_T("Item 2"), hRoot1);

	HTREEITEM hRoot2 = m_wndTree2.InsertItem (_T("Root 2"));
	m_wndTree2.InsertItem (_T("Item 1"), hRoot2);
	m_wndTree2.InsertItem (_T("Item 2"), hRoot2);

	// Attach tree windows to tab:
*/	
//	m_wndTabs.AddTab (&m_wndTree1, _T("First View"), 0 /* Image number */);
//	m_wndTabs.AddTab (&m_wndTree2, _T("Second View"), 1 /* Image number */);
//	m_wndTabs.AddTab (&m_wndProp, _T("Third View"), 1 /* Image number */);

	// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorBar::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}

	//AddProperties();
	return 0;
}

void CPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
//	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
//		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndProp.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


