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
//
//----------------------------------------------------------------------------------------------------------
// Calls with type in the name for backward compatibility.
//
// Similar functions may also be possible for pure-C interface.
//
#ifndef CLGETVALP_CPP
#define CLGETVALP_CPP
#include <clgetValp.h>

//
// //
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
//
template <class T>
int clgetValp(const string& Name, T& val, int& n, SMap smap=SMap())
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
// template <class T>
// int clgetValp(const string& Name, T& val, int& n)
// {
//   HANDLE_EXCEPTIONS(
// 		    SMap emptyMap;
// 		    return clgetValp(Name,val,n,emptyMap);
// 		    )
// }
// template <>
// int clgetValp<std::string>(const string&, string& val, int& n)=delete;
template <>
int clgetValp<std::string>(const string&, string& val, int& n,SMap)=delete;

//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const string& Name, T& val, int& n, SMap smap=SMap())
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
// template <class T>
// int dbgclgetValp(const string& Name, T& val, int& n)
// {
//   SMap empty;
//   HANDLE_EXCEPTIONS(
// 		    return dbgclgetValp(Name,val,n,empty);
// 		    );
// }

//
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clgetN?Valp() functions are wrappers around this function for
// backward compatibility.
//
//
//----------------------------------------------------------------------
template <class T>
int clgetNValp(const string& Name, vector<T>& val, int& m, const SMap smap=SMap())
{
  Symbol *S;
  double d;
  // auto tmp=val[0];
  // decltype(tmp) d;

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
// template <class T>
// int clgetNValp(const string& Name, vector<T>& val, int& m)
// {
//   SMap empty;
//   HANDLE_EXCEPTIONS(
// 		    return clgetNValp(Name, val, m, empty);
// 		    );
// }
// template <>
// int clgetNValp<std::string>(const string&, vector<string>& val, int& n)=delete;
// template <>
// int clgetNValp<std::string>(const string&, vector<string>& val, int& n,const SMap&)=delete;

//
//----------------------------------------------------------------------
//
int clgetValp(const string& Name, std::string& val, int& n, SMap smap=SMap())
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
//
//----------------------------------------------------------------------
//
int clgetNValp(const string& Name, std::vector<std::string>& val, int& m, const SMap smap=SMap())
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
int dbgclgetValp(const string& Name, string& val, int& n, SMap smap=SMap())
{
  Symbol *S;
  string d;
  int N;
  HANDLE_EXCEPTIONS(
		    {
		      S=clgetBaseCode(Name,val,n,smap,true);
		      if ((N=clparseVal(S,&n,d))>0) val = d;
		      return N;
		    }
		    );
}

//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//
// The clget?Valp() functions are wrappers around clget[N]Valp()
// templated functions for backward compatibility.
//
// These calls are used in other parts of parafeed code.  This file
// therefore needs to be compiled into the libparafeed library for
// internal linking.
// 

template<class T>
int cldbggetValp(const string& Name, T& val, int& n)                        {return dbgclgetValp(Name,val,n);}


int clgetFValp(const string& Name, float &val, int &n)                      {return clgetValp(Name,val,n);}
int clgetFValp(const string& Name, float& val, int& n, SMap &smap)          {return clgetValp(Name,val,n,smap);}
int clgetNFValp(const string& Name, vector<float>& val, int& n)             {return clgetNValp(Name,val,n);}
int clgetNFValp(const string& Name, vector<float>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
int cldbggetFValp(const string& Name, float& val, int& n)                   {return dbgclgetValp(Name,val,n);}

// dbgNValp() templates aren't yet defined
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
int clgetSValp(const string& Name, string& val, int& n, SMap &smap)         {return clgetValp(Name,val,n,smap);}
int clgetNSValp(const string& Name, vector<string>& val, int& n) {return clgetNValp(Name,val,n);}
int clgetNSValp(const string& Name, vector<string>& val, int& n, SMap &smap) {return clgetNValp(Name,val,n,smap);}
int clgetNSValp(const string& Name, vector<string>& val, int& n, const SMap smap) {return clgetNValp(Name,val,n,smap);}
int cldbggetSValp(const string& Name, string& val, int& n) {return dbgclgetValp(Name,val,n);}
#endif
