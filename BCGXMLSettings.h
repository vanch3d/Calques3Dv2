//////////////////////////////////////////////////////////////////////
/// @file BCGXMLSettings.h
/// @brief interface for the CBCGXMLSettings and CBCGXMLNode classes.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGXMLSETTINGS_H__290A1AF4_79E3_402D_BD32_7C8A4F8BBBE4__INCLUDED_)
#define AFX_BCGXMLSETTINGS_H__290A1AF4_79E3_402D_BD32_7C8A4F8BBBE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#include "afxtempl.h"
#endif

#include "BCGPRegistry.h"

#define BCGXMLSETTINGS_PRIVATE	1

/////////////////////////////////////////////////////////////////////////////
/// CBCGXMLNode
/////////////////////////////////////////////////////////////////////////////
class CBCGXMLNode : public CObject
{
	friend class CBCGXMLSettings;

public:
	CBCGXMLNode()
	{
		m_pParent = NULL;
	}

	~CBCGXMLNode()
	{
		while (!m_lstChildren.IsEmpty ())
		{
			delete m_lstChildren.RemoveHead ();
		}
	}

	CList<CBCGXMLNode*, CBCGXMLNode*>	m_lstChildren;
	CBCGXMLNode*						m_pParent;

	CString								m_strName;
	CString								m_strValue;

	CBCGXMLNode* FindChild (LPCTSTR lpszName) const
	{
		for (POSITION pos = m_lstChildren.GetHeadPosition (); pos != NULL;)
		{
			CBCGXMLNode* pNode = m_lstChildren.GetNext (pos);
			ASSERT_VALID (pNode);

			if (pNode->m_strName == lpszName)
			{
				return pNode;
			}
		}

		return NULL;
	}

	void AddChild (CBCGXMLNode* pNode)
	{
		m_lstChildren.AddTail (pNode);
		pNode->m_pParent = this;
	}

	int WriteToBuffer (CString& strBuffer, int iOffset);
	BOOL ReadFromBuffer (CString& strBuffer);
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGXMLSettings
/////////////////////////////////////////////////////////////////////////////
class CBCGXMLSettings : public CBCGPRegistry  
{
	DECLARE_DYNCREATE(CBCGXMLSettings)

public:
	CBCGXMLSettings();
	virtual ~CBCGXMLSettings();

// Attributes:
protected:
	static CBCGXMLNode	m_Local;	// "LOCAL_MACHINE"
	static CBCGXMLNode	m_User;		// "CURRENT_USER"

	CBCGXMLNode*	m_pCurrNode;

// Operations
public:

	virtual CBCGXMLNode* GetTree () const;

	static BOOL WriteXMLToFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName);
	static BOOL ReadXMLFromFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName);

	virtual BOOL ReadKeyValues(CStringArray& Values);
	virtual BOOL VerifyKey (LPCTSTR pszPath);
	virtual BOOL VerifyValue (LPCTSTR pszValue);
	virtual BOOL CreateKey (LPCTSTR pszPath);
	virtual BOOL Open (LPCTSTR pszPath);
	virtual void Close();

	virtual BOOL DeleteValue (LPCTSTR pszValue);
	virtual BOOL DeleteKey (LPCTSTR pszPath, BOOL bAdmin = FALSE);

	virtual BOOL Write (LPCTSTR pszKey, int iVal);
	virtual BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	virtual BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);
	virtual BOOL Write (LPCTSTR pszKey, CWordArray& wcArray);
	virtual BOOL Write (LPCTSTR pszKey, const CRect& rect);
	virtual BOOL Write (LPCTSTR pszKey, LPPOINT& lpPoint);
	virtual BOOL Write (LPCTSTR pszKey, LPBYTE pData, UINT nBytes);
	virtual BOOL Write (LPCTSTR pszKey, CObject& obj);
	virtual BOOL Write (LPCTSTR pszKey, CObject* pObj);

	virtual BOOL Read (LPCTSTR pszKey, int& iVal);
	virtual BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	virtual BOOL Read (LPCTSTR pszKey, CString& sVal);
	virtual BOOL Read (LPCTSTR pszKey, CWordArray& wcArray);
	virtual BOOL Read (LPCTSTR pszKey, LPPOINT& lpPoint);
	virtual BOOL Read (LPCTSTR pszKey, CRect& rect);
	virtual BOOL Read (LPCTSTR pszKey, BYTE** ppData, UINT* pBytes);
	virtual BOOL Read (LPCTSTR pszKey, CObject& obj);
	virtual BOOL Read (LPCTSTR pszKey, CObject*& pObj);

	virtual BOOL ReadSubKeys(CStringArray& SubKeys);

protected:
	BOOL WriteTag (LPCTSTR pszKey, LPCTSTR lpszBuffer);
	LPCTSTR ReadTag (LPCTSTR pszKey) const;
};

#endif // !defined(AFX_BCGXMLSETTINGS_H__290A1AF4_79E3_402D_BD32_7C8A4F8BBBE4__INCLUDED_)
