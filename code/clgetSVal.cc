/*
 * Copyright (c) 2000-2021, 2022 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
/* $Id: clgetSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#include <string>
#include <support.h>
#include <iostream>
#include <algorithm>
#include <clstring.h>
//#include <setAutoDefaults.h>
//#include <clgetBaseCode.h>
#ifdef __cplusplus
extern "C" {
#endif
 
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
int clgetSVal(const char *Name, char *val, int *n)
{
  HANDLE_EXCEPTIONS(
		    string valp;
		    int r;
		    if ((r = clgetSValp(std::string(Name), valp, *n))!= CL_FAIL)
		      strncpy(val,valp.c_str(),valp.size()+1);
		    return r;
		    );
}
#ifdef __cplusplus
	   }
#endif

#ifdef __cplusplus

// int clgetSValp(const string &Name, string& val, int& n)
// {
//   HANDLE_EXCEPTIONS(
// 		    SMap emptyMap;
// 		    return clgetSValp(Name, val, n, emptyMap);
// 		    );
// }
// int clgetSValp(const string& Name, string& val, int& n, SMap &smap)
// {
//   Symbol *S;
//   unsigned int N;

// HANDLE_EXCEPTIONS(
// 		  S = clgetBaseCode(Name, val, n, smap);
// 		  N = _ABS(n);
// 		  if (S!=NULL) 
// 		    {
// 		      if (N <= S->NVals) 
// 			{
// 			  val = trim(S->Val[N-1]);
// 			  return val.size();
// 			}
// 		      else 
// 			return CL_FAIL;
// 		    }
// 		  else
// 		    return CL_FAIL;
// 		  );
// }
#endif
