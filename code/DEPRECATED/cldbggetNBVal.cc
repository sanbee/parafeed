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
/* $Id$ */
#include <cllib.h>
#include <sstream>
#include <clparseVal.h>

#ifdef __cplusplus
/* extern "C" { */
/* #endif */
/*---------------------------------------------------------------------------*/
/* Extract a m number of values associated with Key.  If *m is negative,     */
/* fatal error occurs.  If the return value is positive, it is the number of */
/* associated values that are returned in *Vals.  Vals should have enough    */
/* space to hold the returned values.                                        */
/*---------------------------------------------------------------------------*/
int dbgclgetNBVal(char *Key, bool *val, int *m)
{
  int i=1,n;
  double d;
  Symbol *S;

HANDLE_EXCEPTIONS(
		  std::ostringstream os;
		  os << "bool[" << *m << "]";
		  S = SearchQSymb(Key, os.str());
		  
		  /* sprintf(tmp,"bool[%d]",*m); */
		  /* S = SearchQSymb(Key, tmp); */
		  i=1;
		  if (S) S->Class = CL_DBGCLASS;
		  while(i <= *m)
		    if ((n=clparseVal(S,&i,&d))==CL_FAIL) return n;
		    else if (n==0) break;
		    else {val[i-1] = (bool)(d==0?false:true);i++;}
		  
		  return i-1;
		  )
}
/* #ifdef __cplusplus */
/* 	   } */
#endif
