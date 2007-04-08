/* $Id: clgetConfigFile.c,v 2.1 1999/03/19 04:29:49 sanjay Exp sanjay $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>
#include <libgen.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int clgetConfigFile(char *FullName, char *Name)
{
  char *s=NULL;

  if (Name == NULL) return -1;
  if ((s=(char *)getenv(CONFIGENV)) == NULL) s=(char *)getenv(DOCPATH);
  if (s == NULL)  strcpy(FullName,"./");
  else strcpy(FullName,s);

  strcat(FullName,"/");

  strcat(FullName,basename(Name)); 
  return 1;
}
#ifdef __cplusplus
	   }
#endif
