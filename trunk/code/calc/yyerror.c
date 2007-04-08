/* $Id$ */
#include <stdio.h>
/*----------------------------------------------------------------------*/
int yyerror(char *s)
{
  /*  ywarn(s, (char *) 0);*/
  return 1;
}
/*----------------------------------------------------------------------*/
int ywarn(char *s, char *t)
{
  int i,N=0;
  char *str = "###Error:";
  N += strlen(str)+strlen(s)+1;
  fprintf(stderr, "%s %s", str,s);
  if (t) 
    {
      N += strlen(t)+1;
      fprintf(stderr, " %s", t);
    }
  N += 4;
  fprintf(stderr," in ");
  fprintf(stderr, "%s",Calc_line);
  N = N + Calc_index - yyleng;
  for (i=0;i<N; i++) fprintf(stderr," ");
  fprintf(stderr,"^\n");
  return 1;
}
