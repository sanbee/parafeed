/*
 * Copyright (c) 2000-2012, 2013 S.Bhatnagar
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
/* $Id: PrintVals.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>
#include <string>
#include <cl.h>

#ifdef __cplusplus
using namespace std;
extern "C" {
  extern Symbol    *cl_SymbTab,*cl_TabTail;
  extern CmdSymbol *cl_CmdTab;
  extern char      *cl_ProgName;
  extern unsigned short CL_DBG_ON;
#endif
/*----------------------------------------------------------------------*/
int PrintVals(FILE *fd,Symbol *S, unsigned int newline)
{
  unsigned int i;
  unsigned int N=S->NVals;
  if (N==0) 
    {
      N = S->DefaultVal.size();
      char *str;
      if (N>0)
	{
	  for(i=0;i<N-1;i++)
	    {
	      str=(char *)S->DefaultVal[i].c_str();
	      tokenize(fd,str,",",CL_ESC);
	      fputc(',',fd);
	    }
	  str=(char *)S->DefaultVal[N-1].c_str();
	  tokenize(fd,(char *)S->DefaultVal[N-1].c_str(),",",CL_ESC);
	  if (newline) fputc('\n',fd);
	}
      else
	if (newline) fprintf(fd,"\n");
      return 1;
    }

  {
    for (i=0;i<S->NVals-1;i++)
      {tokenize(fd,S->Val[i],",",CL_ESC);fputc(',',fd);}
    tokenize(fd,S->Val[S->NVals-1],",",CL_ESC);
    if (newline) fputc('\n',fd);
    return 1;
  }

  if (newline) fprintf(fd,"\n");
  return 1;
}
  /*----------------------------------------------------------------------*/
  int namePrintFormat(char *format, char *append)
  {
    Symbol *t;
    int maxNameLength=10;
    for (t=cl_SymbTab;t;t=t->Next)
      if ((int)strlen(t->Name) > maxNameLength) maxNameLength = strlen(t->Name);
    
    sprintf(format,"%c%c%d.%ds%s",'\%','-',maxNameLength,maxNameLength,append);
    return maxNameLength;
  }
  /*----------------------------------------------------------------------*/
  void printMap(SMap& smap)
  {
    if ((smap.size() > 0))
      {
	for(SMap::iterator i=smap.begin(); i != smap.end(); i++)
	  {
	    cout << "### " << (*i).first << ": ";
	    vector<string> sv=(*i).second;
	    for(unsigned int j=0;j<sv.size();j++)
	      cout << sv[j] << " ";
	    cout << endl;
	  }
      }
  }
/*----------------------------------------------------------------------*/
int PrintKey(FILE *fd,Symbol *S)
{
  char format[12];
  namePrintFormat(format," = ");
  if (ISSET(S->Attributes,CL_KEYWORD))
    {
      string startSeq,endSeq;
      clTextColouring(S->Name,(unsigned int)S->Attributes, startSeq,endSeq);
      fprintf(fd,"%s",startSeq.c_str());
      fprintf(fd,format,S->Name);
      fprintf(fd,"%s",endSeq.c_str());
    }
  else
    fprintf(fd,format,S->Name);
  return 1;
}
#ifdef __cplusplus
	   }
#endif
