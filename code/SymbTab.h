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
/* $Id: SymbTab.h,v 2.2 2000/03/12 04:51:37 sanjay Exp $ */
CmdSymbol CmdTab[]={
  {"inp", "To see the various keywords and the values they have been set to",NULL,NULL,doinp},
  {"go",  "To run the application",NULL,NULL,dogo},
  {"gob", "To run the application in background",NULL,NULL,dogob},
  {"cd",  "Change working directory",NULL,NULL,docd},
  {"help","This help",NULL,NULL,dohelp},
  {"?",    "Information on the type of the keyword", NULL,NULL,dotypehelp},
  {"explain","Detailed help, optionally of keywords/task [[Key][:Task]]",NULL,NULL,doexplain},
  {"save",   "Save the values, optionally in a file",NULL,NULL,dosave},
  {"load",   "Load the values, optionally from a file",NULL,NULL,doload},
  {"edit",   "Use an editor to (un)set the values",NULL,NULL,doedit},
  {"quit",   "Quit the application",NULL,NULL,doquit},
  { NULL,     NULL,NULL,NULL,NULL}
};
