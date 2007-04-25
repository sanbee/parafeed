/* $Id: cldbggetFVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp sanjay $ */
#include <shell.h>
#include <cllib.h>
#include <support.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an float
------------------------------------------------------------------------*/
int dbgclgetFVal(char *Name, float *val, int *n)
{
  Symbol *S;
  int N;
  double d;
  
HANDLE_EXCEPTIONS(
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,"float");

  if (S != NULL) S->Class=CL_DBGCLASS;

  if ((N=clparseVal(S,n,&d))!=FAIL) *val=(float)d;
  return N;
)
}

int dbgclgetFValp(const string& Name, float &val, int &n)
{
  Symbol *S;
  int N;
  double d;

  HANDLE_EXCEPTIONS(  
  if (n < 0)
    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
  else
    S=SearchQSymb((char *)Name.c_str(),"float");

  if (S != NULL) S->Class=CL_DBGCLASS;
  setAutoFDefaults(S,val);

  if ((N=clparseVal(S,&n,&d))>0) val=(float)d;
  return N;
  )
}   

#ifdef __cplusplus
	   }
#endif
