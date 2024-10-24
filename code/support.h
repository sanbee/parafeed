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
#if !defined(SUPPORT_H)
#define SUPPORT_H

#define PRINTTAB(TAB) {for (Symbol* t=TAB;t;t=t->Next) {cerr << t->Name << "=";PrintVals(stderr,t,false); cerr << " " << t->Exposed << endl;}}

#include <vector>
#include <stdbool.h>
#include <regex>
#include <iosupport.h>
#ifdef __cplusplus
extern "C" {
#endif
  void setAutoIDefaults(Symbol *S, const int& val);
  void setAutoBDefaults(Symbol *S, const bool& val);
  void setAutoFDefaults(Symbol *S, const float& val);
  void setAutoSDefaults(Symbol *S, const string& val, const int fullVal=0);
  //  void setAutoSDefaults(Symbol *S, const string& val);
  void setAutoNIDefaults(Symbol *S, const vector<int>& val);
  void setAutoNBDefaults(Symbol *S, const vector<bool>& val);
  void setAutoNFDefaults(Symbol *S, const vector<float>& val);
  void setAutoNSDefaults(Symbol *S, const string& val);
  int sizeofTab(const Symbol *s);
#ifdef __cplusplus
	   }
#endif
string clMakeDefaultsFilename(int complement=1);
std::vector<std::string> stokenize(const std::string& in, const std::regex& rgx);
int showKeys(char *arg, std::function<void(FILE*, Symbol*)> printer);
void showExposedKeys(Symbol* t, const bool& showAll,
		     std::function<void(FILE*, Symbol*)> printer);
bool checkVal(Symbol* t, vector<string>& mapVal);
int exposeKeys(Symbol *t);
int loadDefaults(int complement);
#endif
