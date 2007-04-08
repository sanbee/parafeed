/* $Id: clgetOpt.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cl.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
int clgetOpt(char *Name)
{
  return clTgetOpt(Name,"Mixed[]");
}
#ifdef __cplusplus
	   }
#endif
