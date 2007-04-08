/* $Id: shell.h,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#if !defined(SHELL_H)
#define SHELL_H
#include <stdio.h>
#include <math.h>
#include <clshelldefs.h>
#ifdef	__cplusplus
extern "C" {
#endif


#define FAIL -1      
#define ESC  '\\'    /* The character used for 'escaping' chars. */
                     /* in a string                              */
/* Bits to determine what needs to be copied in Copy[V,C]Symb routines. */
#define  COPYNAME        1
#define  COPYCLASS       2
#define  COPYVAL         4
#define  COPYTYPE        8
extern char  *sh_Prompt;
extern Symbol *sh_SymbTab;
extern CmdSymbol *sh_CmdTab;

void          SetSymbTab(Symbol *s);
void          SetCmdTab(CmdSymbol *s);
void          SetPrompt(char *p);

Symbol       *SearchVSymb(char *,Symbol *);
Symbol       *SearchVSymbFullMatch(char *,Symbol *);
Symbol       *SearchNode(char *,Symbol *);
CmdSymbol    *SearchCSymb(char *,CmdSymbol *);
Symbol       *AllocSymb(int);
Symbol       *AllocVSymb(int);
CmdSymbol    *AllocCSymb(int);

void          TransferTab(Symbol **Target, Symbol **TTail, 
			  Symbol *Source,int AddVar);
int           FreeVSymb(Symbol *);
int           FreeCSymb(CmdSymbol *);
int           DeleteVTab(Symbol **Head, Symbol **Tail);
int           DeleteCTab(CmdSymbol **Head, CmdSymbol **Tail);
int           AddVNode(Symbol *, Symbol **, Symbol **);
int           AddCNode(CmdSymbol *, CmdSymbol **, CmdSymbol **);

Symbol        *AddVar(char *Name, Symbol **Head, Symbol **Tail);
int           AddCmd(char *Name, char *Doc, int (*func)(char *), 
		     CmdSymbol **Head, CmdSymbol **Tail);

int           DeleteVar(char *Name, Symbol **, Symbol **);
int           DeleteCmd(char *Name, CmdSymbol **, CmdSymbol **);
int           DeleteVNode(Symbol *, Symbol **, Symbol **);
int           DeleteCNode(CmdSymbol *, CmdSymbol **, CmdSymbol **);
int           PrintVals(FILE *,Symbol *);
  int           SetVar(char *name, char *val, Symbol *tab,short int force, short int fullmatch);
void          SetVal(char *, Symbol *, int);
int           CopyVSymb(Symbol *, Symbol *, int);
int           CopyCSymb(CmdSymbol *, CmdSymbol *, int);
Symbol        *minmatch(char *, Symbol *);
void          yyerror(char *x);
void          LexReSet();
void          stripwhite (char *);
#include <clstring.h>
int           redirect(char *, char *);
int           sh_lex();
#ifdef	__cplusplus
}
#endif

#endif

