/* $Id: yyerror.c,v 2.0 1998/11/11 07:13:14 sanjay Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
void sh_error(char *s)
{
  fprintf(stderr,"###Error: %s\n",s);
}
#ifdef __cplusplus
	   }
#endif
