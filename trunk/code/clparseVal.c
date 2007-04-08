/* $Id: clparseVal.c,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#include <shell.h>
#include <cllib.h>
#include <cl.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Convert the n th value of symbol S into a double
------------------------------------------------------------------------*/

int clparseVal(Symbol *S, int *Which, double *d)
{
  unsigned int N = _ABS(*Which),n;

HANDLE_EXCEPTIONS(
  if (S != NULL)
    {
      if (N > S->NVals) return 0;
      if ((n=calc(S->Val[N-1],d)))
	{
	  char msg[128];
	  sprintf(msg,"In conversion of %s[%d]=%s",S->Name,N-1,S->Val[N-1]);
	  clThrowUp(msg,"###Error",FAIL);
	}
      return 1;
    }
  else return FAIL;
)
}
#ifdef __cplusplus
	   }
#endif
