/* $Id: PrintVals.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int PrintVals(FILE *fd,Symbol *S)
{
  int i;
  if (S->NVals > 0)
    {
      for (i=0;i<S->NVals-1;i++)
	{tokenize(fd,S->Val[i],",",ESC);fputc(',',fd);}
      tokenize(fd,S->Val[S->NVals-1],",",ESC);
      fputc('\n',fd);
    }
  else fprintf(fd,"\n");
  return 1;
}
#ifdef __cplusplus
	   }
#endif
