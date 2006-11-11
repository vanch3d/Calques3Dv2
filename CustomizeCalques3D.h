// CustomizeCalques3D.h: interface for the CCustomizeCalques3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_)
#define AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// List of Custom pages to add in the Preference dialog box
/////////////////////////////////////////////////////////////////////////////
typedef CList <CRuntimeClass*, CRuntimeClass*> CustomPageList;

#define BCGCUSTOMIZE_DEFAULT	(BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | \
								 BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS)

/////////////////////////////////////////////////////////////////////////////
/// Main dialog for the customization of Calques3D
///
/// For the preferences of Calques 3D per se, see CPrefMacros, CPrefMathPad, CPrefObjShape, 
/// CPrefSession and CPrefUniverse.
/////////////////////////////////////////////////////////////////////////////
class CCustomizeCalques3D : public CBCGPToolbarCustomize  
{
// Construction
public:
	CCustomizeCalques3D(CFrameWnd* pWndParentFrame, 
						BOOL bAutoSetFromMenus = FALSE,
						UINT uiFlags = BCGCUSTOMIZE_DEFAULT,
						CustomPageList* plistCustomPages = NULL);

	void InitCalques3DPrefs();
};

#endif // !defined(AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_)
