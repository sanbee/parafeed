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
/* $Id: TransferTab.c,v 2.2 1999/06/16 16:49:22 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void TransferTab(Symbol **Target, Symbol **TTail, Symbol *Source,int AddVar)
{
  Symbol *T,*S;
  unsigned int i=COPYNAME|COPYCLASS|COPYVAL;

  for (S=Source;S;S=S->Next)
    {
      //      if ((T = SearchVSymb(S->Name,*Target))==NULL)
      if ((T = SearchVSymbFullMatch(S->Name,*Target))==NULL)
	{
	  T = AllocVSymb(1);
	  CopyVSymb(T,S,i);
	  AddVNode(T,Target,TTail);
	}
      else if (AddVar)  CopyVSymb(T,S,i);

      CopyVSymb(T,S,COPYTYPE);
	  
    }
}
#ifdef __cplusplus
	   }
#endif
