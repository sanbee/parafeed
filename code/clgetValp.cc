/*
 * Copyright (c) 2000-2025, 2026 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
// clgetBaseCode() and clparseVal().
//
// Templates for std::string type is deleted and overloaded since
// val = (T)d cannot work the same way for string as it works
// for float,int, bool types.
//
// For directly using clget?Valp() interface, nothing else is required
// other than these templates.  This usage is recommended.  For
// backward compatibility, the clget[SIBF][N]Valp() functions are
// implemented which are wrappers around clgetValp() functions.  These
// functions are still in use in other parts of the library -- so
// these are required even internally for now.  Similar functions may
// also be possible for pure-C interface.
//
#ifndef CLGETVALP_CPP
#define CLGETVALP_CPP
#include <clgetValp.h>
//
//
//----------------------------------------------------------------------
//
template <class T>
int clgetValp0(const std::string& Name, T& val, int& n, SMap smap=SMap(),bool dbg=false)
{
  Symbol *S;
  int N;
  HANDLE_EXCEPTIONS(
		    S=clgetBaseCode(Name,val,n,smap,dbg);
		    T d;
		    //if ((N=clparseVal(S,&n,d))>0) val = (T)d;
		    if ((N=clparseVal(S,&n,d))>0) val = d;

		    return N;
		    );
}
//
//----------------------------------------------------------------------
//
template <class T>
int clgetValpVec(const std::string& Name, std::vector<T>& val, int& m, const SMap smap=SMap(),bool dbg=false)
{
  Symbol *S;
  HANDLE_EXCEPTIONS(
		    S=clgetNValBaseCode(Name,val,m,smap,dbg);
		    int n0=S->NVals;
		    int i=1;
		    for(int j=0;j<n0;j++)
		      {
			T d;
			if ((m=clparseVal(S,&i,d))!=CL_FAIL)
			  {
			    if (m==0) break;
			    else
			      {
				val.resize(i);
				val[i-1] = d;
				i++;
			      }
			  }
		      }
		    return m=S->NVals=i-1;
		    );

		    // S=clgetNValBaseCode(Name,val,m,smap,dbg);
		    // int n0=S->NVals;
		    // int i=1;
		    // val.resize(0);
		    // for(int j=0;j<n0;j++)
		    //   {
		    // 	T d;
		    // 	if ((m=clparseVal(S,&i,d))!=CL_FAIL)
		    // 	  {
		    // 	    if (m==0) break;
                    //        // What dosen't the following code
                    //        // work when S is of STRING type?
		    // 	    else {val.push_back(d);i++;}
		    // 	  }
		    //   }
		    // //m=S->NVals=i-1;
		    // return m=val.size();
		    // );

}
//
//-------------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clgetN?Valp() functions are wrappers around this function for
// backward compatibility.
//
//----------------------------------------------------------------------
//
template <class T>
int clgetValp(const std::string& Name, T& val, int& n, SMap smap=SMap())
{return clgetValp0(Name,val,n,smap,false);}

template <class T>
int dbgclgetValp(const std::string& Name, T& val, int& n, SMap smap=SMap())
{return clgetValp0(Name,val,n,smap,true);}
//
//----------------------------------------------------------------------
//
template <class T>
int clgetValp(const std::string& Name, std::vector<T>& val, int& n, SMap smap=SMap())
{return clgetValpVec(Name,val,n,smap,false);}

template <class T>
int dbgclgetValp(const std::string& Name, std::vector<T>& val, int& n, SMap smap=SMap())
{return clgetValpVec(Name,val,n,smap,true);}

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
int cldbggetValp(const std::string& Name, T& val, int& n)                             {return dbgclgetValp(Name,val,n);}

int clgetFValp(const std::string& Name, float &val, int &n)                           {return clgetValp(Name,val,n);}
int clgetFValp(const std::string& Name, float& val, int& n, SMap &smap)               {return clgetValp(Name,val,n,smap);}
int clgetNFValp(const std::string& Name, std::vector<float>& val, int& n)             {return clgetValp(Name,val,n);}
int clgetNFValp(const std::string& Name, std::vector<float>& val, int& n, SMap &smap) {return clgetValp(Name,val,n,smap);}

int clgetIValp(const std::string& Name, int &val, int &n)                           {return clgetValp(Name,val,n);}
int clgetIValp(const std::string& Name, int& val, int& n, SMap &smap)               {return clgetValp(Name,val,n,smap);}
int clgetNIValp(const std::string& Name, std::vector<int>& val, int& n)             {return clgetValp(Name,val,n);}
int clgetNIValp(const std::string& Name, std::vector<int>& val, int& n, SMap &smap) {return clgetValp(Name,val,n,smap);}

int clgetBValp(const std::string& Name, bool& val, int &n)                          {return clgetValp(Name,val,n);}
int clgetBValp(const std::string& Name, bool& val, int& n, SMap &smap)              {return clgetValp(Name,val,n,smap);}
int clgetNBValp(const std::string& Name, std::vector<bool>& val, int& n)            {return clgetValp(Name,val,n);}
int clgetNBValp(const std::string& Name, std::vector<bool>& val, int& n, SMap &smap){return clgetValp(Name,val,n,smap);}

int clgetSValp(const std::string& Name, std::string& val, int &n)                                {return clgetValp(Name,val,n);}
int clgetSValp(const std::string& Name, std::string& val, int& n, SMap &smap)                    {return clgetValp(Name,val,n,smap);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n)                  {return clgetValp(Name,val,n);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n, SMap &smap)      {return clgetValp(Name,val,n,smap);}
int clgetNSValp(const std::string& Name, std::vector<std::string>& val, int& n, const SMap smap) {return clgetValp(Name,val,n,smap);}

int cldbggetIValp(const std::string& Name, int& val, int& n)                        {return dbgclgetValp(Name,val,n);}
int cldbggetFValp(const std::string& Name, float& val, int& n)                      {return dbgclgetValp(Name,val,n);}
int cldbggetBValp(const std::string& Name, bool& val, int& n)                       {return dbgclgetValp(Name,val,n);}
int cldbggetSValp(const std::string& Name, std::string& val, int& n)                {return dbgclgetValp(Name,val,n);}

int cldbggetIValp(const std::string& Name, int& val, int& n, SMap& smap)            {return dbgclgetValp(Name,val,n,smap);}
int cldbggetFValp(const std::string& Name, float& val, int& n, SMap& smap)          {return dbgclgetValp(Name,val,n,smap);}
int cldbggetBValp(const std::string& Name, bool& val, int& n, SMap& smap)           {return dbgclgetValp(Name,val,n, smap);}
int cldbggetSValp(const std::string& Name, std::string& val, int& n, SMap& smap)    {return dbgclgetValp(Name,val,n,smap);}

int cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m)           {return dbgclgetValp(Key, val, m);}
int cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m)         {return dbgclgetValp(Key, val, m);}
int cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m)          {return dbgclgetValp(Key, val, m);}
int cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m)   {return dbgclgetValp(Key, val, m);}

int cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m, SMap& smap)   {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m, SMap& smap) {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m, SMap& smap)  {return dbgclgetValp(Key, val, m,smap);}
int cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m, SMap& smap){return dbgclgetValp(Key, val, m,smap);}
#endif
