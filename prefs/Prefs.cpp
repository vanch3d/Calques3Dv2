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
// Prefs.cpp: implementation of the TPref class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TPref::ColourTableEntry TPref::crColours[40] = 
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

BOOL	TPref::VerifyObject = 0;		//signal de création
BOOL	TPref::GrayedHidden=0;

COLORREF	TPref::custColors[16] =
 {
	RGB(255,0,0),RGB(0,192,0),RGB(0,0,255),
	RGB(192,192,0),RGB(0,192,192),RGB(255,0,255),
	RGB(0,0,0),RGB(64,64,64),
	RGB(255,128,128),RGB(128,255,128),RGB(128,128,255),
	RGB(255,255,128),RGB(128,255,255),RGB(255,128,255),
	RGB(128,128,128),RGB(64,64,64)
 };

CList<COLORREF,COLORREF> TPref::docColors;

CString 	TPref::custColorsString[8] =
{
	CString("Red"),CString("Green"),CString("Blue"),
	CString("Yellow"),CString("Skyblue"),CString("Violet"),
	CString("Black"),CString("Grey")
};

COLORREF TPref::Color = RGB(0,0,0);

TPref::CTPoint3D	TPref::TPoint =		{RGB(0,0,0),3,2,0,3};
TPref::CTLine3D		TPref::TLine =		{RGB(0,0,255)};
TPref::CTCercle3D	TPref::TCercle =	{RGB(0,0,255),0,40};
TPref::CTCube3D		TPref::TCube =		{RGB(0,0,255),1};
TPref::CTSphere3D	TPref::TSphere =	{RGB(0,0,255),0,TRUE,TRUE};
TPref::CTLocus3D	TPref::TLocus =		{RGB(255,0,0),20};
TPref::CTCone3D		TPref::TCone =		{RGB(0,0,255),FALSE,24};

TPref::CTHistory	TPref::THistory	 = 
		{
			TRUE
		};

TPref::CTMathPad	TPref::TMathPad = 
		{
			1.0,
			0,
			0,
			0,
			100,
			4,
			0
		};


TPref::CTUniverse	TPref::TUniv = 
		{
			RGB(128,0,0),
			RGB(0,0,0),
			{25.0,45.0,800.0,400.0},
			TPref::MV_HORIZ,
			3,
			6,
			50,
			5,
			1.0,
			FALSE,
			TRUE,
			FALSE,
			FALSE
		};

TPref::CTVerif      TPref::Verif;


BOOL TPref::bMacroLoading = TRUE;
CObList	TPref::TMacroList;
CObList	TPref::TPOVList;


CString	TPref::strProfile = _T("");
CString	TPref::strProfileDesc = _T("");

CStringArray TPref::strUnits;

//////////////////////////////////////////////////////////////////////
/// Default initialisation of the parameters (if applicable)
//////////////////////////////////////////////////////////////////////
void TPref::DefaultInit()
{
	TPref::Verif.strCommand = _T("command.com /E:2048 /C");
	TPref::Verif.strRunTime = _T("c:\\progra~1\\cocoa-4.1\\cocoa.bat -q");
	TPref::Verif.strConvert = _T("test.coc");
	TPref::Verif.strResult = _T("my-file.txt");

	TPref::strUnits.Add(_T("< none >"));
	TPref::strUnits.Add(_T("cm"));
	TPref::strUnits.Add(_T("mm"));

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMacroUserTool, CObject, VERSIONABLE_SCHEMA | 1)

CMacroUserTool::CMacroUserTool()
{
	m_uiCmdId = 0;
	m_hIcon = NULL;
}

CMacroUserTool::~CMacroUserTool()
{
	//DeleteIcon ();
}

void CMacroUserTool::Serialize (CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_strLabel;
		ar >> m_strFile;
		ar >> m_strDef;
		ar >> m_uiCmdId;
	}
	else
	{
		ar << m_strLabel;
		ar << m_strFile;
		ar << m_strDef;
		ar << m_uiCmdId;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPOVUserTool, CObject, VERSIONABLE_SCHEMA | 1)

CPOVUserTool::CPOVUserTool()
{
}

CPOVUserTool::~CPOVUserTool()
{
	//DeleteIcon ();
}

void CPOVUserTool::Serialize (CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading ())
	{
		//ar >> m_strLabel;
	}
	else
	{
		//ar << m_strLabel;
	}
}
