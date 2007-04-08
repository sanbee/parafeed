//-*-C++-*-
//
// $Id: clError.h,v 2.1 1999/09/15 12:00:51 sanjay Exp sanjay $
//
#if !defined(CLERROROBJ_H)
#define CLERROROBJ_H
#ifdef __cplusplus

/*--------------------------------------------------------------------
  The C++ object thrown in case of error.  It's derived from ErrorObj
  class, which is the baseclass for all error objects.
--------------------------------------------------------------------*/

#include <ErrorObj.h>
class clError: public ErrorObj 
{
public:
  clError() {};
  clError(const char *m, const char *i, int l=0): ErrorObj(m,i,l) {};
};

/*---------------------------------------------------------------------
  In the entire library, the following routine is used always when an
  exception is to be thrown.  

  For C++, this in turn, throw's a clError object.
---------------------------------------------------------------------*/

//extern "C" {
  //inline int clThrowUp(const char *m, const char *i, int l){throw(clError(m,i,l));}
#define clThrowUp(m, i, l) ({throw(clError((m),(i),(l)));})
//	   }
#else

/*---------------------------------------------------------------------
  For C linkage it simply prints the error message on the stderr, and
  returns the supplied error value.
---------------------------------------------------------------------*/
#include <stdio.h>
inline int clThrowUp(const char *m, const char *i, int l){fprintf(stderr,"%s: %s\n",i,m);return l;}


#endif /* __cplusplus */

#endif /* CLERROR_H */
