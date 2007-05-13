/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
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
