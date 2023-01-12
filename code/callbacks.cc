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
#include <support.h>
#include <string>
#include <errno.h>
#include <limits.h>
#include <boolError.h>
#include <sstream>
#include <regex>
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
	    // This is a BOOLTYPE keyword.  Check for logical
	    // true/false.  String comparision is not enough
	    // (e.g. string "0" and "no" are both logical false).
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
	    if (t->Val.size() > 0)
	      {
		loc = t->smap.find(string(t->Val[0]));
		found = (loc != t->smap.end());
	      }
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
    auto printer = [](FILE *fd, Symbol *t)
		   {
		     PrintKey(stderr, t);
		     PrintVals(stderr,t,1);
		   };
    return showKeys(arg,printer);
  }
  /*----------------------------------------------------------------------*/
  void formatTypeHelp(Symbol *S, string& fullFormat)
  {
    if (S==NULL)
      clThrowUp(std::string("Key not found or is not currently exposed"), "###Informational", CL_WARNING);

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

  /*----------------------------------------------------------------------*/
  // TODO: The constants in the code below need to be determined
  // programmatically.  The necessary information to do so is in the
  // (global) symbol table (various strings to be printed).
  int dotypehelp(char *arg)
  {
    //
    // Construct and print the header
    //
    std::string format;
    int maxNameLength = namePrintFormat(format,"");
    string fullFormat;
    //  fullFormat << "  " <<format <<"         %-10.10s" << endl << "\0";
    fullFormat = string("  ") + string(format) + string("         %-10.10s\0");
    //    cerr << "Max length: " << maxNameLength << " " << fullFormat << endl;
    string s0;
    s0.insert(0,maxNameLength/2-1,' ');          s0.append("Key");
    s0.insert(s0.end(),maxNameLength/2+10,' ');  s0.append("Type");
    s0.insert(s0.end(),10,' ');                  s0.append("Factory defaults");
    s0.insert(s0.end(),maxNameLength,' ');       s0.append("Options\n");
    // s0.append("Key                Type          Factory defaults        Options\n");
    string s1;//="---------          ----------       ----------------        -------\n";
    s1.insert(0,maxNameLength,'-');
    s1.insert(s1.end(),11,' ');           s1.insert(s1.end(), 6 ,'-');
    s1.insert(s1.end(),9,' ');            s1.insert(s1.end(), 16 ,'-');
    s1.insert(s1.end(),maxNameLength,' ');s1.insert(s1.end(), 7 ,'-');
    s1.insert(s1.end(),'\n');

    cerr << s0; cerr << s1;

    //
    // Print the keys using the supplied printer.  This also
    // interprets the arg string.
    //
    auto printer = [&fullFormat](FILE *fd, Symbol *t)
		   {
		     formatTypeHelp(t,fullFormat);
		   };
    return showKeys(arg,printer);
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

  /*------------------------------------------------------------------
    The argument can be use to give help for a specific command only   
    -------------------------------------------------------------------*/
  int dohelp(char *arg)
  {
    CmdSymbol *S;
    cerr << "Colour coding of the keywords:" << endl;
    cerr << "  Red:   Indicates that the keyword can hide other keywords." << endl;
    cerr << "  Blue:  Indicates that the keyword can be hidden by some other keyword(s)." << endl;
    cerr << "         (usually by the first red or green coloured keyword above)." << endl;
    cerr << "  Green: Indicates that the keyword can be hidden by some other keyword(s)" << endl;
    cerr << "         and can itself be also hidding other keyword(s)." << endl;
    cerr << "--------------------------------------------------------------------" << endl;
    cerr << "Default values:" << endl;
    cerr << "  If a keyword is un-set (or set to BLANK), the displayed value is the" << endl;
    cerr << "  value that would be used." << endl;
    cerr << "--------------------------------------------------------------------" << endl;
#ifdef GNUREADLINE
    cerr << "TABBED completion:" << endl;
    cerr << "  Min-match algorithm is used for matching a user input with keywords." << endl << endl;
    cerr << "  GNU Readline completion mechanism uses all available keywords and " << endl;
    cerr << "  shell commands." << endl;
    cerr << endl;
    cerr << "  A completed keyword followed by two TABs will display the options" << endl;
    cerr << "  available for the particular keyword.  Nothing is done for keywords" << endl;
    cerr << "  for which either no options were defined by the programmer or for " << endl;
    cerr << "  which finite options do not exist (e.g. if a keyword can take any" << endl;
    cerr << "  integer value)." << endl;
    cerr << endl;
    cerr << "  A completed shell command followed by two TABS will display possible" << endl;
    cerr << "  options for the particular shell command." << endl;
#endif
    cerr << "--------------------------------------------------------------------" << endl;
    cerr << "Commands in the interactive mode:" << endl << endl;
    cerr << "Use <Key>=<Val1,Val2,..> to set value for a keyword" << endl;
    cerr << "Use <Key>=<RETURN> to unset value(s) for a keywords" << endl << endl;
    for (S=cl_CmdTab;S;S=S->Next) 
      fprintf(stderr," %-11s : %s\n",S->Name,S->Doc); // Convert this to cerr statement
    cerr << endl << "Any other input is passed to the system shell" << endl << endl;
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
      ss +=  std::string(" ") + script + std::string(" ");
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
	cerr << "###Error: Error in opening file \"" << str << "\"" << endl;
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
    Allow editing of the keyword values using the "favorite" editor.
    -------------------------------------------------------------------------*/
  int doedit(char *arg)
  {
    char tmpname[]="/tmp/cl_XXXXXX";//tempnam("/tmp","cl_");
    int fd;
    if ((fd = mkstemp(tmpname)) == -1)
      {
	std::string msg=std::string("Error in opening tempfile in \"edit")+std::string("\" command. ")+std::string(strerror(errno));
	clThrowUp(msg, "###Error", CL_INFORMATIONAL);
      }
    // Build and issue a system command to edit a file with current
    // keyword=value pairs
    {
      char *editor=(char *)getenv(CL_EDITORENV);
      if (editor==NULL) editor = (char *)getenv("EDITOR");
      std::ostringstream str;
    
      if (dosave(tmpname)>1) return 1;
    
      if (editor != NULL)
	str << editor << " " << tmpname;
      else
	str << "emacs -nw " << tmpname;
      
      system(str.str().c_str());
      doload(tmpname);
    }

    // Remove the temp file
    unlink(tmpname);
    close(fd);
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
    
    cout <<"%%N " << cl_ProgName << endl;
    cout << "\t<Put the explaination for the task here>" << endl << endl;
    cout << "%%P Author" << endl;
    cout << "\t<Put your name and e-mail address here>" << endl << endl;

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
	    cout << "%%A " << S->Name << " (default=" << val <<")";
	    if (S->Options.size() > 0)
	      {
		cout << " Options:[";
		for(auto op : S->Options) cout << " " << op;
		cout << "]";
	      }
	    //
	    // Print watched-keywords map, if available
	    //
	    if (S->smap.begin() != S->smap.end())
	      cout << endl << endl << "\tWatched keywords (<VALUE> : <Keywords exposed>):" << endl;

	    for(auto sm : S->smap) // std::SMap
	      {
		cout <<"          " << sm.first << " : "; // Value that exposes other keywords
		for(auto sv : sm.second) // std::vector<std::string> as list of exposed keywords
		  cout << sv << " ";
		cout << endl;
	      }
	    //
	    // ...rest is upto a human to fill-in.
	    //
	    cout << endl << endl << "\t<Put the explaination for the keyword here>" << endl << endl << endl;
	  }
	if (S->Class==CL_DBGCLASS)
	  cout << "\t***This keyword is exposed with a command-line argument of \"help=dbg\"***" << endl;
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

