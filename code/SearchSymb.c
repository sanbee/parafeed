/* $Id: SearchSymb.c,v 2.0 1998/11/11 07:12:41 sanjay Exp sbhatnag $ */
#include <stdio.h>
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif

Symbol *SearchVSymbFullMatch(char *Name, Symbol *Tab)
{
  Symbol *t;
  for (t=Tab;t!=NULL;t=t->Next)
    if (!strcmp(t->Name,Name)) break;
  return t;
}
Symbol *SearchVSymb(char *Name, Symbol *Tab){return minmatch(Name,Tab);}
/*----------------------------------------------------------------------*/
CmdSymbol *SearchCSymb(char *Name, CmdSymbol *Tab)
{
  CmdSymbol *t;
  for (t=Tab;t!=NULL;t=t->Next)
    if (!strcmp(t->Name,Name)) break;
  return t;
}
#ifdef __cplusplus
	   }
#endif
