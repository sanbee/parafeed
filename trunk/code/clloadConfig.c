/* $Id: clloadConfig.c,v 2.2 2000/02/16 04:20:47 sanjay Exp $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>
#define MAXBUF 128
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int clloadConfig(char *File)
{
  char str[MAXBUF]={0};
  char *Name, *Val;
  int i;
  Symbol *S;
  FILE *fd;

  if (File==NULL) return 1;
  if ((fd = fopen(File,"r"))==NULL)
    {/*fprintf(stderr,"###Error: In opening file \"%s\"\n",File);*/return -2;}

  while (fgets(str,MAXBUF,fd)!=NULL)
    {
      stripwhite(str);str[strlen(str)-1]='\0';
      if (strlen(str) > 0)
	{
	  BreakStr(str,&Name,&Val);

	  S=SearchVSymb(Name,cl_SymbTab);
	  if (S==NULL)
	    {
	      S=AddVar(Name,&cl_SymbTab,&cl_TabTail);
	      S->Class=CL_USERCLASS;
	    }
	  if (S->NVals==0)
	      SetVar(Name,Val,cl_SymbTab,1);
	  if (Name != NULL) free(Name);
	  if (Val != NULL) free(Val);
	  for(i=0;i<MAXBUF;i++) str[i]='\0';
	}
    }
  return 1;
}
#ifdef __cplusplus
	   }
#endif
