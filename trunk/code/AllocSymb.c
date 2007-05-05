/* $Id: AllocSymb.c,v 2.0 1998/11/11 07:12:00 sanjay Exp $ */
#include <cllib.h>
#include <clshelldefs.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
Symbol *AllocVSymb(int n)
{
  Symbol *S;
  S = (Symbol *)getmem(sizeof(Symbol)*n,"AllocVSymb");
  S->Name = NULL;
  S->Type = NULL;
  S->Val = NULL;
  S->Exposed=1;
  S->Used = 0;
  S->Class = CL_APPLNCLASS;
  S->NVals=0;
  return S;
}

CmdSymbol *AllocCSymb(int n)
{
  CmdSymbol *S;
  S = (CmdSymbol *)getmem(sizeof(CmdSymbol)*n,"AllocCSymb");
  S->Name = NULL;
  S->Doc  = NULL;
  S->func = NULL;
  return S;
}
#ifdef __cplusplus
}
#endif
