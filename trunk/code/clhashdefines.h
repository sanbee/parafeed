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
#if !defined(CLHASHDEFINES_H)
#define CLHASHDEFINES_H

#define   CL_USERCLASS   1     // A bad name for a shell constant! ;-)
#define   CL_APPLNCLASS  2
#define   CL_DBGCLASS    3

#define   CL_KEYWORD     0     // A simple keyword
#define   CL_VALUE       2     // A value
#define   CL_HIDDENKEYWORD 4   // A keyword that is hidden by some other keyword(s)
#define   CL_HIDINGKEYWORD 8   // A keyword that may hide other keywords
#define   CL_HIDENSEEKKEYWORD 16// A keyword that can be hidden by other keyword(s) and can itself hide other keyword(s)
#define   CL_FLOATTYPE     32
#define   CL_INTEGERTYPE   64
#define   CL_STRINGTYPE    128

#define   CL_ARG_NONE 0
#define   CL_ARG_FILENAME 2
#define   CL_ARG_KEYWORD  4

#define SETBIT(V,MASK)    ((V) |= (MASK))  // Bitwise OR with the mask
#define RESETBIT(V,MASK)  ((V) &= ~(MASK)) // Bitwise AND with the complement of the mask
#define ISSET(V,MASK)     ((((V) & (MASK))==(MASK)))

#endif
