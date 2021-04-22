/*
 * Copyright (c) 2021 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
/* $Id$ */
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <clError.h>

using namespace std;

#ifdef __cplusplus
#ifdef _GNU_SOURCE
#include <exception>
#else
#include <exception>
#endif
extern "C" {
#endif

/*-------------------------------------------------------------------------*/
/* A C++ version of clstrtok using std::string.                            */
/* Same functionality as strtok, except that this will be sensitive to     */
/* 'escaped' characters.  The return tokens will not have the escape       */
/* character esc when it is used to 'escape' the following character.      */
/*-------------------------------------------------------------------------*/
vector<string> clstrtokp(const string& input,
			 char seperator,
			 char escape)
{
  vector<string> output;
  string token;
 
  bool inEsc = false;
  int n=input.size();
  for (int i=0;i<n;i++)
    {
      char ch=input[i];
      if (inEsc)
	{
	  inEsc = false;
        }
      else if (ch == escape)
	{
	  inEsc = true;
	  continue;
        }
      else if (ch == seperator)
	{
	  output.push_back(token);
	  token = "";
	  continue;
        }
        token += ch;
    }
  if (inEsc)
    {
      clError cl(string("###Informational: Error in the string \"" + input + "\".  Perhaps a trailing escape character?").c_str(),
		 "clstrtokp(): Fatal",
		 ErrorObj::Fatal);
      cl << string("###Error: Error in the string \"" + input + "\".  Perhaps a trailing escape character?") << endl;
      // throw();
    }
 
  output.push_back(token);
  return output;
}
#if defined(TESTBED)
int main(int argc, char **argv) {
    string sample;// = "one\\|uno||three\\\\\\\\|four\\\\\\|\\cuatro|";
 
    cin >> sample;
    cout << sample << endl;
    cout << '[';
    vector<string> tokens  = clstrtokp(sample, ',', '\\');
    for (int i=0;i<tokens.size();i++)
      {
        cout << '"' << tokens[i] << "\", ";
      }
    cout << ']' << endl;
 
    return 0;
}
#endif
#ifdef __cplusplus
}
#endif
