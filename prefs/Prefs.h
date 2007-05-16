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
/// @file Prefs.h
/// @brief interface of the TPref class.
///
///
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREFS_H__E53DB5E3_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFS_H__E53DB5E3_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\objects\Vector4.h"

/////////////////////////////////////////////////////////////////////////////
/// CMacroUserTool
///
/////////////////////////////////////////////////////////////////////////////
class CMacroUserTool : public CObject  
{
	DECLARE_SERIAL(CMacroUserTool)
public:
// Construction
	CMacroUserTool();
	virtual ~CMacroUserTool();

// Overrides
	virtual void Serialize (CArchive& ar);

// Attributes:
public:
	CString		m_strLabel;
	CString		m_strDef;
	CString		m_strFile;
protected:
	UINT		m_uiCmdId;
	HICON		m_hIcon;
};

/////////////////////////////////////////////////////////////////////////////
/// CPOVUserTool
///
/////////////////////////////////////////////////////////////////////////////
class CPOVUserTool : public CObject  
{
	DECLARE_SERIAL(CPOVUserTool)
public:
// Construction
	CPOVUserTool();
	virtual ~CPOVUserTool();

// Overrides
	virtual void Serialize (CArchive& ar);

// Attributes:
public:
	CString		m_strLabel;
	CString		m_strDef;
	CString		m_strFile;
};

/////////////////////////////////////////////////////////////////////////////
/// TPref
/// 
/// \todo Most of the preferences in the main class should be moved in one of the structures
/// of the class (ie Magnet into CTUniverse).
/////////////////////////////////////////////////////////////////////////////
class TPref  
{
private :
	virtual void dummy() = 0;		// To make the class static

public :

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	enum TMoveType {
		MV_HORIZ,					///< Move from the horizontal plane
		MV_LEFT,					///< Move from the left plane
		MV_RIGHT					///< Move from the right plane
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	typedef struct  {
		COLORREF crColour;			///< RGB Color
		TCHAR    *szName;			///< Color name
	} ColourTableEntry;

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTCube3D {
		COLORREF	clrDefault;		///< Default color for the cubes
		int			nShape;			///< Default shape for the cubes
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTSphere3D {
		COLORREF	clrDefault;		///< Default color for the spheres	
		int			nShape;			///< Default shape for the spheres
		BOOL		bEquator;		///< Draw or not the equator of the spheres
		BOOL		bGCircle;		///< Draw or not the great circle of the spheres
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTPoint3D {
		COLORREF	clrDefault;		///< Default color for the points
		int			nPtFree,		///< Default shape for the free-points
					nPtConst,		///< Default shape for the constrained points
					nPtDep;			///< Default shape for the dependent points
		int			nSize;			///< Default size of the points
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTCercle3D {
		COLORREF	clrDefault;		///< Default color for the circles
		int			nShape;			///< Default shape for the circles
		int			nDelta;			///< Default resolution for the circles
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTLocus3D {
		COLORREF	clrDefault;		///< Default color for the loci
		int			nDelta;			///< Default resolution for the loci
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTLine3D {
		COLORREF	clrDefault;		///< Default color for the lines
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTMathPad {
		FCoord	UnitScale;			///< Scale for numerical values
		int		ShowFraction;		///< Fraction (1) or Real (0) values
		int		UnitLinear;			///< Linear Units (0: cm, 1: mm)
		int		UnitAngular;		///< Angular Units (0: degrees, 1: radians)
		int		PrecFraction;		///< 
		int		PrecReal;			///< 
		int		nShowView;			///<	(0: ask, 1: show, 2: don't show)
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTVerif {
		CString	strCommand;			///< Command
		CString	strRunTime;			///< CAS application path
		CString strConvert;			///< COCOA Source File;
		CString strResult;			///< Result of computation
	};

	/////////////////////////////////////////////////////////////////////////////
	/// ColourTableEntry
	/////////////////////////////////////////////////////////////////////////////
	struct CTUniverse {
		COLORREF	clrTrace;		///< Color used for drawing the traces of an object
		SProjection	sDefParam;		///< Parameters for the projection on the universe view
		int			nMoveType; 		///< Axis of deformation (O horizontal, 1 left, 2 right)
		int 		nDefRep; 		///< Type of SoR (0 none, 1 axes, 2 floor, 3 walls)
		int 		nRepPas;		///< Number of units displayed in the SoR
		int 		nUnitRep;		///< Size of each unit of the SoR (in pixel)
		int			nDefPres;		///< Precision for designation area
		float		fZoom;			///< Zoom factor for the universe. 
		BOOL		bMagnet;		///< Magnetism of SoR activated
		BOOL		bSynchron;		///< Synchronize updates between views
		BOOL		bShowMarks;		///< Show the marks of objects
		BOOL		bShowPolygon;	///< TRUE if polygon are drawn plain, FALSE otherwise
	};


public :

	static COLORREF	custColors[16];				///< List of user-defined colors (8 primary + 8 hidden)
	static CString	custColorsString[8];		///< List of user-defined color names
	static CList<COLORREF,COLORREF> docColors;	///< List of other colors used in the documents
	static ColourTableEntry crColours[40];		///< List of predefined colors and (English) names 


	//static int			MoveType; 		///< Axis of deformation (O horizontal, 1 left, 2 right)
	//static BOOL			Magnet;			///< Magnetism of SoR activated
	//static int 			DefRep; 		///< Type of SoR (0 none, 1 axes, 2 floor, 3 walls)
//	static int 			NbRepPas;       ///< Number of units displayed in the SoR
//	static int 			UnitRep;		///< Size of each unit of the SoR (in pixel)
//	static int			DefPres;		///< Precision for designation area
//	static float		Zoom;			///< Zoom factor for the universe. 
//	static SProjection	DefParam;		///< Parameters for the projection on the universe view
//	static BOOL			ShowMarks;		///< Show the marks of objects
//	static BOOL			Synchron;		///< Synchronize updates between views
	static BOOL			GrayedHidden;	///< hidden part of objects in gray


	static CTCube3D		TCube;			///< Default preferences for cubes
	static CTSphere3D	TSphere;		///< Default preferences for spheres
	static CTPoint3D	TPoint;			///< Default preferences for points
	static CTCercle3D	TCercle;		///< Default preferences for circles
	static CTLocus3D	TLocus;			///< Default preferences for loci
	static CTLine3D		TLine;			///< Default preferences for lines

	static CTUniverse	TUniv;			///< Default preferences for the universe view	
	static CTMathPad	TMathPad;		///< Default preferences for the MathPad view	

	static CObList		TMacroList;		///< List of user-defined macros available
	static BOOL			bMacroLoading;	///< Allow user to load any macros
	static CString		strProfile;		///< Name of the current profile
	static CString		strProfileDesc;	///< Description of the current profile

	static CStringArray strUnits;		///< List of Units used in Calques 3D

	
	static BOOL			VerifyObject;	///< Verify if objects are duplicated before creation (NOT YET USED)
	static CObList		TPOVList;		///< List of user-defined PoV (NOT YET USED)

	static CTVerif		Verif;

//	static CString		PrefDescr;		///< Description of the Profile \deprecated
	static COLORREF		Color;			///< Default color for objects \deprecated
//	static BOOL			SignalCreation;	///< Indicate the creation of objects \deprecated
//	static int 			WndHeight,		///< Maximum size of screen \deprecated
//						WndWidth;		///< Maximum size of screen \deprecated

public :
	static void DefaultInit();
};

#endif // !defined(AFX_PREFS_H__E53DB5E3_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
