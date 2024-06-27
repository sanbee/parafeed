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
  void formatTypeHelp(FILE *fd, Symbol *S, const string& fullFormat)
  {
    exposeKeys(S);
    fprintf(fd,fullFormat.c_str(),S->Name,S->Type);
    int n=S->DefaultVal.size(),nchar=0, offset=16;;
    if ((n=S->DefaultVal.size())>0)
      {
	fprintf(fd, "          %-s",S->DefaultVal[0].c_str());
	nchar += strlen(S->DefaultVal[0].c_str());
	for(int i=1;i<n;i++)
	  {
	    fprintf(fd, ",%-s",S->DefaultVal[i].c_str());
	    nchar += strlen(S->DefaultVal[i].c_str());
	  }
	//	      for(int i=0;i<10-nchar;i++) fprintf(fd," ");
      }
    else
      offset+=10;
    for(int i=0;i<offset-nchar;i++) fprintf(fd," ");
	    
    if (ISSET(S->Attributes,CL_BOOLTYPE))
      fprintf(fd, " Use imagination or list by \"%-s=<TAB><TAB>\"", S->Name);
    if ((n=S->Options.size())>0)
      {
	fprintf(fd, " [%-s",S->Options[0].c_str());
	for(int i=1;i<n;i++)
	  fprintf(fd, " %-s",S->Options[i].c_str());
	fprintf(fd,"]");
      }
    fprintf(fd, "\n");
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

    //    cerr << s0; cerr << s1;

    //
    // Print the keys using the supplied printer.  This also
    // interprets the arg string.
    //
    auto printer = [&fullFormat,&s0,&s1](FILE *fd, Symbol *t)
		   {
		     // Print the header (it is in the s0 and s1
		     // strings)...
		     fprintf(fd,"%s%s",s0.c_str(),s1.c_str());
		     // ...and make sure the header is printed only once.
		     s0=s1="";
		     formatTypeHelp(fd,t,fullFormat);
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
    cerr << "Colour coding of the keywords:\n";
    cerr << "  Red:   Indicates that the keyword can hide other keywords.\n";
    cerr << "  Blue:  Indicates that the keyword can be hidden by some other keyword(s).\n";
    cerr << "         (usually by the first red or green coloured keyword above).\n";
    cerr << "  Green: Indicates that the keyword can be hidden by some other keyword(s)\n";
    cerr << "         and can itself be also hidding other keyword(s).\n";
    cerr << "--------------------------------------------------------------------\n";
    cerr << "Default values:\n";
    cerr << "  If a keyword is un-set (or set to BLANK), the displayed value is the\n";
    cerr << "  value that would be used.\n";
    cerr << "--------------------------------------------------------------------\n";
#ifdef GNUREADLINE
    cerr << "TABBED completion:\n";
    cerr << "  Min-match algorithm is used for matching a user input with keywords.\n\n";
    cerr << "  GNU Readline completion mechanism uses all available keywords and \n";
    cerr << "  shell commands.\n";
    cerr << endl;
    cerr << "  A completed keyword followed by two TABs will display the options\n";
    cerr << "  available for the particular keyword.  Nothing is done for keywords\n";
    cerr << "  for which either no options were defined by the programmer or for \n";
    cerr << "  which finite options do not exist (e.g. if a keyword can take any\n";
    cerr << "  integer value).\n";
    cerr << endl;
    cerr << "  A completed shell command followed by two TABS will display possible\n";
    cerr << "  options for the particular shell command.\n";
#endif
    cerr << "--------------------------------------------------------------------\n";
    cerr << "Commands in the interactive mode:\n\n";
    cerr << "Use <Key>=<Val1,Val2,..> to set value for a keyword\n";
    cerr << "Use <Key>=<RETURN> to unset value(s) for a keywords\n\n";
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
    std::string format,FileName=ProgName();

    namePrintFormat(format," = ");
    stripwhite(f);
    if(f==NULL || strlen(f) == 0)
      FileName=ProgName()+".def";
    else
      FileName=f;
    
    if ((fd=fopen(FileName.c_str(),"w"))==NULL)
      {
	clThrowUp(std::string("Error in opening file \"")+FileName+std::string("\" for writing"), "###Error", CL_FATAL);
    	return 2;
      }
    else
      {
	dosavefd(fd);
	fclose(fd);
      }
    return 1;
  }
  /*------------------------------------------------------------------------
    Saves the current setting of the various keywords to the given file
    descriptor
    -------------------------------------------------------------------------*/
  int dosavefd(FILE *fd)
  {
    std::string format,str;
    namePrintFormat(format," = ");

    Symbol *t;
	
    for (t=cl_SymbTab;t;t=t->Next)
      if ((t->Class==CL_APPLNCLASS) ||
	  ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	{
	  fprintf(fd,format.c_str(),t->Name);
	  PrintVals(fd,t,1);
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
    string AppName=ProgName(),fileName;
    stripwhite(f);

    // char rpath[PATH_MAX];
    // if (realpath(ProgName,rpath)==NULL) 
    //   fprintf(stderr,"###Error: %s\n",strerror(errno)); 

    if(f==NULL || strlen(f) == 0)
      fileName=AppName+".cmd";
    else
      fileName=string(f);
    
    if ((fd=fopen(fileName.c_str(),"w"))==NULL)
      {
	clThrowUp(std::string("Error in opening file \"")+fileName+std::string("\" for writing"), "###Error", CL_FATAL);
	return 2;
      }
    else
      {
	Symbol *t;
	fprintf(fd,"%s help=noprompt ",AppName.c_str());
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
    Loads the setting for the keywords from a file (typically written
    by the save command). If *f==NULL, load from ./<ProgName>.def.  If
    f ends with '!' character, do a complementary load; set only
    keywords that are not already set.
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
    int Complement=0;

    ifstream ifs;
    string strcpp;

    cl_do_doinp=0;
    
    stripwhite(f);
    if(f==NULL || strlen(f) == 0)
      {
	strcpp = cl_ProgName; 
#ifdef GNUREADLINE
	//	str[strlen(cl_ProgName)-1]='\0';
	strcpp=strcpp.substr(0,strlen(cl_ProgName)-1);
#endif
	strcpp.append(".def");
      }
    else 
      strcpp = f;
    
    if (strcpp[strcpp.size()-1] == '!') 
      {Complement = 1; strcpp[strcpp.size()-1] = (char)NULL;}

    ifs.open(strcpp.c_str());

    if (!ifs.good())
      {
	clThrowUp(std::string("Error in opening file \"")+strcpp+std::string("\""), "###Error", CL_FATAL);
	return 2;
      }
   else
      {
	Symbol *pos;
	
	while(!ifs.eof())
	  {
	    string line;
	    if (getline(ifs,line))
	      {
		stripwhitep(line);
		if ((line.size() > 0) && (line[0] != '#'))
		  {
		    std::string Name_str, Val_str;
		    BreakStrp(line,Name_str,Val_str);
		    stripwhitep(Name_str);
		    stripwhitep(Val_str);

		    pos = NULL;

		    if (Complement)
		      {
			pos=SearchVSymbFullMatch(Name_str.c_str(),cl_SymbTab);
			if ((pos == (Symbol *)NULL))
			  pos=AddVar(Name_str.c_str(),&cl_SymbTab,&cl_TabTail);
			if ((pos->NVals == 0))
			  pos = (Symbol *)NULL;
		      }
		    if (pos==NULL)
		      {
			pos=AddVar(Name_str.c_str(),&cl_SymbTab,&cl_TabTail);
			SetVar((char*)Name_str.c_str(),(char *)Val_str.c_str(),cl_SymbTab,0,1,cl_do_doinp);
		      }
		  }
	      }
	  }
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
	string val=vecStr2Str(S->DefaultVal);
	//	clgetFullValp(string(S->Name),val);
	
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
  int doprintparams(const char *val)
  {
    Symbol *S;
    if (cl_ProgName[strlen(cl_ProgName)-1]=='>')
      cl_ProgName[strlen(cl_ProgName)-1]='\0';

    cout <<"%%N:" << cl_ProgName << endl;

    for (S=cl_SymbTab;S;S=S->Next)
      {
	if (string(S->Name)=="") break;
	string val = vecStr2Str(S->DefaultVal);
	if ((S->Class==CL_APPLNCLASS) || (S->Class == CL_DBGCLASS))
	  {
	    //
	    // Print name and default value
	    //
	    cout << "%%P:" << S->Name << ":" << S->Type << ":" << val << ":";
	    if (S->Options.size() > 0)
	      for(auto op : S->Options) cout << op << "|";
	    cout << endl;
	  }
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

