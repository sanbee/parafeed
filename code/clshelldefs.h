/* $Id: clshelldefs.h,v 1.3 1998/11/11 04:49:10 sanjay Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#if !defined(CLSHELLDEFS_H)
#define CLSHELLDEFS_H
#define getmem(a,b)  calloc(1,(a))

#define   CL_USERCLASS   1
#define   CL_APPLNCLASS  2
#define   CL_DBGCLASS    3

typedef struct SymbolType {
  char *Name;
  char *Type;
  unsigned short Used;
  unsigned short Class;
  unsigned int NVals;
  char **Val;
  struct SymbolType *Next, *Previous;
} Symbol;

typedef struct CmdSymbolType {
  char *Name;
  char *Doc;
  struct CmdSymbolType *Next, *Previous;
  int (*func)(char *);
} CmdSymbol;
#endif

#ifdef __cplusplus
	   }
#endif
