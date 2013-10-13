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
//-*-C++-*-
//
// $Id: clError.h,v 2.1 1999/09/15 12:00:51 sanjay Exp sanjay $
//
#if !defined(BOOLERROROBJ_H)
#define BOOLERROROBJ_H
#ifdef __cplusplus

/*--------------------------------------------------------------------
  The C++ object thrown in case of error.  It's derived from ErrorObj
  class, which is the baseclass for all error objects.
--------------------------------------------------------------------*/

#include <clError.h>
class boolError: public clError
{
public:
  boolError(): clError() {};
  boolError(const char *m, const char *i, int l=0): clError(m,i,l) {};
};
#endif /* __cplusplus */

#endif /* CLERROR_H */
