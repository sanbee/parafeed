/*
 * Copyright (c) 2000-2012, 2013 S.Bhatnagar
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
  {(char *)"inp", (char *)"To see the various keywords and the values they have been set to",NULL,NULL,doinp,CL_ARG_KEYWORD},
  {(char *)"go",  (char *)"To run the application",NULL,NULL,dogo,CL_ARG_NONE},
  {(char *)"gob", (char *)"To run the application in background",NULL,NULL,dogob,CL_ARG_NONE},
  {(char *)"cd",  (char *)"Change working directory",NULL,NULL,docd,CL_ARG_FILENAME},
  {(char *)"help",(char *)"This help",NULL,NULL,dohelp,CL_ARG_NONE},
  {(char *)"?",   (char *)"Information on all keywords, optionally on a single keyword", NULL,NULL,dotypehelp,CL_ARG_KEYWORD},
  {(char *)"explain",(char *)"Detailed help, optionally of keywords/task [[Key][:Task]]",NULL,NULL,doexplain,CL_ARG_KEYWORD},
  {(char *)"save",   (char *)"Save the values, optionally in a file",NULL,NULL,dosave,CL_ARG_FILENAME},
  {(char *)"savecmd",(char *)"Save the settings as UNIX shell command string",NULL,NULL,docmdsave,CL_ARG_FILENAME},
  {(char *)"load",   (char *)"Load the values, optionally from a file",NULL,NULL,doload,CL_ARG_FILENAME},
  {(char *)"edit",   (char *)"Use an editor to (un)set the values",NULL,NULL,doedit,CL_ARG_FILENAME},
  {(char *)"quit",   (char *)"Quit the application",NULL,NULL,doquit,CL_ARG_NONE},
  { NULL,     NULL,NULL,NULL,NULL,0}
};
