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
/* $Id: clRestartShell.c,v 2.2 1998/12/09 04:57:25 sanjay Exp sanjay $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
  Restart the shell if the symbol table still exist.
------------------------------------------------------------------------*/
void clRestartShell()
{
  cl_InteractiveShell = 1;
  if (cl_DOCLEANUP)
    /*
    if (clThrowUp("Can not restart the shell after clInteractive(1)",
		     "###Fatal",CL_FATAL)==CL_FATAL) exit(-1);
    */
    clThrowUp("Can not restart the shell after clInteractive(1)",
	      "###Fatal",CL_FATAL);
  else if (cl_NoPrompt) exit(1);
  else
    {
      cl_Pass=0;
      longjmp(*cl_env,1);
    }
}
#ifdef __cplusplus
	   }
#endif
