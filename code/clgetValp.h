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
// The code is separated into clgetBaseCode() and clgetGenericValp()
// functions since the code to return the value from the Symbol is
// differen, at least for T=string.  clgetGenericValp() function has
// this code for T=int,float,bool and is therefore usable for
// realizing clget{I,F,B}Valp() functions. clgetSValp() calls
// clgetBaseCode() directly.
//
#ifndef CLGETVALP_H
#define CLGETVALP_H
#include <cllib.h>
#include <shell.h>
#include <support.h>
//#include <setAutoDefaults.h>
#include <type_traits>
#include <clgetBaseCode.h>

//
// //
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clget?Valp() functions are wrappers around this function for
// backward compatibility.
//
int clgetValp(const string& Name, std::string& val, int& n, SMap& smap)
{
  Symbol *S;
  int N;
  HANDLE_EXCEPTIONS(
		    S=clgetBaseCode(Name,val,n,smap);
		    std::string d;
		    if ((N=clparseVal(S,&n,d))>0) val = d;

		    return N;
		    );
}
template <class T>
int clgetValp(const string& Name, T& val, int& n, SMap& smap)
{
  Symbol *S;
  int N;
  HANDLE_EXCEPTIONS(
		    S=clgetBaseCode(Name,val,n,smap);
		    double d;
		    if ((N=clparseVal(S,&n,&d))>0) val = (T)d;

		    return N;
		    );
}
//
//-------------------------------------------------------------------------
//
template <class T>
int clgetValp(const string& Name, T& val, int& n)
{
  HANDLE_EXCEPTIONS(
		    SMap emptyMap;
		    return clgetValp(Name,val,n,emptyMap);
		    )
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
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
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
int clgetNValp(const string& Name, vector<string>& val, int& m, const SMap &smap)
{
  Symbol *S;
  string d;

  HANDLE_EXCEPTIONS(
		    S=clgetNValBaseCode(Name,val,m,smap);
		    int n0=S->NVals;
		    int i=1;
		    for(int j=0;j<n0;j++)
		      {
			if ((m=clparseVal(S,&i,d))!=CL_FAIL)
			  {
			    if (m==0) {m=S->NVals=i-1;return i-1;}
			    else 
			      {
				val.resize(i);
				val[i-1] = d;
				i++;
			      }
			  }
		      }
		    m=S->NVals=i-1;
		    return i-1;
		    );
}

int clgetNValp(const string& Name, vector<string>& val, int& m)
{
  SMap empty;
  HANDLE_EXCEPTIONS(
		    return clgetNValp(Name, val, m, empty);
		    );
}
//
//----------------------------------------------------------------------------------------------------------
// Calls with type in the name for backward compatibility.
//
// Similar functions may also be possible for pure-C interface.
//
int clgetFValp(const string& Name, float &val, int &n)                      {return clgetValp(Name,val,n);}
int clgetFValp(const string& Name, float& val, int& n, SMap &smap)          {return clgetValp(Name,val,n,smap);}
int clgetNFValp(const string& Name, vector<float>& val, int& n)             {return clgetNValp(Name,val,n);}
int clgetNFValp(const string& Name, vector<float>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
int cldbggetFValp(const string& Name, float& val, int& n)                   {return dbgclgetValp(Name,val,n);}
//int cldbggetNFValp(const string& Name, vector<float>& val, int& n) {return dbgclgetNValp(Name,val,n);}

int clgetIValp(const string& Name, int &val, int &n)                      {return clgetValp(Name,val,n);}
int clgetIValp(const string& Name, int& val, int& n, SMap &smap)          {return clgetValp(Name,val,n,smap);}
int clgetNIValp(const string& Name, vector<int>& val, int& n)             {return clgetNValp(Name,val,n);}
int clgetNIValp(const string& Name, vector<int>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
int cldbggetIValp(const string& Name, int& val, int& n)                   {return dbgclgetValp(Name,val,n);}

int clgetBValp(const string& Name, bool& val, int &n)                     {return clgetValp(Name,val,n);}
int clgetBValp(const string& Name, bool& val, int& n, SMap &smap)         {return clgetValp(Name,val,n,smap);}
int clgetNBValp(const string& Name, vector<bool>& val, int& n)            {return clgetNValp(Name,val,n);}
int clgetNBValp(const string& Name, vector<bool>& val, int& n, SMap &smap){return clgetNValp(Name,val,n,smap);}
int cldbggetBValp(const string& Name, bool& val, int& n)                  {return dbgclgetValp(Name,val,n);}

int clgetSValp(const string& Name, string& val, int &n) {return clgetValp(Name,val,n);}
int clgetNSValp(const string& Name, vector<string>& val, int& n) {return clgetNValp(Name,val,n);}
int clgetNSValp(const string& Name, vector<string>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
//int cldbggetSValp(const string& Name, string& val, int& n) {return dbgclgetValp(Name,val,n);}
//int clgetNSValp(const string& Name, vector<string>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
#endif
