/*
 * Copyright (c) 2000-2021, 2022 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
/* $Id: SymbTab.h,v 2.2 2000/03/12 04:51:37 sanjay Exp $ */
#include <clhashdefines.h>
CmdSymbol CmdTab[]={
  {(char *)"inp", (char *)"Show the various keywords and their values.\n"
"                Use option \"-a\" to include hidden keywords",NULL,NULL,doinp,CL_ARG_KEYWORD},
  {(char *)"go",  (char *)"Run the application.  Use option \"-b\" to run in background",NULL,NULL,dogo,CL_ARG_NONE},
  //  {(char *)"gob", (char *)"Run the application in background",NULL,NULL,dogob,CL_ARG_NONE},
  {(char *)"show", (char *)"Same as the \"inp\" command",NULL,NULL,doinp,CL_ARG_KEYWORD},
  {(char *)"run",  (char *)"Same as the \"go\" command",NULL,NULL,dogo,CL_ARG_NONE},
  //  {(char *)"runb", (char *)"Same as the \"gob\" command",NULL,NULL,dogob,CL_ARG_NONE},
  {(char *)"cd",  (char *)"Change working directory",NULL,NULL,docd,CL_ARG_FILENAME},
  {(char *)"help",(char *)"This help",NULL,NULL,dohelp,CL_ARG_NONE},
  {(char *)"?",   (char *)"Information on all exposed keywords, optionally on a single keyword.\n"
"                Use option \"-a\" to include hidden keywords", NULL,NULL,dotypehelp,CL_ARG_KEYWORD},
  {(char *)"explain",(char *)"Detailed help, optionally of keywords/task [[Key][:Task]]",NULL,NULL,doexplain,CL_ARG_KEYWORD},
  {(char *)"save",   (char *)"Save the values, optionally in a given file",NULL,NULL,dosave,CL_ARG_FILENAME},
  {(char *)"savecmd",(char *)"Save the settings as UNIX shell command string, optionally in a file",NULL,NULL,docmdsave,CL_ARG_FILENAME},
  {(char *)"load",   (char *)"Load the values, optionally from a file",NULL,NULL,doload,CL_ARG_FILENAME},
  {(char *)"edit",   (char *)"Use an editor to (un)set the values",NULL,NULL,doedit,CL_ARG_FILENAME},
  {(char *)"warranty",   (char *)"Show warranty information",NULL,NULL,dowarranty,CL_ARG_NONE},
  {(char *)"quit",   (char *)"Quit the application",NULL,NULL,doquit,CL_ARG_NONE},
  { NULL,     NULL,NULL,NULL,NULL,0}
};
