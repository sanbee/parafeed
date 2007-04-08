/* $Id: DeleteCmd.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>
#include "shell.tab.h"
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Delete a node from the list who's head and tail are pointed to by
   Head and Tail pointers.
------------------------------------------------------------------------*/
int DeleteCmd(char *Name, CmdSymbol** Head, CmdSymbol** Tail)
{
  CmdSymbol* S;
  
  if ((S=SearchCSymb(Name,*Head))==NULL) return 0;
  else
    {
      DeleteCNode(S, Head, Tail);
      return 1;
    }
}
#ifdef __cplusplus
	   }
#endif
