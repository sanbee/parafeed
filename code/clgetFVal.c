/* $Id: clgetFVal.c,v 2.1 1999/03/23 15:01:49 sanjay Exp $ */
#include <shell.h>
#include <cllib.h>
#include <string.h>
#include <support.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an float
------------------------------------------------------------------------*/
int clgetFVal(char *Name, float *val, int *n)
{
  Symbol *S;
  int N;
  double d;

HANDLE_EXCEPTIONS(  
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,"float");
  if ((N=clparseVal(S,n,&d))>0) *val=(float)d;
  return N;
)
}   

#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus
int clgetFValp(const string& Name, float &val, int &n)
{
  Symbol *S;
  int N;
  double d;

  HANDLE_EXCEPTIONS(  
  if (n < 0)
    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
  else
    S=SearchQSymb((char *)Name.c_str(),"float");
  setAutoFDefaults(S,val);
  if ((N=clparseVal(S,&n,&d))>0) val=(float)d;
  return N;
  )
}
#endif   

