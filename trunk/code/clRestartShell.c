/* $Id: clRestartShell.c,v 2.2 1998/12/09 04:57:25 sanjay Exp sanjay $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
  Restart the shell if the symbol table still exist.
------------------------------------------------------------------------*/
void clRestartShell()
{
  cl_InteractiveShell = 1;
  if (cl_DOCLEANUP)
    /*
    if (clThrowUp("Can not restart the shell after clInteractive(1)",
		     "###Fatal",CL_FATAL)==CL_FATAL) exit(-1);
    */
    clThrowUp("Can not restart the shell after clInteractive(1)",
	      "###Fatal",CL_FATAL);
  else if (cl_NoPrompt) exit(1);
  else
    {
      cl_Pass=0;
      longjmp(*cl_env,1);
    }
}
#ifdef __cplusplus
	   }
#endif
