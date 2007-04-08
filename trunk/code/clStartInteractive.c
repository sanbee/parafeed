/* $Id: clStartInteractive.c,v 2.0 1998/11/11 07:12:57 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
void clStartInteractive(jmp_buf *e, int i)
{
  cl_tmpTab = cl_tmpTabTail = NULL;
  cl_InteractiveShell = 1;
  cl_DOCLEANUP = i;
  cl_env = e;
}  
#ifdef __cplusplus
	   }
#endif
