#ifndef _STRMAP_H_
 #define _STRMAP_H_

class CStrMap{
    int   FCount, FCapacity;
    int   FExtraLen, FRecordLen;
    int	  FDoDuplicate;
    char *FList;
public:
	CStrMap( int extrabytes=sizeof(double), int dup=0);
	void CreateFromChain( int extrabytes, char *strchain, void *data );
	~CStrMap();
	void AddString(char *str, void *data);
	void AddStrLen(char *str, int len, void *data );
	void ShrinkMem();
	void Trim(int NewCount );
	void TrimClear(int NewCount );
	void SetCapacity(int NewCapacity );
	int IndexOf(char *str, void **data );
	int LenIndexOf(char *str, int len, void **data );
	int Replace( char *str,void *data );
	int LenReplace( char *str, int len, void *data );
	char* GetString(int index, int *len, void **data );
	void FillFromChain(char *strchain, void *data );
};

#endif //_STRMAP_H_