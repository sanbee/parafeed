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
/* $Id: clloadConfig.c,v 2.2 2000/02/16 04:20:47 sanjay Exp $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>
#define MAXBUF 128
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int clloadConfig(char *File)
{
  char str[MAXBUF]={0};
  char *Name, *Val;
  int i;
  Symbol *S;
  FILE *fd;

  if (File==NULL) return 1;
  if ((fd = fopen(File,"r"))==NULL)
    {/*fprintf(stderr,"###Error: In opening file \"%s\"\n",File);*/return -2;}

  while (fgets(str,MAXBUF,fd)!=NULL)
    {
      stripwhite(str);str[strlen(str)-1]='\0';
      if (strlen(str) > 0)
	{
	  BreakStr(str,&Name,&Val);

	  S=SearchVSymb(Name,cl_SymbTab);
	  if (S==NULL)
	    {
	      S=AddVar(Name,&cl_SymbTab,&cl_TabTail);
	      S->Class=CL_USERCLASS;
	    }
	  if (S->NVals==0)
	      SetVar(Name,Val,cl_SymbTab,1,0,1);
	  if (Name != NULL) free(Name);
	  if (Val != NULL) free(Val);
	  for(i=0;i<MAXBUF;i++) str[i]='\0';
	}
    }
  return 1;
}
#ifdef __cplusplus
	   }
#endif
