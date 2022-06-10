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
//
// Generic baseclass type functions that are called in clget?Valp() functions.
//
// setAutoDefaults is a template function in support.cc.  In
// clgetBaseCode() below, it works for T=float,bool,int but not when
// T=string.  I don't quite understand why, but it seems to do with
// the fact that setAutoSDefaults(symbol*, const string&, const int)
// signature in support.cc is different from
// setAuto{F,I,B}Defaults(symbol *,const {float,int,bool}&) functions
// all of which just call the templated setAutoDefaults(symbol *,
// const T&).
//
// Due to this problem, current clgetSValp() functions do not use
// clgetBaseCode().
//
#ifndef CLGETBASECODE_H
#define CLGETBASECODE_H
#include <cllib.h>
#include <shell.h>
#include <support.h>
#include <setAutoDefaults.h>
#include <type_traits>
// #ifdef __cplusplus
// extern "C" {
// #endif

template <class T>
Symbol* clgetBaseCode(const string& Name, T& val, int& n, SMap &smap=SMap())
{
  Symbol *S;
  string type_str="";
  uint type_int=0;
  if (std::is_same<T, int>::value)   {type_str="int";   type_int=CL_INTEGERTYPE;}
  else if (std::is_same<T, float>::value) {type_str="float"; type_int=CL_FLOATTYPE;}
  else if (std::is_same<T, bool>::value)  {type_str="bool";  type_int=CL_BOOLTYPE;}
  else if (std::is_same<T, std::string>::value)
                                     {type_str="string";type_int=CL_STRINGTYPE;}

  HANDLE_EXCEPTIONS(
		    if (n < 0)
		      S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
		    else
		      S=SearchQSymb((char *)Name.c_str(),type_str);
		    //N = _ABS(n);
		    setAutoDefaults(S,val);

		    if (S!=NULL) 
		      {
			SETBIT(S->Attributes,type_int);
			if (!smap.empty())
			  S->smap = smap;
		      }
		    )
    return S;
};

template <class T>
T clgetGenericValp(const string& Name, T& val, int& n, SMap& smap)
{
  Symbol *S;
  double d;
  int N;
  HANDLE_EXCEPTIONS(
		    S=clgetBaseCode(Name,val,n,smap);
		    
		    // if (n < 0)
		    //   S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);  
		    // else
		    //   S=SearchQSymb((char *)Name.c_str(),(char *)"int");
		    // setAutoIDefaults(S,val);
		    // if (S!=NULL) SETBIT(S->Attributes,CL_INTEGERTYPE);

		    if ((N=clparseVal(S,&n,&d))>0) val = (T)d;
		    return N;
		    );
}


// #ifdef __cplusplus
// 	   }
// #endif
#endif
