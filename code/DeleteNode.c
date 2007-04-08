/* $Id: DeleteNode.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Delete a node from the list who's head and tail are pointed to by
   Head and Tail pointers.
------------------------------------------------------------------------*/
int DeleteVNode(Symbol *S, Symbol** Head, Symbol** Tail)
{
  if (*Head == *Tail)           /* Only one elment and that is S */
    {
      FreeVSymb(S); /*free(S);*/
      *Head = *Tail = NULL;
    }
  else if (S==*Head)            /* Top of the table */
    {
      S->Next->Previous=NULL;
      *Head = S->Next;
      FreeVSymb(S);/*free(S);*/
    }
  else if (S==*Tail)           /* Bottom of the table */
    {
      S->Previous->Next = NULL;
      *Tail = S->Previous;
      FreeVSymb(S);/*free(S);*/
    }
  else                        /* Somewhere in between */
    {
      S->Next->Previous = S->Previous;
      S->Previous->Next = S->Next;
      FreeVSymb(S);/*free(S);*/
    }
  return 1;
}

int DeleteCNode(CmdSymbol *S, CmdSymbol** Head, CmdSymbol** Tail)
{
  if (*Head == *Tail)           /* Only one elment and that is S */
    {
      free(S);
      *Head = *Tail = NULL;
    }
  else if (S==*Head)            /* Top of the table */
    {
      S->Next->Previous=NULL;
      *Head = S->Next;
      free(S);
    }
  else if (S==*Tail)            /* Bottom of the table */
    {
      S->Previous->Next = NULL;
      *Tail = S->Previous;
      free(S);
    } 
  else                         /* Somewhere in between */
    {
      S->Next->Previous = S->Previous;
      S->Previous->Next = S->Next;
      free(S);
    }
  return 1;
}
#ifdef __cplusplus
}
#endif
