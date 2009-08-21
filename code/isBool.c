/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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

//#include <clbool.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
  bool clBoolCmp(const string& val, const bool& cmpVal)
  {
#include <clbool.h>
    string lval(val);
    std::transform(lval.begin(), lval.end(), lval.begin(), (int(*)(int)) std::toupper);
    vector<string> list;
    if (cmpVal) list=clBoolTrue;
    else list=clBoolFalse;
    for(unsigned int i=0; i<list.size(); i++)
      if (lval==list[(int)i]) return true;
    return false;
  }
/*----------------------------------------------------------------------*/
bool clIsTrue(const string& val) {return clBoolCmp(val, true);}
bool clIsFalse(const string& val){return clBoolCmp(val, false);}
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
	   }
#endif
