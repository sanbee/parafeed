/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
/* $Id: AddVar.c,v 2.0 1998/11/11 07:08:39 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <shell.tab.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
Symbol *AddVar(const char *Name, Symbol **Head, Symbol **Tail)
{
  Symbol *S = (Symbol *)AllocVSymb(1);
  if (S==NULL) return NULL;
  else S->Name = (char *)getmem(strlen(Name)+1, "AddVar");
  strcpy(S->Name,Name);
  AddVNode(S,Head,Tail);
  return S;
}
#ifdef __cplusplus
	   }
#endif
