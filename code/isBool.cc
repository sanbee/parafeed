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

#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>
#include <stdbool.h>
#include <algorithm>
#include <cctype>
#include <boolError.h>

//#include <clbool.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
  int clBoolCmp(const string& val, const bool& cmpVal)
  {
    int retVal=CL_UNKNOWNBOOL;
#include <clbool.h>
    string lval(val);
    std::transform(lval.begin(), lval.end(), lval.begin(), (int(*)(int)) std::toupper);
    vector<string> list;
    //
    // clBoolTrue and clBoolFalse are vector<string> construed in clbool.h
    //
    int i;
    if (cmpVal) 
      {
	list=clBoolTrue;
	for(i=0; i<(int)list.size(); i++)
	  {
	    //if (lval==list[(int)i]) return true;
	    if (lval==clBoolTrue[i]) {retVal = 1;break;}
	    if (lval==clBoolFalse[i]) {retVal = 0;break;}
	  }
      }
    else 
      {
	list=clBoolFalse;
	for(i=0; i<(int)list.size(); i++)
	  {
	    //if (lval==list[(int)i]) return true;
	    if (lval==clBoolFalse[i]) {retVal = 0;break;}
	    if (lval==clBoolTrue[i]) {retVal = 1;break;}
	  }
      }
      
    return retVal;
  }
/*----------------------------------------------------------------------*/
int clIsTrue(const string& val) 
{
  int retVal=clBoolCmp(val, true);
  if (retVal == CL_UNKNOWNBOOL) 
    {
      boolError bl("Unrecognized value for Bool", "###Error", CL_INFORMATIONAL);
      throw(bl);
    }
  return retVal;
}
int clIsFalse(const string& val)
{
  int retVal=clBoolCmp(val, false);
  if (retVal == CL_UNKNOWNBOOL) 
    {
      boolError bl("Unrecognized value for Bool", "###Error", CL_INFORMATIONAL);
      throw(bl);
    }
  return retVal;
}
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
	   }
#endif
