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
//-*-C++-*-
//
// $Id: clError.h,v 2.1 1999/09/15 12:00:51 sanjay Exp sanjay $
//
#if !defined(CLERROROBJ_H)
#define CLERROROBJ_H
#ifdef __cplusplus

/*--------------------------------------------------------------------
  The C++ object thrown in case of error.  It's derived from ErrorObj
  class, which is the baseclass for all error objects.
--------------------------------------------------------------------*/

#include <ErrorObj.h>
class clError: public ErrorObj 
{
public:
  clError() {};
  clError(const char *m, const char *i, int l=0): ErrorObj(m,i,l) {};
};

/*---------------------------------------------------------------------
  In the entire library, the following routine is used always when an
  exception is to be thrown.  

  For C++, this in turn, throw's a clError object.
---------------------------------------------------------------------*/

//extern "C" {
  //inline int clThrowUp(const char *m, const char *i, int l){throw(clError(m,i,l));}
#define clThrowUp(m, i, l) ({throw(clError((m),(i),(l)));})
//	   }
#else

/*---------------------------------------------------------------------
  For C linkage it simply prints the error message on the stderr, and
  returns the supplied error value.
---------------------------------------------------------------------*/
#include <stdio.h>
inline int clThrowUp(const char *m, const char *i, int l){fprintf(stderr,"%s: %s\n",i,m);return l;}


#endif /* __cplusplus */

#endif /* CLERROR_H */
