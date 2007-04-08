/* $Id: clgetInputFile.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the input file name.  This is just the value of the "in" key
   word.  

   This routine is just of backward compatibility (with the old CLlib).
------------------------------------------------------------------------*/
char *clgetInputFile()
{
  Symbol *S;
  if ((S=SearchQSymb("in","string"))!=NULL)
    if (S->NVals > 0) return S->Val[0];
    else return NULL;
  else return NULL;
}
#ifdef __cplusplus
	   }
#endif
