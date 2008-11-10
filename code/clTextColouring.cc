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
using namespace std;

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

  string E="[";
  char *userColourMapFile=(char *)getenv(CL_COLOURMAP);

  if (userColourMapFile==NULL)
    {
      //
      // Factory setting for colours
      //
      startSeq=E+"1;";
      
      if (ISSET(textType,CL_HIDDENKEYWORD))
	startSeq += FGColourMap["blue"];
      else if (ISSET(textType,CL_HIDINGKEYWORD))
	startSeq += FGColourMap["red"];
      else if (ISSET(textType,CL_HIDENSEEKKEYWORD))
        startSeq += FGColourMap["green"];
      else
	// While I like it "black", its safer to make it "default" if
	// others are also going to use this (have seen people with
	// black background and lighter foreground colours!)
	//
	startSeq += FGColourMap["default"];
	//	startSeq += FGColourMap["black"];
      startSeq += "m";
      endSeq = E + "0m";
    }
  else
    {
      //
      // Read the colour definitions from the user file.
      //
    }
}
