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
