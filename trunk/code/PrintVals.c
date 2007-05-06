/* $Id: PrintVals.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int PrintVals(FILE *fd,Symbol *S)
{
  unsigned int i;
  unsigned int N=S->NVals;
  if (N==0) 
    {
      N = S->DefaultVal.size();
      char *str;
      if (N>0)
	{
	  for(i=0;i<N-1;i++)
	    {
	      str=(char *)S->DefaultVal[i].c_str();
	      tokenize(fd,str,",",ESC);
	      fputc(',',fd);
	    }
	  str=(char *)S->DefaultVal[N-1].c_str();
	  tokenize(fd,(char *)S->DefaultVal[N-1].c_str(),",",ESC);
	  fputc('\n',fd);
	}
      else
	fprintf(fd,"\n");
      return 1;
    }

  {
    for (i=0;i<S->NVals-1;i++)
      {tokenize(fd,S->Val[i],",",ESC);fputc(',',fd);}
    tokenize(fd,S->Val[S->NVals-1],",",ESC);
    fputc('\n',fd);
    return 1;
  }

  fprintf(fd,"\n");
  return 1;
}
#ifdef __cplusplus
	   }
#endif
