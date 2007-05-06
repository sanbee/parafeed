/* $Id: cl.h,v 2.0 1998/11/11 07:12:46 sanjay Exp sanjay $ */
#if !defined(CL_H)
#define CL_H

/*
  Names of the environment variables used for user customizaion.
  */
#define HIST_DEFAULT   ".g_hist"
#define DOCPATH        "GDOC"
#define EDITORENV      "EDITOR"
#define HOMEENV        "HOME"
#define STDOUTENV      "GOUT"
#define STDERRENV      "GERR"
#define DEFAULTSENV    "GDEFAULTS"
#define CONFIGENV      "GCONF"
#define HIST_LIMIT     100
#define FAIL           -1

#include <clshelldefs.h>
/* #include <clinteract.h> */
#include <setjmp.h>
#include <stdio.h>
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
Symbol   *SearchQSymb(char *Name, char *Type);
Symbol   *SearchVSymb(char *Name, Symbol *Tab);
Symbol   *AddQKey(char *Name, char *Type, 
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
int       clgetSVal(char  *Name, char  *Val, int *N);
int       dbgclgetSVal(char  *Name, char  *Val, int *N);
int       clgetNIVal(char *Key,  int   *Val, int *m);
int       dbgclgetNIVal(char *Key,  int   *Val, int *m);
int       clgetNFVal(char *Name, float *Val, int *N);
int       dbgclgetNFVal(char *Name, float *Val, int *N);
int       clgetNSVal(char *Name, char **Val, int *N);
int       dbgclgetNSVal(char *Name, char **Val, int *N);

#ifdef __cplusplus
int       clTgetOptp(const string& Name, string& Type);
int       clgetOptp(const string& Name);
int       clgetNValsp(const string& Name);
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
void      clReset();
void      clRetry();
FILE      *clstrtstream_(char *, char *, char *);
void      stripwhite (char *);
int       redirect(char *, char *);
void      yyerror(char *);
int       clgetConfigFile(char *, char *);
int       AddCmd(char *Name, char *Doc, int (*func)(char *), 
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
int       doload(char *);
int       dotypehelp(char *);
int       doademo(char *);
int       doprintdoc(char *);
int       loadDefaults();
int       clparseVal(Symbol *, int *, double *);
int       PrintVals(FILE *,Symbol *);
int       clIsInteractive();

void mkfilename(char *out,char *envvar,char *name,char *type);
void save_hist(char *EnvVar, char *Default);
void limit_hist(char *EnvVar, int Default);
void load_hist(char *EnvVar, char *Default);
int  InstallSymb();
int  sh_parse();
int  UnsetVar(Symbol *,int);
int  SetVar(char *Name, char *val, Symbol *tab,short int force, short int fullmatch=0);
void SetVal(char *, Symbol *, int);
int  CopyVSymb(Symbol *, Symbol *,int);
int  FreeVSymb(Symbol *);
int  FreeCSymb(CmdSymbol *);
int  DeleteCmd(char *, CmdSymbol**, CmdSymbol**);
void clSaneExit(int);
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

int       clgetFValp(const string& Name, float& Val, int& N);
int       dbgclgetFValp(const string& Name, float& Val, int& N);
int       clgetNFValp(const string& Name, vector<float>& Val, int& N);
int       dbgclgetNFValp(const string& Name, float& Val, int& N);

int       clgetSValp(const string& Name, string& Val, int& N);
int       dbgclgetSValp(const string& Name, string& Val, int& N);
int       clgetNSValp(const string& Name, vector<string>& Val, int& N);
int       dbgclgetNSValp(const string& Name, vector<string>& Val, int& N);
int       clgetSValp(const string& Name, string& Val, int& N, SMap &smap);

#endif
#if !defined(FORTRAN)
#include <clconvert.h>
#endif
#endif

