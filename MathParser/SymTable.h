// SymTable.h: Schnittstelle für die Klasse CSymTable.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_)
#define AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSymTable  
{
public:
	typedef struct {
		char Sym[4];
		char Len;
		char Index;
		char More;
	} SymbolRec;
private:
	SymbolRec* table[256];
public:
	void PrepareSymTable( char *symbols );
	int FindSymbol( char *str, int *nchars );
	CSymTable();
	virtual ~CSymTable();

};

#endif // !defined(AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_)
