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
/* $Id: clgetNSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return N values of key Name as a strings.
------------------------------------------------------------------------*/
int clgetNSVal(char *Name, char **Val, int *m)
{
  int i,j,r=0,n=0;
  char tmp[8], *buf;
  Symbol *S;

  if (*m <= 0) sprintf(tmp,"string[]");
  else sprintf(tmp,"string[%d]",*m);

  S = SearchQSymb(Name, tmp);
  if (S==NULL) return FAIL;
  else 
    {
      i = S->NVals;
      if (i < *m)r = n = i;
      if (*m <= i){ n = *m; r = n;}
      for (j=1;j<= n;j++)
	{
	  buf = S->Val[j-1];
	  while (*buf == ' ') buf++;
	  strncpy(Val[j-1],buf,strlen(buf)+1);
	}
      return r;
    }
}

#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus
int clgetNSValp(const string& Name, vector<string>& Val, int& m)
{
  int j,r=0,n=0;
  char tmp[8], *buf;
  Symbol *S;

  if (m <= 0) sprintf(tmp,"string[]");
  else sprintf(tmp,"string[%d]",m);

  S = SearchQSymb((char *)Name.c_str(), tmp);
  if (S==NULL) return FAIL;
  else 
    {
      n = S->NVals;
      for (j=0;j< n;j++)
	{
	  buf = S->Val[j];
	  while (*buf == ' ') buf++;
	  Val.resize(j+1);
	  Val[j]=buf;
	  //	  strncpy(Val[j-1],buf,strlen(buf)+1);
	}
      return r;
    }
}
#endif
