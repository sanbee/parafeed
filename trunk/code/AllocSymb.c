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
/* $Id: AllocSymb.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <cllib.h>
#include <clshelldefs.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
Symbol *AllocVSymb(int n)
{
  Symbol *S;
  S = (Symbol *)getmem(sizeof(Symbol)*n,"AllocVSymb");
  S->Name = NULL;
  S->Type = NULL;
  S->Val = NULL;
  S->Exposed=1;
  S->Used = 0;
  S->Class = CL_APPLNCLASS;
  S->NVals=0;
  S->DefaultVal.resize(0);
  S->Options.resize(0);
  //  S->Attributes=CL_KEYWORD;
  S->Attributes=0;
  SETBIT(S->Attributes,CL_KEYWORD);
  return S;
}

CmdSymbol *AllocCSymb(int n)
{
  CmdSymbol *S;
  S = (CmdSymbol *)getmem(sizeof(CmdSymbol)*n,"AllocCSymb");
  S->Name = NULL;
  S->Doc  = NULL;
  S->func = NULL;
  return S;
}
#ifdef __cplusplus
}
#endif
