/* $Id: clstring.h,v 2.0 1998/11/11 07:22:14 sanjay Exp $ */
#ifndef CLSTRING_H
#define CLSTRING_H
#include <stdio.h>
#ifdef	__cplusplus
extern "C" {
#endif

char*         clstrtok(char *, char *,char);
int           ntok(char *, char *,char);
int           tokenize(FILE *,char *, char *,char);

#ifdef	__cplusplus
}
#endif
#endif
