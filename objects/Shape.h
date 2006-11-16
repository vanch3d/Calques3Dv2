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
	static void DrawShapes(int type,CDC* pDC, CRect rect,int nIndex);
private:
	virtual void DrawPoint(CDC*,CRect);

	static void DrawShapesLn(CDC* pDC, CRect rect,int nIndex);
	static void DrawShapesPt(CDC* pDC, CRect rect,int nIndex);
	static void DrawShapesVol(CDC* pDC, CRect rect,int nIndex);
	static void DrawShapesPol(CDC* pDC, CRect rect,int nIndex);
};

#endif // !defined(AFX_SHAPE_H__4315D5D6_71DA_4852_AE32_7A1F1725A20D__INCLUDED_)
