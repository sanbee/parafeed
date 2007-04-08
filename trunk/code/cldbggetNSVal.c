/* $Id: cldbggetNSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return N values of Name as a integers
------------------------------------------------------------------------*/
int dbgclgetNSVal(char *Name, char **Val, int *m)
{
  int i,j,r=0,n=0;
  char tmp[8], *buf;
  Symbol *S;

  sprintf(tmp,"string[%d]",*m);
  S = SearchQSymb(Name, tmp);
  if (S==NULL) return FAIL;
  else 
    {
      S->Class = CL_DBGCLASS;
      i = S->NVals;
      if (i < *m)r = n = i;
      if (*m <= i){ n = *m; r = n;}
      for (j=1;j<= n;j++)
	{
	  buf = S->Val[j-1];
	  while (*buf == ' ') buf++;
	  strncpy(Val[j-1],buf,strlen(buf)+1);
	}
      return r;
    }
}

#ifdef __cplusplus
	   }
#endif
