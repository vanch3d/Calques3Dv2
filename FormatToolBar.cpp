// FormatToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "FormatToolBar.h"
#include "MainFrm.h"
#include "objects\Object3d.h"
#include "objects\Text3D.h"
#include "Prefs\Prefs.h"
#include "BCGPShapeMenuButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPalette	CFormatToolBar::m_palObjectColorPicker;

/////////////////////////////////////////////////////////////////////////////
// CFormatToolBar
IMPLEMENT_SERIAL(CFormatToolBar, CBCGPToolBar,  VERSIONABLE_SCHEMA | 1)

BEGIN_MESSAGE_MAP(CFormatToolBar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CFormatToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFormatToolBar::CFormatToolBar()
{
}

CFormatToolBar::~CFormatToolBar()
{
}

CBCGPToolbarFontCombo* CFormatToolBar::CreateTextFontButton ()
{
	return new CBCGPToolbarFontCombo(
				ID_FORMAT_TXTFONT, 
				CImageHash::GetImageOfCommand (ID_FORMAT_TXTFONT, FALSE),
				DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE,
				DEFAULT_CHARSET,
				WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWN |
				CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED,
				150);
}

CBCGPToolbarFontSizeCombo* CFormatToolBar::CreateTextFontSizeButton ()
{
	return new CBCGPToolbarFontSizeCombo(
				ID_FORMAT_TXTSIZE, 
				CImageHash::GetImageOfCommand (ID_FORMAT_TXTSIZE, FALSE),
				WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWN,
				40);
}

CBCGPColorMenuButton* CFormatToolBar::CreateTextColorButton ()
{
	CString strName;
	strName.LoadString(ID_FORMAT_TXTCOLOR);
	int nfind = strName.Find('\n',0);
	if (nfind!=-1) strName.Delete(0,nfind+1);
	CBCGPColorMenuButton* pColorButton = new CBCGPColorMenuButton(ID_FORMAT_TXTCOLOR,strName);

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	strDoc.LoadString(PREF_NAME_CLRDOC);
	
	pColorButton->EnableAutomaticButton (strDef, RGB (0, 0, 0));
	pColorButton->EnableOtherButton (strOther);
	pColorButton->EnableDocumentColors (strDoc);
	//pColorButton->EnableTearOff (ID_FREE_TEAROFF1, 5, 2);

	pColorButton->SetColumnsNumber (8);

	return pColorButton;
}

CBCGPColorMenuButton* CFormatToolBar::CreateObjectColorButton ()
{
	if (m_palObjectColorPicker.GetSafeHandle () == NULL)
	{
		// Create the palette
		struct 
		{
			LOGPALETTE    LogPalette;
			PALETTEENTRY  PalEntry[8];
		} pal;

		LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
		pLogPalette->palVersion    = 0x300;
		pLogPalette->palNumEntries = (WORD) 8; 

		for (int i = 0; i < 8; i++)
		{
			pLogPalette->palPalEntry[i].peRed   = GetRValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peGreen = GetGValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peBlue  = GetBValue(TPref::custColors[i]);
			pLogPalette->palPalEntry[i].peFlags = 0;
		}

		m_palObjectColorPicker.CreatePalette (pLogPalette);
	}

		// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorMenuButton::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}

	CString strName;
	strName.LoadString(ID_FORMAT_OBJCOLOR);
	int nfind = strName.Find('\n',0);
	if (nfind!=-1) strName.Delete(0,nfind+1);

	CBCGPColorMenuButton* pColorButton = new CBCGPColorMenuButton(ID_FORMAT_OBJCOLOR,strName,&m_palObjectColorPicker);

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	strDoc.LoadString(PREF_NAME_CLRDOC);

	pColorButton->EnableAutomaticButton (strDef, RGB (0, 0, 0));
	pColorButton->EnableOtherButton (strOther);
	pColorButton->EnableDocumentColors (strDoc);
	pColorButton->SetColumnsNumber (8);
	for (i = 0; i < 8; i++)
	{
		pColorButton->SetColorName(TPref::custColors[i], TPref::custColorsString[i]);
	}

	return pColorButton;
}

void CFormatToolBar::OnReset ()
{
	CBCGPToolbarFontCombo* pFontBtn = CreateTextFontButton ();
	ReplaceButton (ID_FORMAT_TXTFONT, *pFontBtn); 
	delete pFontBtn;

	CBCGPToolbarFontSizeCombo* pFontSizeBtn = CreateTextFontSizeButton ();
	ReplaceButton (ID_FORMAT_TXTSIZE, *pFontSizeBtn); 
	delete pFontSizeBtn;

	CBCGPColorMenuButton* pTextClrBtn = CreateTextColorButton ();
	ReplaceButton (ID_FORMAT_TXTCOLOR, *pTextClrBtn);
	delete pTextClrBtn;

	CBCGPColorMenuButton* pObjClrBtn = CreateObjectColorButton ();
	ReplaceButton (ID_FORMAT_OBJCOLOR, *pObjClrBtn);
	delete pObjClrBtn;

	ReplaceButton (ID_FORMAT_OBJSHAPE, CBCGPShapeMenuButton(ID_FORMAT_OBJSHAPE,1,1));

}

void CFormatToolBar::OnUpdateCmdUI(CObject3D *pObj)
{
	if (!pObj) return;

	CObList listButtons;
	CString strFontName;
	int nFontSize=-1;
	COLORREF clrText=-1;

	CText3D *pText = DYNAMIC_DOWNCAST (CText3D, pObj);
	if (pText)
	{
		clrText = pText->mColorText;
		LOGFONT lf;
		if (pText->mTextFont.GetLogFont(&lf))
		{
			CWindowDC dc (NULL);
			strFontName = CString(lf.lfFaceName);
			nFontSize = lf.lfHeight;
			nFontSize = 20*-MulDiv(lf.lfHeight, 72,dc.GetDeviceCaps(LOGPIXELSY));

		}
	}

	if (CBCGPToolBar::GetCommandButtons (ID_FORMAT_TXTFONT, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); posCombo != NULL;)
		{
			CBCGPToolbarFontCombo* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarFontCombo, listButtons.GetNext (posCombo));

			if (pCombo != NULL && !pCombo->HasFocus ())
			{
				if (pText && !strFontName.IsEmpty ())
				{
					pCombo->SetFont (strFontName, DEFAULT_CHARSET, TRUE);
				}
				else
				{
					pCombo->SetText(_T(""));
				}
			}
		}
	}

	if (CBCGPToolBar::GetCommandButtons (ID_FORMAT_TXTSIZE, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); posCombo != NULL;)
		{
			CBCGPToolbarFontSizeCombo* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarFontSizeCombo, listButtons.GetNext (posCombo));

			if (pCombo != NULL && !pCombo->HasFocus ())
			{
				if (!strFontName.IsEmpty () && pCombo->GetCount () == 0)
				{
					pCombo->RebuildFontSizes (strFontName);
				}

				pCombo->SetTwipSize(nFontSize);
			}
		}
	}

	if (CBCGPToolBar::GetCommandButtons (ID_FORMAT_TXTCOLOR, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); posCombo != NULL;)
		{
			CBCGPColorMenuButton* pButton = 
				DYNAMIC_DOWNCAST (CBCGPColorMenuButton, listButtons.GetNext (posCombo));

			if (pButton != NULL && !pButton->HasFocus ())
			{
				if (!strFontName.IsEmpty ())
				{
					pButton->SetColor(clrText);
				}

				
			}
		}
	}

	if (CBCGPToolBar::GetCommandButtons (ID_FORMAT_OBJCOLOR, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); posCombo != NULL;)
		{
			CBCGPColorMenuButton* pButton = 
				DYNAMIC_DOWNCAST (CBCGPColorMenuButton, listButtons.GetNext (posCombo));

			if (pButton != NULL && !pButton->HasFocus ())
			{
					pButton->SetColor(pObj->pObjectShape.GetObjectColor());
			}
		}
	}

	if (CBCGPToolBar::GetCommandButtons (ID_FORMAT_OBJSHAPE, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); posCombo != NULL;)
		{
			CBCGPShapeMenuButton* pButton = 
				DYNAMIC_DOWNCAST (CBCGPShapeMenuButton, listButtons.GetNext (posCombo));

			if (pButton != NULL && !pButton->HasFocus ())
			{
					pButton->SetShape(pObj->pObjectShape.GetShapeType(),pObj->pObjectShape.nShapeId);
			}
		}
	}
}

const CBCGPFontDesc* CFormatToolBar::GetFontByCmdID (UINT uiCmdID)
{
	CBCGPToolbarFontCombo* pSrcCombo = 
		(CBCGPToolbarFontCombo*) CBCGPToolbarComboBoxButton::GetByCmd (uiCmdID, FALSE);
	if (pSrcCombo == NULL)
	{
		return NULL;
	}

	return pSrcCombo->GetFontDesc ();
}

int	CFormatToolBar::GetFontSizeByCmdID (UINT uiCmdID)
{
	CBCGPToolbarFontSizeCombo* pSrcCombo = 
		(CBCGPToolbarFontSizeCombo*) CBCGPToolbarComboBoxButton::GetByCmd (uiCmdID, FALSE);
	if (pSrcCombo == NULL)
		return -1;
	else
	{
		int twips = pSrcCombo->GetTwipSize();
			CWindowDC dc (NULL);
		if (twips<0) twips = 180;
		int nFontSize = -MulDiv(twips/20, dc.GetDeviceCaps(LOGPIXELSY),72);
		return nFontSize;
	}
}

void CFormatToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CBCGPToolBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
//	CView *pView = GetTopLevelFrame ()->GetActiveView ();
//	CMDIChildWnd* pChild = ((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
//	if (pChild)
//		pChild->SetFocus ();

}

/////////////////////////////////////////////////////////////////////////////
// CFormatToolBar message handlers
