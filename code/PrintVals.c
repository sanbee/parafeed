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
/* $Id: PrintVals.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int PrintVals(FILE *fd,Symbol *S)
{
  unsigned int i;
  unsigned int N=S->NVals;
  if (N==0) 
    {
      N = S->DefaultVal.size();
      char *str;
      if (N>0)
	{
	  for(i=0;i<N-1;i++)
	    {
	      str=(char *)S->DefaultVal[i].c_str();
	      tokenize(fd,str,",",CL_ESC);
	      fputc(',',fd);
	    }
	  str=(char *)S->DefaultVal[N-1].c_str();
	  tokenize(fd,(char *)S->DefaultVal[N-1].c_str(),",",CL_ESC);
	  fputc('\n',fd);
	}
      else
	fprintf(fd,"\n");
      return 1;
    }

  {
    for (i=0;i<S->NVals-1;i++)
      {tokenize(fd,S->Val[i],",",CL_ESC);fputc(',',fd);}
    tokenize(fd,S->Val[S->NVals-1],",",CL_ESC);
    fputc('\n',fd);
    return 1;
  }

  fprintf(fd,"\n");
  return 1;
}
#ifdef __cplusplus
	   }
#endif
