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
/* $Id: cl.h,v 2.0 1998/11/11 07:12:46 sanjay Exp sanjay $ */
#if !defined(CL_H)
#define CL_H
#include <functional>
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
#define CL_UNKNOWNBOOL    -2

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
#define vecStr2Str       vecStr2Str_
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
  Symbol   *SearchQSymb(const std::string& Name, const std::string& Type);
  Symbol   *SearchVSymb(const char *Name, Symbol *Tab);
  Symbol   *AddQKey(const char *Name, const char *Type, 
		    Symbol **Head, Symbol **Tail);
  int       ParseCmdLine(int, char **);
  std::vector<std::string> clMakeArgvFromFile(const std::string& Name);

  int startShell();
  void clLoadSymb();
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
std::string       vecStr2Str(const std::vector<std::string>& src);
int       clgetFull(char  *Arg,  int   *N);
int       clgetFullVal(char  *Name,  char **Val);
int       clgetIVal(char  *Name, int   *Val, int *N);
int       dbgclgetIVal(char  *Name, int   *Val, int *N);
int       clgetFVal(char  *Name, float *Val, int *N);
int       dbgclgetFVal(char  *Name, float *Val, int *N);
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

int       clgetSVal(const char  *Name, char  *Val, int *N,SMap smap=SMap(),bool dbg=false);
inline int       dbgclgetSVal(const char  *Name, char  *Val, int *N) {return clgetSVal(Name,Val,N,SMap(),true);};

#ifdef __cplusplus
int       clTgetOptp(const std::string& Name, std::string& Type);
int       clgetOptp(const std::string& Name);
int       clgetNValsp(const std::string& Name);
int       clSetOptions(const std::string& Name, const VString& options);
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
#ifdef __cplusplus
int       clBoolCmp(const std::string&, const bool& cmpVal);
int       clIsTrue(const std::string&);
int       clIsFalse(const std::string&);
#endif
void      clReset();
void      clRetry();
FILE      *clstrtstream_(char *, char *, char *);
void      stripwhite (char *);
std::string      stripwhitep (std::string& str);
int       redirect(char *, char *);
void      yyerror(char *);
int       clgetConfigFile(char *, char *);
int       AddCmd(const char *Name, char *Doc, int (*func)(char *), 
		 CmdSymbol **Head, CmdSymbol **Tail);
int       BreakStr(char *, char **, char **);
int       BreakStrp(const std::string& str, std::string& Name, std::string& Val,
		    const std::string& sep="=");
std::string ProgName();
int       dogo(char *);
int       dogob(char *);
int       docd(char *);
#ifdef __cplusplus
int       doinp(char *arg=NULL);
#endif
int       doquit(char *);
int       doedit(char *);
int       dohelp(char *);
int       doexplain(char *);
int       dosave(char *);
int       dosavefd(FILE *,const std::vector<std::string>& opts={});
int       docmdsave(char *);
int       doload(char *);
int       doload_and_register(char *,const bool doregister=true);
int       dotypehelp(char *);
int       doademo(char *);
int       doprintdoc(const char *);
int       doprintparams(const char *);
#ifdef __cplusplus
  int       docopyright(const std::string& arg);
#endif
int       dowarranty(char *);
// #ifdef __cplusplus
// int       loadDefaults(int complement=1);
// #endif
  //int       clparseVal(Symbol *, int *, double *);
#ifdef __cplusplus
void      reportParseError(clError& excp,const Symbol& S, const int& N);
int       PrintVals(FILE *f,Symbol *S,unsigned int newline=1);
#endif
int       PrintKey(FILE *f, Symbol *S);
#ifdef __cplusplus
void      printMap(SMap& smap);
#endif
int       namePrintFormat(std::string& format, const std::string& append,
			  const std::string& qualifier="");
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
int  SetVar(char *Name, char *val, Symbol *tab,short int force, short int fullmatch, short int doinp);
  //void SetVal(const char *, Symbol *, int);
#ifdef __cplusplus
  void VerifyVal(const char *, Symbol *,std::string &,
		 std::function<bool(const std::string&, const Symbol& S )> matchOptsLambda,
		 std::function<void(const Symbol&)> noMatchExceptionLambda);
#endif
int  CopyVSymb(Symbol *, Symbol *,int);
int  FreeVSymb(Symbol *);
int  FreeCSymb(CmdSymbol *);
int  DeleteCmd(char *, CmdSymbol**, CmdSymbol**);
void clSaneExit(int);
int exposeKeys(Symbol *t);
  //int  clThrowUp(const char *, const char *,int);

void clDefaultErrorHandler();
void clSigHandler(int);
void      clSetPrompt(const bool& prompt);
#ifdef	__cplusplus
}
#endif
#ifdef __cplusplus
Symbol   *SearchVSymb(const char *Name);

int  clgetBaseCore(const std::string& Name, int& Val, int& N, SMap &smap);
int  clgetFullValp(const std::string& Name, std::string& Val);
int  dbgclgetFullValp(const std::string& Name, std::string& Val);
int  clgetFullp(const std::string& Arg,  int &N);
int  clgetSValp(const std::string& Name, std::string& Val, int& N, SMap &smap,bool dbg);
//
//------------------------------------------------------------------------------------------------
// Wrappers for clgetValp(...,T& val,...) and clgetValp(...,vector<T>&,...)
//
int  clgetIValp(const std::string& Name, int& Val, int& N);
int  clgetFValp(const std::string& Name, float& Val, int& N);
int  clgetBValp(const std::string& Name, bool& val, int& N);
int  clgetSValp(const std::string& Name, std::string& Val, int& N);
//------------------------------------------------------------------------------------------------
int  clgetNIValp(const std::string& Key,  std::vector<int>& Val, int& m);
int  clgetNFValp(const std::string& Name, std::vector<float>& Val, int& N);
int  clgetNBValp(const std::string& Name, std::vector<bool>& val, int& N);
int  clgetNSValp(const std::string& Name, std::vector<std::string>& Val, int& N);
//
//------------------------------------------------------------------------------------------------
// Wrappers for clgetValp(...,T&,..., SMap&) and clgetValp(...,vector<T>&,...,SMap&)
//
int  clgetIValp(const std::string& Name, int& Val, int& N, SMap &smap);
int  clgetFValp(const std::string& Name, float& Val, int& N, SMap &smap);
int  clgetBValp(const std::string& Name, bool& Val, int& N, SMap &smap);
int  clgetSValp(const std::string& Name, std::string& Val, int& N, SMap &smap);
//------------------------------------------------------------------------------------------------
int  clgetNIValp(const std::string& Name, std::vector<int>& Val, int& N, SMap &smap);
int  clgetNFValp(const std::string& Name, std::vector<float>& Val, int& N, SMap &smap);
int  clgetNBValp(const std::string& Name, std::vector<bool>& Val, int& N, SMap &smap);
int  clgetNSValp(const std::string& Name, std::vector<std::string>& Val, int& N, SMap& smap);
//
//------------------------------------------------------------------------------------------------
// Wrappers for clgetValp(...,T&,...,bool dbg=true) and clgetValp(...,vector<T>&,...,bool dbg=true)
//
int  dbgclgetIValp(const std::string& Name, int& Val, int& N);
int  dbgclgetFValp(const std::string& Name, float& Val, int& N);
int  dbgclgetBValp(const std::string& Name, bool& val, int& N);
int  dbgclgetSValp(const std::string& Name, std::string& Val, int& N);
//------------------------------------------------------------------------------------------------
int  dbgclgetNIValp(const std::string& Key,  std::vector<int>& Val, int& m);
int  dbgclgetNFValp(const std::string& Name, std::vector<float>& Val, int& N);
int  dbgclgetNBValp(const std::string& Name, std::vector<bool>& val, int& N);
int  dbgclgetNSValp(const std::string& Name, std::vector<std::string>& Val, int& N);
//
//------------------------------------------------------------------------------------------------
// Wrappers for clgetValp(...,T&,...,SMap&, bool dbg=true) and clgetValp(...,vector<T>&,...,SMap&, bool dbg=true)
//
int  dbgclgetIValp(const std::string& Name, int& Val, int& N, SMap &smap);
int  dbgclgetFValp(const std::string& Name, float& Val, int& N, SMap &smap);
int  dbgclgetBValp(const std::string& Name, bool& val, int& N, SMap &smap);
int  dbgclgetSValp(const std::string& Name, std::string& Val, int& N, SMap &smap);
//------------------------------------------------------------------------------------------------
int  dbgclgetNIValp(const std::string& Key,  std::vector<int>& Val, int& N, SMap &smap);
int  dbgclgetNFValp(const std::string& Name, std::vector<float>& Val, int& N, SMap &smap);
int  dbgclgetNBValp(const std::string& Name, std::vector<bool>& val, int& N, SMap &smap);
int  dbgclgetNSValp(const std::string& Name, std::vector<std::string>& Val, int& N, SMap &smap);
//
//------------------------------------------------------------------------------------------------
// Wrappers to translate dbgclget*() to cldbgget*()
//
int  cldbggetIValp(const std::string& Name, int& val, int& n);
int  cldbggetFValp(const std::string& Name, float& val, int& n);
int  cldbggetBValp(const std::string& Name, bool& val, int& n);
int  cldbggetSValp(const std::string& Name, std::string& val, int& n);

int  cldbggetIValp(const std::string& Name, int& val, int& n, SMap& smap);
int  cldbggetFValp(const std::string& Name, float& val, int& n, SMap& smap);
int  cldbggetBValp(const std::string& Name, bool& val, int& n, SMap& smap);
int  cldbggetSValp(const std::string& Name, std::string& val, int& n, SMap& smap);

int  cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m);
int  cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m);
int  cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m);
int  cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m);

int  cldbggetNIValp(const std::string& Key, std::vector<int>& val, int& m, SMap& smap);
int  cldbggetNFValp(const std::string& Key, std::vector<float>& val, int& m, SMap& smap);
int  cldbggetNBValp(const std::string& Key, std::vector<bool>& val, int& m, SMap& smap);
int  cldbggetNSValp(const std::string& Key, std::vector<std::string>& val, int& m, SMap& smap);


void clTextColouring(const std::string& text, const unsigned int textType,
		     std::string& startSeq, string& endSeq);

extern std::function<int (char *, size_t)> cl_shell_input_g;
void set_shell_input(std::function<int (char *, size_t)> inp_func);
std::function<int (char *, size_t)> get_shell_input();
#endif
#if !defined(FORTRAN)
#include <clconvert.h>
#endif
#endif
