/* $Id: clgetCommandLine.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
char *clgetCommandLine()
{
  return cl_CommandLine;
}
#ifdef __cplusplus
	   }
#endif
