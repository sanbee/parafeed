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
/* $Id: clgetNIVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <vector>
#include <support.h>
#include <sstream>
#include <clgetBaseCode.h>
#include <clgetValp.h>
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
    Symbol *S;
    
    HANDLE_EXCEPTIONS(
		      std::ostringstream os;
		      if (*m <= 0)
			os << "int[]";
		      //sprintf(tmp,"bool[]");
		      else
			os << "int[" << *m << "]";
		      //sprintf(tmp,"bool[%d]",*m);
		      
		      S = SearchQSymb(Key, os.str());
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
  HANDLE_EXCEPTIONS(
		    SMap EmptyMap;
		    return clgetNValp(Key,val,m,EmptyMap);
		    )
}
int clgetNIValp(const string& Key, vector<int>& val, int& m, SMap& smap)
{
  HANDLE_EXCEPTIONS(
		    return clgetNValp(Key,val,m,smap);
		    )
}
#endif
