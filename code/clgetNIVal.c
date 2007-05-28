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
/* $Id: clgetNIVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <vector>
#include <support.h>
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/* Extract a m number of values associated with Key.  If *m is negative,     */
/* fatal error occurs.  If the return value is positive, it is the number of */
/* associated values that are returned in *Vals.  Vals should have enough    */
/* space to hold the returned values.                                        */
/*---------------------------------------------------------------------------*/
int clgetNIVal(char *Key, int *val, int *m)
{
  int i=1,n;
  double d;
  char tmp[8];
  Symbol *S;

HANDLE_EXCEPTIONS(
  if (*m <= 0) sprintf(tmp,"int[]");
  else sprintf(tmp,"int[%d]",*m);

  S = SearchQSymb(Key, tmp);
  i=1;
  while(i <= *m)
    if ((n=clparseVal(S,&i,&d))==CL_FAIL) return n;
    else if (n==0) break;
    else {val[i-1] = (int)d;i++;}

  return i-1;
)
}

#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus
  int clgetNIValp(const string& Key, vector<int>& val, int& m)
  {
    int n,n0;
    double d;
    char tmp[8];
    Symbol *S;
    
   HANDLE_EXCEPTIONS(
		      if (m <= 0) sprintf(tmp,"int[]");
		      else sprintf(tmp,"int[%d]",m);
		      
		      S = SearchQSymb((char *)Key.c_str(), tmp);
		      //
		      // Remember the number of values set by the user.
		      //
		      n0=S->NVals;
		      setAutoNIDefaults(S,val);
		      int i=1;
		      for(int j=0;j<n0;j++)
			{
			  if ((n=clparseVal(S,&i,&d))!=CL_FAIL)
			    {
			      if (n==0) {m=S->NVals=i-1;return i-1;}
			      else 
				{
				  val.resize(i);
				  val[i-1] = (int)d;
				  i++;
				}
			    }
			}
		      m=S->NVals=i-1;
		      return i-1;
	    );
  }
#endif
