/* $Id: DeleteVar.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
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
int DeleteVar(char *Name, Symbol** Head, Symbol** Tail)
{
  Symbol* S;
  
  if ((S=SearchVSymb(Name,*Head))==NULL) return 0;
  else
    {
      DeleteVNode(S, Head, Tail);
      return 1;
    }
}
#ifdef __cplusplus
	   }
#endif
