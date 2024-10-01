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
/* $Id: clgetNSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <sstream>

#ifdef __cplusplus
extern "C" {
#include <clstring.h>
#endif
/*------------------------------------------------------------------------
   Return N values of key Name as a strings.
------------------------------------------------------------------------*/
int clgetNSVal(char *Name, char **Val, int *m)
{
  int i,j,r=0,n=0;
  char  *buf;
  Symbol *S;

  std::ostringstream os;
  if (*m <= 0)
    os << "string[]";
  //sprintf(tmp,"bool[]");
  else
    os << "string[" << *m << "]";
  //sprintf(tmp,"bool[%d]",*m);


  S = SearchQSymb(Name, os.str());
  if (S==NULL) return CL_FAIL;
  else 
    {
      S->Class=CL_APPLNCLASS;
      i = S->NVals;
      if (i < *m)r = n = i;
      if (*m <= i){ n = *m; r = n;}
      for (j=1;j<= n;j++)
	{
	  string tt=ltrim(S->Val[j-1]);
	  strncpy(Val[j-1],tt.c_str(),tt.length()+1);
	  // buf = (char *)S->Val[j-1].c_str();
	  // while (*buf == ' ') buf++;
	  // strncpy(Val[j-1],buf,strlen(buf)+1);
	}
      return r;
    }
}

#ifdef __cplusplus
	   }
#endif

// #ifdef __cplusplus
// int clgetNSValp(const string& Name, vector<string>& Val, int& m)
// {
//   int j,r=0,n=0;
//   char *buf;
//   Symbol *S;

//   std::ostringstream os;
//   if (m <= 0)
//     os << "string[]";
//   //sprintf(tmp,"bool[]");
//   else
//     os << "string[" << m << "]";
//   //sprintf(tmp,"bool[%d]",*m);

//   S = SearchQSymb((char *)Name.c_str(), os.str());
//   // if (S!=NULL) SETBIT(S->Attributes,CL_STRINGTYPE);
//   if (S==NULL) return CL_FAIL;
//   else 
//     {
//       SETBIT(S->Attributes,CL_STRINGTYPE);
//       S->Class=CL_APPLNCLASS;

//       n = S->NVals;
//       for (j=0;j< n;j++)
// 	{
// 	  string tt=ltrim(S->Val[j]);
// 	  Val.resize(j+1);
// 	  Val[j]=tt;
	  
// 	  // buf = (char *)S->Val[j].c_str();
// 	  // while (*buf == ' ') buf++;
// 	  // Val.resize(j+1);
// 	  // Val[j]=buf;
// 	  //	  strncpy(Val[j-1],buf,strlen(buf)+1);
// 	}
//       return r;
//     }
// }
// #endif
