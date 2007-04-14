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
/// @file Shape.h
/// @brief interface of the CShape class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SHAPE_H__4315D5D6_71DA_4852_AE32_7A1F1725A20D__INCLUDED_)
#define AFX_SHAPE_H__4315D5D6_71DA_4852_AE32_7A1F1725A20D__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

//////////////////////////////////////////////////////////////////////
/// Build and draw the various shapes of geometrical objects
//////////////////////////////////////////////////////////////////////
class CShape : public CObject  
{
public:
	DECLARE_SERIAL(CShape);

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the type of shape of a given object
	//////////////////////////////////////////////////////////////////////
	enum TShapeType { 
				LineShape,		///< Shapes for lines and planes
				PointShape,		///< Shapes for points
				VolShape,		///< Shapes for volumes
				PolShape		///< Shapes for polygons
			};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the shape of a line
	//////////////////////////////////////////////////////////////////////
	enum TShapeLine { 
			LineDot,		///< 
			LineDash,		///<
			LineDashDot,	///<
			LineDashDotDot,	///<
			Line1,			///<
			Line2,			///<
			Line3,			///<
			Line4			///<
		};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the shape of a point
	//////////////////////////////////////////////////////////////////////
	enum TShapePoint{ 
			PointCirclePlain,	///<
			PointCircle,		///<
			PointSquarePlain,	///<
			PointSquare,		///<
			PointCrossX,		///<
			PointCross			///<
		};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the shape of a composite
	//////////////////////////////////////////////////////////////////////
	enum TShapeVolume{ 
			VolTransparent,	///<
			VolOpaque,		///<
			VolComposite	///<
		};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the shape of a polygon
	//////////////////////////////////////////////////////////////////////
	enum TShapePolygon{ 
			PolTransparent,	///<
			PolOpaque,		///<
			PolDot,			///<
			PolDash,		///<
			PolDashDot,		///<
			PolDashDotDot,	///<
			PolLine1,		///<
			PolLine2,		///<
			PolLine3,		///<
			PolLine4		///<
		};

	COLORREF	clrObject;		///< The color of the geometrical object
	int 		nShapeId;		///< Zero-based index of the shape of the geometrical object
	int 		nShapeNb;		///< Number of possibles shapes for this object
protected:
	int			nShapeType;		///< The type of shape associated with the object
	int			nShapeDef;		///< Zero-based index of the default shape for the object
public:
	CShape(int nType = LineShape,int id=4);
	CShape(CShape& src);
	virtual void Serialize( CArchive& ar );

	void SetShapeType(int nType=LineShape);

	//////////////////////////////////////////////////////////////////////
	/// Get the type of shape associated with the object
	/// @return The zero-based identifier of the shape (TShapeType)
	//////////////////////////////////////////////////////////////////////
	int GetShapeType() { return nShapeType;};

	virtual void		Draw(CDC*,CRect);
	//virtual void		DrawLine(CDC*,CRect);
	//virtual void		DrawObj(CDC*,CRect);
	virtual CString		GetShapeDef(int i=-1);
	virtual	COLORREF	GetObjectColor();
	virtual	COLORREF	GetObjectHiddenColor();
	virtual int			GetShapeSize() { return nShapeNb;} ;
	virtual int			GetColorIdx();

	virtual void operator =(const CShape& other);

	LOGPEN GetPenStyle(int nb=-1);
	LOGPEN GetHiddenPenStyle(COLORREF clr = RGB(192,192,192));

	static CString GetShapeDef(int type,int index);
	static void DrawShapes(int type,CDC* pDC, CRect rect,int nIndex,BOOL bEnable);
private:
	virtual void DrawPoint(CDC*,CRect);

	static void DrawShapesLn(CDC* pDC, CRect rect,int nIndex,BOOL bEnable);
	static void DrawShapesPt(CDC* pDC, CRect rect,int nIndex,BOOL bEnable);
	static void DrawShapesVol(CDC* pDC, CRect rect,int nIndex,BOOL bEnable);
	static void DrawShapesPol(CDC* pDC, CRect rect,int nIndex,BOOL bEnable);
};

#endif // !defined(AFX_SHAPE_H__4315D5D6_71DA_4852_AE32_7A1F1725A20D__INCLUDED_)
