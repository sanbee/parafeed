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
// //
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clget?Valp() functions are wrappers around this function for
// backward compatibility.
//
template <class T>
int clgetValp(const string& Name, T& val, int& n, SMap& smap)
{
  Symbol *S;
  double d;
  int N;
  HANDLE_EXCEPTIONS(
		    {
		      S=clgetBaseCode(Name,val,n,smap);
		      if ((N=clparseVal(S,&n,&d))>0) val = (T)d;
		      return N;
		    }
		    );
}
//
//-------------------------------------------------------------------------
//
template <class T>
int clgetValp(const string& Name, T& val, int& n)
{
  SMap empty;
  HANDLE_EXCEPTIONS(
		    return clgetValp(Name,val,n,empty);
		    );
}


//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const string& Name, T& val, int& n, SMap& smap)
{
  Symbol *S;
  double d;
  int N;
  HANDLE_EXCEPTIONS(
		    {
		      S=clgetBaseCode(Name,val,n,smap,true);
		      if ((N=clparseVal(S,&n,&d))>0) val = (T)d;
		      return N;
		    }
		    );
}
//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const string& Name, T& val, int& n)
{
  SMap empty;
  HANDLE_EXCEPTIONS(
		    return dbgclgetValp(Name,val,n,empty);
		    );
}
//
//-------------------------------------------------------------------------
//



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
// The templated AIP-level function that can be used in the applications.
// The clgetN?Valp() functions are wrappers around this function for
// backward compatibility.
//
template <class T>
int clgetNValp(const string& Name, vector<T>& val, int& m, const SMap &smap)
{
  Symbol *S;
  double d;

  HANDLE_EXCEPTIONS(
		    S=clgetNValBaseCode(Name,val,m,smap);
		    int n0=S->NVals;
		    int i=1;
		    for(int j=0;j<n0;j++)
		      {
			if ((m=clparseVal(S,&i,&d))!=CL_FAIL)
			  {
			    if (m==0) {m=S->NVals=i-1;return i-1;}
			    else 
			      {
				val.resize(i);
				val[i-1] = (T)d;
				i++;
			      }
			  }
		      }
		    m=S->NVals=i-1;
		    return i-1;
		    );
}
template <class T>
int clgetNValp(const string& Name, vector<T>& val, int& m)
{
  SMap empty;
  HANDLE_EXCEPTIONS(
		    return clgetNValp(Name, val, m, empty);
		    );
  // Symbol *S;
  // double d;
  // SMap empty;
  // HANDLE_EXCEPTIONS(
  // 		    S=clgetNValBaseCode(Name,val,m,empty);
  // 		    int n0=S->NVals;
  // 		    int i=1;
  // 		    for(int j=0;j<n0;j++)
  // 		      {
  // 			if ((m=clparseVal(S,&i,&d))!=CL_FAIL)
  // 			  {
  // 			    if (m==0) {m=S->NVals=i-1;return i-1;}
  // 			    else 
  // 			      {
  // 				val.resize(i);
  // 				val[i-1] = (T)d;
  // 				i++;
  // 			      }
  // 			  }
  // 		      }
  // 		    m=S->NVals=i-1;
  // 		    return i-1;
  // 		    );
}

#endif
