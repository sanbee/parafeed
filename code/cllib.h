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
/* $Id: cllib.h,v 2.2 2000/07/01 17:50:24 sanjay Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cl.h>
#include <setjmp.h>
#include "shell.tab.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _ABS
#undef _ABS
#endif
#define _ABS(x)         ((x) < 0 ? -(x) : (x))
 
#define POSITIVE(str,a)   {if ((a) < 0)                                \
                             {fprintf(stderr,"%s: Index is negative\n" \
					,(str));exit(-1);}}

extern int calc(char *, double *);
extern Symbol    *cl_tmpTab, *cl_tmpTabTail;
extern CmdSymbol *cl_CmdTab, *cl_CmdTail;
extern unsigned short cl_CmdLineFirst, cl_InteractiveShell;
extern unsigned short cl_DOCLEANUP, cl_SymbLoaded;
extern unsigned short cl_Pass, cl_FORTRAN, cl_NoOfOpts, cl_NoPrompt;
extern jmp_buf *cl_env;

extern Symbol *cl_SymbTab, *cl_TabHead, *cl_TabTail;
extern char *cl_ProgName, *cl_CommandLine;

#ifdef __cplusplus
	   }
#endif
