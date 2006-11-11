//=========================================================
//	TITLE:		Array extention
//				for WinNT, MSVC 6.0, MFC 6.00
//				Copyright (C) Matrix Baltic Software
//				Vilnius, Lithuania
//	MODULE:		ArrayEx.h
//	PURPOSE:	Interface of the CArrayEx class.
//
//	AUTHOR:		Audrius Vasiliauskas
// 
//	NOTES:		
//
//=========================================================


#ifndef __ARRAY_EX_H__
#define __ARRAY_EX_H__

#if _MSC_VER < 0x0600
	#pragma warning(disable : 4786) //identifier was truncated to '255' characters in the debug information
#endif

#ifndef __AFXTEMPL_H__
	#include <afxtempl.h>
#endif

////////////////////////////////////////////////////////////////////////////
//
// tips & triks. Speed optimization and bug detecting compromise
//

#ifdef _DEBUG
	#define DATA_ACCESS_OPERATOR(i) ((*this)[i]) // for better bug tracking
	#define FAST_ACCESS_OPERATOR(var,i) ((var)[(i)]) // for better bug tracking
#else
	#define DATA_ACCESS_OPERATOR(i) (m_pData[i]) // 10 times faster
	#define FAST_ACCESS_OPERATOR(var,i) ((var).GetData()[(i)]) 
#endif

//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//
// Special compare functions for Sorted Array
//

/* // this compare function copy from mfc
template<class TYPE, class ARG_TYPE>
AFX_INLINE BOOL AFXAPI CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	return *pElement1 == *pElement2;
}
*/

template<class TYPE, class ARG_TYPE>
AFX_INLINE BOOL AFXAPI CompareElementsLess(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	return ((*pElement1) < (*pElement2));
}

//
////////////////////////////////////////////////////////////////////////////



template <class TYPE, class ARG_TYPE>
class CArrayEx : public CArray<TYPE, ARG_TYPE>
{
// Construction
public:
	CArrayEx ();
	CArrayEx (const CArrayEx &x);

// Assigment
public:
	CArrayEx &operator = (const CArrayEx &x);

// Comparison 
public:
	BOOL operator <  (const CArrayEx &x) const;
	BOOL operator <= (const CArrayEx &x) const;
	BOOL operator == (const CArrayEx &x) const;
	BOOL operator != (const CArrayEx &x) const;
	BOOL operator >  (const CArrayEx &x) const;
	BOOL operator >= (const CArrayEx &x) const;

// Operator
public:
	operator CArray<TYPE, ARG_TYPE>();
};

template <class TYPE, class ARG_TYPE>
inline 
	CArrayEx<TYPE, ARG_TYPE>::CArrayEx ()
{
}

template <class TYPE, class ARG_TYPE>
inline 
	CArrayEx<TYPE, ARG_TYPE>::CArrayEx (const CArrayEx &x)
{
	*this = x;
}

template <class TYPE, class ARG_TYPE>
inline CArrayEx<TYPE, ARG_TYPE> &
	   CArrayEx<TYPE, ARG_TYPE>::operator = (const CArrayEx &x)
{
	if(this != &x)
	{
		Copy(x);
	}

	return *this;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator <  (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize != x.GetSize())
	{
		return FALSE;
	}

	for(i = 0;i < nSize;i ++)
	{
		if(!(DATA_ACCESS_OPERATOR(i) < FAST_ACCESS_OPERATOR(x,i)))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator <= (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize != x.GetSize())
	{
		return FALSE;
	}

	for(i = 0;i < nSize;i ++)
	{
		if(!(DATA_ACCESS_OPERATOR(i) <= FAST_ACCESS_OPERATOR(x,i)))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator == (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize != x.GetSize())
	{
		return FALSE;
	}

	for(i = 0;i < nSize;i ++)
	{
		if(!(DATA_ACCESS_OPERATOR(i) == FAST_ACCESS_OPERATOR(x,i)))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator != (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize == x.GetSize())
	{
		for(i = 0;i < nSize;i ++)
		{
			if(!(DATA_ACCESS_OPERATOR(i) != FAST_ACCESS_OPERATOR(x,i)))
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator >  (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize != x.GetSize())
	{
		return FALSE;
	}

	for(i = 0;i < nSize;i ++)
	{
		if(!(DATA_ACCESS_OPERATOR(i) > FAST_ACCESS_OPERATOR(x,i)))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline BOOL 
	   CArrayEx<TYPE, ARG_TYPE>::operator >= (const CArrayEx &x) const
{
	register int i;
	register int nSize(GetSize());

	if(nSize != x.GetSize())
	{
		return FALSE;
	}

	for(i = 0;i < nSize;i ++)
	{
		if(!(DATA_ACCESS_OPERATOR(i) >= FAST_ACCESS_OPERATOR(x,i)))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

template <class TYPE, class ARG_TYPE>
inline 
	   CArrayEx<TYPE, ARG_TYPE>::operator CArray<TYPE, ARG_TYPE>()
{
	return *this;
}



#endif 