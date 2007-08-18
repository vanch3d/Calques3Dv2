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
/// @file BallController.h
/// @brief interface of the CBallController class.
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_)
#define AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_

#include "3dmath.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Auxiliary Type Definitions
enum AxisSet
{
	NO_AXES,
	CAMERA_AXES,
	BODY_AXES,
	OTHER_AXES
};

//////////////////////////////////////////////////////////////////////
/// Implement the track-ball controller for the OpenGL window
///
//////////////////////////////////////////////////////////////////////
class CBallController  
{
private:
	bool bDrawConstraints;
	tmatrix bodyorientation;
	int angleKeyIncrement;
	void DrawConstraints();
	vector* GetUsedAxisSet();
//	vector vcurr; //vettori iniz e finali per il rubber banding della rotaz
//	vector vdown;
	vector BallColor;
	bool bProjectionMethod2;
	bool bDrawBallArea;
	int GLdisplayList;
	unitquaternion currentQuat;
	unitquaternion previousQuat;
	real radius;
	real winWidth;
	real winHeight;
	real xprev;
	real yprev;
	CPoint center;
	bool mouseButtonDown;
	AxisSet whichConstraints;
	int currentAxisIndex;
	vector cameraAxes[3];
	vector bodyAxes[3];
	vector* otherAxes;
	int otherAxesNum;

	void InitDisplayLists();
	void initVars(void);
	void ProjectOnSphere(vector& v) const;
	unitquaternion RotationFromMove(const vector& vfrom,const vector& vto);
	vector ConstrainToAxis(const vector& loose,const vector& axis);
	int NearestConstraintAxis(const vector& loose);
public:
	bool GetDrawConstraints();
	void SetDrawConstraints(bool flag=true);
	void DrawBall();
	int GetAngleKeyIncrement();
	void SetAngleKeyIncrement(int ang);
	void UseConstraints(AxisSet constraints);
	void ToggleMethod();
	void SetAlternateMethod(bool flag=true);
	vector GetColorV();
	COLORREF GetColor();
	void SetColor(COLORREF col);
	void SetColorV(vector colvec);
	void DrawBallLimit();
	CBallController();
	CBallController(const real& rad);
	CBallController(const real& rad,const unitquaternion& initialOrient);
	CBallController(const CBallController& other);
	virtual ~CBallController();
	CBallController& operator=(const CBallController& other);
	void Resize(const real& newRadius);
	void ClientAreaResize(const CRect& newSize);
	void MouseDown(const CPoint& location);
	void MouseUp(const CPoint& location);
	void MouseMove(const CPoint& location);
	void IssueGLrotation();
	void Key(UINT nChar);
};

//---------------------------------------------------------------------------
// inlines

inline CBallController::~CBallController()
{
	if(otherAxes) delete[] otherAxes;
}

inline CBallController::CBallController(const CBallController& other)
{
	*this=other;
}

inline void CBallController::Resize(const real& newRadius)
{
	radius=newRadius;
}

inline void CBallController::ClientAreaResize(const CRect& newSize)
{
	winWidth=real(newSize.Width());
	winHeight=real(newSize.Height());
//	center=CPoint( (newSize.right-newSize.left)/2 , (newSize.bottom-newSize.top)/2);
}

inline CBallController::CBallController()
{
	initVars();
}

inline COLORREF CBallController::GetColor()
{
	return RGB(int(BallColor.x()*255),int(BallColor.y()*255),int(BallColor.z()*255));
}

inline vector CBallController::GetColorV()
{
	return BallColor;
}

inline void CBallController::SetAlternateMethod(bool flag)
{
	bProjectionMethod2=flag;
}

inline void CBallController::ToggleMethod()
{
	if(bProjectionMethod2) bProjectionMethod2=false;
	else bProjectionMethod2=true;
}

inline void CBallController::UseConstraints(AxisSet constraints)
{
	whichConstraints=constraints;
}

inline int CBallController::GetAngleKeyIncrement()
{
	return angleKeyIncrement;
}

inline void CBallController::SetAngleKeyIncrement(int ang)
{
	angleKeyIncrement=abs(ang)%360;
}

inline bool CBallController::GetDrawConstraints()
{
	return bDrawConstraints;
}

inline void CBallController::SetDrawConstraints(bool flag)
{
	bDrawConstraints=flag;
}


#endif // !defined(AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_)
