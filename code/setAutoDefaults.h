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
/* $Id: support.c,v 2.3 1999/01/25 04:03:33 sanjay Exp sanjay $ */
#ifndef SETAUTODEFAULTS_H
#define SETAUTODEFAULTS_H
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cl.h>
#include <clGlobals.h>
#include <sstream>
#include <clstring.h>
/*---------------------------------------------------------------------------*/
/* Template function to set a single value as default for the Symbol S */
/*---------------------------------------------------------------------------*/
template <class T>
void setAutoDefaults(Symbol *S, T const & val)
{
  std::vector<T> tmp;
  tmp.push_back(val);
  setAutoDefaults(S, tmp);
  return;
}
/*---------------------------------------------------------------------------*/
/* Template function to set a vector of values as default for the Symbol S */
/*---------------------------------------------------------------------------*/
template <class T>
void setAutoDefaults(Symbol *S, const vector<T>& val,const bool def2val=false)
{
  int n=val.size();
  if (cl_RegistrationMode == 1)
    {
      S->DefaultVal.resize(n,"");
      for(int i=0;i<n;i++)
	{
	  ostringstream os;
	  //os << (val[i]==0?false:true);
	  os << val[i];
	  S->DefaultVal[i] = os.str();
	}
    }
    
  if (def2val)
    {
      if (S->Val.size() == 0)
	{
	  S->Val.resize(n,"");
	  n = S->DefaultVal.size();
	  for(int i=0;i<n;i++)
	    {
	      stringstream os;
	      //os << S->DefaultVal[i];
	      os << val[i];
	      S->Val[i]=os.str();
	    }
	  S->NVals = n;
	}
    }
}

template <class T>
void setValsFromDefaults(Symbol *S)
{
  if (S->Val.size() == 0)
    {
      int n = S->DefaultVal.size();
      S->Val.resize(n,"");
      for(int i=0;i<n;i++)
	{
	  stringstream os;
	  os << S->DefaultVal[i];
	  //os << val[i];
	  S->Val[i]=os.str();
	}
      S->NVals = n;
    }
}
#endif
