/* $Id: clReset.c,v 2.1 1999/09/15 11:45:33 sanjay Exp $ */

#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned short cl_RegistrationMode,cl_Pass,cl_SymbLoaded;

/*----------------------------------------------------------------------*/
  void clReset(){ cl_Pass = 0; cl_SymbLoaded = 1;}

#ifdef __cplusplus
	   }
#endif
