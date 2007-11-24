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
// BCGXMLSettings.cpp: implementation of the CBCGXMLSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGXMLSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGXMLSettings, CBCGPRegistry)

CBCGXMLNode	CBCGXMLSettings::m_Local;
CBCGXMLNode	CBCGXMLSettings::m_User;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGXMLSettings::CBCGXMLSettings()
{
	m_pCurrNode = NULL;

	if (m_Local.m_strName.IsEmpty ())
	{
		m_Local.m_strName = _T("LOCAL_MACHINE");
	}

	if (m_User.m_strName.IsEmpty ())
	{
		m_User.m_strName = _T("CURRENT_USER");
	}
}

CBCGXMLSettings::~CBCGXMLSettings()
{
}

BOOL CBCGXMLSettings::VerifyKey (LPCTSTR /*pszPath*/)
{
	ASSERT (FALSE);	// TODO
	return TRUE;
}

BOOL CBCGXMLSettings::VerifyValue (LPCTSTR /*pszValue*/)
{
	ASSERT (FALSE);	// TODO
	return TRUE;
}

BOOL CBCGXMLSettings::CreateKey (LPCTSTR pszPath)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (pszPath != NULL);

	CString strPath = pszPath;
	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}

	CBCGXMLNode* pNode = GetTree ();

	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			pChild = new CBCGXMLNode;
			pChild->m_strName = strSubKey;
			pNode->AddChild (pChild);
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	m_pCurrNode = pNode;
	return TRUE;
}

BOOL CBCGXMLSettings::Open (LPCTSTR pszPath)
{
	ASSERT (pszPath != NULL);

	CBCGXMLNode* pNode = GetTree ();

	m_sPath = pszPath;

	CString strPath = pszPath;
	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}

	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			m_pCurrNode = NULL;
			return FALSE;
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	m_pCurrNode = pNode;
	return TRUE;
}
//***************************************************************************************
void CBCGXMLSettings::Close()
{
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, int iVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CString str;
	str.Format (_T("%d"), iVal);

	return WriteTag (pszKey, str);
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, DWORD dwVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CString str;
	str.Format (_T("%d"), dwVal);

	return WriteTag (pszKey, str);
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	return WriteTag (pszKey, pszData);
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, CObject& obj)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			obj.Serialize (ar);
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, CObject* pObj)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);
			ar << pObj;
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, CWordArray& wcArray)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << wcArray.GetSize ();
			for (int i = 0; i < wcArray.GetSize (); i ++)
			{
				ar << wcArray [i];
			}

			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write(LPCTSTR pszKey, const CRect& rect)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << rect;
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	const int iMaxChars = 20;
	CDWordArray dwcArray;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, lpPoint->x);
	dwcArray.SetAt(1, lpPoint->y);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	LPBYTE lpbData = file.Detach ();

	BOOL bRes = Write (pszKey, lpbData, (UINT) dwLen);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Write (LPCTSTR pszKey, LPBYTE pData, UINT nBytes)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (pszKey != NULL);
	ASSERT (pData != NULL);
	ASSERT (AfxIsValidAddress (pData, nBytes, FALSE));

	char* lpszBuffer = new char [nBytes * 2 + 1];
	ASSERT (lpszBuffer != NULL);

	lpszBuffer [0] = 0;

	char lpszByte [3];
	int j = 0;

	for (UINT i = 0; i < nBytes; i++)
	{
		sprintf (lpszByte, "%02x", pData [i]);

		lpszBuffer [j++] = lpszByte [0];
		lpszBuffer [j++] = lpszByte [1];
	}

	lpszBuffer [j] = 0;

	BOOL bRes = WriteTag (pszKey, lpszBuffer);

	delete [] lpszBuffer;
	return bRes;
}
//***************************************************************************************
BOOL CBCGXMLSettings::WriteTag (LPCTSTR pszKey, LPCTSTR lpszBuffer)
{
	ASSERT (pszKey != NULL);
	ASSERT (lpszBuffer != NULL);

	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (m_pCurrNode == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT_VALID (m_pCurrNode);

	CString strKey = pszKey;
	strKey.Remove (_T(' '));

	CBCGXMLNode* pNode = m_pCurrNode->FindChild (strKey);
	if (pNode == NULL)
	{
		pNode = new CBCGXMLNode;
		pNode->m_strName = strKey;

		m_pCurrNode->AddChild (pNode);
	}

	pNode->m_strValue = lpszBuffer;
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read(LPCTSTR pszKey, int& iVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	iVal = _ttoi (lpszVal);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	dwVal = _ttol (lpszVal);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, CString& sVal)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		sVal.Empty ();
		return FALSE;
	}

	sVal = lpszVal;
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, CWordArray& wcArray)
{
	wcArray.SetSize (0);

	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		int iSize;
		ar >> iSize;

		wcArray.SetSize (iSize);
		for (int i = 0; i < iSize; i ++)
		{
			ar >> wcArray [i];
		}

		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read(LPCTSTR pszKey, CRect& rect)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		ar >> rect;
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		CDWordArray dwcArray;
		dwcArray.SetSize(5);
		dwcArray.Serialize (ar);
		lpPoint->x = dwcArray.GetAt(0);
		lpPoint->y = dwcArray.GetAt(1);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGXMLSettings::Read ()!\n"));
	}

	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, BYTE** ppData, UINT* pBytes)
{
	LPCTSTR lpszVal = ReadTag (pszKey);
	if (lpszVal == NULL)
	{
		return FALSE;
	}

	int nLen = lstrlen (lpszVal);
	if ((nLen % 2) != 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	*pBytes = nLen / 2;
	*ppData = new BYTE [*pBytes];

	char szByte [3];
	szByte [2] = 0;

	int j = 0;
	for (int i = 0; i < nLen; i += 2)
	{
		szByte [0] = lpszVal [i];
		szByte [1] = lpszVal [i + 1];

		BYTE b;
		sscanf (szByte, "%x", &b);
		(*ppData) [j++] = b;
	}
	
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, CObject& obj)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		obj.Serialize (ar);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGXMLSettings::Read (LPCTSTR pszKey, CObject*& pObj)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		ar >> pObj;

		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CBCGXMLSettings::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CBCGXMLSettings::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}
//***************************************************************************************
BOOL CBCGXMLSettings::DeleteValue (LPCTSTR pszValue)
{
	if (m_pCurrNode == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pCurrNode);

	for (POSITION pos = m_pCurrNode->m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGXMLNode* pNode = m_pCurrNode->m_lstChildren.GetNext (pos);
		ASSERT_VALID (pNode);

		if (pNode->m_strName == pszValue)
		{
			m_pCurrNode->m_lstChildren.RemoveAt (posSave);
			delete pNode;
			return TRUE;
		}
	}

	return FALSE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::DeleteKey (LPCTSTR pszPath, BOOL bAdmin)
{
	if (m_bReadOnly)
	{
		return FALSE;
	}
	
	CBCGXMLNode* pNode = bAdmin ? &m_Local : &m_User;

	ASSERT (pszPath != NULL);
	CString strPath = pszPath;

	int iPathLen = strPath.GetLength ();
	if (iPathLen > 0 && strPath [iPathLen - 1] != _T('\\'))
	{
		strPath += _T('\\');
	}
	
	for (int iFrom = 0; iFrom < strPath.GetLength ();)
	{
		int iEnd = strPath.Find (_T('\\'), iFrom);
		if (iEnd < 0)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		CString strSubKey = strPath.Mid (iFrom, iEnd - iFrom);
		strSubKey.Remove (_T(' '));
		
		CBCGXMLNode* pChild = pNode->FindChild (strSubKey);
		if (pChild == NULL)
		{
			return FALSE;
		}

		pNode = pChild;
		iFrom = iEnd + 1;
	}

	ASSERT_VALID (pNode->m_pParent);
	for (POSITION pos = pNode->m_pParent->m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGXMLNode* pCurrNode = pNode->m_pParent->m_lstChildren.GetNext (pos);
		ASSERT_VALID (pCurrNode);

		if (pNode == pCurrNode)
		{
			pNode->m_pParent->m_lstChildren.RemoveAt (posSave);
			delete pNode;
			return TRUE;
		}
	}

	ASSERT(FALSE);
	return FALSE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::ReadSubKeys(CStringArray& /*SubKeys*/)
{
	ASSERT (FALSE);
	return FALSE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::ReadKeyValues(CStringArray& /*Values*/)
{
	ASSERT (FALSE);
	return FALSE;
}
//***************************************************************************************
LPCTSTR CBCGXMLSettings::ReadTag (LPCTSTR pszKey) const
{
	ASSERT (pszKey != NULL);

	if (m_pCurrNode == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (m_pCurrNode);

	CString strKey = pszKey;
	strKey.Remove (_T(' '));

	CBCGXMLNode* pNode = m_pCurrNode->FindChild (strKey);
	if (pNode == NULL)
	{
		return NULL;
	}

	return pNode->m_strValue;
}
//***************************************************************************************
BOOL CBCGXMLSettings::WriteXMLToFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName)
{
	CBCGXMLNode& node = bIsLocalMachine ? m_Local : m_User;

	// Create XML buffer:
	CString strBuffer;
	if (node.WriteToBuffer (strBuffer, 0) <= 0)
	{
		return FALSE;
	}

	try
	{
		CStdioFile file;
		file.Open (lpszFileName, 
					CFile::modeWrite | CFile::modeCreate | CFile:: typeText);

		file.WriteString (strBuffer);
	}
	catch (CFileException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();

		return FALSE;
	}

	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLSettings::ReadXMLFromFile (BOOL bIsLocalMachine, LPCTSTR lpszFileName)
{
	CString strBuffer;

	try
	{
		CStdioFile file;
		if (!file.Open (lpszFileName, CFile::modeRead))
		{
			TRACE(_T("File not found: %s"), lpszFileName);
			return FALSE;
		}

		CString str;

		while (file.ReadString (str))
		{
			strBuffer += str;
		}
	}
	catch (CFileException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();

		return FALSE;
	}

	CBCGXMLNode& node = bIsLocalMachine ? m_Local : m_User;
	return node.ReadFromBuffer (strBuffer);
}
//***************************************************************************************
int CBCGXMLNode::WriteToBuffer (CString& strBuffer, int iOffset)
{
	if (m_lstChildren.IsEmpty () && m_pParent != NULL)
	{
		CString strTagValue;
		strTagValue.Format (_T("<%s>\"%s\"</%s>\n"), m_strName, m_strValue, m_strName);

		strBuffer.Insert (iOffset, strTagValue);
		return strTagValue.GetLength ();
	}

	int iOffsetOrig = iOffset;

	CString strTagStart;
	strTagStart.Format (_T("<%s>\n"), m_strName);

	strBuffer.Insert (iOffset, strTagStart);
	iOffset += strTagStart.GetLength ();

	for (POSITION pos = m_lstChildren.GetHeadPosition (); pos != NULL;)
	{
		CBCGXMLNode* pNode = m_lstChildren.GetNext (pos);
		ASSERT_VALID (pNode);

		iOffset += pNode->WriteToBuffer (strBuffer, iOffset);
	}

	CString strTagEnd;
	strTagEnd.Format (_T("</%s>\n"), m_strName);

	strBuffer.Insert (iOffset, strTagEnd);
	iOffset += strTagEnd.GetLength ();

	return iOffset - iOffsetOrig;
}
//***************************************************************************************
static BOOL ExcludeTag (CString& strBuffer, 
						LPCTSTR lpszTag,
						CString& strTag)
{
	const int iBufLen = strBuffer.GetLength ();

	CString strTagStart = _T("<");
	strTagStart += lpszTag;
	strTagStart += _T(">");

	const int iTagStartLen = strTagStart.GetLength ();

	int iStart = -1;

	int iIndexStart = strBuffer.Find (strTagStart);
	if (iIndexStart < 0)
	{
		return FALSE;
	}

	iStart = iIndexStart + iTagStartLen;

	CString strTagEnd = _T("</");
	strTagEnd += lpszTag;
	strTagEnd += _T(">");

	const int iTagEndLen = strTagEnd.GetLength ();

	int iIndexEnd =  -1;
	int nBalanse = 1;
	for (int i = iStart; i < iBufLen - iTagEndLen + 1; i ++)
	{
		if (strBuffer [i] != '<')
		{
			continue;
		}

		if (i < iBufLen - iTagStartLen &&
			_tcsncmp (strBuffer.Mid (i), strTagStart, iTagStartLen) == 0)
		{
			i += iTagStartLen - 1;
			nBalanse ++;
			continue;
		}

		if (_tcsncmp (strBuffer.Mid (i), strTagEnd, iTagEndLen) == 0)
		{
			nBalanse --;
			if (nBalanse == 0)
			{
				iIndexEnd = i;
				break;
			}

			i += iTagEndLen - 1;
		}
	}

	if (iIndexEnd == -1 || iStart > iIndexEnd)
	{
		return FALSE;
	}

	strTag = strBuffer.Mid (iStart, iIndexEnd - iStart);
	strTag.TrimLeft ();
	strTag.TrimRight ();

	strBuffer.Delete (iIndexStart, iIndexEnd + iTagEndLen - iIndexStart);
	return TRUE;
}
//***************************************************************************************
BOOL CBCGXMLNode::ReadFromBuffer (CString& strBuffer)
{
	m_strValue.Empty ();

	while (!m_lstChildren.IsEmpty ())
	{
		delete m_lstChildren.RemoveHead ();
	}

	if (strBuffer [0] != '<')
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int iTagEnd = strBuffer.Find ('>');
	if (iTagEnd < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_strName = strBuffer.Mid (1, iTagEnd - 1);

	CString strTag;
	if (!ExcludeTag (strBuffer, m_strName, strTag))
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (strTag.IsEmpty ())
	{
		return TRUE;
	}

	if (strTag [0] == '\"')
	{
		ASSERT (strTag [strTag.GetLength () - 1] == '\"');
		m_strValue = strTag.Mid (1, strTag.GetLength () - 2);

		return TRUE;
	}

	if (strTag [0] != '<')
	{
		ASSERT(FALSE);
		return FALSE;
	}

	while (!strTag.IsEmpty ())
	{
		CBCGXMLNode* pChild = new CBCGXMLNode;
		if (!pChild->ReadFromBuffer (strTag))
		{
			delete pChild;
			return FALSE;
		}

		AddChild (pChild);
	}

	return TRUE;
}
//***************************************************************************************
CBCGXMLNode* CBCGXMLSettings::GetTree () const
{
	return m_bAdmin ? &m_Local : &m_User;
}
