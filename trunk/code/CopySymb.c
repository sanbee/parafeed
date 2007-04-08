/* $Id: CopySymb.c,v 2.1 1999/06/16 16:50:36 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int CopyVSymb(Symbol *t, Symbol *S,int CopyMode)
{
  unsigned int i;
  if (CopyMode & COPYNAME)
    {
      t->Name = (char *)getmem(strlen(S->Name)+1,"InstallSymb");
      strcpy(t->Name,S->Name);
    }

  if (CopyMode & COPYCLASS) t->Class = S->Class;

  if ((CopyMode & COPYTYPE) && S->Type)
    {
      if (t->Type == NULL)
	t->Type=(char *)getmem(strlen(S->Type)+1,"CopyVSymb");
      else t->Type = (char *)realloc(t->Type,strlen(S->Type)+1);
      strcpy(t->Type,S->Type);
    }
      
  if ((CopyMode & COPYVAL) && S->NVals>0)
    {
      t->NVals = S->NVals;
      t->Used = S->Used;
      if (t->Val!=NULL) 
	t->Val = (char **)realloc(t->Val,sizeof(char **)*S->NVals);
      else t->Val = (char **)calloc(1,sizeof(char **)*S->NVals);
      for (i=0;i<S->NVals;i++)
	{
	  t->Val[i] = (char *)getmem(strlen(S->Val[i])+1,"CopySymb");
	  strcpy(t->Val[i],S->Val[i]);
	}
    }
  return 1;
}
/*----------------------------------------------------------------------*/
int CopyCSymb(CmdSymbol *t, CmdSymbol *S, int CopyName)
{
  if (CopyName)
    {
      if (t->Name==NULL)
	t->Name = (char *)getmem(strlen(S->Name)+1,"InstallSymb");
      else 
	t->Name = (char *)realloc(t->Name,strlen(S->Name)+1);
      strcpy(t->Name,S->Name);
    }
  t->func = S->func;
  if (S->Doc)
    {
      if (t->Doc==NULL)
	t->Doc = (char *)getmem(strlen(S->Doc)+1,"CopyCSymb");
      else t->Doc = (char *)realloc(t->Doc,strlen(S->Doc)+1);
      strcpy(t->Doc,S->Doc);
    }
  return 1;
}
#ifdef __cplusplus
}
#endif
