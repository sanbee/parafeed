/* $Id: ParseCmdLine.c,v 2.2 1999/07/17 17:03:44 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <cl.h>
#include <setjmp.h>
#include "shell.tab.h"
#include <stdlib.h>
#include <clError.h>
  /*#include <signal.h>*/
#ifdef __cplusplus
#ifdef _GNU_SOURCE
#include <exception>
#else
#include <exception>
#endif
extern "C" {
#endif


#define POSITIVE(str,a)   {if ((a) < 0)                                \
                             {fprintf(stderr,"%s: Index is negative\n" \
					,(str));exit(-1);}}
/*------------------------------------------------------------------------
   Globals for the entire library.  These can be eleminated if the lib.
   developed as a C++ object - but that cannot interface with FORTRAN!
------------------------------------------------------------------------*/
unsigned short cl_CmdLineFirst=0;
unsigned short cl_InteractiveShell=0;
unsigned short cl_DOCLEANUP=1, cl_SymbLoaded = 0;
unsigned short cl_Pass = 0, cl_FORTRAN=0, cl_NoOfOpts=0;
unsigned short cl_RegistrationMode=1, cl_NoPrompt=0;
unsigned short CL_DBG_ON=0;
jmp_buf *cl_env=0;

Symbol *cl_tmpTab, *cl_tmpTabTail;
CmdSymbol *cl_CmdTab, *cl_CmdTail;
Symbol *cl_SymbTab, *cl_TabTail;
char *cl_ProgName, *cl_CommandLine;
typedef void (*sighandler_t)(int);
sighandler_t cl_defaultSigHandler;

char **cl_fargv;
int cl_fargc=0;

/*-------------------------------------------------------------------------*/
/* Parse the comandline of the form <Key>=<Value(s)>.  It produces a list  */
/* objects with the associated Key, the no. of associated values, and list */
/* of values.  Everything in the object (which is a structure here) is in  */
/* ASCII format.                                                           */
/*                                                                         */
/*                                                                         */
/* There are two Symbol tables for variables of the shell and one command  */
/* table for the commands of the shell.  These tables are filled here and  */
/* then passed on to the shell to interact with the user.                  */
/*                                                                         */
/* The way these tables are filled depends on three flags:                 */
/*    cl_CmdLineFirst, cl_InteractiveShell, cl_RegistrationMode            */
/*                                                                         */
/* The first flag controls the order in which the arguments should be      */
/* presented to the user as well as the various clget[IFS]Val routines.    */
/* The second flag determines if the interactive shell should be started.  */
/* The third one is only for internal use of the lib.  This is true to     */
/* to begin with, but is set to false after the lib. has determined all    */
/* the variables that the user would be presented with (via commandline    */
/* arg or/and via clget* querries).                                        */
/*                                                                         */
/* The algorithm used for filling the symbol table is:                     */
/*                                                                         */
/* if (cl_InteractiveShell==1 && cl_CmdLineFirst==1)                       */
/*     Load Cmd. Line in cl_SymbTab                                        */
/*     Load info. from the querries in cl_tmpTab                           */
/*     Transfer cl_tmpTab to cl_SymbTab.                                   */
/*     Delete(cl_tmpTab)                                                   */
/*     set cl_tmpTab to point to cl_SymbTab                                */
/*                                                                         */
/* if (cl_InteractiveShell==1 && cl_CmdLineFirst==0)                       */
/*     Load Cmd. Line in cl_SymbTab                                        */
/*     Load info. from the querries in cl_tmpTab                           */
/*     Transfer cl_SymbTab to cl_tmpTab.                                   */
/*     Delete(cl_SymbTab)                                                  */
/*     set cl_SymbTab to point to cl_tmpTab                                */
/*                                                                         */
/* if (cl_InteractiveShell==0 && cl_CmdLineFirst==0)                       */
/*     Load Cmd. Line in cl_SymbTab                                        */
/*     set cl_SymbTab to point to cl_tmpTab                                */
/*                                                                         */
/* When cl_InteractiveShell is false, interactive shell will never be      */
/* and therefore no need to record the querries.                           */
/*-------------------------------------------------------------------------*/
int ParseCmdLine(int argc, char *argv[])
{
  int i,n,m,NOpts;
  char *tok, *buf;
  Symbol *S;

  if ((cl_Pass > 0) || (cl_SymbLoaded)) return cl_NoOfOpts;

  n = sizeof(Symbol)*argc;
  
  n=0;
  for (i=0;i<argc; i++)
    n += strlen(argv[i]) + 1;
  cl_CommandLine = (char *)getmem(n+1, "BeginParam:1");
  
  for (i=0; i<argc; i++)
    {
      strcat (cl_CommandLine, argv[i]);
      strcat (cl_CommandLine, " ");
    }

  if ((buf = strrchr(argv[0],'/'))==NULL) 
    buf = argv[0];
  else
    buf++;
#ifdef GNUREADLINE
  i = strlen(buf)+2;
#else
  i = strlen(buf)+1;
#endif

  cl_ProgName = (char *)getmem(i,"BeginParam:2"); /* Store the program name */
  strcpy(cl_ProgName, buf);

#ifdef GNUREADLINE
  strcat(cl_ProgName,">");
#endif

  NOpts = argc-1;
  argc = 1;
  cl_NoOfOpts=0;
  for (i=0;i<NOpts;i++)             /* Parse the rest of the options */
    {
      int j;
      buf = argv[i+1];
      while (*buf == ' ') buf++;

      if (*buf == '=')
	{
	  char str[128];
	  sprintf( "%s> Unknown token \"%s\" found.\n",cl_ProgName,buf);
	  clThrowUp(str,"###Fatal ",CL_FATAL);
	  exit(-1);
	}

/* 
   A nasty way of pushing the un-used commandline options up the 
   argv list.  This is a clear memory leak, but since argv is a static
   variable, this is the only way!
*/
      if (!strstr(buf,"=")) 
	argv[argc++] = buf;
      else
	{              
	  cl_NoOfOpts++;
	  tok = strtok(buf,"="); 
	  n = strlen(tok);

	  S = AllocVSymb(1);
	  S->Used = 0;         /*Mark the option as unsed */
	  
	  while (tok[n] == ' ') n--;
	  S->Name = (char *)getmem(n+1,"BeginParam:2");
	  strncpy(S->Name, tok,n);
	  S->Name[n]='\0';  /* Put NULL since strncpy *may* not put it */

	  buf += n+1;              /* Skip the '=' sign */
	  while (*buf == ' ') buf++;
	  S->NVals = 0;    /* Copy multiple values in the Val list */
	  n=j=0;

	  n = ntok(buf,",",ESC);
	  if (n) S->Val = (char **)
		   getmem(sizeof(char **)*(n+1),"Parse:Symb->Val");
	  n=0;
	  tok=(char *)clstrtok(buf,",",ESC);
	  while (tok)
	    {
	      m = strlen(tok);
	      if (m > 0)
		{
		  m--;
		  while (tok[m] == ' ') m--;
		  if (m >= 0) 
		    {
		      S->NVals++;
		      tok[m+1]='\0';
		      S->Val[n] = (char *)getmem(m+2,"BeginParam:3");
		      strncpy(S->Val[n], tok,m+1);
		      S->Val[n][m+1]='\0';
		      n++;
		    }
		}
	      tok=(char *)clstrtok(NULL,",",ESC);
	    }
	  AddVNode(S,&cl_SymbTab,&cl_TabTail);
	}
    }
  cl_tmpTab = cl_SymbTab;  cl_tmpTabTail = cl_TabTail;
  {
    Symbol *S;
    if ((S=SearchNode("help",cl_SymbTab))!=NULL && S->NVals > 0)
      {
	if (!strcmp(S->Val[0],"noprompt"))
	  {
	    cl_RegistrationMode=0;
	    cl_NoPrompt = 1;
	  }
	if (!strcmp(S->Val[0],"dbg"))
	  CL_DBG_ON = 1;
      }
  }
  
  argc--;
#ifdef __cplusplus
  set_terminate(clDefaultErrorHandler);
#endif
  return cl_NoOfOpts;
}
/*------------------------------------------------------------------------
  Start the CL library
------------------------------------------------------------------------*/
int BeginCL(int argc, char **argv)
{
  int N=ParseCmdLine(argc,argv);
  InstallSymb();
  //  clLoadSymb();
  //
  // Switched this ON here again.  With the setAuto?Defaults 
  // used in clget?Valp, the defaults from the .def files are 
  // never loaded if this is not called here.
  //
  if (!cl_NoPrompt) loadDefaults();
  return argc-N;
}
/*------------------------------------------------------------------------
  First load the list of options that come from commandline in the 
  symbol table.
------------------------------------------------------------------------*/
void clCmdLineFirst()
{  
/*  
   TransferTab(&cl_SymbTab,&cl_TabTail,cl_tmpTab,1);
*/
  cl_CmdLineFirst = 1; 
}
/*------------------------------------------------------------------------
  Till this is called, all the symbols, as detected from the application
  querry are NOT in the symbol table.  So load them now and clear the
  temporary table (that is of no use for the rest of the life of the
  application).
------------------------------------------------------------------------*/
void clLoadSymb()
{
  Symbol *S, *T;
  if (cl_InteractiveShell)
    {
      for (S=cl_tmpTab;S;S=S->Next)
	{
	  if ((T = SearchVSymb(S->Name,cl_SymbTab))!=NULL)
	    CopyVSymb(T,S,COPYCLASS);
	}
      if (cl_CmdLineFirst)
	{
	  TransferTab(&cl_SymbTab,&cl_TabTail,cl_tmpTab,0);
	  DeleteVTab(&cl_tmpTab, &cl_tmpTabTail);
	  cl_tmpTab = cl_SymbTab;
	  cl_tmpTabTail = cl_TabTail;
	}
      else
	{
	  TransferTab(&cl_tmpTab,&cl_tmpTabTail,cl_SymbTab,1);
	  DeleteVTab(&cl_SymbTab, &cl_TabTail);
	  cl_SymbTab = cl_tmpTab;
	  cl_TabTail = cl_tmpTabTail;
	}
    }
  else 
    {
      cl_SymbTab = cl_tmpTab;
      cl_TabTail = cl_tmpTabTail;
    }
  cl_SymbLoaded = 1;
}
/*------------------------------------------------------------------------
  Start the interactive shell if it needs to.
  If InteractiveShell is true (i.e. the user wants the shell) and this
  is the first pass (i.e. the user has not yet interacted with the 
  shell) then, transfer the symbol table from the temp. table to the
  permanent one and start the shell.
  On return from the shell, do a long jump - this will be a re-entry in
  the CL lib. so that application program can now see the new values of
  the variables.
------------------------------------------------------------------------*/
int startShell()
{
  int i;
  if (!cl_InteractiveShell && cl_FORTRAN==0) return 0;
  if ((cl_InteractiveShell || cl_FORTRAN==1) && cl_Pass==0)
    {
      if (!cl_SymbLoaded) 
	{
	  clLoadSymb();   /* Transfer symbols from temp. to permanent table*/
	  loadDefaults(); /* Load the defaults */
	  doinp(NULL);    /* Display the keywords */

#ifdef GNUREADLINE
/* Load the history from the history file*/
	  limit_hist("MAXGHIST",HIST_LIMIT);
	  load_hist("GHIST",HIST_DEFAULT);
#endif
	}
#if defined(FORTRAN)
/**********************
   This is a work around for a bug that appears when using the lib. from 
   FORTRAN.  The name of the program carries an extra, unknown character
   in FORTRAN.  That disappears if cl_ProgName is accessed for ANYTHING!
   Hence this harmless, yet useful statement!  I expect that this will
   crack on various machines - the source of the problem has to found.
      sprintf(cl_ProgName,"%s",cl_ProgName);
**********************/
#endif
#if !defined(GNUREADLINE)
      fprintf(stderr,"%s>",cl_ProgName);
#endif
      SetPrompt(cl_ProgName);
      SetSymbTab(cl_SymbTab);
      SetCmdTab(cl_CmdTab);
      LexReSet();
      while ((i=sh_parse())); cl_Pass++;

      if (cl_FORTRAN) return GOBACK;
      else longjmp(*cl_env,1);
      /*
      return GOBACK;
      */
    }
  return 1;
}
/*------------------------------------------------------------------------
  End the CL lib.  This will start the shell if 
    - "help" symbol is not found
    - the value of "help" is != "noprompt"
  This will also remove the "help" symbol from the symbol table.
  If the value of "help" is "doc", it will output a "empty" doc file
  one the stdout and exit with value 0.
  If the value is "explain", it will print the help file and exit with
  a vaule of 0.  This is useful to get help on applications which are
  not intented to be interactive.
------------------------------------------------------------------------*/
int EndCL()
{
  char val[128]={'\0'};
  int i=1;

  cl_RegistrationMode=0;
  i = clgetSVal("help",val,&i);
  DeleteVar("help",&cl_SymbTab,&cl_TabTail);

  if (!strcmp(val,"doc")) 
    {
      if (!cl_SymbLoaded) 
	{
	  clLoadSymb();
	  loadDefaults();
	}
      //      DeleteVar("help",&cl_SymbTab,&cl_TabTail);
      doprintdoc(val);
    }
  /* If explaination is asked for, look for an argument of explain
    (within brackets).  The argument should of the syntax used by
    doexplain ([Key][:Application]).  If arg is present, extract and
    pass it to doexplain.  Else pass NULL to doexplain.  Hence, by
    default, no arg would give help about the parent application.
    With an arg, it can give help about other applications as well as
    keywords of other applications.  */
  else if (!strncmp(val,"explain",7))
    {
      char *arg=NULL;
      if ((arg=strtok(&val[7],"("))) 
	if (arg[strlen(arg)-1]==')') arg[strlen(arg)-1] = '\0';

      doexplain(arg);

      exit(0);
    }

  if ((i==FAIL) || !cl_NoPrompt)
    {
      i=startShell();

      if (cl_DOCLEANUP) clCleanUp();
      return i;
    }
  /*
  else  
    if (!cl_NoPrompt) 
      i=startShell();
      */


#ifdef GNUREADLINE
/* Put the history in the history file */
       save_hist("GHIST",HIST_DEFAULT);
#endif
  return 1;
}
/*------------------------------------------------------------------------
  Return the symbol with name Name and add it to the list of qurried
  symbol list
------------------------------------------------------------------------*/
Symbol *SearchQSymb(char *Name, char *Type)
{
  /*
    By the time this is called, cl_SymbTab has been populated
    either from the commandline (via cl_tmpTab) or from the
    loadDefaults sequence.  Hence do a querry-add on cl_SymbTab
    itself.

  Symbol **Tab, **Tail;
  
  if (cl_RegistrationMode) {Tab = &cl_tmpTab; Tail = &cl_tmpTabTail;}
  else {Tab = &cl_SymbTab; Tail = &cl_TabTail;}

  return AddQKey(Name,Type,Tab,Tail);
*/
  return AddQKey(Name,Type,&cl_SymbTab,&cl_TabTail);
}
/*------------------------------------------------------------------------
   Get the position of Name in the commandline
------------------------------------------------------------------------*/
int clTgetOpt(char *Name, char *Type)
{
  Symbol *S; int N=0;

  for (S=cl_tmpTab;S;S=S->Next)
    {
      if (!strcmp(S->Name,Name)) break;
      else N++;
    }
  if (N > 0) 
    {
      AddQKey(Name,Type,
	      &cl_tmpTab, &cl_tmpTabTail);
      return N+1;
    }
  else return FAIL;
}
#if defined(FORTRAN)
/*---------------------------------------------------------------------------*/
int fbegincl_()
{
  int n;
  finit_();       /* FORTRAN call to build the argc, argv type stuff*/
  n=ParseCmdLine(cl_fargc, cl_fargv);
  InstallSymb();
/*  fclsetupioluns_();
  clsetUpIOStreams();*/
  return n;
}
/*---------------------------------------------------------------------------*/
void makeargv_(int *arg,char *argv)
{
   int i = strlen(argv);
   cl_fargv[cl_fargc]=(char *)getmem(i+1,"aarg_");
   strncpy(cl_fargv[cl_fargc],argv,i);
   cl_fargc++;
}

void allocargv_(int *n)
{
  cl_fargv=(char **)calloc(n,sizeof(char **));
}
#endif
#ifdef GNUREADLINE
/*#include <readline/history.h>*/
/************************************************************************/
/*----------------------------------------------------------------------*/
void mkfilename(char *out,char *envvar,char *name,char *type)
{
#ifdef vms
  if(envvar && *envvar)sprintf(out,"%s:%s%s",envvar,name,type);
  else       sprintf(out,"%s%s",name,type);
#else
  char *s;
  if(envvar && *envvar){
    s = (char *)getenv(envvar);
    if(s == NULL)
      fprintf(stderr,"Unable to find environment variable %s.",envvar);
    else 
      sprintf(out,"%s/%s%s",s,name,type);
  }else sprintf(out,"%s%s",name,type);
#endif
}
/************************************************************************/
void save_hist(char *EnvVar, char *Default)
{
  char *HistFile;
  char hfile[FILENAME_MAX];
  if ((HistFile = (char *)getenv(EnvVar)) == NULL)
    HistFile = Default;
  mkfilename(hfile,"HOME",HistFile,"\0");
  write_history(hfile);
}
/************************************************************************/
void limit_hist(char *EnvVar, int Default)
{
  char *NHist;
  int n;
  
  if ((NHist = (char *)getenv(EnvVar)) == NULL)
    n = Default;
  else
    sscanf(NHist, "%d",&n);
  
  stifle_history(n);
}
/************************************************************************/
void load_hist(char *EnvVar, char *Default)
{
  char *HistFile;
  char hfile[FILENAME_MAX];
  static unsigned int Loaded=0;

  if (!Loaded)
    {
      if ((HistFile = (char *)getenv(EnvVar)) == NULL)
	HistFile = Default;
  
      mkfilename(hfile,"HOME",HistFile,"\0");
      read_history(hfile);
      Loaded = !Loaded;
    }
}
/************************************************************************/
/*
void list_hist()
{
  int i;
  register HIST_ENTRY **list = history_list();
  
  if (list)
    for (i = 0; list[i]; i++)
      fprintf (stdout, "%d: %s\n",
	       i + history_base, list[i]->line);
}
*/
#endif
#ifdef __cplusplus
}
#endif