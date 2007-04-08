/* $Id: clgetNOpts.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Get the total number of commandline options detected
------------------------------------------------------------------------*/
int clgetNOpts()
{
  return cl_NoOfOpts;
}
#ifdef __cplusplus
	   }
#endif
