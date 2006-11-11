//////////////////////////////////////////////////////////////////////
/// @file Vector4.h
/// @brief interface of the CVector4 class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VECTOR4_H__E53DB5CF_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VECTOR4_H__E53DB5CF_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of double  as FCoord
/////////////////////////////////////////////////////////////////////////////
typedef double	FCoord;

#define M_ZERO	0.0001
#define M_PI 3.1415926535  

#define FCZero(a)	(fabsl(a) < M_ZERO)

/*typedef struct tagPOINT3D
{
    FCoord  x;
    FCoord  y;
    FCoord  z;
    FCoord  w;
    FCoord  N;
} POINT3D;*/

class CPoint3D;
class CPlan3D;

/////////////////////////////////////////////////////////////////////////////
/// CVector4
///
/////////////////////////////////////////////////////////////////////////////
class CVector4
{
public:
	FCoord	x;		///< X-coordinate of the vector
	FCoord	y;		///< Y-coordinate of the vector
	FCoord	z;		///< Z-coordinate of the vector
	FCoord	w;		///< W-coordinate of the vector
	FCoord	N;		///< Squared length of the vector

	//////////////////////////////////////////////////////////////////////
	/// Create an uninitialized vector 
	//////////////////////////////////////////////////////////////////////
	CVector4() {};

	//////////////////////////////////////////////////////////////////////
	/// Create a vector from 3 (or 4) coordinates
	//////////////////////////////////////////////////////////////////////
	CVector4(FCoord _x, FCoord _y,FCoord _z, FCoord _w=1)
		{	x = _x; y = _y; z = _z; w = _w; N=0.;
		};

	//////////////////////////////////////////////////////////////////////
	/// Create a vector from another vector
	//////////////////////////////////////////////////////////////////////
	CVector4(const CVector4 far& point)
		{	x = point.x; y = point.y;z = point.z; w = point.w;
			N = point.N;
		}

	void operator =(const CVector4& other);				// Assignment

	/// @name Equality functions/operators
	//@{
	bool operator ==(const CVector4& other) const;		// Equal vectors
	bool operator !=(const CVector4& other) const;		// Different vectors
	bool NullVector() const;							// Null vectors
    //@}

	/// @name Cast operators
	//@{
	operator CPoint() const;							// Convert to CPoint
	operator FCoord() const;							// Squared length
    //@}

	/// @name Functions/binary-operators that return Vector
	//@{
	CVector4 operator +(const CVector4& size) const;	// Add vectors	
	CVector4 operator -(const CVector4& size) const;	// Substract vectors
	CVector4 operator *(const FCoord& lambda) const;	// Multiply by scalar
	CVector4 operator %(const CVector4& vec) const;		// Vector product
	CVector4 operator -() const;						// Inverse vector
	CVector4 Normalized();								// Normalized vector (affect N)
    //@}

	/// @name Functions/binary-operators that return FCoord
	//@{
	FCoord		operator *(const CVector4& vec) const;	// Dot product
	FCoord		Norme();								// Squared length (affect N)
    //@}

	/// @name Serialization
	//@{
	friend CArchive& operator<<(CArchive& ar, const CVector4& pt);
	friend CArchive& operator>>(CArchive& ar, CVector4& pt);
    //@}

protected :
	int		round(FCoord x) const;						// Round the scalar
};


/////////////////////////////////////////////////////////////////////////////
/// Redefinition of CArray<CVector4,CVector4> as CxVector4Set
/////////////////////////////////////////////////////////////////////////////
typedef CArray<CVector4,CVector4> CxVector4Set;

/////////////////////////////////////////////////////////////////////////////
/// CLocalRep
///
/////////////////////////////////////////////////////////////////////////////
class CLocalRep {
public :
	CVector4 O;
	CVector4 I;
	CVector4 J;
	CVector4 K;
	
	CLocalRep(	CVector4 o = CVector4(0,0,0),CVector4 i = CVector4(0,0,0),
				CVector4 j = CVector4(0,0,0),CVector4 k = CVector4(0,0,0))
		{O = o; I = i; J = j; K = k;}
	CLocalRep(const CLocalRep &rep)
		{O = rep.O; I = rep.I; J = rep.J; K = rep.K;}
};



/////////////////////////////////////////////////////////////////////////////
/// TSProjection
///
/////////////////////////////////////////////////////////////////////////////
typedef struct TSProjection
{   FCoord phi;         			//0
	FCoord theta;					//0
	FCoord rho;					//400
	FCoord dis;					//400
}  SProjection;

/////////////////////////////////////////////////////////////////////////////
/// CVisualParam
///
/////////////////////////////////////////////////////////////////////////////
class CVisualParam : public CObject 
{
public:
	//////////////////////////////////////////////////////////////////////
	/// Types of System of Reference (SoR) used in Calques3D
	//////////////////////////////////////////////////////////////////////
	enum TVisuType { 
			VisuNone,		///< No SoR is visible
			VisuRep,		///< SoR materialised by the three axes
			VisuPlane,		///< SoR materialised by an horizontal floor
			VisuClois		///< SoR materialised by three perpendicular walls
		};

	//////////////////////////////////////////////////////////////////////
	/// Identifiers of the projection parameters
	//////////////////////////////////////////////////////////////////////
	enum TVisuParam {
			ID_THETA,		///< Theta angle
			ID_PHI,			///< Phi angle
			ID_RHO,			///< Distance of the projection plane
			ID_DIS			///< Distance from the observer
		};

	int				nVisuKind;			// Type of Visual
	FCoord 			ST,SP,CT,CP;		// sauvegarde des params
	BOOL			bParProj;			// Projection parallèle
	SProjection 	ProjParam;			// Paramètres de projection

	int				nWndWidth,
					nWndHeight;			// dimension de l'espace
	CPoint			ptRepCoord;			// position du centre du repere
	int 			nCalqueNum;			// numero du calque
	FCoord 			nZoom;				// Facteur de zoom de l'univers
	BOOL			bDrawMark;			// Display objects' marks
	BOOL			bFixed;				// Don't move the observer's PoV
	BOOL			bKeepProj;			// Keep a given projection when moving
	BOOL			bFeedPlane;			// TRUE if the base plane (for point creation/deplacement) is materialised

	//CWindow*		Parent;				// Fenêtre proprietaire
	CPoint3D		*Orig;
	CPlan3D			*F,*G,*H;
	CVisualParam();
	CVisualParam(const CVisualParam& param);
	CVisualParam(SProjection proj);
	virtual ~CVisualParam();
	virtual void Serialize( CArchive& ar );

	void SetProjParam();
	void SetProjParam(FCoord,int);
	void SetProjParam(SProjection);
	void AddProjParam(FCoord NewVal,TVisuParam Id);
	SProjection GetProjParam();
	virtual bool IsPointVisible(CPoint3D* pt) { return TRUE;};
	virtual bool IsPointVisible(CVector4) { return TRUE;};

	CVector4 GetEyePos();
	CVector4 ProjectPoint(CVector4 src);
	CVector4 GetProjectedPoint(CPoint MouseClic);
	void ApplyMagnet(CVector4& pt);

	virtual void Draw(CDC*) {};
	virtual void DrawFeedBack(CDC*,CPoint3D*){};
};

class CVisuNone : public CVisualParam
{
public:
	CVisuNone();
	CVisuNone(const CVisualParam& param);
	virtual ~CVisuNone() {};
	//virtual void Serialize( CArchive& ar );

	virtual void Draw(CDC*);
	virtual void DrawFeedBack(CDC*,CPoint3D*);
};

class CVisuRep : public CVisualParam
{
public:
	CVisuRep();
	CVisuRep(const CVisualParam& param);
	virtual ~CVisuRep() {};
	//virtual void Serialize( CArchive& ar );

	void DrawArrow(CDC* pDC,CVector4 p1,CVector4 p2,CVector4 p3);

	virtual void Draw(CDC*);
	virtual void DrawFeedBack(CDC*,CPoint3D*);
};

class CVisuPlane : public CVisualParam
{
public:
	CVisuPlane();
	CVisuPlane(const CVisualParam& param);
	virtual ~CVisuPlane() {};
	//virtual void Serialize( CArchive& ar );

	virtual bool IsPointVisible(CPoint3D* pt);
	virtual bool IsPointVisible(CVector4);

	void DrawOnePLane(CDC* pDC,int min1,int max1,
						int min2,int max2,int co);


	virtual void Draw(CDC*);
	virtual void DrawFeedBack(CDC*,CPoint3D*);
};

class CVisuCloison : public CVisuPlane
{
public:
	CVisuCloison();
	CVisuCloison(const CVisualParam& param);
	virtual ~CVisuCloison() {};
	
	//virtual void Serialize( CArchive& ar );
	virtual bool IsPointVisible(CPoint3D* pt);
	virtual bool IsPointVisible(CVector4);

	void DrawLineRep(CVector4 ptA[],CDC * pDC);
	void DrawScale(int order[],CDC *pDC);

	virtual void Draw(CDC*);
	virtual void DrawFeedBack(CDC*,CPoint3D*);
};


//////////////////////////////////////////////////////////////////////
/// Assign the source vector to this one
//////////////////////////////////////////////////////////////////////
inline void CVector4::operator =(const CVector4& other) {
	x=other.x;
	y=other.y;
	z=other.z;
	w=other.w;
	N=other.N;
}

//////////////////////////////////////////////////////////////////////
/// Return TRUE if the coordinates of the two vectors are equal
//////////////////////////////////////////////////////////////////////
inline bool CVector4::operator ==(const CVector4& other) const {
	return (other.x==x && other.y==y && other.z==z && other.w==w);
}

//////////////////////////////////////////////////////////////////////
/// Return TRUE if one of the coordinates of the two vectors is different
//////////////////////////////////////////////////////////////////////
inline bool CVector4::operator !=(const CVector4& other) const {
	return (other.x!=x || other.y!=y || other.z!=z || other.w!=w);
}

//////////////////////////////////////////////////////////////////////
/// Convert the vector into its squared length
//////////////////////////////////////////////////////////////////////
inline CVector4::operator FCoord() const {
	return sqrtl(x*x + y*y + z*z);
}

//////////////////////////////////////////////////////////////////////
/// Convert the vector into a point (by rounding and returning x and y)
//////////////////////////////////////////////////////////////////////
inline 	CVector4::operator CPoint() const {
	return CPoint(round(x),round(y));
}

//////////////////////////////////////////////////////////////////////
/// Return TRUE if the vector is null
//////////////////////////////////////////////////////////////////////
inline bool CVector4::NullVector() const {
	return (FCZero(x) && FCZero(y) && FCZero(z));
}


//////////////////////////////////////////////////////////////////////
/// Add the two vectors
//////////////////////////////////////////////////////////////////////
inline CVector4 CVector4::operator +(const CVector4& other) const {
	return CVector4(other.x+x,other.y+y,other.z+z,1);
}

//////////////////////////////////////////////////////////////////////
/// Substract the two vectors
//////////////////////////////////////////////////////////////////////
inline CVector4 CVector4::operator -(const CVector4& other) const {
	return CVector4(x-other.x,y-other.y,z-other.z,1);
}

//////////////////////////////////////////////////////////////////////
/// Multiply the vector by a given scalar
//////////////////////////////////////////////////////////////////////
inline CVector4 CVector4::operator *(const FCoord& lambda) const {
	return CVector4(x*lambda,y*lambda,z*lambda,1);
}

//////////////////////////////////////////////////////////////////////
/// Calculate the vector product of the two vectors
//////////////////////////////////////////////////////////////////////
inline CVector4 CVector4::operator %(const CVector4& vec) const {
	return CVector4(y*vec.z - vec.y*z,vec.x*z - x*vec.z,x*vec.y - vec.x*y,1);
}

//////////////////////////////////////////////////////////////////////
/// Inverse the vector
//////////////////////////////////////////////////////////////////////
inline CVector4 CVector4::operator -() const {
	return CVector4(-x, -y, -z,w);
}

//////////////////////////////////////////////////////////////////////
/// Normalize the vector (and assign its squared length to N)
//////////////////////////////////////////////////////////////////////
inline CVector4	CVector4::Normalized()  {
	N = (FCoord) (*this);
	return CVector4(x/N,y/N,z/N);
}

//////////////////////////////////////////////////////////////////////
/// Affect the squared length of the vector to N and return it
//////////////////////////////////////////////////////////////////////
inline FCoord	CVector4::Norme()  {
	N = (FCoord) (*this);
	return N;
}

//////////////////////////////////////////////////////////////////////
/// Calculate the dot product of the two vectors
//////////////////////////////////////////////////////////////////////
inline FCoord CVector4::operator *(const CVector4& vec) const {
	return (x*vec.x + y*vec.y + z*vec.z);
}

//////////////////////////////////////////////////////////////////////
/// Return the nearest integer of the scalar
//////////////////////////////////////////////////////////////////////
inline int CVector4::round(FCoord x) const {
	return (int) (((x-(int)x)<.44449) ? floor(x) :  ceill(x));
	//	return ceill(x);
}

#endif // !defined(AFX_VECTOR4_H__E53DB5CF_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
