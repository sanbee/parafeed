/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
 *
 * This program is free software; you can redistribute it and/or modify
 nv * it under the terms of the GNU General Public License as published by
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
/* $Id: clgetNFVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <support.h>
#include <sstream>
#include <clgetBaseCode.h>
#ifdef __cplusplus
extern "C" {
#endif
  /*------------------------------------------------------------------------
    Return N values of Name as a integers
    ------------------------------------------------------------------------*/
  int clgetNFVal(char *Name, float *val, int *m)
  {
    int i=1,n;
    double d;
    Symbol *S;
    
    HANDLE_EXCEPTIONS(
		      std::ostringstream os;
		      if (*m <= 0)
			os << "float[]";
		      //sprintf(tmp,"bool[]");
		      else
			os << "float[" << *m << "]";
		      //sprintf(tmp,"bool[%d]",*m);
		      
		      S = SearchQSymb(Name, os.str());
		      i=1;
		      
		      while(i <= *m)
			if ((n=clparseVal(S,&i,&d))==CL_FAIL) return n;
			else if (n==0) break;
			else {val[i-1] = (float)d;i++;}
		      )
      return i-1;
  }
  
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
int clgetNFValp(const string& Key, vector<float>& val, int& m)
{
  SMap EmptyMap;
  return clgetNValp(Key,val,m,EmptyMap);
  // int n,n0;
  // double d;
  // Symbol *S;
  
  // HANDLE_EXCEPTIONS(
  // 		      std::ostringstream os;
  // 		      if (m <= 0)
  // 			os << "float[]";
  // 		      //sprintf(tmp,"bool[]");
  // 		      else
  // 			os << "float[" << m << "]";
  // 		      //sprintf(tmp,"bool[%d]",*m);
  
  // 		      S = SearchQSymb((char *)Key.c_str(), os.str());
  // 		      setAutoNFDefaults(S,val);
  // 		      n0=S->NVals;
  // 		      int i=1;
  // 		      for (int j=0;j<n0;j++)
  // 			{
  // 			  if ((n=clparseVal(S,&i,&d))!=CL_FAIL)
  // 			    {
  // 			      if (n==0) {m=S->NVals=i-1;return i-1;}
  // 			      else 
  // 				{
  // 				  val.resize(i);
  // 				  val[i-1] = (float)d;
  // 				  i++;
  // 				}
  // 			    }
  // 			}
  // 		      m=S->NVals=i-1;
  // 		      return i-1;
  // 		      );
}
int clgetNFValp(const string& Key, vector<float>& val, int& m, SMap& smap)
{
  return clgetNValp(Key,val,m,smap);
}
#endif
