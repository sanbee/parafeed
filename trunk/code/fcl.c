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
