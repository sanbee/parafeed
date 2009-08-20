/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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
/* $Id$ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cl.h>
//------------------------------------------------------------------
//    Break a string of the type <Name>=<Value> into Name and Value.
//------------------------------------------------------------------
int clBreakStr(const string& str, string& Name, string& val)
{
  if (str.length() == 0) return 0;
  char *t,*off;
  char *cstr=(char *)str.c_str();

  Name="";
  val="";
  int n;
  n = str.find_first_of('=',0);
  string valstr=str.substr(n);
  off=(char *)valstr.c_str();
  off++;
  if ((t=strtok(cstr,"="))!=NULL)
    {
      stripwhite(t);
      Name=t;
    }

  
  if (off)
    {
      stripwhite(off);
      val=off;
    }

  return 1;
}
