/* $Id: SetPrompt.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <shell.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void SetPrompt(char *p)      {sh_Prompt  = p;}
#ifdef __cplusplus
	   }
#endif
