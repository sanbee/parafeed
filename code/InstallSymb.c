/* $Id: InstallSymb.c,v 2.1 1998/11/30 05:37:47 sanjay Exp sanjay $ */
#include <stdio.h>
#include <cl.h>
#include <shell.h>
#include "shell.tab.h"

#ifdef __cplusplus
extern "C" {
#endif
extern Symbol *cl_SymbTab, *cl_TabTail;
extern CmdSymbol *cl_CmdTab, *cl_CmdTail;
/*----------------------------------------------------------------------*/
int InstallSymb()
{
#include "SymbTab.h"

  CmdSymbol *t;
  int i;
  for (i=0;CmdTab[i].Name!=NULL;i++)
    {
      t = AllocCSymb(1);
      if (CopyCSymb(t,&CmdTab[i],1)<0)
	clThrowUp("Error in install symbol table","###Fatal",
			 CL_FATAL);
      AddCNode(t,&cl_CmdTab,&cl_CmdTail);
    }
  return 1;
}

#ifdef __cplusplus
	   }
#endif
