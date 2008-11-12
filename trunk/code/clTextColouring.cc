/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
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

#include <cl.h>
#include <clshelldefs.h>
#include <map>
#include <string>
#include <fstream>
using namespace std;
int split(vector<string>& v, const string& str, char c)
{
  v.clear();
  string::const_iterator s = str.begin();
  while (true) 
    {
      string::const_iterator begin = s;

      while (*s != c && s != str.end()) ++s;

      v.push_back(string(begin, s));

      if (s == str.end()) 
	break;
      
      if (++s == str.end()) 
	{
	  v.push_back("");
	  break;
	}
      return v.size();
    }
  return v.size();
}

void clTextColouring(const string& text, const unsigned int textType,string& startSeq,string& endSeq)
{
  string term(getenv("TERM"));
  if ((term != "xterm") && // The current terminal cannot display colours
      (term != "vt220")
      )
    {
      startSeq="";
      endSeq="";
      return;
    }
  map<string,string> FGColourMap, BGColourMap;
  FGColourMap["default"] = "0";
  FGColourMap["black"]   = "30";
  FGColourMap["red"]     = "31";
  FGColourMap["green"]   = "32";
  FGColourMap["yellow"]  = "33";
  FGColourMap["blue"]    = "34";
  FGColourMap["magenta"] = "35";
  FGColourMap["cyan"]    = "36";
  FGColourMap["white"]   = "37";

  BGColourMap["default"] = "0";
  BGColourMap["black"]   = "40";
  BGColourMap["red"]     = "41";
  BGColourMap["green"]   = "42";
  BGColourMap["yellow"]  = "43";
  BGColourMap["blue"]    = "44";
  BGColourMap["magenta"] = "45";
  BGColourMap["cyan"]    = "46";
  BGColourMap["white"]   = "47";

  string Esc="[";
  char *userColourMapFile=(char *)getenv(CL_COLOURMAP);
  //
  // Factory setting for colours
  //
  string FG_HiddenKeyWord="blue", FG_HidingKeyWord="red", FG_HidenSeekKeyWord="green",
    FG_Default="default";

  //
  // Read the colour definitions from the user file.
  //
  if (userColourMapFile!=NULL)
    {
      fstream fd;
      string line;
      vector<string> toks;
      fd.open(userColourMapFile);
      while (!fd.eof())
	{
	  getline(fd,line);
	  int n=split(toks,line,' ');
	}
      FG_Default="black";
    }
  //
  // Use the colour as determined by the user defined file or by
  // factory settings above.
  //
  {
    startSeq=Esc+"1;";
      
    if (ISSET(textType,CL_HIDDENKEYWORD))          
      startSeq += FGColourMap[FG_HiddenKeyWord];
    else if (ISSET(textType,CL_HIDINGKEYWORD))     
      startSeq += FGColourMap[FG_HidingKeyWord];
    else if (ISSET(textType,CL_HIDENSEEKKEYWORD))  
      startSeq += FGColourMap[FG_HidenSeekKeyWord];
    //
    // While I like it "black", its safer to make it "default" if
    // others are also going to use this (have seen people with
    // black background and lighter foreground colours!)
    //
    else                                           startSeq += FGColourMap[FG_Default];
    //                                                     startSeq += FGColourMap["black"];
    startSeq += "m";
    endSeq = Esc + "0m";
  }
}
