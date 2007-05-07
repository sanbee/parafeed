/* $Id: SetVar.c,v 2.5 2002/04/11 20:31:09 sbhatnag Exp $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <stdlib.h>
#include <cl.h>
#ifdef __cplusplus
extern "C" {
#endif
/* #define getmem(a,b)   malloc((a))*/
/*---------------------------------------------------------------------------*/
/* Unset the value(s) of the variable indicated by the given symbol          */
/* If setFactoryDefaults==1, then load the values from S->DefaultVal list    */
/*---------------------------------------------------------------------------*/
int UnsetVar(Symbol *S, int setFactoryDefaults)
{
  unsigned int j;
  for (j=0;j<S->NVals;j++)
    {if (S->Val[j]!=NULL) free(S->Val[j]);
     S->Val[j]=NULL;}
  S->NVals=0;

  if (setFactoryDefaults)
    {
      int n=S->DefaultVal.size();
      if (n>0)
	{
	  S->Val=(char **)calloc(1,sizeof(char **)*n);
	  int m;
	  for(int i=0;i<n;i++)
	    {
	      S->Val[i]=(char *)getmem(strlen(S->DefaultVal[i].c_str())+1,"cl:UnsetVar");
	      strncpy(S->Val[i],S->DefaultVal[i].c_str(),(m=strlen(S->DefaultVal[i].c_str())));
	      S->Val[i][m]='\0';
	    }
	  S->NVals=n;
	}
    }
  return 1;
 }
/*---------------------------------------------------------------------------*/
/* Loads the keyword key and its value in the list pointed to by p           */
/*---------------------------------------------------------------------------*/
  int SetVar(char *key, char *val, Symbol *Tab,short int Force, short int fullmatch)
{
  unsigned int i,j;
  int coma=0;
  Symbol *pos;
  char *k=NULL,*v=NULL;

  /*---------------------------------------------------------------
    Search for the key in the table pointed to by Tab If not found,
    return -2 If the value is NULL, unset the found key and return
   ---------------------------------------------------------------*/
  if (fullmatch)
    pos=SearchVSymbFullMatch(key,Tab);
  else
    pos=SearchVSymb(key,Tab);
  //  if ((pos=SearchVSymb(key,Tab))==0) return -2;
  if(pos==0) return -2;
  if (!Force)
    if (pos->Class == CL_USERCLASS)
      {
	clThrowUp("Tying to set a constant, hun?  Not allowd.",
		  "###Error",FAIL);
	return -2;
	/*
	fprintf(stderr,"###Error: Trying to set a constant, hun?  Not allowed.\n");
	return -2;
	*/
      }

  if ((val==NULL) || (strlen(val) == 0)) 
    {
      return UnsetVar(pos,1);
    }

  cltruncateFromBack(val);
  /*
  i=strlen(val)-1;
  while ((val[i] == ' ') || (val[i] == '\t'))i--;  val[i+1]='\0';
  */
  /* 
     Count the number of commas. Strip of leading and trailing white
     spaces from val, copy into another buffer, take comma seperated
     tokens from the new buffer and put it in Tab.
  */

  if ((coma = ntok(val,",",ESC))==-1) return 1;

  i=0;j=0;
  while(val[j] == ' ')j++;


  k = (char *)getmem(strlen(&val[j])+1,"cl:loadParams");
  strncpy(k,&val[j],(i=strlen(&val[j])));k[i]='\0';

  v = (char *)clstrtok(k,",",ESC);

  for (i=0;i<pos->NVals;i++) free(pos->Val[i]);
  pos->Val=(char **)calloc(1,sizeof(char **)*(coma));
  pos->NVals=0;

  for (i=0;i<(unsigned int)coma;i++)
    {
      SetVal(v,pos,i);
      if ((v = (char *)clstrtok(NULL,",",ESC))==NULL) break;
    }
  free(k);
  return 1;
}
/*----------------------------------------------------------------------*/
void SetVal(char *v, Symbol *S, int i)
{
  int len;

  stripwhite(v);
  if ((unsigned int)i >= S->NVals)
    {
      S->Val = (char **)realloc(S->Val,sizeof(char **)*(i+1));
      len = S->NVals;
      do S->Val[len++]=NULL; while (len < i);
    }

  len=strlen(v);

  S->Val[i] = (char *)realloc(S->Val[i],len+1);

  if (strlen(v))
    {
      strncpy(S->Val[i],v,len+1);
      S->Val[i][len] = '\0';
      if ((unsigned int)i>=S->NVals) S->NVals++;
      S->Used=0;
    }
}
#ifdef __cplusplus
}
#endif