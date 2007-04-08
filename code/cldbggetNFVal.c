/* $Id: cldbggetNFVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return N values of Name as a integers
------------------------------------------------------------------------*/
int dbgclgetNFVal(char *Name, float *val, int *m)
{
  int i=1,n;
  double d;
  char tmp[8];
  Symbol *S;

HANDLE_EXCEPTIONS(
  sprintf(tmp,"float[%d]",*m);
  S = SearchQSymb(Name, tmp);
  i=1;
  if (S) S->Class = CL_DBGCLASS;
  while(i <= *m)
    if ((n=clparseVal(S,&i,&d))==FAIL) return n;
    else if (n==0) break;
    else {val[i-1] = (float)d;i++;}

  return i-1;
)
}
#ifdef __cplusplus
	   }
#endif
