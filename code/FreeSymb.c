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
/* $Id: FreeSymb.c,v 2.0 1998/11/11 07:12:27 sanjay Exp $ */

#include <stdio.h>
#include <clshelldefs.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int FreeVSymb(Symbol *S)
{
  unsigned int i;
  if (S != NULL)
    {
      if (S->Name != NULL) free(S->Name);
      if (S->Type != NULL) free(S->Type);
      if (S->NVals >0)
	{
	  for (i=0;i<S->NVals;i++)
	    S->Val[i]="";//free((S->Val)[i]);
	  S->Val.resize(0);//free((S->Val));
	}
#ifdef __cplusplus
      S->smap.erase(S->smap.begin(),S->smap.end());
      S->DefaultVal.resize(0);
      S->Options.resize(0);
#endif
      //      free(S);
      delete S;
      return 1;
    }
  return 0;
}
/*----------------------------------------------------------------------*/
int FreeCSymb(CmdSymbol *S)
{
  if (S!=NULL)
    {
      if (S->Doc != NULL) free(S->Doc);
      if (S->Name != NULL) free(S->Name);
      free(S);
      return 1;
    }
  return 0;
}

#ifdef __cplusplus
}
#endif
