#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "StrMap.h"

CStrMap::CStrMap(int extrabytes, int dup){
	FList=NULL;
	FCount=0;
	FCapacity=0;
    FDoDuplicate = dup;
    FExtraLen = extrabytes;
    FRecordLen = sizeof(char*) + sizeof(int) + extrabytes;
}

void CStrMap::FillFromChain(char *strchain, void *data)
{
    while ( *strchain ) {
	int len = strlen( strchain );
	AddStrLen(strchain, len, data );
	strchain += len+1;
	data = (char*)data + FExtraLen;
    }
}

void CStrMap::CreateFromChain( int extrabytes, char *strchain, void *data )
{
    FDoDuplicate = 0;
    FExtraLen = extrabytes;
    FRecordLen = sizeof(char*) + sizeof(int) + extrabytes;
    FillFromChain( strchain, data );
    ShrinkMem();
}

CStrMap::~CStrMap(){
    if (FDoDuplicate)
	TrimClear( 0 );
    if (FList){
		TrimClear(0);
		free(FList);
	}
}

void CStrMap::AddString( char *str, void *data )
{
     AddStrLen( str, strlen(str), data );
}

void CStrMap::AddStrLen(char *str, int len, void *data ){
    char *Rec;
    if (FCount >= FCapacity ) {
		int delta = (FCapacity > 64) ? FCapacity / 4 : 16;
		SetCapacity( FCapacity + delta );
    }
    Rec = FList + FCount * FRecordLen;
    *(char**)Rec = (char *)malloc(len+1);
	strncpy(*(char**)Rec,str,len);
    *(int*)(Rec + sizeof(char*)) = len;
    if (data) {
	void *recdata = (Rec + sizeof(char*) + sizeof(int));
	memcpy( recdata, data, FExtraLen );
    }
    ++ FCount;
}

void CStrMap::ShrinkMem()
{
    SetCapacity( FCount );
}

void CStrMap::Trim(int NewCount)
{
    FCount = NewCount;
}

void CStrMap::TrimClear(int NewCount)
{
    int i;
    char *Rec = FList + NewCount * FRecordLen;
    for (i=NewCount; i < FCount; i++) {
		free( *(char**)Rec );
		Rec += FRecordLen;
    }
    FCount = NewCount;
}

void CStrMap::SetCapacity(int NewCapacity)
{
    FCapacity = NewCapacity;
    if (FCount >FCapacity )
        FCount = FCapacity;
    FList = (char*) realloc(FList,FCapacity*FRecordLen );
}

int CStrMap::IndexOf(char *str, void **data)
{
    return LenIndexOf( str, strlen(str), data );
}

int CStrMap::LenIndexOf( char *str, int len, void **data )
{
    int i;
    char *Rec = FList;
    for (i=0; i<FCount; i++) {
        int recLen = *(int*)(Rec + sizeof(char*));
		if (recLen==len && strncmp( str, *(char**)Rec, recLen )==0 ) {
			*data = (Rec + sizeof(char*) + sizeof(int));
			return i;
		}
		Rec += FRecordLen;
    }
    *data = NULL;
    return -1;
}

int CStrMap::Replace( char *str, void *data ){
	return LenReplace( str, strlen(str), data );
}

int CStrMap::LenReplace( char *str, int len, void *data )
{
    int i;
    char *Rec = FList;
    for (i=0; i<FCount; i++) {
        int recLen = *(int*)(Rec + sizeof(char*));
		if (recLen==len && strncmp( str, *(char**)Rec, recLen )==0 ) {
			void *recdata = (Rec + sizeof(char*) + sizeof(int));
			memcpy( recdata, data, FExtraLen );			
			return i;
		}
		Rec += FRecordLen;
    }
    return -1;
}

char* CStrMap::GetString( int index, int *len, void **data )
{
    char *Rec = FList + index * FRecordLen;
    *len =  *(int*)(Rec + sizeof(char*));
    if (data!=NULL && FExtraLen>0)
        *data = (Rec + sizeof(char*) + sizeof(int));
    return *(char**)Rec;
}