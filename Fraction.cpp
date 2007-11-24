// Code for handling fraction strings
// By Dean Wyant  dwyant@mindspring.com
/////////////////////////////////////////////////////
//
// CFraction - This class is used like a double. Do anything you would do with a double,
// plus assign strings to it. When you need to get a string representation, use one of the 
// String calls
//

#include "stdafx.h"     // precompiled header - remark out if necessary

#include "fraction.h"


#ifdef __cplusplus


#ifdef FRAC_CSTRING

CString CFraction::ToString(double AllowedError)
{
  CString sRet;
  Fracftoa(m_d,sRet.GetBuffer(40),AllowedError);
  sRet.ReleaseBuffer();
  return sRet;
}

CString CFraction::ToString(int MaxDen)
{
  CString sRet;
  Fracftoa(m_d,sRet.GetBuffer(40),MaxDen);
  sRet.ReleaseBuffer();
  return sRet;
}

CString CFraction::ToStockString(int MaxDen)
{
  CString sRet;
  DoubleToStockString(m_d,sRet.GetBuffer(40),MaxDen);
  sRet.ReleaseBuffer();
  return sRet;
}

CString CFraction::ForceToStockString(int MaxDen)
{
  CString sRet;
  ForceDoubleToStockString(m_d,sRet.GetBuffer(40),MaxDen);
  sRet.ReleaseBuffer();
  return sRet;
}

#endif // FRAC_CSTRING

char *CFraction::ToStr(char *pszBuffer, double AllowedError)  
{
  return Fracftoa(m_d,pszBuffer,AllowedError);
}

char *CFraction::ToStr(char *pszBuffer, int MaxDen)  
{
  return Fracftoa(m_d,pszBuffer,MaxDen);
}

char *CFraction::ToStockStr(char *pszBuffer, int MaxDen)  
{
  return DoubleToStockString(m_d,pszBuffer,MaxDen);
}

char *CFraction::ForceToStockStr(char *pszBuffer, int MaxDen)  
{
  return ForceDoubleToStockString(m_d,pszBuffer,MaxDen);
}

double CFraction::Parts(__int64 &Whole, __int64 &Num, __int64 &Den, double AllowedError)
{
  return FracParts(m_d,Whole,Num,Den,AllowedError);
}

double CFraction::Parts(__int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen)
{
  return FracParts(m_d,Whole,Num,Den,MaxDen);
}

#endif //__cplusplus

inline double Abs(double f) { if (f < 0.0) return -f; return f; }

double Fracatof(const char *pszNum)
{
  double f = 0;
  __int64 W,N,D;
  char *s = strchr((char*)pszNum,'/');
  if (!s)
    f = atof(pszNum);
  else
  {
    s++;
    D = _atoi64(s);
    if (D != 0) // do not divide by zero
    { // We need a copy
      char *S = (char *)malloc(strlen(pszNum)+1);
      strcpy(S,pszNum);
      s = strchr(S,'/');
      s[0] = 0; // terminate string at '/'
      s = strchr(S,' ');
      if (!s)
      {
        s = S;
        N = _atoi64(s);
        f = (double)N/D;
      }
      else
      {
        s++;
        N = _atoi64(s);
        s--;
        s[0] = 0;
        s = S;
        while (s[0] == ' ') s++;
        W = _atoi64(s);
        if (W > 0)
          f = W + ((double)N/D);
        else
          f = W - ((double)N/D);
      }
      free(S);
    }
  }
  return f;
}


char *Fracftoa(double f, char *pszBuffer, double AllowedError)
{
  char *s = pszBuffer;
  __int64 Whole;
  __int64 Num;
  __int64 Den;
  FracParts(f,Whole,Num,Den,AllowedError);
  if (Whole)
    _i64toa(Whole,s,10);
  if (Den)
  {
    if (Whole)
    {
      s += strlen(s);
      s[0] = ' ';
      s++;
    }
    _i64toa(Num,s,10);
    s += strlen(s);
    s[0] = '/';
    s++;
    _i64toa(Den,s,10);
  }
  else
  {
    if (!Whole)
      _i64toa(Whole,s,10);
  }
  return pszBuffer;
}

char *Fracftoa(double f, char *pszBuffer, int MaxDen)
{
  char *s = pszBuffer;
  __int64 Whole;
  __int64 Num;
  __int64 Den;
  FracParts(f,Whole,Num,Den,MaxDen);
  if (Whole)
    _i64toa(Whole,s,10);
  else if (f<0)
  {
	  s[0]='-';
	  s++;
  }
  if ((Num) && (Den))
  {
    if (Whole)
    {
      s += strlen(s);
      s[0] = ' ';
      s++;
    }
    _i64toa(Num,s,10);
    s += strlen(s);
    s[0] = '/';
    s++;
    _i64toa(Den,s,10);
  }
  else
  {
    if (!Whole)
      _i64toa(Whole,s,10);
  }
  return pszBuffer;
}

char *DoubleToStockString(double f, char *pszBuffer, int MaxDen)
{
  if (MaxDen > 256) MaxDen = 256;
  char *s = pszBuffer;
  __int64 Whole;
  __int64 Num;
  __int64 Den;
  FracParts(f,Whole,Num,Den,MaxDen);
  if (Whole)
    _i64toa(Whole,s,10);
  if (Den)
  {
    if ((Den % 2 == 0) && (Den <= MaxDen) &&
        ((Den == 2) || (Den == 4) || (Den == 8) || (Den == 16) || 
         (Den == 32) || (Den == 64) || (Den == 128) || (Den == 256)))
    {
      if (Whole)
      {
        s += strlen(s);
        s[0] = ' ';
        s++;
      }
      _i64toa(Num,s,10);
      s += strlen(s);
      s[0] = '/';
      s++;
      _i64toa(Den,s,10);
    }
    else
    {
      sprintf(pszBuffer,"%6.4f",f);
    }
  }
  else
  {
    if (!Whole)
      _i64toa(Whole,s,10);
  }
  return pszBuffer;
}

char *ForceDoubleToStockString(double f, char *pszBuffer, int MaxDen)
{
  char *s = pszBuffer;
  __int64 Whole;
  __int64 Num;
  __int64 Den;
  FracPartsStock(f,Whole,Num,Den,MaxDen);
  if (Whole)
    _i64toa(Whole,s,10);
  if (Den)
  {
    if (Whole)
    {
      s += strlen(s);
      s[0] = ' ';
      s++;
    }
    _i64toa(Num,s,10);
    s += strlen(s);
    s[0] = '/';
    s++;
    _i64toa(Den,s,10);
  }
  else
  {
    if (!Whole)
      _i64toa(Whole,s,10);
  }
  return pszBuffer;
}

double FracParts(double f, __int64 &Whole, __int64 &Num, __int64 &Den, double AllowedError)
{
  __int64 i;
  __int64 A = 0;
  __int64 B = 0;
  double Precision = AllowedError; 
  double d = f;
  Whole = (__int64)(d+Precision);
  d -= Whole;
  d = Abs(d);
  double Frac = d;
  double Diff = Frac;
  Num = 1;
  Den = 0;
  if (Frac > Precision)
  while (TRUE) 
  {
    d = 1.0/d;
    i = (__int64)(d+Precision);
    d -= i;
    if (A)
      Num = i * Num + B;
    Den = __int64(Num/Frac+0.5);
    Diff = Abs((double)Num/Den-Frac);
    if ((Diff <= AllowedError) || (d < Precision))
      break;
    Precision = AllowedError/Diff; 
      // This calcualtion of Precision does not always provide results within
      // Allowed Error. It compensates for loss of significant digits that occurs.
      // It helps to round the inprecise reciprocal values to i. 
    B = A;
    A = Num;
  }
  if (Num == Den)
  {
    Whole++;
    Num = 0;
    Den = 0;
  }
  else
  if (!Den) 
    Num = 0;
  return Diff;
}

double FracParts(double f, __int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen)
{
  __int64 i;
  __int64 A = 0;
  __int64 B = 0;
  double AllowedError = FRACTION_MAX_PRECISION; 
  double Precision = AllowedError;
  double d = f;
  Whole = (__int64)(d+Precision);
  d -= Whole;
  d = Abs(d);
  double Frac = d;
  double Diff = Frac;
  Num = 1;
  Den = 0;
  if (Frac > Precision)
  while (TRUE) 
  {
    d = 1.0/d;
    i = (__int64)(d+Precision);
    d -= i;
    if (A)
      Num = i * Num + B;
    Den = __int64(Num/Frac+0.5);
    Diff = Abs((double)Num/Den-Frac);
    if (Den > MaxDen)
    {
      if (A)
      { 
        Num = A;
        Den = __int64(Num/Frac+0.5);
        Diff = Abs((double)Num/Den-Frac);
      }
      else
      {
        Den = MaxDen;
        Num = 1;
        Diff = Abs((double)Num/Den-Frac);
        if (Diff > Frac)
        {
          Num = 0;
          Den = 1; // Keeps final check below from adding 1 and keeps Den from being 0
          Diff = Frac;
        }
      }
      break;
    }
    if ((Diff <= AllowedError) || (d < Precision))
      break;
    Precision = AllowedError/Diff;
      // This calcualtion of Precision does not always provide results within
      // Allowed Error. It compensates for loss of significant digits that occurs.
      // It helps to round the inprecise reciprocal values to i. 
    B = A;
    A = Num;
  }
  if (Num == Den)
  {
    Whole++;
    Num = 0;
    Den = 0;
  }
  else
  if (!Den) 
    Num = 0;
  return Diff;
}

double FracPartsStock(double f, __int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen)
{ // Convert a double to its fractional parts. Returns Error difference. 
  // Limits the denominator to MaxDen and always returns Den as 2,4,8,16,32,64,128,256 
  if (MaxDen > 256) MaxDen = 256;
  double Diff;
  Diff = FracParts(f,Whole,Num,Den,MaxDen);
  double Frac = Abs(f - Whole);
  if (Den)
  {
    if ((Den != 2) && (Den != 4) && (Den != 8) && (Den != 16) && 
         (Den != 32) && (Den != 64) && (Den != 128) && (Den != 256))
    {
      Den = MaxDen;
      Num = __int64((Den * Frac)+0.5);
      while (!(Num % 2))
      {
        Num /= 2;
        Den /= 2;
      }
      Diff = Abs(((double)Num/Den)-Frac);
    }
  }
  return Diff;
}
