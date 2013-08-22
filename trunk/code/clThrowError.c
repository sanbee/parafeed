/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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
/* $Id: clThrowError.c,v 2.1 2001/09/11 05:59:21 sanjay Exp $ */
#include <stdio.h>
#include <cl.h>
#ifdef __cplusplus
#include <iostream>
#include <clError.h>
using namespace std;
/*----------------------------------------------------------------------*/
void clDefaultErrorHandler()
{
  char *Throes = (char *)"###Fatal: Uncaught exception of type ",
    *LastWords = (char *)"###Fatal: Exiting the application.";

  try
    {
      throw;
    }
  catch(clError& x)
    {
      x << Throes << "clError" << endl;
      x << "###Fatal: The exceptional message was:" << endl;
      x << "\t\"" <<  x.what() << "\"" << endl;
      x << LastWords << endl;
    }
  catch (...)
    {
      clError x;
      x << Throes << "UNKNOWN" << endl;
      x << LastWords << endl;
    }
  exit(-1);
}


#endif
