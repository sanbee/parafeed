/* $Id: clfInteractive.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int clfInteractive(int *i)
{  
  cl_tmpTab = cl_tmpTabTail = NULL;
  cl_InteractiveShell = 1;
  cl_DOCLEANUP = *i;
  cl_FORTRAN=1;
  return 1;
}
#ifdef __cplusplus
	   }
#endif
