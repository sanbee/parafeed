/* $Id: clshelldefs.h,v 1.3 1998/11/11 04:49:10 sanjay Exp $ */
#ifdef __cplusplus
#include <map>
#include <string>
#include <vector>
typedef std::map<std::string, std::vector<std::string> > SMap;

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
    unsigned short Exposed;
    unsigned short Used;
    unsigned short Class;
    unsigned int NVals;
    char **Val;
#ifdef __cplusplus
    SMap smap;
    std::vector<std::string> DefaultVal;
#endif
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
