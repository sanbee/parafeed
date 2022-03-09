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
/* $Id: AddCmd.c,v 2.1 1999/08/13 12:18:30 sanjay Exp $ */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <shell.h>
#include <shell.tab.h>

#ifdef __cplusplus
extern "C" {
#endif
  vector<string> clMakeArgvFromFile(const string& Name)
  {
    cerr << "From clMakeArgvFromFile " << Name << endl;
    vector<string> argv;
    ifstream defFile;
    defFile.open(Name.c_str());

    string line;
    while(getline(defFile,line))
      {
	string tt;
	for(auto c : line) if ((c != ' ') && (c != '\t')) tt.push_back(c);
	argv.push_back(tt);
      }
    return argv;
  }
#ifdef __cplusplus
	   }
#endif
