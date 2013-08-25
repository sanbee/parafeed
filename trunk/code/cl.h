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
/* $Id: cl.h,v 2.0 1998/11/11 07:12:46 sanjay Exp sanjay $ */
#if !defined(CL_H)
#define CL_H

/*
  Names of the environment variables and other default values used for
  user customizaion.
*/
#define CL_HIST_DEFAULT   ".g_hist"
#define CL_DOCPATH        "GDOC"
#define CL_EDITORENV      "EDITOR"
#define CL_HOMEENV        "HOME"
#define CL_STDOUTENV      "GOUT"
#define CL_STDERRENV      "GERR"
#define CL_DEFAULTSENV    "GDEFAULTS"
#define CL_CONFIGENV      "GCONF"
#define CL_COLOURMAP      "GCOLOURMAP"
#define CL_HIST_LIMIT     100
#define CL_FAIL           -1

#include <clshelldefs.h>
/* #include <clinteract.h> */
#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <clError.h>
#include <string.h>
#include <vector>
#define HANDLE_EXCEPTIONS(str)  try         \
                                  {         \
                                    str     \
                                  }         \
                                 catch(...) \
                                  {         \
                                    throw;  \
                                  }
                                

#else
#define HANDLE_EXCEPTIONS(str)  str
#endif

#ifdef	__cplusplus
extern "C" {
#endif


#define PROCEED      1
#define GOBACK       2
#define CL_INFORMATIONAL -30
#define CL_SEVERE    -20
#define CL_FATAL     -10
#define CL_WARNING   -5
  /* 
     Define readline stuff as readline.h is
     NOT ANSI C 
  */
#ifdef __cplusplus
/*
extern int write_history(char *);
extern int read_history(char *);
extern int stifle_history(int);
extern char *readline(char *);
extern int add_history(char *);
*/
#ifdef GNUREADLINE
#include <readline/history.h>
#endif
#endif
  /*
    If the lib. is compiled for use in FORTRAN, the function names
    should all be lower case with a "_" appended to it.
  */
#ifdef FORTRAN
#define BeginCL          begincl_
#define EndCL            endcl_
#define clCmdLineFirst   clcmdlinefirst_
#define clgetOpt         clgetopt_
#define clgetNOpts       clgetnopts_
#define clgetNVals       clgetnvals_
#define clgetFull        clgetfull_
#define clgetFullVal     clgetfullval_
#define clgetIVal        clgetival_
#define clgetFVal        clgetfval_
#define clgetSVal        clgetsval_
#define clgetNIVal       clgetnival_
#define clgetNFVal       clgetnfval_
#define clgetNSVal       clgetnsval_
#define dbgclgetIVal     dbgclgetival_
#define dbgclgetFVal     dbgclgetfval_
#define dbgclgetSVal     dbgclgetsval_
#define dbgclgetNIVal    dbgclgetnival_
#define dbgclgetNFVal    dbgclgetnfval_
#define dbgclgetNSVal    dbgclgetnsval_
#define clRestartShell   clrestartshell_
#define clfInteractive   clfinteractive_
#define clgetCommandLine clgetcommandline_
#define clstrtStream     clstrtstream_
#define clgetConfigFile  clgetconfigfile_
#endif
  /*
    CL package's internal stuff
  */
Symbol   *IntallSymb(char *, char *, Symbol *);
Symbol   *SearchQSymb(const char *Name, char *Type);
Symbol   *SearchVSymb(const char *Name, Symbol *Tab);
Symbol   *AddQKey(const char *Name, char *Type, 
		  Symbol **Head, Symbol **Tail);
int       ParseCmdLine(int, char **);
  /*
    Programmer's interface to CL package
  */
int       BeginCL(int argc, char **argv);
int       EndCL();
int       clgetNOpts();
void      clCmdLineFirst();
int       clTgetOpt(char *Name, char *Type);
int       clgetOpt(char *Name);
int       clgetNVals(char *Name);
int       clgetFull(char  *Arg,  int   *N);
int       clgetFullVal(char  *Name,  char **Val);
int       clgetIVal(char  *Name, int   *Val, int *N);
int       dbgclgetIVal(char  *Name, int   *Val, int *N);
int       clgetFVal(char  *Name, float *Val, int *N);
int       dbgclgetFVal(char  *Name, float *Val, int *N);
int       clgetSVal(const char  *Name, char  *Val, int *N);
int       dbgclgetSVal(const char  *Name, char  *Val, int *N);
int       clgetNIVal(char *Key,  int   *Val, int *m);
int       dbgclgetNIVal(char *Key,  int   *Val, int *m);
int       clgetNFVal(char *Name, float *Val, int *N);
int       dbgclgetNFVal(char *Name, float *Val, int *N);
int       clgetNSVal(char *Name, char **Val, int *N);
int       dbgclgetNSVal(char *Name, char **Val, int *N);
int       clgetBVal(char  *Name, bool   *Val, int *N);
int       dbgclgetBVal(char  *Name, bool   *Val, int *N);
int       clgetNBVal(char  *Name, bool   *Val, int *N);
int       dbgclgetNBVal(char  *Name, bool   *Val, int *N);

#ifdef __cplusplus
int       clTgetOptp(const string& Name, string& Type);
int       clgetOptp(const string& Name);
int       clgetNValsp(const string& Name);
int       clSetOptions(const string& Name, const VString& options);
#endif

void      clRestartShell();
void      clStartInteractive(jmp_buf *, int);
int       clfInteractive(int *);
void      cltruncateFromBack(char *);
void      clloadBuf(char **target, char *source, char *def);
char      *clgetCommandLine();
char      *clgetInputFile();
char      *clgetOutputFile();
int       clgetOptsList(char ***);
int       clclearOptsList(char ***,int);
int       clloadConfig(char *);
void      clCleanUp();
bool      clBoolCmp(const string&, const bool& cmpVal);
bool      clIsTrue(const string&);
bool      clIsFalse(const string&);
void      clReset();
void      clRetry();
FILE      *clstrtstream_(char *, char *, char *);
void      stripwhite (char *);
int       redirect(char *, char *);
void      yyerror(char *);
int       clgetConfigFile(char *, char *);
int       AddCmd(const char *Name, char *Doc, int (*func)(char *), 
		 CmdSymbol **Head, CmdSymbol **Tail);
int       BreakStr(char *, char **, char **);
int       dogo(char *);
int       dogob(char *);
int       docd(char *);
int       doinp(char *arg=NULL);
int       doquit(char *);
int       doedit(char *);
int       dohelp(char *);
int       doexplain(char *);
int       dosave(char *);
int       docmdsave(char *);
int       doload(char *);
int       dotypehelp(char *);
int       doademo(char *);
int       doprintdoc(char *);
int       loadDefaults(int complement=1);
int       clparseVal(Symbol *, int *, double *);
int       PrintVals(FILE *f,Symbol *S,unsigned int newline=1);
int       PrintKey(FILE *f, Symbol *S);
void      printMap(SMap& smap);
int       namePrintFormat(char *format, char *append);
int       clIsInteractive();
  /*
void mkfilename(char *out,char *envvar,char *name,char *type);
void save_hist(char *EnvVar, char *Default);
void limit_hist(char *EnvVar, int Default);
void load_hist(char *EnvVar, char *Default);
  */
int  InstallSymb();
int  sh_parse();
int  UnsetVar(Symbol *,int);
int  SetVar(char *Name, char *val, Symbol *tab,short int force, short int fullmatch=0, short int doinp=1);
void SetVal(char *, Symbol *, int);
void VerifyVal(char *, Symbol *,string &);
int  CopyVSymb(Symbol *, Symbol *,int);
int  FreeVSymb(Symbol *);
int  FreeCSymb(CmdSymbol *);
int  DeleteCmd(char *, CmdSymbol**, CmdSymbol**);
void clSaneExit(int);
int exposeKeys(Symbol *t);
  //int  clThrowUp(const char *, const char *,int);

void clDefaultErrorHandler();
void clSigHandler(int);
#ifdef	__cplusplus
}
#endif
#ifdef __cplusplus
int       clgetFullp(const string& Arg,  int &N);
int       clgetFullValp(const string& Name, string& Val);
int       dbgclgetFullValp(const string& Name, string& Val);

int       clgetIValp(const string& Name, int& Val, int& N);
int       dbgclgetIValp(const string& Name, int& Val, int& N);
int       clgetNIValp(const string& Key,  vector<int>& Val, int& m);
int       dbgclgetNIValp(const string& Key,  int& Val, int& m);

int       clgetBValp(const string& Name, bool& val, int& N);
int       dbgclgetBValp(const string& Name, bool& val, int& N);
int       clgetNBValp(const string& Name, bool& val, int& N);
int       dbgclgetNBValp(const string& Name, bool& val, int& N);

int       clgetFValp(const string& Name, float& Val, int& N);
int       dbgclgetFValp(const string& Name, float& Val, int& N);
int       clgetNFValp(const string& Name, vector<float>& Val, int& N);
int       dbgclgetNFValp(const string& Name, float& Val, int& N);

int       clgetSValp(const string& Name, string& Val, int& N);
int       dbgclgetSValp(const string& Name, string& Val, int& N);
int       clgetNSValp(const string& Name, vector<string>& Val, int& N);
int       dbgclgetNSValp(const string& Name, vector<string>& Val, int& N);
int       clgetSValp(const string& Name, string& Val, int& N, SMap &smap);
int       clgetBValp(const string& Name, bool& Val, int& N, SMap &smap);
void      clTextColouring(const string& text, const unsigned int textType, string& startSeq, string& endSeq);
#endif
#if !defined(FORTRAN)
#include <clconvert.h>
#endif
#endif

