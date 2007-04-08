/* $Id: AddVar.c,v 2.0 1998/11/11 07:08:39 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <shell.tab.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
Symbol *AddVar(char *Name, Symbol **Head, Symbol **Tail)
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
