/* $Id: clclearOptsList.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <stdlib.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
void clclearOptsList(char ***l,int n)
{
  int i;
  if (n > 0)
    {
      for (i=0;i<n;i++)
	free((*l)[i]);
      free(l);
    }
  l=0;
}
#ifdef __cplusplus
	   }
#endif
