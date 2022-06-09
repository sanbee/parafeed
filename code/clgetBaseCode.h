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
#include <cllib.h>
#include <shell.h>
#include <support.h>
#include <type_traits>
// #ifdef __cplusplus
// extern "C" {
// #endif
  template <class T>
  Symbol* clgetBaseCode(const string& Name, T& val, int& n, SMap &smap)
  {
    Symbol *S;
    unsigned int N;
    double d;
    string type="";
    if (std::is_same<T, int>::value) {cerr << "It's an Int" << endl;type="int";}

    HANDLE_EXCEPTIONS(
		      if (n < 0)
			S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
		      else
			S=SearchQSymb((char *)Name.c_str(),type);
		      N = _ABS(n);
		      setAutoIDefaults(S,val);
		      if (S!=NULL) 
			{
			  SETBIT(S->Attributes,CL_INTEGERTYPE);
			  S->smap = smap;
			}
		      )
      return S;
      };

// #ifdef __cplusplus
// 	   }
// #endif
