/* $Id: clgetNVals.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Get the number of values associated with Name
------------------------------------------------------------------------*/
int clgetNVals(char *Name)
{
  Symbol *S = SearchQSymb(Name, "Mixed[]");
  if (S!=NULL) 
    {
/*      S=AddQKey(Name,"UNKNOWN[]",
	      &cl_tmpTab, &cl_tmpTabTail);*/
      return S->NVals;
    }
  else
    return FAIL;
}
#ifdef __cplusplus
	   }
#endif
