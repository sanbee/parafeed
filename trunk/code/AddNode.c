/* $Id: AddNode.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <stdio.h>
#include <clshelldefs.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Add a node to the end of the list and move the Tail pointer to the
   new node.
   
   If Head is NULL, then it is assumed that there is no node in the list
   (the list is starting now).
------------------------------------------------------------------------*/
int AddVNode(Symbol* S, Symbol** Head, Symbol** Tail)
{
  if (*Head == NULL)
    {
      *Head = S;
      (*Head)->Next = (*Head)->Previous = NULL;
      *Head = *Tail = S;
    }
  else
    {
      S->Next = NULL;
      (*Tail)->Next = S;
      S->Previous = *Tail;
      *Tail = S;
    }
  return 1;
}
int AddCNode(CmdSymbol* S, CmdSymbol** Head, CmdSymbol** Tail)
{
  if (*Head == NULL)
    {
      *Head = S;
      (*Head)->Next = (*Head)->Previous = NULL;
      *Head = *Tail = S;
    }
  else
    {
      S->Next = NULL;
      (*Tail)->Next = S;
      S->Previous = *Tail;
      *Tail = S;
    }
  return 1;
}
#ifdef __cplusplus
}
#endif
