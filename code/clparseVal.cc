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
/* $Id: clparseVal.c,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#include <shell.h>
#include <cllib.h>
#include <cl.h>
//#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Convert the n th value of symbol S into a double
------------------------------------------------------------------------*/

void reportParseError(const Symbol& S, const int& N)
{
  ostringstream os;
  os << "In conversion of " << S.Name << "[" << N << "]=" << S.Val[N];
  clThrowUp(os.str().c_str(), "###Error", CL_FAIL);
}
#ifdef __cplusplus
	   }
#endif
int clparseVal(Symbol *S, int *Which, string& val)
{
  unsigned int N = _ABS(*Which);
  
  if (S!=NULL) 
    {
      if (N <= S->NVals) 
	{
	  val = trim(S->Val[N-1]);
	  return val.size();
	}
      else 
	return CL_FAIL;
    }
  else
    return CL_FAIL;

}


int clparseVal(Symbol *S, int *Which, double *d)
{
  unsigned int N = _ABS(*Which),n;
HANDLE_EXCEPTIONS(
  if (S != NULL)
    {
      if (N > S->NVals) return 0;
      if (ISSET(S->Attributes,CL_BOOLTYPE))
	{
	  int retVal;
	  string val(S->Val[N-1]);
	  if ((retVal=clIsTrue(val))==1) *d=1;
	  else if ((retVal=clIsFalse(val))==1) *d=0;
	  //*d = clIsTrue(val);
	  if (retVal == CL_UNKNOWNBOOL ) 
	    reportParseError(*S, N-1);
	}
      else if ((n=calc((char *)S->Val[N-1].c_str(),d)))
	reportParseError(*S, N-1);
      return 1;
    }
  else return CL_FAIL;
)
}
