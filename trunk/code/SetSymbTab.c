/* $Id: SetSymbTab.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <shell.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void SetSymbTab(Symbol *s)   {sh_SymbTab = s;}
#ifdef __cplusplus
	   }
#endif
