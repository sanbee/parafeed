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
/* $Id: SetVar.c,v 2.5 2002/04/11 20:31:09 sbhatnag Exp $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <stdlib.h>
#include <cl.h>
#include <cllib.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <boolError.h>
#ifdef __cplusplus
extern "C" {
#endif
/* #define getmem(a,b)   malloc((a))*/
/*---------------------------------------------------------------------------*/
/* Unset the value(s) of the variable indicated by the given symbol          */
/* If setFactoryDefaults==1, then load the values from S->DefaultVal list    */
/*---------------------------------------------------------------------------*/
int UnsetVar(Symbol *S, int setFactoryDefaults)
{
  unsigned int j;
  for (j=0;j<S->NVals;j++)
    {if (S->Val[j]!=NULL) free(S->Val[j]);
     S->Val[j]=NULL;}
  S->NVals=0;

  if (setFactoryDefaults)
    {
      int n=S->DefaultVal.size();
      if (n>0)
	{
	  S->Val=(char **)calloc(1,sizeof(char **)*n);
	  int m;
	  for(int i=0;i<n;i++)
	    {
	      S->Val[i]=(char *)getmem(strlen(S->DefaultVal[i].c_str())+1,"cl:UnsetVar");
	      strncpy(S->Val[i],S->DefaultVal[i].c_str(),(m=strlen(S->DefaultVal[i].c_str())));
	      S->Val[i][m]='\0';
	    }
	  S->NVals=n;
	}
    }
  return 1;
 }
/*---------------------------------------------------------------------------*/
/* Loads the keyword key and its value in the list pointed to by p           */
/*---------------------------------------------------------------------------*/
  int SetVar(char *key, char *val, Symbol *Tab,short int Force, short int fullmatch, short int dodoinp)
{
  unsigned int i,j;
  int coma=0;
  Symbol *pos;
  char *k=NULL,*v=NULL;

  /*---------------------------------------------------------------
    Search for the key in the table pointed to by Tab If not found,
    return -2 If the value is NULL, unset the found key and return
   ---------------------------------------------------------------*/
  if (fullmatch)
    pos=SearchVSymbFullMatch(key,Tab);
  else
    pos=SearchVSymb(key,Tab);
  //  if ((pos=SearchVSymb(key,Tab))==0) return -2;
  if(pos==0) return -2;

  if (!Force)
    if (pos->Class == CL_USERCLASS)
      {
	string msg="Attempted modification of a shell-constant (named \'" + string(pos->Name) + "\').  Ignored."; 
	clThrowUp(msg.c_str(),"###Error",CL_INFORMATIONAL);
	return -2;
      }

  if ((val==NULL) || (strlen(val) == 0)) 
    {
      return UnsetVar(pos,1);
    }

  cltruncateFromBack(val);
  /*
  i=strlen(val)-1;
  while ((val[i] == ' ') || (val[i] == '\t'))i--;  val[i+1]='\0';
  */
  /* 
     Count the number of commas. Strip off leading and trailing white
     spaces from val, copy into another buffer, take comma seperated
     tokens from the new buffer and put it in Tab.
  */

  if ((coma = ntok(val,",",CL_ESC))==-1) return 1;

  i=0;j=0;
  while(val[j] == ' ')j++;

  k = (char *)getmem(strlen(&val[j])+1,"cl:loadParams");
  strncpy(k,&val[j],(i=strlen(&val[j])));k[i]='\0';

  v = (char *)clstrtok(k,",",CL_ESC);

  //  cerr << "before = " << pos->NVals << " " << coma << endl;

  for (i=coma;i<pos->NVals;i++) free(pos->Val[i]);
  pos->Val=(char **)calloc(1,sizeof(char **)*(coma));
  pos->NVals=coma;

  for (i=0;i<(unsigned int)coma;i++)
    {
      SetVal(v,pos,i);
      if ((v = (char *)clstrtok(NULL,",",CL_ESC))==NULL) break;
    }
  //  cerr << "after = " << pos->NVals << " " << i << endl;

  if (dodoinp) doinp(key);
  free(k);
  return 1;
}
/*----------------------------------------------------------------------*/
  void VerifyVal(char *v, Symbol *S,string& newval)
  {
    int n = S->Options.size();
    int Matched=1;
    ostringstream os;
    newval=v;
    if (ISSET(S->Attributes,CL_BOOLTYPE))
      {
	Matched=0;
	int n,b=0;
	n=sscanf(v,"%d",&b);
	if (n>0)
	  {
	    b=(b==0?0:1);Matched=1;
	    os << b;
	  }
	else
	  {
	    //	    Matched=(clIsTrue(newval) || clIsFalse(newval));
	    Matched=0;
	    try
	      {
		int val=clIsTrue(newval);
		if (val==1) {Matched=1;}
		else 
		  {
		    val = clIsFalse(newval);
		    if (val==1) {Matched=1;}
		  }
	      }
	    catch (clError& x)
	      {
		//		x << x.what() << endl;
	      }

	    os << newval;
	  }
	//	os << b;
	newval=os.str();
      }
    else if (n > 0)
      {
	Matched=0;
	for(int i=0; i<n; i++)
	  if (S->Options[i] == v)
	    {Matched=1;break;}
      }
    if (!Matched)
      {
	string msg;
	msg = "###Warning:       Value did not match any factory supplied options for keyword \"";
	msg += S->Name; msg += "\"";
	clError errObj;
	errObj << msg.c_str() << endl;
	if (n>0)
	  {
	    msg = "###Informational: Valid options are ";
	    for(int i=0; i<n; i++)
	      {msg += "\"" + S->Options[i];msg += "\" ";}
	    errObj << msg.c_str() << endl;
	  }
      }
  }
/*----------------------------------------------------------------------*/
void SetVal(char *v, Symbol *S, int i)
{
  int len;
  string vv;
  stripwhite(v);
  try
    {
      VerifyVal(v,S,vv);
    }
  catch (clError& x)
    {
      cerr << x.what()  << " " << S->Val[i] << endl;
      throw(x);
    }

  if ((unsigned int)i >= S->NVals)
    {
      S->Val = (char **)realloc(S->Val,sizeof(char **)*(i+1));
      len = S->NVals;
      do S->Val[len++]=NULL; while (len < i);
    }

  len=strlen(vv.c_str());

  S->Val[i] = (char *)realloc(S->Val[i],len+1);

  if (strlen(vv.c_str()))
    {
      strncpy(S->Val[i],vv.c_str(),len+1);
      S->Val[i][len] = '\0';
      if ((unsigned int)i>=S->NVals) S->NVals++;
      S->Used=0;
    }
}
#ifdef __cplusplus
}
#endif
