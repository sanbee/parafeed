/* $Id: cldbggetNIVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
/* extern "C" { */
/* #endif */
/*---------------------------------------------------------------------------*/
/* Extract a m number of values associated with Key.  If *m is negative,     */
/* fatal error occurs.  If the return value is positive, it is the number of */
/* associated values that are returned in *Vals.  Vals should have enough    */
/* space to hold the returned values.                                        */
/*---------------------------------------------------------------------------*/
int dbgclgetNIVal(char *Key, int *val, int *m)
{
  int i=1,n;
  double d;
  char tmp[8];
  Symbol *S;

HANDLE_EXCEPTIONS(
  sprintf(tmp,"int[%d]",*m);
  S = SearchQSymb(Key, tmp);
  i=1;
  if (S) S->Class = CL_DBGCLASS;
  while(i <= *m)
    if ((n=clparseVal(S,&i,&d))==FAIL) return n;
    else if (n==0) break;
    else {val[i-1] = (int)d;i++;}

  return i-1;
)
}
/* #ifdef __cplusplus */
/* 	   } */
#endif
