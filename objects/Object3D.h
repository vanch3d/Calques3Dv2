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

/// @name Objects Identifiers
/// These identifiers are used to uniquely identify each geometrical objects.of Calques 3D 
/// 
/// All identifiers have the following form : MAKELONG(X,Y), where 
/// - X (low-order) is the category of the object (i.e. point, line, sphere, etc.)
/// - Y (high-order) is the particular type of the object (i.e. point on a line, etc.)
/// 
/// See CObject3D::MaskObject for information about how the bitwise combination of identifiers is
/// used.
//@{
const DWORD TObject3DClass			=	MAKELONG(1,1);		///< ALL geometrical objects

	const DWORD TAllPointClass		= 	MAKELONG(2,1);		///< ALL points
	const DWORD TAllBasePointClass	= 	MAKELONG(2,8674);	///< ALL base-points (ie with at leat of degree of freedom)

const DWORD TPoint3DClass			= 	MAKELONG(2,2);		///< Free point
const DWORD TPointMilieu3DClass		=	MAKELONG(2,4);		///< Point middle of a bipoint or a segment
const DWORD TPointInterDD3DClass	=	MAKELONG(2,8);		///< Point intersection of 2 lines
const DWORD TPointInterDP3DClass	=	MAKELONG(2,16);		///< Point intersection of a line and a plane
const DWORD TPointSurD3DClass 		=	MAKELONG(2,32);		///< Point on a line
const DWORD TPointSurC3DClass 		=	MAKELONG(2,64);		///< Point on a circle
const DWORD TPointSurP3DClass 		=	MAKELONG(2,128);	///< Point on a plane
const DWORD TPointSurS3DClass 		=	MAKELONG(2,256);	///< Point on a sphere
const DWORD TPointCalc3DClass 		=	MAKELONG(2,512);	///< Point internally calculated
const DWORD TPointCenter3DClass		=	MAKELONG(2,1024);	///< Point center of a circle
const DWORD TPointSymetric3DClass	=	MAKELONG(2,2048);	///< Point symetrical of a point/line/plane
const DWORD TPointTranslat3DClass	=	MAKELONG(2,4096);	///< Point translated according to a vector
const DWORD TPointSurCyl3DClass 	=	MAKELONG(2,8192);	///< Point on a cylinder


	const DWORD TAllDroiteClass		= 	MAKELONG(12,1);		///< ALL lines (includes segments)

const DWORD TDroite3DClass			= 	MAKELONG(4,2);		///< Line defined by 2 points
const DWORD TDroitePar3DClass		=	MAKELONG(4,8);		///< Line parallel to another line
const DWORD TDroiteInterPP3DClass	=	MAKELONG(4,16);		///< Line intersection of 2 planes
const DWORD TDroitePerp3DClass		=	MAKELONG(4,32);		///< Line perpendicular to a line or a plane
const DWORD TDemiDroite3DClass		=	MAKELONG(4,64);		///< Half-line defined by 2 points
const DWORD TDroitePerpDD3DClass	=	MAKELONG(4,128);	///< Lines perpendicular to 2 lines

	const DWORD TAllSegmentClass	= 	MAKELONG(8,1);		///< ALL segments
	const DWORD TSegment3DClass		=	MAKELONG(8,2);		///< Segment defined by 2 points

	const DWORD TAllPlanClass		= 	MAKELONG(16,1);		///< ALL planes
const DWORD TPlan3DClass			=	MAKELONG(16,2);		///< Plane defined by 3 points
const DWORD TPlanPerp3DClass		=	MAKELONG(16,4);		///< Plane perpendicular to a line
const DWORD TPolygon3DClass			=	MAKELONG(16,8);		///< Polygon defined by at least 3 points 

	const DWORD TAllCercleClass		=	MAKELONG(32,1);		///< ALL circles
const DWORD TCercle3DClass			=	MAKELONG(32,2);		///< Circle defined by 3 points
const DWORD TArcCercle3DClass		=	MAKELONG(32,4);		///< Arc of circle defined by 3 points
const DWORD TEllipse3DClass			=	MAKELONG(32,8);		///< Ellipse defined by the intersection of a plane and a cylinder
const DWORD TCercleInterSS3D		=	MAKELONG(32,16);	///< Circle defined by the intersection of 2 spheres


const DWORD TCompositeObject3DClass	=	MAKELONG(64,1);		///< ALL composites objects
const DWORD TCube3DClass			=	MAKELONG(64,4);		///< Cube defined by 3 points
const DWORD TInterSphDr3DClass		=	MAKELONG(64,8);		///< Bipoint defined by intersection of a sphere and a line
const DWORD TMacro3DClass			=	MAKELONG(64,16);	///< Macro-construction
const DWORD TDivSegment3DClass		=	MAKELONG(64,32);	///< Points equally spread on a segment

const DWORD TSphere3DClass			=	MAKELONG(128,1);	///< ALL spheres

const DWORD TCylindre3DClass		= 	MAKELONG(256,1);	///< ALL cylinder
const DWORD TCone3DClass			= 	MAKELONG(256,1);	///< ALL cones

const DWORD TValue3DClass			=   MAKELONG(1024,120);	///< ALL values (used in the MathPad)

const DWORD TText3DClass			= 	MAKELONG(1024,1);	///< ALL text items
const DWORD TLabel3DClass			= 	MAKELONG(1024,2);	///< Label
const DWORD TEquation3DClass		= 	MAKELONG(1024,4);	///< Equation
const DWORD TDistance3DClass		= 	MAKELONG(1024,8);	///< Distance
const DWORD TAngle3DClass			= 	MAKELONG(1024,16);	///< Angle
const DWORD TVolume3DClass			= 	MAKELONG(1024,32);	///< Volume
const DWORD TArea3DClass			= 	MAKELONG(1024,64);	///< Area
const DWORD TCComment3DClass		= 	MAKELONG(1024,128);	///< Comment
const DWORD TMathOp3DClass			= 	MAKELONG(1024,256);	///< Mathematical expression

const DWORD TLocus3DClass			= 	MAKELONG(2048,1);	///< ALL locus
//@}

	const DWORD TAllVectorClass		=	MAKELONG(4096,1);	// NOT YET USED
const DWORD TVector3DClass			= 	MAKELONG(4096,2);	// NOT YET USED 
const DWORD TVectorAdd3DClass		= 	MAKELONG(4096,4);	// NOT YET USED 
const DWORD TVectorProd3DClass		= 	MAKELONG(4096,8);	// NOT YET USED

const DWORD TUnivers3DClass			=	MAKELONG(800,2);	// NOT USED


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
	unsigned	nStepID[10];		///< A list of filters for the designation of target objects, step by step
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
/// This class is used for transfering, copying or exchanging more easily attributes (ie shape,
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
	BOOL		m_bMarked;			///< TRUE if the visual marks of the object are visibles, FALSE otherwise
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
/// All geometrical objects (like points, lines, cubes, etc.) are specialisations of this class.
/////////////////////////////////////////////////////////////////////////////
class CObject3D : public CObject  
{
	DECLARE_SERIAL(CObject3D);

public:
	//////////////////////////////////////////////////////////////////////
	/// Used to identify the type of shape of a given object
	//////////////////////////////////////////////////////////////////////
	enum TGraphType 
	{ 
		GRAPH_NONE = 0,	///< No particular highlighting
		GRAPH_PARENT,	///< Highlight the parents of the object
		GRAPH_CHILDREN,	///< Highlight the (direct) children of the object
		GRAPH_FULL,		///< Highlight all the dependents of the object
		GRAPH_BASE		///< Highlight the base points connected to the object
	};

	BOOL		bValidate;			///< TRUE if the object is analytically valide, FALSE otherwise
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
	int			nSortKind;			///< Sort order for object (0 Zorder, 1 depth) \deprecated Not used anymore
	int			nDepth;				///< Depth of the object in the dependency graph
	int			nCalque;			///< The object is extracted in Tracing X (bit X)

	CObject3D*	pComposite;			///< Pointeur to the composite this object belongs to, NULL if not

	HTREEITEM	pHistItem;			///< Item in the History View
	CRect		rGraphRect;			///< Location of the object in the Graph Window

	CxObject3DSet	cDependList;	///< List of dependents
	CxVector4Set	cTracePosition;	///< List of the object's coordinates, used for the trace.

protected:
	BOOL 		bIsSelected;		///< TRUE if the object is selected, FALSE otherwise

public:
	CObject3D();				
	CObject3D(const CObject3D & );
	virtual ~CObject3D();

	virtual CObject3D* CopyObject();
	BOOL operator == ( const CObject3D &other ) const ;
	BOOL operator < ( const CObject3D &other ) const ;

	/// @name Serialisation Functions
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
	virtual BOOL	MaskObject(DWORD mask);
	virtual DWORD	isA() const;
    //@}

	/// @name Dependence Graph Functions
	/// These functions are used to create and/or modify the dependence graph associated with a geometrical
	/// figure.
	//@{
	virtual int		SetDepth();
	virtual CxObject3DSet* GetParents();
	virtual void	GetDependList(CxObject3DSet*,BOOL bAll=TRUE);
	virtual void	GetPolygons(CxObject3DSet*);
	virtual BOOL	AddObjToDependList(CxObject3DSet* pList);
	virtual void	SetInGraph(BOOL bAdd=TRUE);
	virtual BOOL	ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
	virtual BOOL	SetParents(CxObject3DSet* pSet);
	virtual BOOL	GraftOn(CObject3D *pNew);
   //@}

	/// @name Analytical Functions
	/// These functions are used to access or modify the analytical definition of the geometrical
	/// object: coordinates of the points, equations of the lines, etc.
	//@{
	virtual UINT	CalculConceptuel();
	virtual void	CalculVisuel(CVisualParam *);
	virtual void	PrepareMoveObject(BOOL bMove = TRUE) {};
	virtual BOOL	MoveObject(CVisualParam *,UINT,CPoint ,CVector4&) { return FALSE;};
	virtual void	TranslateBy(CVector4 ptVec);
	virtual void	CopyPointPosition(CObject3D* ) {};
	virtual void	CopyPointPosition(CVector4 ) {};
	virtual BOOL	IsEqual(CObject3D &other);
	virtual UINT	IsParallelTo(CObject3D *pObj);
	virtual UINT	IsPerpendicularTo(CObject3D *pObj);
	virtual UINT	IsAlignedWith(CObject3D *pObj1,CObject3D *pObj2);
    //@}


	/// @name Region Functions
	/// These functions are used, mostly in the Universe/Traving views, to handle the detection of objects
	/// by mouse-clicks.
	//@{
	virtual CObject3D*	HitTest(CPoint pt,UINT mask=0,int nCalcNum = 0,BOOL bSub = TRUE,CxObject3DSet* pSet = NULL);
	virtual BOOL		IsInActiveArea(CPoint pt) {return FALSE;}
	virtual CRgn*		InvalideRect();
	virtual	CRgn*		DoSegRgn(CPoint p1,CPoint p2);
    //@}

	/// @name Attribute/Information Functions
	/// These functions are used to access or to modify the attributes of the geometrical object: 
	/// name, colour, shape, tracing, etc.
	//@{
	virtual BOOL		IsVisible() {return bVisible;};
	virtual void		SetVisible(BOOL bVis) { bVisible = bVis;};
	virtual BOOL		IsSelected() { return bIsSelected;};
	virtual void		SetSelected(BOOL bSel=TRUE) {bIsSelected = bSel; };
	virtual BOOL		IsInCalque(int CalcNum);
	virtual BOOL		AddInCalque(int CalcNum,BOOL add=1);
	virtual void		SetName(CString mstr);
	virtual int			SetObjectID(int nID);
	virtual void		SetColor(COLORREF rColor);
	virtual COLORREF	GetDefaultColor();
	virtual void		SetStyle(int nStyle);
	virtual void		SetAvailHisto();
	virtual void		GetRange(CVector4 &min,CVector4 &max);
	virtual int			SetProperties(CxObject3DSet *pSet=NULL);
	virtual	void		SetAttributes(CObject3DAttr pAttr);
 	virtual CObject3DAttr	GetAttributes();
  	virtual CxSchemeSet*	GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};
  //@}

	/// @name Display Functions
	/// These functions are used to display the geometrical object according to
	/// the format of the various views: Universe, Tracing, History, etc.
	//@{
	virtual void		Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void		DrawRetro(CDC *pDC,CVisualParam *vp);
	virtual void		DrawSelected(CDC* pDC,CVisualParam *vp);
	virtual HTREEITEM	DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent=TVI_ROOT);
	virtual void		ClearHistorique() { pHistItem = NULL;};
	virtual CString		DrawSymbolic(); 
	virtual void		DrawMathPad(CDC*);
	virtual void		Draw3DRendering();

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
