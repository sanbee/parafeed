/* $Id: FreeSymb.c,v 2.0 1998/11/11 07:12:27 sanjay Exp $ */

#include <stdio.h>
#include <clshelldefs.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int FreeVSymb(Symbol *S)
{
  unsigned int i;
  if (S != NULL)
    {
      if (S->Name != NULL) free(S->Name);
      if (S->Type != NULL) free(S->Type);
      if (S->NVals >0)
	{
	  for (i=0;i<S->NVals;i++)
	    free((S->Val)[i]);
	  free((S->Val));
	}
      free(S);
      return 1;
    }
  return 0;
}
/*----------------------------------------------------------------------*/
int FreeCSymb(CmdSymbol *S)
{
  if (S!=NULL)
    {
      if (S->Doc != NULL) free(S->Doc);
      if (S->Name != NULL) free(S->Name);
      free(S);
      return 1;
    }
  return 0;
}

#ifdef __cplusplus
}
#endif
