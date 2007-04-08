/* $Id: clCleanUp.c,v 2.0 1998/11/11 07:12:46 sanjay Exp $ */

#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned short cl_RegistrationMode,cl_Pass,cl_SymbLoaded;
/*----------------------------------------------------------------------*/
void clCleanUp()
{
  if (cl_SymbTab) DeleteVTab(&cl_SymbTab,&cl_TabTail);
  if (cl_CmdTab) DeleteCTab(&cl_CmdTab,&cl_CmdTail);
  if (cl_ProgName) {free(cl_ProgName); cl_ProgName = NULL;}
  if (cl_CommandLine) {free(cl_CommandLine); cl_CommandLine = NULL;}
  cl_RegistrationMode=1;
  cl_Pass = 0;
  cl_SymbLoaded= 0;
}

#ifdef __cplusplus
	   }
#endif
