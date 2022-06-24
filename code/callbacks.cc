/* -*- C++ -*-
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
/* $Id: callbacks.c,v 2.5 1999/07/12 09:49:34 sanjay Exp sanjay $ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <cl.h>
#include <shell.h>
#include <shell.tab.h>
#include <string>
#include <errno.h>
#include <limits.h>
#include <boolError.h>
#include <sstream>
//#include <strstream>
#ifdef GNUREADLINE
#include <readline/history.h>
#endif

#ifdef __cplusplus
using namespace std;
extern "C" {
#endif
#include <rl_interface.h>

  extern Symbol    *cl_SymbTab,*cl_TabTail;
  extern CmdSymbol *cl_CmdTab;
  extern char      *cl_ProgName;
  extern unsigned short CL_DBG_ON;
  short int cl_do_doinp=1;
#define MAXBUF     256
  /*------------------------------------------------------------------------
    A macro which is actually an nawk program!  If one arg is given to
    this nawk program, it is treated to be the name of .doc file to
    print on the stdout.  An optional 2nd. arg. can be provided which
    should be the name of a keyword of the task.  This program will then
    print help only for that keyword.
    ------------------------------------------------------------------------*/
#define KEYHELP_AWK "__REPLACE_ME_WITH_AWK_PROG_NAME__ 'BEGIN{\
if (ARGC < 3) Key=\"\";					      \
  else							      \
    {							      \
      Key=ARGV[2];					      \
      FILENAME=ARGV[1];					      \
      ARGC=2;						      \
    }							      \
  Stat=1;						      \
  NLines=0;						      \
}							      \
{							      \
  if (Key ==  \"\")					      \
    while(Stat)						      \
      {							      \
	print $0;					      \
	Stat=getline;					      \
        NLines++;					      \
      }							      \
  else if (($1 ~ \"^%\") && ($2 == Key))		      \
    do							      \
      {							      \
	print $0;					      \
	Stat=getline;					      \
        NLines++;					      \
      }							      \
    while (($1 !~ \"^%\") && Stat);			      \
}							      \
END{									\
   if (NLines == 0) printf(\"No help for keyword %c%s%c\",34,Key,34);	\
}'"
  
  /*----------------------------------------------------------------------*/
  int dogo(char *arg)
  {
    if (arg==NULL) return EOF;
    else
      {
	if (string(arg) == "")
	  return EOF;
	else if (string(arg) == "-b")
	  {
	    dogob(arg);
	    return EOF;
	  }
      }
    return EOF;
  }
  /*----------------------------------------------------------------------*/
  bool checkVal(Symbol* t, vector<string>& mapVal)
  {
    bool found=false;
    SMap::iterator loc;
    try
      {
	if (ISSET(t->Attributes,CL_BOOLTYPE))
	  {
	    //
	    // This a BOOLTYPE keyword.  Check for logical true/false.
	    // String comparision is not enough (e.g. string "0" and "no"
	    // are both logical false).
	    //
	    for(loc=t->smap.begin(); loc!=t->smap.end(); loc++)
	      {
		bool logicalKey = clIsTrue((*loc).first.c_str());
		if ((found = (clBoolCmp(t->Val[0],logicalKey)==logicalKey))) break;
	      }
	  }
	else
	  {
	    //
	    // For all other types, check by string comparision only.
	    //
	    loc = t->smap.find(string(t->Val[0]));
	    found = (loc != t->smap.end()); 
	  }
	if (found) mapVal=(*loc).second;
      }
    catch (clError &x)
      {
	x << x.what() << endl;
      }
    return found;
  }
  /*----------------------------------------------------------------------*/
  int exposeKeys(Symbol *t)
  {
    Symbol *S;
    int exposedSomething=0;
    /*    char *name = t->Name;*/
    
    if ((t->smap.size() > 0))
      {
	//
	// Irrespective of the current value of the symbol, hide all
	// keys on which a watch was set by this symbol
	//
        if (ISSET(t->Attributes,CL_HIDDENKEYWORD))
          SETBIT(t->Attributes,CL_HIDENSEEKKEYWORD);
        else
          SETBIT(t->Attributes,CL_HIDINGKEYWORD);
	for(SMap::iterator i=t->smap.begin(); i != t->smap.end(); i++)
	  {
	    vector<string> sv=(*i).second;
	    for(unsigned int j=0;j<sv.size();j++)
	      {
		if ((S=SearchVSymb((char *)sv[j].c_str(),cl_SymbTab))==NULL)
		    {
		      string mesg = "Programmer error: Watch key \"" + sv[j] + "\" not found.";
		      clThrowUp(mesg.c_str(),"###Fatal ",CL_FATAL);
		    }
		S->Exposed=0;
                SETBIT(S->Attributes,CL_HIDDENKEYWORD);
	      }
	  }
	//
	// Now set for display those watched-keys which are exposed by
	// the current setting of this symbol. If the current symbol
	// (t) is not exposed itself, the keys are that watching
	// remain unexposed. For now, the "current setting" is only
	// the first value (i.e. ignores other possible comma
	// seperated values).
	//
	if ((t->NVals > 0) && (t->Exposed==1))
	  {
	    // SMap::iterator loc = (t->smap.find(string(t->Val[0])));
	    // if (loc != t->smap.end())
	    vector<string> mapVal;
	    if (checkVal(t,mapVal))
	      {
		//		vector<string> sv=(*loc).second;
		vector<string> sv=mapVal;
		for(unsigned int j=0;j<sv.size();j++)
		  {
		    S=SearchVSymb((char*)sv[j].c_str(),cl_SymbTab);
		    S->Exposed=1;
		    exposedSomething=1;
		    //
		    // Recursively expose the keys.
		    //
		    exposeKeys(S);
                    //
                    // Remove the CL_KEYWORD attribute if a keyword is
                    // hidden due to a setting of another keyword which 
                    // is made into a shell constant (by the .config
                    // file).  Such a keyword should not be colour coded
                    // (since the other keyword which will hide/expose
                    // this keyword is not exposed itself...hence user
                    // has really no control on the exposure of these
                    // keywords).
                    // 
                    if (t->Class==CL_USERCLASS) 
                      {
                        RESETBIT(S->Attributes,CL_KEYWORD);
                      }
		  }
	      }
	  }
      }
    return exposedSomething;
  }
  /*----------------------------------------------------------------------*/
  int doinp(char *arg)
  {
    Symbol *t;
    //
    // First expose the keywords for this session
    //
    for (t=cl_SymbTab;t;t=t->Next) exposeKeys(t);

    //    
    // Now print the viewable keywords.
    //
    if (arg == NULL)
      for (t=cl_SymbTab;t;t=t->Next)
	{
	  if ((t->Exposed) && 
	     ((t->Class==CL_APPLNCLASS) || 
	      ((t->Class==CL_DBGCLASS) && (CL_DBG_ON))))
	    {
	      //              if (t->smap.begin() != t->smap.end())
	      PrintKey(stderr, t);
	      PrintVals(stderr,t,1);
	    }
	}
    else if (string(arg) == "-a")
      for (t=cl_SymbTab;t;t=t->Next)
	{
	  if ((t->Class==CL_APPLNCLASS) || 
	      ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	    {
	      //              if (t->smap.begin() != t->smap.end())
	      PrintKey(stderr, t);
	      PrintVals(stderr,t,1);
	    }
	}
    else
      {
	t=SearchVSymb((char*)arg,cl_SymbTab);
	if (t==NULL)
	  {
	    string mesg = "Illegal command \"inp "+string(arg)+"\"";
	    clThrowUp(mesg.c_str(),"###Fatal ",CL_FATAL);
	  }
	// if ((t->Exposed) && (t->Class==CL_APPLNCLASS) || 
	//     ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	//   {
	//     fprintf(stderr,format,t->Name);
	//     PrintVals(stderr,t);
	//   }
	vector<string> mapVal;
	checkVal(t,mapVal);
	for (unsigned int j=0; j < mapVal.size(); j++)
	  {
	    Symbol *S;
	    S=SearchVSymb((char *)mapVal[j].c_str(),cl_SymbTab);
	    if (((S->Exposed) && (S->Class == CL_APPLNCLASS)) ||
		(((S->Class == CL_DBGCLASS) && (CL_DBG_ON))))
	      {
		PrintKey(stderr, S);
		PrintVals(stderr,S,1);
	      }
	  }
      }
    return 1;
  }
  /*----------------------------------------------------------------------*/
  int doquit(char *arg)
  {
#ifdef GNUREADLINE
    /* Put the history in the history file*/
    char *var=(char *)"GHIST";
    save_hist(var,(char *)CL_HIST_DEFAULT);
#endif
    
    if (!arg) exit(0);
    return 1;
  }

  /*----------------------------------------------------------------------*/
  void formatTypeHelp(Symbol *S, string& fullFormat)
  {
	{
	  /*      fprintf(stderr,"  %-10.10s         %-10.10s\n",S->Name,S->Type);*/
	  exposeKeys(S);
	  fprintf(stderr,fullFormat.c_str(),S->Name,S->Type);
	  int n=S->DefaultVal.size(),nchar=0, offset=16;;
	  if ((n=S->DefaultVal.size())>0)
	    {
	      fprintf(stderr, "          %-s",S->DefaultVal[0].c_str());
	      nchar += strlen(S->DefaultVal[0].c_str());
	      for(int i=1;i<n;i++)
		{
		  fprintf(stderr, ",%-s",S->DefaultVal[i].c_str());
		  nchar += strlen(S->DefaultVal[i].c_str());
		}
	      //	      for(int i=0;i<10-nchar;i++) fprintf(stderr," ");
	    }
	  else
	    offset+=10;
	  for(int i=0;i<offset-nchar;i++) fprintf(stderr," ");
	    
	  if (ISSET(S->Attributes,CL_BOOLTYPE))
	    fprintf(stderr, " Use imagination or list by \"%-s=<TAB><TAB>\"", S->Name);
	  if ((n=S->Options.size())>0)
	    {
	      fprintf(stderr, " [%-s",S->Options[0].c_str());
	      for(int i=1;i<n;i++)
		fprintf(stderr, " %-s",S->Options[i].c_str());
              fprintf(stderr,"]");
	    }
	  fprintf(stderr, "\n");
	}
  }

  /*----------------------------------------------------------------------*/
  int dotypehelp(char *arg)
  {
    //char format[12];
    std::string format;
    int maxNameLength = namePrintFormat(format,"");
    string fullFormat;
    //  fullFormat << "  " <<format <<"         %-10.10s" << endl << "\0";
    fullFormat = string("  ") + string(format) + string("         %-10.10s\0");
    //    cerr << "Max length: " << maxNameLength << " " << fullFormat << endl;
    string s0;
    s0.insert(0,maxNameLength/2-1,' ');
    s0.append("Key");
    s0.insert(s0.end(),maxNameLength/2+5,' ');
    s0.append("Type");
    s0.insert(s0.end(),10,' ');
    s0.append("Factory defaults");
    s0.insert(s0.end(),maxNameLength,' ');
    s0.append("Options\n");
    // s0.append("Key                Type          Factory defaults        Options\n");
    string s1="---------          ----------       ----------------        -------\n";
    Symbol *S;

    if (arg==NULL)
     {
       fprintf(stderr,s0.c_str());
       fprintf(stderr,s1.c_str());
       for (S=cl_SymbTab;S;S=S->Next)
         if (((S->Class==CL_APPLNCLASS) ||
	     ((S->Class==CL_DBGCLASS) && (CL_DBG_ON))) &&
             (S->Exposed)
            )
           formatTypeHelp(S,fullFormat);
     }
    else if (string(arg) == "-a")
      {
       fprintf(stderr,s0.c_str());
       fprintf(stderr,s1.c_str());
       for (S=cl_SymbTab;S;S=S->Next)
         if (((S->Class==CL_APPLNCLASS) ||
	      ((S->Class==CL_DBGCLASS) && (CL_DBG_ON))))
           formatTypeHelp(S,fullFormat);
      }
    else 
     {
       if ((S=SearchVSymb(arg,cl_SymbTab))!=NULL) 
         if (((S->Class==CL_APPLNCLASS) ||
	     ((S->Class==CL_DBGCLASS) && (CL_DBG_ON))) &&
             (S->Exposed)
            )
           {
             fprintf(stderr,s0.c_str());
             fprintf(stderr,s1.c_str());

             formatTypeHelp(S,fullFormat);
           }
     }
    return 1;
  }
  /*------------------------------------------------------------------
    The argument can be use to give help for a specific command only   
    -------------------------------------------------------------------*/

  int dohelp(char *arg)
  {
    CmdSymbol *S;
    fprintf(stderr,"Colour coding of the keywords:\n");
    fprintf(stderr,"  Red:   Indicates that the keyword can hide other keywords.\n");
    fprintf(stderr,"  Blue:  Indicates that the keyword can be hidden by some other keyword(s).\n");
    fprintf(stderr,"         (usually by the first red or green coloured keyword above).\n");
    fprintf(stderr,"  Green: Indicates that the keyword can be hidden by some other keyword(s)\n");
    fprintf(stderr,"         and can itself be also hidding other keyword(s).\n");
    fprintf(stderr,"--------------------------------------------------------------------\n");
    fprintf(stderr,"Default values:\n");
    fprintf(stderr,"  If a keyword is un-set (or set to BLANK), the displayed value is the");
    fprintf(stderr,"  value that would be used.\n");
    fprintf(stderr,"--------------------------------------------------------------------\n");
#ifdef GNUREADLINE
    fprintf(stderr,"TABBED completion:\n");
    fprintf(stderr,"  Min-match algorithm is used for matching a user input with keywords.\n\n");
    fprintf(stderr,"  GNU Readline completion mechanism uses all available keywords and \n");
    fprintf(stderr,"  shell commands.\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"  A completed keyword followed by two TABs will display the options\n");
    fprintf(stderr,"  available for the particular keyword.  Nothing is done for keywords\n");
    fprintf(stderr,"  for which either no options were defined by the programmer or for \n");
    fprintf(stderr,"  which finite options do not exist (e.g. if a keyword can take any\n");
    fprintf(stderr,"  integer value).\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"  A completed shell command followed by two TABS will display possible\n");
    fprintf(stderr,"  options for the particular shell command.\n");
#endif
    fprintf(stderr,"--------------------------------------------------------------------\n");
    fprintf(stderr,"Commands in the interactive mode:\n\n");
    fprintf(stderr,"Use <Key>=<Val1,Val2,..> to set value for a keyword\n");
    fprintf(stderr,"Use <Key>=<RETURN> to unset value(s) for a keywords\n\n");
    for (S=cl_CmdTab;S;S=S->Next) 
      fprintf(stderr," %-11s : %s\n",S->Name,S->Doc);
    fprintf(stderr,"\nAny other input is passed to the system shell\n\n");
    return 1;
  }
  /*---------------------------------------------------------------------
    Searches for the file <ProgName>.def in $GDOC or in ./ and "more's"
    it. The argument can be used for giving explaination for a specific
    context
    ----------------------------------------------------------------------*/
  int doexplain(char *arg)
  {
    char *path=(char *)getenv(CL_DOCPATH);
    char *sed_script=(char *)"|sed -e \"s/%[ANP]//\"|more";
    char *script = (char *)KEYHELP_AWK,*key=0,*task=0;
    //    char *str=0;
    std::string ss;
    
    //    str=(char *)calloc(1,strlen(script)+FILENAME_MAX);
    
    if (path)
      ss += script + std::string(" ") + path + std::string("/");
    //sprintf(str,"%s %s/",script,path);
    else
      ss +=  std::string(" ") + script;
    //sprintf(str,"%s ",script);
    
    if (arg)
      {
	if(arg[0]==':') task=&arg[1];
	else if ((key=strtok(arg,":"))!=NULL)
	  task = strtok(NULL,":");
      }
    if (task)
      ss += task;
      //strcat(str,task);
    else
#ifdef GNUREADLINE
      ss += std::string(cl_ProgName).substr(0,strlen(cl_ProgName)-1);
    //strncat(str,cl_ProgName,strlen(cl_ProgName)-1);
#else
      ss += cl_ProgName;
    //strncat(str,cl_ProgName,strlen(cl_ProgName));
#endif
    
      ss += std::string(".doc ");
      //strcat(str,".doc ");
      if (key) ss += key;
      //if (key) strncat(str,key,strlen(key));
      ss += sed_script;
      //strcat(str,sed_script);

      system(ss.c_str());
      //if (str) free(str);
    return 1;
  }
  /*------------------------------------------------------------------------
    Saves the current setting of the various keywords to the given file
    If *f==NULL, save in ./<ProgName>.def
    -------------------------------------------------------------------------*/
  int dosave(char *f)
  {
    FILE *fd;
    char str[MAXBUF];
    //char format[12];
    std::string format;
    namePrintFormat(format," = ");
    stripwhite(f);
    if(f==NULL || strlen(f) == 0)
      {
	strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
	str[strlen(cl_ProgName)-1]='\0';
#endif
	strcat(str,".def");
      }
    else strcpy(str,f);
    
    if ((fd=fopen(str,"w"))==NULL)
      {
	fprintf(stderr,"###Error: Error in opening file \"%s\"\n",str);
	return 2;
      }
    else
      {
	Symbol *t;
	
	for (t=cl_SymbTab;t;t=t->Next)
	  if ((t->Class==CL_APPLNCLASS) ||
	      ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	    {
	      fprintf(fd,format.c_str(),t->Name);
	      PrintVals(fd,t,1);
	    }
	fclose(fd);
      }
    return 1;
  }
  /*------------------------------------------------------------------------
    Saves the current setting of the various keywords as a UNIX shell command
    string to the given file. If *f==NULL, save in ./<ProgName>.cmd
    -------------------------------------------------------------------------*/
  int docmdsave(char *f)
  {
    FILE *fd;
    char str[MAXBUF],ProgName[MAXBUF]="";
    stripwhite(f);

    strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
    str[strlen(cl_ProgName)-1]='\0';
#endif
    strcpy(ProgName,str);

    char rpath[PATH_MAX];
    
/*     if (realpath(ProgName,rpath)==NULL) */
/*       fprintf(stderr,"###Error: %s\n",strerror(errno)); */
    strcpy(rpath,ProgName);

    if(f==NULL || strlen(f) == 0)
      {
	strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
	str[strlen(cl_ProgName)-1]='\0';
#endif
	strcpy(ProgName,str);
	strcat(str,".cmd");
      }
    else strcpy(str,f);
    
    if ((fd=fopen(str,"w"))==NULL)
      {
	fprintf(stderr,"###Error: Error in opening file \"%s\"\n",str);
	return 2;
      }
    else
      {
	Symbol *t;
	//	fprintf(fd,"%s help=noprompt ",ProgName);
	fprintf(fd,"%s help=noprompt ",rpath);
	for (t=cl_SymbTab;t;t=t->Next)
	  if ((t->Class==CL_APPLNCLASS) ||
	      ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	    {
	      fprintf(fd,"%s=\"",t->Name);
	      PrintVals(fd,t,0); // Print without NEWLINE
	      fprintf(fd,"\"%c",' ');
	    }
	fclose(fd);
      }
    return 1;
  }
  /*-----------------------------------------------------------------------
    Loads the setting for the keywords from a file (typically written by
    the save command). If *f==NULL, load from ./<ProgName>.def.  If f ends
    with '!' character, do a complementary load; set only those keywords
    are not already set.
    ------------------------------------------------------------------------*/
  int doload(char *f)
  {
    // FILE *fd;
    // char str[MAXBUF];
    int Complement=0;

    ifstream ifs;
    string strcpp;

    cl_do_doinp=0;
    
    stripwhite(f);
    if(f==NULL || strlen(f) == 0)
      {
	strcpp = cl_ProgName; 
	//	strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
	//	str[strlen(cl_ProgName)-1]='\0';
	strcpp=strcpp.substr(0,strlen(cl_ProgName)-1);
#endif
	//	strcat(str,".def");
	strcpp.append(".def");
      }
    else 
      //      strcpy(str,f);
      strcpp = f;
    
    //    if (str[strlen(str)-1] == '!') 
    if (strcpp[strcpp.size()-1] == '!') 
      {Complement = 1; strcpp[strcpp.size()-1] = (char)NULL;}

    ifs.open(strcpp.c_str());
    //    if ((fd = fopen(str,"r"))==NULL)
    if (!ifs.good())
      {
	//    	fprintf(stderr,"###Error: Error in opening file \"%s\"\n",strcpp.c_str());
	cerr << "###Error: Error in opening file \"" << strcpp << "\"" << endl;
    	return 2;
      }
    else
      {
	char *Name=NULL, *Val=NULL;
	Symbol *pos;
	
	//	while(!feof(fd))
	while(!ifs.eof())
	  {
	    string line;
	    //	    for (i=0;i<MAXBUF;i++)str[i]='\0';
	    //	    if (fgets(str,MAXBUF,fd)!=NULL)
	    if (getline(ifs,line))
	      {
		char *str_p=(char *)line.c_str();
		//		cerr << line << endl;
		stripwhite(str_p);//str_p[strlen(str_p)-1]='\0';
		if (strlen(str_p) > 0)
		  {
		    BreakStr(str_p,&Name,&Val);
		    pos = NULL;
		    if (Complement)
		      {
			//		      pos=SearchVSymb(Name,cl_SymbTab);
			pos=SearchVSymbFullMatch(Name,cl_SymbTab);
			if ((pos == (Symbol *)NULL))
			  pos=AddVar(Name,&cl_SymbTab,&cl_TabTail);
			if ((pos->NVals == 0))
			  pos = (Symbol *)NULL;
		      }
		    if (pos==NULL)
		      SetVar(Name,Val,cl_SymbTab,0,1,cl_do_doinp);
		    if (Name != NULL) {free(Name);Name=NULL;}
		    if (Val != NULL) {free(Val);Name=NULL;}
		  }
	      }
	  }
	//	fclose(fd);
      }
    cl_do_doinp=0;
    return 1;
  }
  /*-----------------------------------------------------------------------
    Functional difference from doload is that this loads the first
    adds (AddVar) and then sets the variables (SetVar).  This is
    called for help=def,<FILE> option.

    This needs code cleanup to remove parts from doload() that aren't
    necessary here.  08Mar, 2022.
    ------------------------------------------------------------------------*/
  int doload_and_register(char *f)
  {
    // FILE *fd;
    // char str[MAXBUF];
    int Complement=0;

    ifstream ifs;
    string strcpp;

    cl_do_doinp=0;
    
    stripwhite(f);
    if(f==NULL || strlen(f) == 0)
      {
	strcpp = cl_ProgName; 
	//	strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
	//	str[strlen(cl_ProgName)-1]='\0';
	strcpp=strcpp.substr(0,strlen(cl_ProgName)-1);
#endif
	//	strcat(str,".def");
	strcpp.append(".def");
      }
    else 
      //      strcpy(str,f);
      strcpp = f;
    
    //    if (str[strlen(str)-1] == '!') 
    if (strcpp[strcpp.size()-1] == '!') 
      {Complement = 1; strcpp[strcpp.size()-1] = (char)NULL;}

    //    cerr << "Loading from " << strcpp << "...not yet working" << endl;
    ifs.open(strcpp.c_str());
    //    if ((fd = fopen(str,"r"))==NULL)
    if (!ifs.good())
      {
	//    	fprintf(stderr,"###Error: Error in opening file \"%s\"\n",strcpp.c_str());
	clThrowUp(std::string("Error in opening file \"")+strcpp+std::string("\""), "###Error", CL_FATAL);
	//cerr << "###Error: Error in opening file \"" << strcpp << "\"" << endl;
	return 2;
      }
    else
      {
	char *Name=NULL, *Val=NULL;
	Symbol *pos;
	
	//	while(!feof(fd))
	while(!ifs.eof())
	  {
	    string line;
	    //	    for (i=0;i<MAXBUF;i++)str[i]='\0';
	    //	    if (fgets(str,MAXBUF,fd)!=NULL)
	    if (getline(ifs,line))
	      {
		char *str_p=(char *)line.c_str();
		//		cerr << line << endl;
		stripwhite(str_p);//str_p[strlen(str_p)-1]='\0';
		if (strlen(str_p) > 0)
		  {
		    BreakStr(str_p,&Name,&Val);
		    pos = NULL;
		    if (Complement)
		      {
			//		      pos=SearchVSymb(Name,cl_SymbTab);
			pos=SearchVSymbFullMatch(Name,cl_SymbTab);
			if ((pos == (Symbol *)NULL))
			  pos=AddVar(Name,&cl_SymbTab,&cl_TabTail);
			if ((pos->NVals == 0))
			  pos = (Symbol *)NULL;
		      }
		    if (pos==NULL)
		      {
			pos=AddVar(Name,&cl_SymbTab,&cl_TabTail);
			SetVar(Name,Val,cl_SymbTab,0,1,cl_do_doinp);
		      }
		    if (Name != NULL) {free(Name);Name=NULL;}
		    if (Val != NULL) {free(Val);Name=NULL;}
		  }
	      }
	  }
	//	fclose(fd);
      }
    cl_do_doinp=0;
    return 1;
  }
  /*------------------------------------------------------------------------
    Allow editing of the keyword values using the "favourate" editor.
    -------------------------------------------------------------------------*/
  int doedit(char *arg)
  {
    char *tmpname=tempnam("/tmp","cl_");
    // Build and issue a system command to edit a file with current
    // keyword=value pairs
    {
      char *editor=(char *)getenv(CL_EDITORENV);
      std::ostringstream str;
    
      if (dosave(tmpname)>1) return 1;
    
      if (editor != NULL)
	str << editor << " " << tmpname;
      else
	str << "emacs -nw " << tmpname;
      
      system(str.str().c_str());
      doload(tmpname);
    }

    // Build and issue a system command to remove the temp file
    {
      std::ostringstream str;
      str << "/bin/rm -rf " << tmpname << "*";
      system(str.str().c_str());
    }
    return 1;
  }
  /*----------------------------------------------------------------------*/
  int docd(char *dir)
  {
    char *s=dir;
    
    if (dir == NULL || strlen(dir)==0) s=(char *)getenv(CL_HOMEENV);
    
    if (chdir(s)==-1)   perror(s);
    else                system("/bin/pwd");
    return 1;
  }
  /*----------------------------------------------------------------------*/
  int dogob(char *arg)
  {
    int PID,OUT=0,ERR=0;
    char *gout=(char *)getenv(CL_STDOUTENV), *gerr=(char *)getenv(CL_STDERRENV);
    
    if (gout == NULL)
      {
	gout = (char *)malloc(10);
	strcpy(gout,"/dev/null");
	OUT=1;
      }
    
    if (gerr == NULL)
      {
	gerr = (char *)malloc(10);
	strcpy(gerr,"/dev/null");
	ERR=1;
      }
    
    if ((PID=fork()) != 0) 
      {
	/*
	  Fork succeeded - this means that the parent can exit.
	  Since the child will be executing the code, parent must exit.
	*/
	fprintf(stderr,"\"%s\" is running in background (PID=%d)\n",
		cl_ProgName,PID);
	exit(0);
      }
    /*
      This will be executed by the child after it's forked.  It will
      first redirect the stderr and stdout and then return an EOF to 
      the shell indicating that the user interaction is over and it 
      must proceed further,  i.e. transfer control to the application 
      code.
    */
    redirect(gout,gerr);
    if (OUT) free(gout);
    if (ERR) free(gerr);
    return EOF;
  }
  /*----------------------------------------------------------------------*/
  int doprintdoc(const char *val)
  {
    Symbol *S;
    if (cl_ProgName[strlen(cl_ProgName)-1]=='>')
      cl_ProgName[strlen(cl_ProgName)-1]='\0';
    
    fprintf(stdout,"%%N %s\n",cl_ProgName);
    fprintf(stdout,"\t<Put the explaination for the task here>\n\n");
    fprintf(stdout,"%%P Author\n");
    fprintf(stdout,"\t<Put your name and e-mail address here>\n\n");
    
    for (S=cl_SymbTab;S;S=S->Next)
      {
	if (string(S->Name)=="") break;
	string val;
	clgetFullValp(string(S->Name),val);
	
	if ((S->Class==CL_APPLNCLASS) || (S->Class == CL_DBGCLASS))
	  {
	    //
	    // Print name and default value
	    //
	    fprintf(stdout,"%%A %s (default=%s)",
		    S->Name,val.c_str());
	    //
	    // Print options, if available
	    //
	    if (S->Options.size() > 0)
	      {
		fprintf(stdout," Options:[");
		for(unsigned int i=0;i<S->Options.size();i++)
		  fprintf(stdout,"%s ",S->Options[i].c_str());
		fprintf(stdout,"]");
	      }
	    //
	    // Print watched-keywords map, if available
	    //
	    if (S->smap.begin() != S->smap.end())
	      fprintf(stdout,"\n\n\tWatched keywords (<VALUE>: <Keywords exposed>):\n");
	    for(SMap::iterator i=S->smap.begin(); i != S->smap.end(); i++)
	      {
		fprintf(stdout,"          %s: ",(*i).first.c_str());
		vector<string> sv=(*i).second;
		for(unsigned int j=0;j<sv.size();j++)
		  {
		    fprintf(stdout,"%s ",(char *)sv[j].c_str());
		  }
		fprintf(stdout,"\n");
	      }
	    //
	    // ...rest is upto a human to fill-in.
	    //
	    fprintf(stdout,
		    "\n\n\t<Put the explaination for the keyword here>\n\n\n");
	  }
	if (S->Class==CL_DBGCLASS)
	  fprintf(stdout,
		  "\t***This keyword is exposed with a command-line argument of \"help=dbg\"***\n");
      }
    exit(0);
    return 1;
  }
  /*----------------------------------------------------------------------*/
  int doademo(char *val)
  {
    if (val) fprintf(stderr,"Argument to the command was: %s\n",val);
    else fprintf(stderr,"No argument(s) given to the command\n");
    return 1;
  }
  /*----------------------------------------------------------------------*/
  int loadDefaults(int complement)
  {
    char out[FILENAME_MAX+2]="./", *t;
    FILE *fd;
    
    /*
      First, do a complimentary load from the .def file available
      locally.
    */
    
#ifdef GNUREADLINE
    strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
#else
    strcat(out,cl_ProgName);
#endif
    strcat(out,".def");
    
    if ((fd = fopen(out,"r")) != NULL)  
      {
	fclose(fd);  
	if (complement) strcat(out,"!");  /* Perform a complimentery load */
	doload(out);
      }
    
    /*
      If CL_DEFAULTSENV env. var. is set, look for a .def file
      there and if found, do a complimentary load from there too.
    */

    t=(char *)getenv(CL_DEFAULTSENV);
    if (t && strlen(t))     {strncpy(out,t,FILENAME_MAX);strcat(out,"/");}
    
#ifdef GNUREADLINE
    strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
#else
    strcat(out,cl_ProgName);
#endif
    strcat(out,".def");
    
    if ((fd = fopen(out,"r")) != NULL)  
      {
	fclose(fd);  
	//	if (complement) 
	  strcat(out,"!");  /* Perform a complimentery load */
	doload(out);
      }
    
    /*
      Final effect will be that first any commandline setting will be
      set.  Next, the local .def file will be hououred for all those
      keys which still remain unset.  Next, .def file from CL_DEFAULTSENV
      area will he honoured for those keys which continue to remain
      unset.  */
    
    return 1;
  }
//
//-----------------------------------------------------------------
// Show copyright info.
//
  int docopyright(const std::string& Msg)
{
  cerr << "   parafeed 2.0" << endl
       << "   Copyright (c) 2000-2021, 2022 S. Bhatnagar (bhatnagar (DOT) sanjay (AT) gmail (DOT) com)"
       << endl
       << "   This is free software with ABSOLUTELY NO WARRANTY." << endl;
  if (Msg!="") cerr << Msg << endl << endl;
  return 1;
}
//
//-----------------------------------------------------------------
// Show warranty info.
//
int dowarranty(char *msg)
{
  docopyright("");
 
  cerr << "   This program is free software; you can redistribute it and/or modify" << endl
       << "   it under the terms of the GNU General Public License as published by" << endl
       << "   the Free Software Foundation; either version 2 of the License, or" << endl
       << "   (at your option) any later version." << endl << endl
       << "   This program is distributed in the hope that it will be useful," << endl
       << "   but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl
       << "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl
       << "   GNU General Public License for more details." << endl << endl
       << "   You should have received a copy of the GNU General Public License" << endl
       << "   along with this program. If not, write to the Free Software" << endl
       << "   Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA." << endl;
  return 1;
}
#ifdef __cplusplus
}
#endif

