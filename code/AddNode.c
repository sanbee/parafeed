/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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
