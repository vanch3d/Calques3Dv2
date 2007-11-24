// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

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

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1B2AE4A6_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
