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
/* $Id: AddCmd.c,v 2.1 1999/08/13 12:18:30 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <shell.tab.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
int AddCmd(char *Name, char *Doc, int (*func)(char *), 
	   CmdSymbol **Head, CmdSymbol **Tail)
{
  CmdSymbol *S = AllocCSymb(1);
  if (S==NULL) return -1;

  S->Name = (char *)getmem(strlen(Name)+1, "AddCmd");
  strcpy(S->Name,Name);
  S->func = func;

  if (Doc)
    {
      S->Doc = (char *)getmem(strlen(Doc)+1, "AddCmd");
      strcpy(S->Doc,Doc);
    }
  AddCNode(S,Head,Tail);
  return 1;
}
#ifdef __cplusplus
	   }
#endif
