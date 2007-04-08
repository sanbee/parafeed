/* $Id: DeleteTab.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <stdio.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int DeleteVTab(Symbol **Head, Symbol **Tail)
{
  while ((*Head)) DeleteVNode(*Head,Head,Tail);
  return 1;
}
/*----------------------------------------------------------------------*/
int DeleteCTab(CmdSymbol **Head, CmdSymbol **Tail)
{
  while ((*Head)) DeleteCNode(*Head,Head,Tail);
  return 1;
}

#ifdef __cplusplus
}
#endif
