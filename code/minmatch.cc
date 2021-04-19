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
/* $Id: minmatch.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <string.h>
#include <clshelldefs.h>
#include <cllib.h>
#define NN 50

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
Symbol *minmatch(const char *str, Symbol *List)
{
  int i,m=0;
  Symbol *s,*l[NN];
  
  for (s=List;s;s=s->Next)
    if (!strncmp(str,s->Name,strlen(str))) 
      {
	if (m<NN) l[m++]=s;
	else
	  {fprintf(stderr,"###Error: Mem overflow in minmatch\n");exit(-1);}
	if (strlen(str) == strlen(s->Name)) break;
      }  
  if (m > 1)
    {
      //
      // If more than one min-matched names were found, see if there
      // is any that matches completely.  If a complete match is
      // found, return that symbol.  Else there is an ambiguity.
      //  
      for(int j=0;j<m;j++)
	if (strcmp(str,l[j]->Name)==0) return l[j];
      fprintf(stderr,"###Ambiguous min-match: ");
      for (i=0;i<m;i++) fprintf(stderr,"%s ",l[i]->Name);
      fprintf(stderr,"\n");
      return NULL;
    }
  if (m==1)
    {
      s=l[0];
      return s;
    }
  else
    return NULL;
}
#ifdef __cplusplus
	   }
#endif
