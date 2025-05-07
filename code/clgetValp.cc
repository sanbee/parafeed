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
// There are templated version of clgetValp(), clgetNValp(),
// dbgclgetValp() and dbgclgetNValp(). These use the templated
// clgetBaseCode() and clparseVal().  This file needs to be included
// in any code that uses clgetValp() interface.  It is therefore
// included in clinteract.h (which is required in the client code
// anyway).
//
// Templates for std::string type is deleted and overloaded since
// val = (T)d cannot work the same way for string as it works
// for float,int, bool types.
//
// For directly using clget?Valp() interface, nothing else is required
// other than these templates.  This useage is recommended.
// For backward compatibility, the clget[SIBF][N]Valp() functions are
// implemented which are wrappers around clgetValp() functions.  These
// functions are still in use in other parts of the library -- so these
// are required even internally for now.
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
//
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
//
template <class T>
int clgetValp(const std::string& Name, T& val, int& n, SMap smap=SMap())
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
template <>
int clgetValp<std::string>(const std::string&, std::string& val, int& n,SMap)=delete;
//
//----------------------------------------------------------------------
//
int clgetValp(const std::string& Name, std::string& val, int& n, SMap smap=SMap())
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
//-------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clgetN?Valp() functions are wrappers around this function for
// backward compatibility.
//
//
//----------------------------------------------------------------------
template <class T>
int clgetValp(const std::string& Name, std::vector<T>& val, int& m, const SMap smap=SMap())
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
template <>
int clgetValp<std::string>(const std::string&, std::vector<std::string>& val, int& n,const SMap)=delete;

//
//----------------------------------------------------------------------
//
int clgetValp(const std::string& Name, std::vector<std::string>& val, int& m, const SMap smap=SMap())
{
  Symbol *S;
  std::string d;

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
//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const std::string& Name, T& val, int& n, SMap smap=SMap())
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
//----------------------------------------------------------------------
//
template <>
int dbgclgetValp<std::string>(const std::string& Name, std::string& val, int& n, SMap smap)=delete;

int dbgclgetValp(const std::string& Name, std::string& val, int& n, SMap smap=SMap())
{
  Symbol *S;
  std::string d;
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
//
template <class T>
int dbgclgetValp(const std::string& Name, std::vector<T>& val, int& n, SMap smap=SMap())
{
  Symbol *S;
  double d;
  int N;
  HANDLE_EXCEPTIONS(
		    {
		      S=clgetBaseCode(Name,val,n,smap,true);
		      int i=1;
		      while(i <= n)
			{
			  if ((N=clparseVal(S,&n,&d))==CL_FAIL) return N;
			  else if (N==0) break;
			  else {val.push_back((T)d); i++;}
			}
		      return i-1;
		    }
		    );
}
template <>
int dbgclgetValp<std::string>(const std::string& Name, std::vector<std::string>& val, int& n, SMap smap)=delete;
//
//----------------------------------------------------------------------
//
int dbgclgetValp(const std::string& Name, std::vector<std::string>& val, int& n, SMap smap=SMap())
{
  Symbol *S;
  std::string d;
  int N;
  HANDLE_EXCEPTIONS(
		    {
		      S=clgetBaseCode(Name,val,n,smap,true);
		      int i=1;
		      while(i <= n)
			{
			  if ((N=clparseVal(S,&n,d))==CL_FAIL) return N;
			  else if (N==0) break;
			  else {val.push_back(d); i++;}
			}
		      return i-1;
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
int cldbggetValp(const std::string& Name, T& val, int& n)                        {return dbgclgetValp(Name,val,n);}

int clgetFValp(const std::string& Name, float &val, int &n)                      {return clgetValp(Name,val,n);}
int clgetFValp(const std::string& Name, float& val, int& n, SMap &smap)          {return clgetValp(Name,val,n,smap);}
int clgetNFValp(const std::string& Name, std::vector<float>& val, int& n)             {return clgetValp(Name,val,n);}
int clgetNFValp(const std::string& Name, std::vector<float>& val, int& n, SMap &smap) {return clgetValp(Name,val,n,smap);}
int cldbggetFValp(const std::string& Name, float& val, int& n)                   {return dbgclgetValp(Name,val,n);}

int clgetIValp(const std::string& Name, int &val, int &n)                      {return clgetValp(Name,val,n);}
int clgetIValp(const std::string& Name, int& val, int& n, SMap &smap)          {return clgetValp(Name,val,n,smap);}
int clgetNIValp(const std::string& Name, std::vector<int>& val, int& n)             {return clgetValp(Name,val,n);}
int clgetNIValp(const std::string& Name, std::vector<int>& val, int& n, SMap &smap) {return clgetValp(Name,val,n,smap);}
int cldbggetIValp(const std::string& Name, int& val, int& n)                   {return dbgclgetValp(Name,val,n);}

int clgetBValp(const std::string& Name, bool& val, int &n)                     {return clgetValp(Name,val,n);}
int clgetBValp(const std::string& Name, bool& val, int& n, SMap &smap)         {return clgetValp(Name,val,n,smap);}
int clgetNBValp(const std::string& Name, std::vector<bool>& val, int& n)            {return clgetValp(Name,val,n);}
int clgetNBValp(const std::string& Name, std::vector<bool>& val, int& n, SMap &smap){return clgetValp(Name,val,n,smap);}
int cldbggetBValp(const std::string& Name, bool& val, int& n)                  {return dbgclgetValp(Name,val,n);}

int clgetSValp(const std::string& Name, std::string& val, int &n)                           {return clgetValp(Name,val,n);}
int clgetSValp(const std::string& Name, std::string& val, int& n, SMap &smap)               {return clgetValp(Name,val,n,smap);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n)                  {return clgetValp(Name,val,n);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n, SMap &smap)      {return clgetValp(Name,val,n,smap);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n, const SMap smap) {return clgetValp(Name,val,n,smap);}
int cldbggetSValp(const std::string& Name, std::string& val, int& n)                        {return dbgclgetValp(Name,val,n);}

int cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m)               {return dbgclgetValp(Key, val, m);}
int cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m, SMap& smap)   {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m)             {return dbgclgetValp(Key, val, m);}
int cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m, SMap& smap) {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m)              {return dbgclgetValp(Key, val, m);}
int cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m, SMap& smap)  {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m)            {return dbgclgetValp(Key, val, m);}
int cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m, SMap& smap){return dbgclgetValp(Key, val, m,smap);}
#endif
