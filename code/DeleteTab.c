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
/* $Id: DeleteTab.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int DeleteVTab(Symbol **Head, Symbol **Tail)
{
  while ((*Head)) DeleteVNode(*Head,Head,Tail);
  return 1;
}
/*----------------------------------------------------------------------*/
int DeleteCTab(CmdSymbol **Head, CmdSymbol **Tail)
{
  while ((*Head)) DeleteCNode(*Head,Head,Tail);
  return 1;
}

#ifdef __cplusplus
}
#endif