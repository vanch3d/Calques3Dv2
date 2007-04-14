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

#define M_ZERO	0.0001			///< Tolerance for zero test
#define M_PI	3.1415926535	///< Internal encoding for PI

/////////////////////////////////////////////////////////////////////////////
/// Test for null value
/////////////////////////////////////////////////////////////////////////////
#define FCZero(a)	(fabsl(a) < M_ZERO)

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
class CxVectorSet : public CArray<CVector4,CVector4>
{};
//typedef CArray<CVector4,CVector4> CxVector4Set;

/////////////////////////////////////////////////////////////////////////////
/// CLocalRep
///
/////////////////////////////////////////////////////////////////////////////
class CLocalRep {
public :
	CVector4 O;		///< Origin of the local referential
	CVector4 I;		///< X-axis of the local referential
	CVector4 J;		///< Y-axis of the local referential
	CVector4 K;		///< Z-axis of the local referential
	
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
{   FCoord phi;		///< Rotation according to the Z axis (default: 25)
	FCoord theta;	///< Elevation according the the xOy plane (default: 45)
	FCoord rho;		///< Distance of the observer point of view.
	FCoord dis;		///< Distance of the projection plane.
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
			VisuRep,		///< SoR materialized by the three axes
			VisuPlane,		///< SoR materialized by an horizontal floor
			VisuClois		///< SoR materialized by three perpendicular walls
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

public:
	int				nVisuKind;		///< Nature of the SoR
	BOOL			bParProj;		///< TRUE if parallel projection, FALSE otherwise
	SProjection 	ProjParam;		///< The current projection parameters

	int				nWndWidth,		///< Width of the projection plane
					nWndHeight;		///< Height of the projection plane
	CPoint			ptRepCoord;		///< Center of the projected SoR
	int 			nCalqueNum;		///< Index of the current tracing
	FCoord 			nZoom;			///< Zoom factor of the projection

	BOOL			bDrawMark;		///< TRUE if the objects' marks are drawn, FALSE otherwise
	BOOL			bFixed;			///< TRUE if the projection parameters are fixed, FALSE otherwise
	BOOL			bKeepProj;		///< TRUE if the projection is maintained during deformation, FALSE otherwise
	BOOL			bFeedPlane;		///< TRUE if the base plane (for point creation/deformation) is materialized

	CPoint3D		*Orig;			///< A pointer to the point materializing the origin of the SoR
	CPlan3D			*F,				///< A pointer to the plane materializing the horizontal wall
					*G,				///< A pointer to the plane materializing the left wall
					*H;				///< A pointer to the plane materializing the right wall

private:
	FCoord 			ST,	///< Saving computation of sin(t)
					SP,	///< Saving computation of sin(p)
					CT,	///< Saving computation of cos(t)
					CP;	///< Saving computation of cos(p)
public:
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
	CVector4 GetScreenProjection(CVector4 vec);
	CVector4 GetScreenProjectionInf(CVector4 vec);
	void ApplyMagnet(CVector4& pt);

	virtual void Draw(CDC*) {};
	virtual void DrawFeedBack(CDC*,CPoint3D*){};
};

/////////////////////////////////////////////////////////////////////////////
/// CVisualParam
///
/////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
/// CVisualParam
///
/////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
/// CVisualParam
///
/////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
/// CVisualParam
///
/////////////////////////////////////////////////////////////////////////////
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
/// Subtract the two vectors
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
	CVector4 vec(x/N,y/N,z/N);
	vec.N = this->N;
	return vec;
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
