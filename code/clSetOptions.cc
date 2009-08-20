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
/* $Id: clgetSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#include <string>
#include <support.h>
#include <clshelldefs.h>
#include <clGlobals.h>
#ifdef __cplusplus
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
  int clSetOptions(const string& Name, const VString& options)
  {
    Symbol *S;
  
    if (cl_RegistrationMode)
      {
	S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);

	if (S!=NULL) S->Options = options;
      }
    return 1;
  }
#endif
