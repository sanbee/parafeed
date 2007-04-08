/* $Id: cldbggetIVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an integer
------------------------------------------------------------------------*/
int dbgclgetIVal(char *Name, int *val, int *n)
{
  Symbol *S;
  double d;
  int N;

HANDLE_EXCEPTIONS(

  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);  
  else
    S=SearchQSymb(Name,"int");

  if (S != NULL) S->Class=CL_DBGCLASS;

  if ((N=clparseVal(S,n,&d))!=FAIL) *val = (int)d;

  return N;
)
}
#ifdef __cplusplus
	   }
#endif
