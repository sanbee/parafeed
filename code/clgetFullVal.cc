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
/* $Id: clgetFullVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <support.h>
#include <setAutoDefaults.h>
#include <clparseVal.h>
#ifdef __cplusplus
extern "C" {
#endif
  /*------------------------------------------------------------------------
    Get the value associated with Key as one string.
    ------------------------------------------------------------------------*/
  int clgetFullVal(char *Name, char **val)
  {
    int n,i,len=0;
    char tmp[FILENAME_MAX];

    if (!*val) free(*val); *val=NULL;
    if ((n=clgetNVals(Name))>0)
      {
	for (i=1;i<=n;i++)
	  {
	    clgetSVal(Name,tmp,&i);
	    len += strlen(tmp)+1;
	  }

	*val = (char *)getmem(len,"clgetFullVal");
	for (i=0;i<len;i++) (*val)[i]=' ';
	i=1; clgetSVal(Name,tmp,&i);
	strcpy(*val,tmp);

	for (i=2;i<=n;i++)
	  {
	    strcat(*val,",");
	    clgetSVal(Name,tmp,&i);
	    strcat(*val,tmp);
	  }
	*(*val+strlen(*val))='\0';
      }
    return n;
  }
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/*----------------------------------------------------------------------*/
std::string vecStr2Str(const std::vector<std::string>& src)
{
  std::string val="";
  if (src.size() > 0) val=src[0];
  for(int i=1;i< src.size();i++) val = val + ',' + src[i];
  return val;
};
//
// Base function called in API-level functions below.
//
Symbol* clgetFullValpBase(const string& Name, string& val, bool dbg)
{
  Symbol *S=NULL;

  HANDLE_EXCEPTIONS(
		    S=SearchQSymb((char*)Name.c_str(),"Mixed[]");
		    if (dbg && (S == NULL))
		      S = SearchVSymb((char *)Name.c_str(),cl_SymbTab);
		    if (S != NULL)
		      {
			SETBIT(S->Attributes,CL_MIXEDTYPE);
			S->Class=CL_APPLNCLASS;
			if (dbg) S->Class=CL_DBGCLASS;

			VString vstr={val};
			setAutoDefaults(S,vstr);

			val = vecStr2Str(S->Val);
		      }
		    );

    return S;
}
//
//----------------------------------------------------------------------
// The API-level function that can be used in the applications.
// Return 0 if symbol not found, 1 otherwise.
int clgetFullValp(const string& Name, string& val)
{
  HANDLE_EXCEPTIONS(return (clgetFullValpBase(Name,val,false)!=NULL););
}
//
//-------------------------------------------------------------------------
//
int dbgclgetFullValp(const string& Name, string& val)
{
  HANDLE_EXCEPTIONS(return (clgetFullValpBase(Name,val,true)!=NULL););
}
#endif
