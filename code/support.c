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
/* $Id: support.c,v 2.3 1999/01/25 04:03:33 sanjay Exp sanjay $ */
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
#ifdef __cplusplus
extern "C" {
#endif
  /*---------------------------------------------------------------------------*/
  string clMakeDefaultsFilename(int complement)
  {
    string fname;
    char out[FILENAME_MAX+2]="./";
#ifdef GNUREADLINE
    strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
#else
    strcat(out,cl_ProgName);
#endif
    fname = out;
    fname = fname + ".def";
    if (complement) fname = fname + "!";
    return fname;
  }
  /*----------------------------------------------------------------------
    Puts a NULL character after the first alpha-numeric character 
    that it finds in buf, starting from the back.
    ----------------------------------------------------------------------*/
  void cltruncateFromBack(char *buf)
  {
    int n;
    n=strlen(buf)-1;
    
    while(((buf[n] == '\t') || (buf[n] == ' ') || (buf[n] == '\n')) && (n >= 0))
      n--;
    n++;
    if (n>0) buf[n] = '\0';
  }
  /*----------------------------------------------------------------------
    Loads the source in the target buffer.  The mem. for target
    is allocated inside by malloc.  If source==NULL, the source
    is filled with default given by def.
    ----------------------------------------------------------------------*/
  void clloadBuf(char **target, char *source, char *def)
  {
    int n;
    n = strlen(source);
    if (n<=0) 
      {
	if (def != NULL) 
	  {
	    n = strlen(def);
	    if (*target != NULL) free(*target);
	    *target = (char *)getmem(sizeof(char)*(n+1),"loadBuf");
	    strcpy(*target,def);
	  }
	else
	  {
	    if (*target != NULL) free(*target);
	    *target = NULL;
	  }
      }
    else
      {
	if (*target != NULL) free(*target);
	*target = (char *)getmem(sizeof(char)*(n+1),"loadBuf");
	strcpy(*target,source);
      }
    source[0]='\0';
  }
  /*----------------------------------------------------------------------
    Strip any leading white spaces (' ',TAB).  This is useful while 
    reading strings typed by humans.
    ----------------------------------------------------------------------*/
  void stripwhite (char *string)
  {
    register int i = 0;
    
    if (string!=NULL)
      {
	while (string[i] == ' ' || string[i]=='\t') i++;
	if (i) strcpy (string, string + i);
	i = strlen (string) - 1;
	while (i > 0 && (string[i]==' '||string[i]=='\t')) i--;
	string[++i] = '\0';
      }
  }
  /*----------------------------------------------------------------------
    Break a string of the type <Name>=<Value> into Name and Value.
    ----------------------------------------------------------------------*/
  int BreakStr(char *str, char **Name, char **val)
  {
    char *t,*off;
    
    if ((off = strchr(str,'='))) off++;
    
    if ((t=strtok(str,"="))!=NULL)
      {
	stripwhite(t);
	*Name = (char *)getmem(strlen(t)+1,"BreakStr");
	strcpy(*Name,t);
      }
    else *Name = NULL;
    if (off)
      {
	stripwhite(off);
	*val = (char *)getmem(strlen(off)+1,"BreakStr");
	strcpy(*val,off);
      }
    else *val = NULL;
    return 1;
  }
  /*----------------------------------------------------------------------
    Redirect the stdout and stderr to files given by out and err.  This is
    useful when wanting to put an application in the background.
    ----------------------------------------------------------------------*/
  int redirect(char *out, char *err)
  {
    int Same=0,OUT=1, ERR=1;
    int fd,fd1;
    
    if (out == NULL) OUT = 0;
    if (err == NULL) ERR = 0;
    
    if (OUT && ERR) 
      {if (!strcmp(out,err)) Same=1; else Same=0;}
    else
      Same=0;
    
    if (OUT)
      {
	if ((fd=creat(out,0644))<0)
	  {
	    perror(out);
	    clThrowUp("In STDOUT redirection.","###Error",
		      CL_SEVERE);
	  }
	close(1);
	dup(fd);
      }
    
    if (ERR)
      {
	if (!Same)
	  {
	    if ((fd1=creat(err,0644))<0)
	      {
		perror(out);
		/*
		  if (clThrowUp("In STDERR redirection.","###Error",
		  CL_SEVERE)==CL_SEVERE)
		  exit(-1);
		*/
		clThrowUp("In STDERR redirection.","###Error",
			  CL_SEVERE);
	      }
	    close(2);
	    dup(fd1);
	  }
	else
	  {
	    close(2);
	    dup(fd);
	  }
      }
    return 1;
  }
#include <clGlobals.h>
#include <sstream>
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetIValp() call.
    ----------------------------------------------------------------------*/
  void setAutoIDefaults(Symbol *S, const int& val)
  {
    ostringstream os;
    os << val;
    if (cl_RegistrationMode == 1) {S->DefaultVal.resize(1); S->DefaultVal[0] = os.str().c_str();}
    
    if (S->Val == NULL)
      {
	S->Val = (char **) getmem(sizeof(char *),"setAutoIDefaults");
	S->Val[0] = (char *) getmem(sizeof(char)*(strlen(os.str().c_str())+1),"setAutoIDefaults");
	sprintf(S->Val[0],"%d%c",val,NULL);
      }
    S->NVals = 1;
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetFValp() call.
    ----------------------------------------------------------------------*/
  void setAutoFDefaults(Symbol *S, const float& val)
  {
    ostringstream os;
    os << val;
    if (cl_RegistrationMode == 1) {S->DefaultVal.resize(1); S->DefaultVal[0] = os.str().c_str();}
    
    if (S->Val == NULL)
      {
	S->Val = (char **) getmem(sizeof(char *),"setAutoFDefaults");
	S->Val[0] = (char *) getmem(sizeof(char)*(strlen(os.str().c_str())+1),"setAutoFDefaults");
	// It appears that C++ ostringstream default formatting is better than that of "%f" 
	// in sprintf (print 0.1 as "0.1" vs. "0.1000000").
	//	sprintf(S->Val[0],"%f%c",val,NULL);
	strcpy(S->Val[0],os.str().c_str());
      }
    S->NVals = 1;
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetSValp() call.
    ----------------------------------------------------------------------*/
  void setAutoSDefaults(Symbol *S, const string& val, const int fullVal)
  {
    if (fullVal)
      {
	int comma;
	char *valstr; valstr=(char *)val.c_str();
	if ((comma = ntok(valstr,",",CL_ESC))==-1) 
	  {
	    ostringstream os;
	    os << "Error in counting commas in the string " << val << endl;
	    throw(clError(os.str().c_str(), "setAutoSDefaults",0));
	  }
	
	if (cl_RegistrationMode==1)
	  {
	    S->DefaultVal.resize(comma);
	    valstr=(char *)val.c_str();
	    char *t;
	    if ((t=clstrtok(valstr,",", CL_ESC)) != NULL)
	      {
		S->DefaultVal[0]=t;
		for(int i=1;i<comma;i++) S->DefaultVal[i]=clstrtok(NULL,",", CL_ESC);
	      }
	  }
      }
    else
      {
	ostringstream os;
	os << val;
	if (cl_RegistrationMode == 1) {S->DefaultVal.resize(1); S->DefaultVal[0] = os.str().c_str();}
	
	if (S->Val == NULL)
	  {
	    S->Val = (char **) getmem(sizeof(char *),"setAutoSDefaults");
	    S->Val[0] = (char *) getmem(sizeof(char)*(strlen(os.str().c_str())+1),"setAutoSDefaults");
	    sprintf(S->Val[0],"%s",os.str().c_str());
	  }
	S->NVals = 1;
      }
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetNIValp() call.
    ----------------------------------------------------------------------*/
  void setAutoNIDefaults(Symbol *S, const vector<int>& val)
  {
    int n=val.size();
    if (cl_RegistrationMode == 1)
      {
	S->DefaultVal.resize(n);
	for(int i=0;i<n;i++)
	  {
	    ostringstream os;
	    os << val[i];
	    S->DefaultVal[i] = os.str().c_str();
	  }
      }
    
    if (S->Val == NULL)
      {
	S->Val = (char **) getmem(sizeof(char *)*n,"setAutoNIDefaults");
	
	for(int i=0;i<n;i++)
	  {
	    ostringstream os;
	    os << val[i];
	    S->Val[i] = (char *) getmem(sizeof(char)*(strlen(os.str().c_str())+1),"setAutoNIDefaults");
	    sprintf(S->Val[i],"%d%c",val[i],NULL);
	  }
      }
    
    S->NVals = n;
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetNFValp() call.
    ----------------------------------------------------------------------*/
  void setAutoNFDefaults(Symbol *S, const vector<float>& val)
  {
    int n=val.size();
    if (cl_RegistrationMode == 1)
      {
	S->DefaultVal.resize(n);
	for(int i=0;i<n;i++)
	  {
	    ostringstream os;
	    os << val[i];
	    S->DefaultVal[i] = os.str().c_str();
	  }
      }
    
    if (S->Val == NULL)
      {
	S->Val = (char **) getmem(sizeof(char *)*n,"setAutoNFDefaults");
	
	for(int i=0;i<n;i++)
	  {
	    ostringstream os;
	    os << val[i];
	    S->Val[i] = (char *) getmem(sizeof(char)*(strlen(os.str().c_str())+1),"setAutoNFDefaults");
	    //	    sprintf(S->Val[i],"%f%c",val[i],NULL);
	    strcpy(S->Val[i],os.str().c_str());
	  }
      }
    
    S->NVals = n;
  }
#ifdef __cplusplus
}
#endif
