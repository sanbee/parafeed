/* $Id: SetCmdTab.c,v 2.0 1998/11/11 07:12:41 sanjay Exp $ */
#include <shell.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void SetCmdTab(CmdSymbol *s) {sh_CmdTab  = s;}
#ifdef __cplusplus
	   }
#endif
