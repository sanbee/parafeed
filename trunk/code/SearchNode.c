/* $Id: SearchNode.c,v 2.0 1998/11/11 07:12:41 sanjay Exp sanjay $ */
#include <shell.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
Symbol *SearchNode(char *Name, Symbol* Head)
{
  Symbol *S;
  for (S=Head;S;S=S->Next)
    if (!strcmp(S->Name,Name))
      return S;
  return NULL;
}
#ifdef __cplusplus
	   }
#endif
