//////////////////////////////////////////////////////////////////////
/// @file Fraction.h
/// @brief Interface of the CFraction class - Code for handling fraction strings
///
/// \author Dean Wyant  dwyant@mindspring.com
///
/// This class is used like a double. Do anything you would do with a double,
/// plus assign strings to it. When you need to get a string representation, use one of the 
/// String calls. My favorite is ForceToStockString which returns a string for the closest
/// valid stock transaction denominator up to MaxDen (default 256). This is great for 
/// applications that need to show stock prices in fractions without resorting to a table
/// lookup or showing unusual prices in decimal. 
///
/// C calls and plain calls for ASCIIZ support are provided
///
/// A description of the \ref Fraction is also given.
//////////////////////////////////////////////////////////////////////
#ifndef __FRACTION_H__
#define __FRACTION_H__

// If you are not using MFC or CString you can remark out this define
#define FRAC_CSTRING

#define FRACTION_MAX_PRECISION  10.0e-15  ///< Significant digits

double Abs(double f);  // Internal use to avoid <math.h>


// C support

//////////////////////////////////////////////////////////////////////
/// Convert string that may include a fraction to floating point (double)
///
/// \param pszNum	todo
/// \return double
//////////////////////////////////////////////////////////////////////
double Fracatof(const char *pszNum);

//////////////////////////////////////////////////////////////////////
/// Convert double to a string that includes the whole part, space, and the fraction (numerator/denominator).
/// If you want to get a Num and Den for a fractional part less than the default AllowedError,
/// pass an allowed error less than the fractional part. Otherwise the default should be fine.
//////////////////////////////////////////////////////////////////////
char *Fracftoa(double f, char *pszBuffer, double AllowedError = FRACTION_MAX_PRECISION);

//////////////////////////////////////////////////////////////////////
/// Convert double to a string that includes the whole part, space, and the fraction (numerator/denominator). 
/// Limits the denominator to MaxDen  
//////////////////////////////////////////////////////////////////////
char *Fracftoa(double f, char *pszBuffer, int MaxDen);

//////////////////////////////////////////////////////////////////////
/// Returns %6.4f unless denominator is less than or equal to MaxDen 
/// and in 2,4,8,16,32,64,128,256
//////////////////////////////////////////////////////////////////////
char *DoubleToStockString(double f, char *pszBuffer, int MaxDen = 256);

//////////////////////////////////////////////////////////////////////
/// Returns closest fraction string with a denominator less than or equal to MaxDen 
/// and in 2,4,8,16,32,64,128,256
//////////////////////////////////////////////////////////////////////
char *ForceDoubleToStockString(double f, char *pszBuffer, int MaxDen = 256);

//////////////////////////////////////////////////////////////////////
/// Convert a double to its fractional parts. Returns Error difference. 
/// If you want to get a Num and Den for a fractional part less than the default AllowedError,
/// pass an allowed error less than the fractional part. Otherwise the default should be fine.
//////////////////////////////////////////////////////////////////////
double FracParts(double f, __int64 &Whole, __int64 &Num, __int64 &Den, double AllowedError = FRACTION_MAX_PRECISION);

//////////////////////////////////////////////////////////////////////
/// Convert a double to its fractional parts. Returns Error difference. 
/// Limits the denominator to MaxDen  
//////////////////////////////////////////////////////////////////////
double FracParts(double f, __int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen);

//////////////////////////////////////////////////////////////////////
/// Convert a double to its fractional parts. Returns Error difference. 
/// Limits the denominator to MaxDen and always returns Den as 2,4,8,16,32,64,128,256 
//////////////////////////////////////////////////////////////////////
double FracPartsStock(double f, __int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen);


//#ifdef __cplusplus

/////////////////////////////////////////////////////////////////////////////
/// CFraction - Code for handling fraction strings
///
/// \author Dean Wyant  dwyant@mindspring.com
/////////////////////////////////////////////////////////////////////////////
///
/// CFraction - This class is used like a double. Do anything you would do with a double,
/// plus assign strings to it. When you need to get a string representation, use one of the 
/// String calls. My favorite is ForceToStockString which returns a string for the closest
/// valid stock transaction denominator up to MaxDen (default 256). This is great for 
/// applications that need to show stock prices in fractions without resorting to a table
/// lookup or showing unusual prices in decimal. 
///
/// C calls and plain calls for ASCIIZ support are provided.
///
/// A description of the \ref Fraction is also given.
/////////////////////////////////////////////////////////////////////////////
class CFraction
{
public:

  CFraction(const double d = 0) { m_d = d; }
  CFraction(const char *pszNum) { m_d = Fracatof(pszNum); }  

  operator double() const { return m_d; }

  // String calls 

#ifdef FRAC_CSTRING  // CString calls
  CFraction(const CString& S) { m_d = Fracatof(S); }  

	//////////////////////////////////////////////////////////////////////
	/// See ::Fracftoa
	//////////////////////////////////////////////////////////////////////
	CString ToString(double AllowedError = FRACTION_MAX_PRECISION);  

	//////////////////////////////////////////////////////////////////////
	/// See ::Fracftoa
	//////////////////////////////////////////////////////////////////////
	CString ToString(int MaxDen);                                    

	//////////////////////////////////////////////////////////////////////
	/// See ::DoubleToStockString
	//////////////////////////////////////////////////////////////////////
	CString ToStockString(int MaxDen = 256);         

	//////////////////////////////////////////////////////////////////////
	/// See ::ForceDoubleToStockString
	//////////////////////////////////////////////////////////////////////
	CString ForceToStockString(int MaxDen = 256);    

#endif

  // ASCIIZ string support
  char *ToStr(char *pszBuffer, double AllowedError = FRACTION_MAX_PRECISION);
  char *ToStr(char *pszBuffer, int MaxDen);
    // See Fracftoa above. Puts string in fraction format into pszBuffer

  char *ToStockStr(char *pszBuffer, int MaxDen = 256);      // See DoubleToStockString above.
  char *ForceToStockStr(char *pszBuffer, int MaxDen = 256); // See ForceDoubleToStockString above.

  // Mostly used internally, but you may need them?
  double Parts(__int64 &Whole, __int64 &Num, __int64 &Den, double AllowedError = FRACTION_MAX_PRECISION);
  double Parts(__int64 &Whole, __int64 &Num, __int64 &Den, int MaxDen);
    // See FracParts above

private:
  double m_d;	///< Internal storage for the double

};

//#endif //__cplusplus



/**
 \page Fraction Fraction Algorithm
 The formula for converting a decimal fraction to a string fraction is based on 
 reciprocals. Any fractional part can be represented as 1/(I1 + 1/(I2 + 1/(I3 + 1/In...
 where I1 is the integer part of the reciprocal of the fraction and I2 is the 
 integer part of the reciprocal minus I1... etc.
 
 To determine the integer numerator and denominator, the expression can be reduced. 
 It turns out that the numerator is simpler to calculate than the denominator, so the 
 denominator can simply be determined by dividing the numerator by the original decimal.

  \verbatim

  Reduction of 1/(I1 + 1/(I2 + 1/(I3 + 1/In... -

  1 I term:

   1
   --  Numerator N1 = 1
   I1

  
  2 I terms:

   1                          I2
   -----   Multiply by I2  =  -----------    
   I1 + 1    to reduce --     (I1*I2) + 1
        --             I2
        I2

      Numerator N2 = I2

  3 I terms

   1           1                (I2*I3) + 1                        
   -----    =  -------        = ---------------------   
   I1 + 1      I1 + I3          I1*((I2*I3) + 1) + I3                           
        ------      -----------                          
        I2 + 1      (I2*I3) + 1                              
             --                                                                   
             I3

      Numerator N3 = N2*I3 + N1     

  4 I terms
   1           1              1                           I2*((I3*I4) + 1) + I4                  
   ------      --------    =  ----------------         =  -----------------------------------------                    
   I1 + 1      I1 + 1         I1 + ((I3*I4) + 1)          I1*(I2*((I3*I4) + 1) + I4) + ((I3*I4) + 1)                 
        ----  =     -------         ---------------------                  
        I2 + 1      I2 + I4         I2*((I3*I4) + 1) + I4                 
             -------     -----------                               
             I3 + 1      (I3*I4) + 1
                  --                                
                  I4         
                  
       Numerator N4 = I4*(I3*I2 + 1) + I2 = N3*I4 + N2

                                                    
  5 I terms
   1            1                1                    1
   ------       ------           ------                --------------
   I1 + 1       I1 + 1           I1 + 1                I1 + (I3*((I4*I5) + 1) + I5)
        ------  =    ------   =       ----------------      -----------------------
        I2 + 1       I2 + 1           I2 + (I4*I5) + 1      I2*(I3*((I4*I5) + 1) + I5) + (I4*I5) + 1
             -------      -------          ---------------------
             I3 + 1       I3 + I5          I3*((I4*I5) + 1) + I5
                  -------      ------------
                  I4 + 1       (I4*I5) + 1
                       --          
                       I5

   = I2*(I3*((I4*I5) + 1) + I5) + (I4*I5) + 1
     ----------------------------------------------------------------------
     I1*(I2*(I3*((I4*I5) + 1) + I5) + (I4*I5) + 1) + (I3*((I4*I5) + 1) + I5)


        Numerator N5 = I5*(I4*(I3*I2 + 1) + I2) + I3*I2  + 1 = N4*I5 + N3
  
  \endverbatim 
 
  So, the numerator is fairly simple to calcualte based on two previous numerators.


  N1 = 1
  N2 = I2
  Nn = In*N[n-2] + N[n-3] 

  And since N1 = 1 and original saved numerator can be 0, the last equation can be used
  for all numerators except the first:
  N1 = 1
  Nn = In*N[n-1] + N[n-2] where N[0] = 0;

  So, this is fairly simple.

*/

#endif //__FRACTION_H__