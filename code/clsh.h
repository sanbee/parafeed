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
/* $Id: clsh.h,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#ifndef CLSHL_H
#define CLSHL_H
#include <cllib.h>

#ifdef	__cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
#define clDeleteCmd(a)   DeleteCmd((a),&cl_CmdTab,&cl_CmdTail);
#define clAddCmd(a,d,f)    AddCmd((a),(d),(f),&cl_CmdTab,&cl_CmdTail);
#ifdef	__cplusplus
}
#endif

#endif
