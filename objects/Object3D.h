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
/// @file Object3D.h
/// @brief interface of the CObject3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_OBJECT3D_H__E53DB5C8_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_OBJECT3D_H__E53DB5C8_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector4.h"
#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////
/// The identifier of a geometrical object in Calques 3D.
/////////////////////////////////////////////////////////////////////////////
class CObjectId
{
public:
	UINT	m_nMajorId;			///< The major component of the identifier
	UINT	m_nMinorId;			///< The minor component of the identifier
public:

	/////////////////////////////////////////////////////////////////////////////
	/// Default constructor (set to the ALL objects identifier)
	/////////////////////////////////////////////////////////////////////////////
	CObjectId(): m_nMinorId(1),m_nMajorId(1) {};

	/////////////////////////////////////////////////////////////////////////////
	/// Identifier constructor
	/// @param maj	The major component of the identifier
	/// @param min	The minor component of the identifier
	/////////////////////////////////////////////////////////////////////////////
	CObjectId(UINT maj,UINT min): m_nMinorId(min),m_nMajorId(maj) {};
public:
	//////////////////////////////////////////////////////////////////////
	/// Assign the source identifier to this one
	//////////////////////////////////////////////////////////////////////
	void operator =(const CObjectId& other) 
			{m_nMajorId = other.m_nMajorId; m_nMinorId = other.m_nMinorId;};

	//////////////////////////////////////////////////////////////////////
	/// Return TRUE if both major and minor components of both identifiers are equal
	//////////////////////////////////////////////////////////////////////
	bool operator ==(const CObjectId& other) const
			{ return (m_nMajorId == other.m_nMajorId && m_nMinorId == other.m_nMinorId); };

	//////////////////////////////////////////////////////////////////////
	/// Bitwise OR operation on the identifier
	//////////////////////////////////////////////////////////////////////
	CObjectId operator |(const CObjectId& other) const
			{
				CObjectId newid;
				newid.m_nMajorId = m_nMajorId|other.m_nMajorId;
				newid.m_nMinorId = m_nMinorId|other.m_nMinorId;
				return newid;
			};

	//////////////////////////////////////////////////////////////////////
	/// Bitwise OR assignment operation on the identifier
	//////////////////////////////////////////////////////////////////////
	CObjectId operator |=(const CObjectId& other)
			{
				m_nMajorId |= other.m_nMajorId;
				m_nMinorId |= other.m_nMinorId;
				return *this;
			};
};

/// @name Objects Identifiers
/// These identifiers are used to uniquely identify each geometrical objects of Calques 3D. 
/// 
/// All identifiers have the following form : CObjectId(X,Y), where 
/// - X (major) is the category of the object (i.e. point, line, sphere, etc.)
/// - Y (minor) is the particular type of the object (i.e. point on a line, etc.)
/// 
/// @see CObject3D::MaskObject for information about how the bitwise combination of identifiers is
/// used.
//@{
const CObjectId TObject3DClass			=	CObjectId(1,1);			///< ALL geometrical objects

	const CObjectId TAllPointClass		= 	CObjectId(2,1);			///< ALL points
	const CObjectId TAllBasePointClass	= 	CObjectId(2,8674);		///< ALL base-points (ie with at least of degree of freedom)

const CObjectId TPoint3DClass			= 	CObjectId(2,2);			///< Free point
const CObjectId TPointMilieu3DClass		=	CObjectId(2,4);			///< Point middle of a bi-point or a segment
const CObjectId TPointInterDD3DClass	=	CObjectId(2,8);			///< Point intersection of 2 lines
const CObjectId TPointInterDP3DClass	=	CObjectId(2,16);		///< Point intersection of a line and a plane
const CObjectId TPointSurD3DClass 		=	CObjectId(2,32);		///< Point on a line
const CObjectId TPointSurC3DClass 		=	CObjectId(2,64);		///< Point on a circle
const CObjectId TPointSurP3DClass 		=	CObjectId(2,128);		///< Point on a plane
const CObjectId TPointSurS3DClass 		=	CObjectId(2,256);		///< Point on a sphere
const CObjectId TPointCalc3DClass 		=	CObjectId(2,512);		///< Point internally calculated
const CObjectId TPointCenter3DClass		=	CObjectId(2,1024);		///< Point center of a circle
const CObjectId TPointSymetric3DClass	=	CObjectId(2,2048);		///< Point symmetrical of a point/line/plane
const CObjectId TPointTranslat3DClass	=	CObjectId(2,4096);		///< Point translated according to a vector
const CObjectId TPointSurCyl3DClass 	=	CObjectId(2,8192);		///< Point on a cylinder
const CObjectId TPointSurCone3DClass 	=	CObjectId(2,16384);		///< Point on a cone


	const CObjectId TAllDroiteClass		= 	CObjectId(12,1);	///< ALL lines (includes segments)

const CObjectId TDroite3DClass			= 	CObjectId(4,2);			///< Line defined by 2 points
const CObjectId TDroitePar3DClass		=	CObjectId(4,8);			///< Line parallel to another line
const CObjectId TDroiteInterPP3DClass	=	CObjectId(4,16);		///< Line intersection of 2 planes
const CObjectId TDroitePerp3DClass		=	CObjectId(4,32);		///< Line perpendicular to a line or a plane
const CObjectId TDemiDroite3DClass		=	CObjectId(4,64);		///< Half-line defined by 2 points
const CObjectId TDroitePerpDD3DClass	=	CObjectId(4,128);		///< Lines perpendicular to 2 lines

	const CObjectId TAllSegmentClass	= 	CObjectId(8,1);			///< ALL segments
	const CObjectId TSegment3DClass		=	CObjectId(8,2);			///< Segment defined by 2 points

	const CObjectId TAllPlanClass		= 	CObjectId(16,1);		///< ALL planes
const CObjectId TPlan3DClass			=	CObjectId(16,2);		///< Plane defined by 3 points
const CObjectId TPlanPerp3DClass		=	CObjectId(16,4);		///< Plane perpendicular to a line
const CObjectId TPolygon3DClass			=	CObjectId(16,8);		///< Polygon defined by at least 3 points 

	const CObjectId TAllCercleClass		=	CObjectId(32,1);		///< ALL circles
const CObjectId TCercle3DClass			=	CObjectId(32,2);		///< Circle defined by 3 points
const CObjectId TArcCercle3DClass		=	CObjectId(32,4);		///< Arc of circle defined by 3 points
const CObjectId TEllipse3DClass			=	CObjectId(32,8);		///< Ellipse defined by the intersection of a plane and a cylinder
const CObjectId TCercleInterSS3D		=	CObjectId(32,16);		///< Circle defined by the intersection of 2 spheres
const CObjectId TCercleInterPS3D		=	CObjectId(32,32);		///< Circle defined by the intersection of a plane and a sphere


const CObjectId TCompositeObject3DClass	=	CObjectId(64,1);		///< ALL composites objects
const CObjectId TCube3DClass			=	CObjectId(64,4);		///< Cube defined by 3 points
const CObjectId TInterSphDr3DClass		=	CObjectId(64,8);		///< Bi-point defined by intersection of a sphere and a line
const CObjectId TMacro3DClass			=	CObjectId(64,16);		///< Macro-construction
const CObjectId TDivSegment3DClass		=	CObjectId(64,32);		///< Points equally spread on a segment
const CObjectId TInterCircDr3DClass		=	CObjectId(64,64);		///< Bi-point defined by intersection of a circle and a line
const CObjectId TInterCircPl3DClass		=	CObjectId(64,128);		///< Bi-point defined by intersection of a circle and a plane
const CObjectId TInterConeDr3DClass		=	CObjectId(64,256);		///< Bi-point defined by intersection of a cone and a line

const CObjectId TSphere3DClass			=	CObjectId(128,1);		///< ALL spheres

const CObjectId TCylindre3DClass		= 	CObjectId(256,1);		///< ALL cylinder
const CObjectId TCone3DClass			= 	CObjectId(256,2);		///< ALL cones

const CObjectId TValue3DClass			=   CObjectId(1024,120);	///< ALL values (used in the MathPad)

const CObjectId TText3DClass			= 	CObjectId(1024,1);		///< ALL text items
const CObjectId TLabel3DClass			= 	CObjectId(1024,2);		///< Label
const CObjectId TEquation3DClass		= 	CObjectId(1024,4);		///< Equation
const CObjectId TDistance3DClass		= 	CObjectId(1024,8);		///< Distance
const CObjectId TAngle3DClass			= 	CObjectId(1024,16);		///< Angle
const CObjectId TVolume3DClass			= 	CObjectId(1024,32);		///< Volume
const CObjectId TArea3DClass			= 	CObjectId(1024,64);		///< Area
const CObjectId TCComment3DClass		= 	CObjectId(1024,128);	///< Comment
const CObjectId TMathOp3DClass			= 	CObjectId(1024,256);	///< Mathematical expression

const CObjectId TLocus3DClass			= 	CObjectId(2048,1);		///< ALL locus
//@}

	const CObjectId TAllVectorClass		=	CObjectId(4096,1);		// NOT YET USED
const CObjectId TVector3DClass			= 	CObjectId(4096,2);		// NOT YET USED 
const CObjectId TVectorAdd3DClass		= 	CObjectId(4096,4);		// NOT YET USED 
const CObjectId TVectorProd3DClass		= 	CObjectId(4096,8);		// NOT YET USED

const CObjectId TUnivers3DClass			=	CObjectId(800,2);		// NOT USED


class CObject3D;
class CRedefineScheme;
class CVisualParam;

/////////////////////////////////////////////////////////////////////////////
/// List of geometrical objects
/////////////////////////////////////////////////////////////////////////////
//typedef CTypedPtrArray<CObArray,CObject3D*>			CxObject3DSet;
class CxObject3DSet : public CTypedPtrArray<CObArray,CObject3D*>
{};

/////////////////////////////////////////////////////////////////////////////
/// List of possible redefinition schemes for an object
/////////////////////////////////////////////////////////////////////////////
//typedef CTypedPtrArray<CObArray,CRedefineScheme*>	CxSchemeSet;
class CxSchemeSet : public CTypedPtrArray<CObArray,CRedefineScheme*>
{};


/////////////////////////////////////////////////////////////////////////////
/// Implementation of a redefinition schema, specifying how an object can be geometrically redefined.
/// 
/// The redefinition of a object consists in relaxing or adding ONE constraint 
/// in the geometrical relations that link it with other objects in the construction. 
/// This class contains all information required for performing, step-by-step, the task. It includes the instance 
/// of the new object to be created, the resource IDs for the task and steps descriptions, the number and type
/// of the target objects - if any - to designate for adding constraints.
///
/// See
///		- the task CRedefine3DTask for details of the redefinition procedure.
///		- the method CPoint3D::GetRedefineSchemes and derived versions for implementations of redefinition sets.
///
/// \todo Replace the static array of IDs by a dynamic set
/////////////////////////////////////////////////////////////////////////////
class CRedefineScheme : public CObject  
{
public:
	CObject3D*	pNewObj;			///< An instance of the new object to create, one the task successful
	int			nObjResID;			///< The resource ID for the name of the redefinition task 
	unsigned	nTaskResID[10];		///< A list of resource ID for the names of target objects, step by step
	CObjectId	nStepID[10];		///< A list of filters for the designation of target objects, step by step
	unsigned	nAvail;				///< The ID of the corresponding construction command (to check availability of target objects)
	int			nRedefStep;			///< The number of steps in the designation of target objects (assumed to be 1)
	BOOL		bRelax;				///< TRUE if the task consists in relaxing a constraint, FALSE otherwise

	CRedefineScheme();
	~CRedefineScheme();

	DECLARE_SERIAL(CRedefineScheme);
};


/////////////////////////////////////////////////////////////////////////////
/// Extract all the visual attributes of a geometrical object into a single class.
/// 
/// This class is used for transferring, copying or exchanging more easily attributes (ie shape,
/// color, markers, etc.) between objects.
///
/// See CCalques3DDoc::ModifyPropObject and the CCopyShapeTask task.
///
/// \todo Replace the individual attributes of the CObject3CD class by CObject3DAttr
/////////////////////////////////////////////////////////////////////////////
class CObject3DAttr: public CObject 
{
public:
	BOOL		m_bVisible;			///< TRUE if the object is visible, FALSE otherwise
	BOOL		m_bMarked;			///< TRUE if the visual marks of the object are visible, FALSE otherwise
	CString		m_strObjectName;	///< The name of the object
	int			m_nCalque;			///< The object is extracted in tracing X (bit X)
	CShape		m_pObjectShape;		///< The shape of the object

public:
	/////////////////////////////////////////////////////////////////////////////
	/// Default constructor
	/////////////////////////////////////////////////////////////////////////////
	CObject3DAttr() {};

	/////////////////////////////////////////////////////////////////////////////
	/// Copy constructor
	/// @param pObj	A reference to the object's attribute to be duplicated
	/////////////////////////////////////////////////////////////////////////////
	CObject3DAttr(const CObject3DAttr& pObj)
		{	m_bVisible = pObj.m_bVisible; 
			m_bMarked = pObj.m_bMarked;
			m_strObjectName = pObj.m_strObjectName; 
			m_nCalque = pObj.m_nCalque;
			m_pObjectShape = pObj.m_pObjectShape;
		};

	void operator =(const CObject3DAttr& other);

};

//////////////////////////////////////////////////////////////////////
/// Assignment operator
///
/// @param pObj	A reference to the object's attribute to be duplicated
//////////////////////////////////////////////////////////////////////
inline void CObject3DAttr::operator =(const CObject3DAttr& other)
{
	m_bVisible = other.m_bVisible; 
	m_bMarked = other.m_bMarked;
	m_strObjectName = other.m_strObjectName; 
	m_nCalque = other.m_nCalque;
	m_pObjectShape = other.m_pObjectShape;
}


/////////////////////////////////////////////////////////////////////////////
/// Implements a generic 3-dimensional geometrical objects.
///
/// This class is the central piece of Calques 3D, specifying the properties and procedures that
/// define the behavior of any geometrical objects.
///
/// Methods of this class are divided in several categories:
///	- Identification of the object by defining its name, its description, its designation, etc.
/// - Management of the object in the dependence graph that underlies any geometrical construction.
/// - Calculation and verification of the analytical representation of the object
/// - Display of the object in the different views available in Calques 3D
/// - Definition and modification of the attributes (shape, color, etc.) of the object
///
/// All geometrical objects (like points, lines, cubes, etc.) are specializations of this class.
/////////////////////////////////////////////////////////////////////////////
class CObject3D : public CObject  
{
	DECLARE_SERIAL(CObject3D);

public:
	//////////////////////////////////////////////////////////////////////
	/// Used to identify the type of graph highlighted with the object.
	/// @see CObject3D::DrawDepGraph()
	//////////////////////////////////////////////////////////////////////
	enum TGraphType 
	{ 
		GRAPH_NONE = 0,	///< No particular highlighting
		GRAPH_PARENT,	///< Highlight the parents of the object
		GRAPH_CHILDREN,	///< Highlight the (direct) children of the object
		GRAPH_FULL,		///< Highlight all the dependents of the object
		GRAPH_BASE		///< Highlight the base points connected to the object
	};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the rendering mode for volumes in the OpenGL view
	/// @see CObject3D::Draw3DRendering()
	//////////////////////////////////////////////////////////////////////
	enum TRenderType 
	{ 
		RENDER_SILHOUETTE = 0,	///< Volumes are rendered as wireframe
		RENDER_FILL,			///< Volumes are rendered as plain solids
		RENDER_STIPPLE			///< Volumes are rendered as translucent shapes
	};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify modification of the object's attribute(s)
	//////////////////////////////////////////////////////////////////////
	enum TAttributeType 
	{ 
		ATTRIB_NONE = 0,	///< No attribute modified
		ATTRIB_NAME,		///< The name of the object is modified
		ATTRIB_COLOR,		///< The color of the object is modified
		ATTRIB_SHAPE,		///< The shape of the object is modified
		ATTRIB_MARK,		///< The visual mark(s) of the object is modified
		ATTRIB_VISIBLE,		///< The visibility state of the object is modified
		ATTRIB_LOCUS		///< The size of the locus is modified
	};

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the format for the symbolic export of construction
	/// @see CObject3D::ExportSymbolic()
	//////////////////////////////////////////////////////////////////////
	enum TExportType
	{
		EXPORT_MATHEMATICA = 0,	///< Mathematica export - used for property discovery
		EXPORT_MAPLE,			///< Maple export - used for property discovery
		EXPORT_DOT				///< DOT export - used for graph visualization 
	};

	BOOL		bValidate;			///< TRUE if the object is analytically valid, FALSE otherwise
	BOOL		bVisible;			///< TRUE if the object is visible, FALSE if it is hidden
	BOOL		bMarked;			///< TRUE if the object has its visual marks activated, FALSE otherwise 
	BOOL		bDrawInGraph;		///< TRUE if the object can be displayed in the Graph, FALSE otherwise
	BOOL		bNotInHisto;		///< TRUE if the object cannot be displayed in the History, FALSE otherwise
	BOOL		bShowTrace;			///< TRUE if the visual marks of the object are displayed, FALSE otherwise

	unsigned	nObjectId;			///< Unique ID of the object (usually index in the document list)

	CString		strObjectName;		///< Name of the object
	CString		strObjectHelp;		///< Designation
	CString		strObjectDef;		///< Definition
	CShape		pObjectShape;		///< Shape and color of the object
	int			nSortKind;			///< Sort order for object (0 Z-order, 1 depth) \deprecated Not used anymore
	int			nDepth;				///< Depth of the object in the dependency graph
	int			nCalque;			///< The object is extracted in Tracing X (bit X)

	CObject3D*	pComposite;			///< Pointer to the composite this object belongs to, NULL if not

	HTREEITEM	pHistItem;			///< Item in the History View
	CRect		rGraphRect;			///< Location of the object in the Graph Window

	CxObject3DSet	cDependList;	///< List of dependents
	CxVectorSet	cTracePosition;	///< List of the object's coordinates, used for the trace.

protected:
	BOOL 		bIsSelected;		///< TRUE if the object is selected, FALSE otherwise

public:
	CObject3D();				
	CObject3D(const CObject3D & );
	virtual ~CObject3D();

	virtual CObject3D* CopyObject();
	BOOL operator == ( const CObject3D &other ) const ;
	BOOL operator < ( const CObject3D &other ) const ;

	/// @name Serialization Functions
	/// These functions are used to store/retrieve the geometrical object.
	//@{
	virtual void Serialize( CArchive& ar );
	virtual CObject3D* SerializeObj(CArchive& ar);
    //@}

	/// @name Identification Functions
	/// These functions are used to create and/or modify the dependence graph associated with a geometrical
	/// figure.
	//@{
	virtual UINT	GetNameID() const;
	virtual UINT	GetHelpID() const;
	virtual UINT	GetDefID() const;
	virtual CString	GetObjectDef();
	virtual CString	GetObjectHelp();
	virtual CString	GetObjectName();
	virtual CString	GetObjectNameRedux();
	virtual BOOL	MaskObject(CObjectId mask);
	virtual CObjectId	isA() const;
    //@}

	/// @name Dependence Graph Functions
	/// These functions are used to create and/or modify the dependence graph associated with a geometrical
	/// figure.
	//@{
	virtual int		SetDepth();
	virtual void	GetDependList(CxObject3DSet*,BOOL bAll=TRUE);
	virtual void	GetPolygons(CxObject3DSet*);
	virtual BOOL	AddObjToDependList(CxObject3DSet* pList);
	virtual void	SetInGraph(BOOL bAdd=TRUE);
	virtual BOOL	GraftOn(CObject3D *pNew);
	virtual BOOL	ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
	virtual BOOL	SetParents(CxObject3DSet* pSet);
	virtual CxObject3DSet* GetParents();
   //@}

	/// @name Analytical Functions
	/// These functions are used to access or modify the analytical definition of the geometrical
	/// object: coordinates of the points, equations of the lines, etc.
	//@{
	virtual UINT	CalculConceptuel();
	virtual void	CalculVisuel(CVisualParam *);
	virtual void	GetRange(CVector4 &min,CVector4 &max);
	virtual BOOL	IsEqual(CObject3D &other);
	virtual UINT	IsParallelTo(CObject3D *pObj);
	virtual UINT	IsPerpendicularTo(CObject3D *pObj);
	virtual UINT	IsAlignedWith(CObject3D *pObj1,CObject3D *pObj2);
	virtual void	PrepareMoveObject(BOOL bMove = TRUE);
	virtual BOOL	MoveObject(CVisualParam *pVisual,UINT nModKey,CPoint pt,CVector4& loc);
	virtual void	TranslateBy(CVector4 ptVec);
	virtual void	CopyPointPosition(CObject3D* pObj);
	virtual void	CopyPointPosition(CVector4 ptLoc);
    //@}


	/// @name Region Functions
	/// These functions are used, mostly in the Universe/Tracing views, to handle the detection of objects
	/// by mouse-clicks.
	//@{
	virtual CObject3D*	HitTest(CPoint pt,CObjectId mask,int nCalcNum = 0,BOOL bSub = TRUE,CxObject3DSet* pSet = NULL);
	virtual BOOL		IsInActiveArea(CPoint pt);
	virtual CRgn*		InvalideRect();
	static  CRgn*		DoSegRgn(CPoint p1,CPoint p2);
    //@}

	/// @name Attribute/Information Functions
	/// These functions are used to access or to modify the attributes of the geometrical object: 
	/// name, color, shape, tracing, etc.
	//@{
	virtual BOOL		IsVisible();
	virtual void		SetVisible(BOOL bVis);
	virtual void		SetValidate(UINT nCode);
	virtual BOOL		IsSelected();
	virtual void		SetSelected(BOOL bSel=TRUE);
	virtual BOOL		IsInCalque(int CalcNum);
	virtual BOOL		AddInCalque(int CalcNum,BOOL bAdd=TRUE);
	virtual void		SetName(CString strName);
	virtual int			SetObjectID(int nID);
	virtual void		SetColor(COLORREF rColor);
	virtual COLORREF	GetDefaultColor();
	virtual void		SetStyle(int nStyle);
	virtual int			SetProperties(CxObject3DSet *pSet=NULL);
	virtual	void		SetAttributes(CObject3DAttr pAttr);
 	virtual CObject3DAttr	GetAttributes();
  	virtual CxSchemeSet*	GetRedefineSchemes(CxSchemeSet* pSet);
  //@}

	/// @name Display Functions
	/// These functions are used to display the geometrical object according to
	/// the format of the various views: Universe, Tracing, History, etc.
	//@{
	virtual void		Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void		DrawRetro(CDC *pDC,CVisualParam *vp);
	virtual void		DrawSelected(CDC* pDC,CVisualParam *vp);
	virtual void		SetHistoryVisibility();
	virtual HTREEITEM	DrawHistory(CTreeCtrl& mListCtrl,HTREEITEM pParent=TVI_ROOT);
	virtual void		ClearHistory() { pHistItem = NULL;};
	virtual CString		ExportSymbolic(int nFormat); 
	virtual void		DrawMathPad(CDC* pDC);
	virtual void		Draw3DRendering(int nVolMode);

	virtual void		DrawDepGraph(CDC* pDC,CImageList *pImgList=NULL,int nTrace=GRAPH_NONE,BOOL bDrawNode=TRUE,BOOL bDrawLink=TRUE);
	virtual void		DrawDepGraphLink(CDC* pDC,CObject3D *pSrc=NULL,CObject3D *pDest=NULL,int nTrace=GRAPH_NONE);
	virtual void		DrawDepGraphNode(CDC* pDC,CImageList *pImgList=NULL,CObject3D *pSrc=NULL,int nTrace=GRAPH_NONE);
	void				DrawGraphArrow(CDC *pDC,CPoint ptStart, CPoint ptEnd, int nArrowSize ,int nArrowlength,CBrush* pbrush);
    //@}

	/// @name Error Functions
	/// These functions are used to provide information about errors resulting from the
	/// object construction or specification
	//@{
	virtual void	HandleObjectError(int,BOOL bShow=FALSE);
	virtual void	TestAcces();
    //@}
};


#endif // !defined(AFX_OBJECT3D_H__E53DB5C8_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
