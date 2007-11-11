// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <BCGCBProInc.h>	// BCG Control Bar
#include "OGLTools/OGLT.h"	// OpenGL support

#include <afxcview.h>		// RichEdit classes

#ifdef _C3D_PARAMGEO
	#include "C:\Program Files\Maple 10\extern\include\maplec.h"
	#pragma comment(lib,"C:\\Program Files\\Maple 10\\bin.win\\maplec.lib" ) 
	#pragma message("Automatically linking with MAPLEC.lib")
#else
	//#pragma comment(lib,"BCGCBPRO731d.lib") 
	#pragma message("Warning: MAPLEC.lib is not linked; check if this is right")
#endif

#include <atlbase.h>
#import "C:\Program Files\3Dconnexion\3Dconnexion 3DxSoftware\3DxWare\win32\TDxInput.dll" no_namespace

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
