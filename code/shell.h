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
/* $Id: shell.h,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#if !defined(SHELL_H)
#define SHELL_H
#include <stdio.h>
#include <math.h>
#include <clshelldefs.h>
#ifdef	__cplusplus
extern "C" {
#endif


#define CL_FAIL -1      
#define CL_ESC  '\\'    /* The character used for 'escaping' chars. */
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

  Symbol       *SearchVSymb(const char * ,Symbol *);
  Symbol       *SearchVSymbFullMatch(const std::string& ,Symbol *);
Symbol       *SearchNode(const char *,Symbol *);
  CmdSymbol    *SearchCSymb(const std::string&,CmdSymbol *);
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

Symbol        *AddVar(const char *Name, Symbol **Head, Symbol **Tail);
int           AddCmd(const char *Name, char *Doc, int (*func)(char *), 
		     CmdSymbol **Head, CmdSymbol **Tail);

int           DeleteVar(const char *Name, Symbol **, Symbol **);
int           DeleteCmd(char *Name, CmdSymbol **, CmdSymbol **);
int           DeleteVNode(Symbol *, Symbol **, Symbol **);
int           DeleteCNode(CmdSymbol *, CmdSymbol **, CmdSymbol **);
int           PrintVals(FILE *f,Symbol *S,unsigned int newline);
int           SetVar(char *name, char *val, Symbol *tab,short int force, short int fullmatch, short int doinp);
void          SetVal(char *, Symbol *, int);
int           CopyVSymb(Symbol *, Symbol *, int);
int           CopyCSymb(CmdSymbol *, CmdSymbol *, int);
Symbol        *minmatch(const char *, Symbol *);
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

