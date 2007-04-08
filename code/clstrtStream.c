/* $Id: clstrtStream.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <stdio.h>
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
FILE *clstrtstream_(char *ProgName, char *name, char *mode)
{
  FILE *f=NULL;
  
  if ((name != NULL))
    if ((f = fopen(name, mode))==NULL)
      {
	fprintf(stderr,
		"%s:clstrtstream>  Error opening \"%s\".  Mode=\"%s\"\n",
		ProgName,name,mode);
	exit(-1);
      }
  return f;
}
#ifdef __cplusplus
	   }
#endif
