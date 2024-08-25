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
#include <clgetValp.cc>
//
// //
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clget?Valp() functions are wrappers around this function for
// backward compatibility.
//
int clgetValp(const string& Name, std::string& val, int& n, SMap& smap);
template <class T>
int clgetValp(const string& Name, T& val, int& n, SMap& smap);
//
//-------------------------------------------------------------------------
//
template <class T>
int clgetValp(const string& Name, T& val, int& n);

//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const string& Name, T& val, int& n, SMap& smap);

//
//-------------------------------------------------------------------------
//
template <class T>
int dbgclgetValp(const string& Name, T& val, int& n);

//
//----------------------------------------------------------------------
// The templated API-level function that can be used in the applications.
// The clgetN?Valp() functions are wrappers around this function for
// backward compatibility.
//
template <class T>
int clgetNValp(const string& Name, vector<T>& val, int& m, const SMap &smap);

template <class T>
int clgetNValp(const string& Name, vector<T>& val, int& m);

int clgetNValp(const string& Name, vector<string>& val, int& m, const SMap &smap);

int clgetNValp(const string& Name, vector<string>& val, int& m);

#endif
