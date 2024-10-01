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
//
// Generic baseclass type functions that are called in clget?Valp()
// functions.
//
// The code is separated into clgetBaseCode() and the generic
// clgetValp() functions since the code to return the value from the
// Symbol is different, at least for T=string.  The generic functions
// has this code for T=int,float,bool and is therefore usable for
// realizing clget{I,F,B}Valp() functions. clgetSValp() calls
// clgetBaseCode() directly.
//
#ifndef CLGETBASECODE_H
#define CLGETBASECODE_H
#include <cllib.h>
#include <shell.h>
#include <support.h>
#include <setAutoDefaults.h>
#include <clparseVal.h>
#include <type_traits>
#include <cl.h>
using namespace parafeedio;
template <class T>
Symbol* clgetBaseCode(const string& Name, T& val, int& n, SMap &smap=SMap(), bool dbg=false)
{
  Symbol *S=NULL;
  string type_str="Mixed";
  uint type_int=CL_MIXEDTYPE;
  //
  // Set type string and type integer value based on the type of T
  //
  if      (std::is_same<T, int>::value)   {type_str="int";   type_int=CL_INTEGERTYPE;}
  else if (std::is_same<T, float>::value) {type_str="float"; type_int=CL_FLOATTYPE;}
  else if (std::is_same<T, bool>::value)  {type_str="bool";  type_int=CL_BOOLTYPE;}
  else if (std::is_same<T, std::string>::value)
                                          {type_str="string";type_int=CL_STRINGTYPE;}

  HANDLE_EXCEPTIONS(
		    if (n < 0)
		      S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
		    else
		      S=SearchQSymb((char *)Name.c_str(),type_str);

		    if (S!=NULL) 
		      {
			// Use templated function that works for all values of T
			setAutoDefaults(S,val);

			S->Class=CL_APPLNCLASS;
			if (dbg) S->Class=CL_DBGCLASS;
			SETBIT(S->Attributes,type_int);
			if (!smap.empty())
			  S->smap = smap;
		      }
		    )
    return S;
};

//
// Templated functions for NVal calls.  
//
template <class T>
Symbol *clgetNValBaseCode(const string& Name, vector<T>& val, int& m, const SMap &smap=SMap())
{
  Symbol *S;
  std::ostringstream os;

  if      (std::is_same<T, int>::value)   (m <= 0) ? os << "int[]"   : os << "int[" << m << "]";
  else if (std::is_same<T, float>::value) (m <= 0) ? os << "float[]" : os << "float[" << m << "]";
  else if (std::is_same<T, bool>::value)  (m <= 0) ? os << "bool[]"  : os << "bool[" << m << "]";
  else if (std::is_same<T, std::string>::value) (m <= 0) ? os << "string[]" : os << "string[" << m << "]";
    
  HANDLE_EXCEPTIONS(
		    S = SearchQSymb((char *)Name.c_str(), os.str());
		    //
		    // Remember the number of values set by the user.
		    //
		    setAutoDefaults(S,val);

		    if (S!=NULL) 
		      {
			S->Class=CL_APPLNCLASS;
			if (!smap.empty()) S->smap = smap;
		      }
		    return S;
		    );
}
//
//----------------------------------------------------------------------
// This has the API-level templated functions that use the BaseCode
// functions above.
//#include <clgetValp.h>

#endif
