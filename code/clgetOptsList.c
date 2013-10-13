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
/* $Id: clgetOptsList.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#define OPTSLEN 128

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/* Pritty prints the keywords and values in a array of strings.  Returns     */
/* the number of strings it outputs.                                         */
/*---------------------------------------------------------------------------*/
int clgetOptsList(char ***list)
{
  unsigned k;
  int n=0,i=0;
  Symbol *s;
  char t[OPTSLEN],**tmp;
  
  for (s=cl_SymbTab;s;s=s->Next) if (s->Class == CL_APPLNCLASS) n++;
  if (cl_InteractiveShell==1) 
    {
      *list = (char **)getmem(sizeof(char **)*n,"getOptsList");
      tmp = *list;
      tmp[0] = (char *)getmem(sizeof(char)*strlen(cl_ProgName),
			      "getOptsList");
      strcpy(tmp[0],cl_ProgName);
      for (s=cl_SymbTab;s;s=s->Next)
	if (s->Class==CL_APPLNCLASS)
	  {
	    i++;
	    sprintf(t,"   %-10.10s = %c",s->Name,'\0');
	    if (s->NVals > 0) 
	      {
		if (strlen(s->Val[0]) >= OPTSLEN - strlen(t))
		  fprintf(stderr,
			  "###Informational:  Not enough space for putting parameters in history\n");
		else
		  {
		    strcat(t,s->Val[0]);
		    for (k=1;k<s->NVals;k++)
		      {
			strcat(t,",");
			strcat(t,s->Val[k]);
		      }
		  }
	      }
	    tmp[i] = (char *)getmem(sizeof(char)*strlen(t),"getOptsList");
	    strcpy(tmp[i],t);
	  }
    }
  return n+1;
}
#ifdef __cplusplus
	   }
#endif
