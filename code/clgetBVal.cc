/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
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
#include <cllib.h>
#include <shell.h>
#include <string>
#include <support.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
int clgetBVal(char *Name, bool *val, int *n)
{
  Symbol *S;
  bool tval;
  double d;
  int N;
  tval=(val==0?false:true);

  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);  
  else
    S=SearchQSymb(Name,"bool");
  setAutoBDefaults(S,tval);
  if ((N=clparseVal(S,n,&d))>0) *val = (bool)(d==0?false:true);
  if (S!=NULL) SETBIT(S->Attributes,CL_BOOLTYPE);
  return N;
}
#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus
int clgetBValp(const string& Name, bool& val, int& n)
{
  Symbol *S;
  double d;
  int N;
  bool tval;
  tval=(val==0?false:true);
  
  if (n < 0)
    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);  
  else
    S=SearchQSymb((char *)Name.c_str(),"bool");
  setAutoBDefaults(S,tval);
  if ((N=clparseVal(S,&n,&d))>0) val = (bool)(d==0?false:true);
  if (S!=NULL) SETBIT(S->Attributes,CL_BOOLTYPE);
  return N;
}

int clgetBValp(const string& Name, bool& val, int& n, SMap &smap)
{
  Symbol *S;
  unsigned int N;
  char *buf,*c;
  double d;
  bool tval;
  tval=(val==0?false:true);
  if (n < 0)
    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
  else
    S=SearchQSymb((char *)Name.c_str(),"bool");
  N = _ABS(n);
  setAutoBDefaults(S,val);
  if ((N=clparseVal(S,&n,&d))>0) val = (bool)(d==0?false:true);
  if (S!=NULL) 
    {
      SETBIT(S->Attributes,CL_BOOLTYPE);
      S->smap = smap;
    }
  return N;
}
#endif
