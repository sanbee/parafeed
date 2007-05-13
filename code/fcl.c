/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
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
/* $Id: fcl.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <cl.h>

#ifdef __cplusplus
extern "C" {
#endif
char **fargv;
int fargc=0;
int MAXARG;
/*----------------------------------------------------------------------*/
int fBeginCL_()
{
  int n;
  finit_();       /* FORTRAN call to build the argc, argv type stuff*/
  n=Parse(fargc, fargv);
/*  fclsetupioluns_();
  clsetUpIOStreams();*/
  return n;
}
/*----------------------------------------------------------------------*/
void makeargv_(int *arg,char *argv)
{
   int i = strlen(argv);
   if (fargc >= MAXARG)
     {
       fprintf(stderr,"###Fatal: makeargv_ can handle upto %d arg list\n",
	       MAXARG);
       exit(-1);
     }
   fargv[fargc]=(char *)getmem(i+1,"aarg_");
   strncpy(fargv[fargc],argv,i);
   fargc++;
}
/*----------------------------------------------------------------------*/
void AllocArgv(int *n)
{
  fargv=(char **)calloc(sizeof(char **)*n);
  MAXARG = *n;
}
#ifdef __cplusplus
}
#endif
