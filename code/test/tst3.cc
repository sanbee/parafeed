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
/* $Id: tst3.c,v 2.0 1998/11/11 07:13:05 sanjay Exp $ */
// #include <cl.h>
// #include <clsh.h>
// #include <clinteract.h>
// #include <stdio.h>
// #include <shell.h>
// #include <rl_interface.h>

/*
  Test program to test the embedded shell via the commandline library
  */
void UI();

int main(int argc, char **argv)
{
    char val[9];
    int i=-1;
    
//   BeginCL(argc,argv);

//     cl_RegistrationMode=0;
//     if ((i = clgetSVal("help",val,&i))!=CL_FAIL)
//       DeleteVar("help",&cl_SymbTab,&cl_TabTail);
    
    
//     if (!strcmp(val,"doc")) 
//       {
// 	if (!cl_SymbLoaded) clLoadSymb();
// 	DeleteVar("help",&cl_SymbTab,&cl_TabTail);
// 	doprintdoc(val);
//       }
//     else if (!strcmp(val,"explain"))
//       {
// 	doexplain(NULL);
// 	exit(0);
//       }
    
//     if (i==CL_FAIL)  startShell();
//     else  if (!cl_NoPrompt) startShell();
    
//     if (cl_DOCLEANUP) clCleanUp();
    
// #ifdef GNUREADLINE
//     /* Put the history in the history file */
//     //    save_hist("GHIST",CL_HIST_DEFAULT);
// #endif
    return 0;  
}

void UI()
{
}
