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
/* $Id$ */

#include <cl.h>
#include <clError.h>
#include <clshelldefs.h>
#include <map>
#include <string>
#include <fstream>
using namespace std;
extern int clBreakStr(const string& str, string& Name, string& val);

void clTextColouring(const string& text, const unsigned int textType,string& startSeq,string& endSeq)
{
  string term(getenv("TERM"));
  if ((term.find("xterm") != std::string::npos) && // The current terminal cannot display colours
      (term.find("vt200") != std::string::npos)
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
  try
    {
      if (userColourMapFile!=NULL)
	{
	  fstream fd;
	  string line;
	  string name,val;
	  fd.open(userColourMapFile);
	  while (!fd.eof())
	    {
	      string mesg;
	      getline(fd,line);
	      clBreakStr(line,name,val);

	      if (FGColourMap.find(val)==FGColourMap.end())
		{
		  mesg = "User supplied colour " + val + " for " + name + " not supported.";
		  clThrowUp(mesg.c_str(),"###Informational ",CL_INFORMATIONAL);
		}
	      if (name == "defaultfg") FG_Default=val;
	      else if (name == "hiddenseekfg") FG_HidenSeekKeyWord=val;
	      else if (name == "hiddenfg") FG_HiddenKeyWord=val;
	      else if (name == "hidingfg") FG_HidingKeyWord=val;
	    }
	}
    }
  catch (clError& x)
    {
      x << x << endl;
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
    else
      startSeq += FGColourMap[FG_Default];

    startSeq += "m";
    endSeq = Esc + "0m";
  }
}
