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
/* $Id: support.c,v 2.3 1999/01/25 04:03:33 sanjay Exp sanjay $ */
#include <stdio.h>
#include <string.h>
#include <sstream>
using namespace std;
//
//---------------------------------------------------------------
//
// Get the value of the named variable from an env. variable
//
template <class T>
T cl_getenv(const char *name, const T defaultVal)
{
  T val=defaultVal;
  stringstream defaultStr;
  defaultStr << defaultVal;
  {
    char *valStr=NULL;
    std::string tt(name);
    unsigned long pos;
    while((pos=tt.find(".")) != tt.npos)
      tt.replace(pos, 1, "_");

    if ((valStr = std::getenv(tt.c_str())) != NULL)
      {
	stringstream toT2(valStr);
	toT2 >> val;
      }
  }
  return val;
}
template int cl_getenv(const char *name, const int defaultVal);
 // template 
 //   bool cl_getenv(const char *name, const bool defaultVal);
 // template 
 //   float cl_getenv(const char *name, const float defaultVal);
 // template 
 //   double cl_getenv(const char *name, const double defaultVal);
 // template 
 //   std::string cl_getenv(const char *name, const std::string defaultVal);

