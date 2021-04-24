/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* $Id: clshelldefs.h,v 1.3 1998/11/11 04:49:10 sanjay Exp $ */
#ifdef __cplusplus
#include <map>
#include <string>
#include <vector>
using namespace std;
typedef std::map<std::string, std::vector<std::string> > SMap;
typedef std::vector<std::string> VString;

extern "C" {
#endif
#if !defined(CLSHELLDEFS_H)
#define CLSHELLDEFS_H
#define getmem(a,b)  calloc(1,(a))
#ifdef __cplusplus
  inline void ClearMap(SMap &m) {m.erase(m.begin(),m.end());};
  inline void ClearKeys(VString& keys) {keys.resize(0);};
  inline void InitMap(SMap &m, VString& keys) {ClearMap(m);ClearKeys(keys);};
#endif
  
#include <clhashdefines.h>
  
  typedef struct SymbolType {
    char *Name;
    char *Type;
    unsigned short Exposed;
    unsigned short Used;
    unsigned short Class;
    unsigned int NVals;
    unsigned int Attributes;
    std::vector<std::string> Val;
#ifdef __cplusplus
    SMap smap;
    VString DefaultVal;
    VString Options;
    SymbolType(): Val(0), smap(), DefaultVal(0), Options(0) {};
#endif
    struct SymbolType *Next, *Previous;
  } Symbol;
  
  typedef struct CmdSymbolType {
    char *Name;
    char *Doc;
    struct CmdSymbolType *Next, *Previous;
    int (*func)(char *);
    int Attributes;
  } CmdSymbol;
#endif
  
#ifdef __cplusplus
}
#endif
