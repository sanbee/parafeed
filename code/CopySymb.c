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
/* $Id: CopySymb.c,v 2.1 1999/06/16 16:50:36 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int CopyVSymb(Symbol *t, Symbol *S,int CopyMode)
{
  if (CopyMode & COPYNAME)
    {
      t->Name = (char *)getmem(strlen(S->Name)+1,"InstallSymb");
      strcpy(t->Name,S->Name);
    }

  if (CopyMode & COPYCLASS) t->Class = S->Class;

  if ((CopyMode & COPYTYPE) && S->Type)
    {
      if (t->Type == NULL)
	t->Type=(char *)getmem(strlen(S->Type)+1,"CopyVSymb");
      else t->Type = (char *)realloc(t->Type,strlen(S->Type)+1);
      strcpy(t->Type,S->Type);
    }
      
  if ((CopyMode & COPYVAL) && S->NVals>0)
    {
      t->NVals = S->NVals;
      t->Used = S->Used;
      t->Val = S->Val;
      /* if (t->Val!=NULL)  */
      /* 	t->Val = (char **)realloc(t->Val,sizeof(char **)*S->NVals); */
      /* else t->Val = (char **)calloc(1,sizeof(char **)*S->NVals); */
      /* for (i=0;i<S->NVals;i++) */
      /* 	{ */
      /* 	  if (S->Val[i] != NULL) */
      /* 	    { */
      /* 	      t->Val[i] = (char *)getmem(strlen(S->Val[i])+1,"CopySymb"); */
      /* 	      strcpy(t->Val[i],S->Val[i]); */
      /* 	    } */
      /* 	} */
    }
  //
  // The following are pure C++ objects. Features due to these are not available
  // vis C interface
  //
#ifdef __cplusplus
  t->smap = S->smap;
  t->Exposed = S->Exposed;
  t->DefaultVal=S->DefaultVal;
  t->Options = S->Options;
#endif
  t->Attributes = S->Attributes;
  return 1;
}
/*----------------------------------------------------------------------*/
int CopyCSymb(CmdSymbol *t, CmdSymbol *S, int CopyName)
{
  if (CopyName)
    {
      if (t->Name==NULL)
	t->Name = (char *)getmem(strlen(S->Name)+1,"InstallSymb");
      else 
	t->Name = (char *)realloc(t->Name,strlen(S->Name)+1);
      strcpy(t->Name,S->Name);
    }
  t->func = S->func;
  t->Attributes = S->Attributes;
  if (S->Doc)
    {
      if (t->Doc==NULL)
	t->Doc = (char *)getmem(strlen(S->Doc)+1,"CopyCSymb");
      else t->Doc = (char *)realloc(t->Doc,strlen(S->Doc)+1);
      strcpy(t->Doc,S->Doc);
    }
  return 1;
}
#ifdef __cplusplus
}
#endif
