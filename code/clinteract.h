/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
/* $Id: clinteract.h,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <clgetValp.cc>
#ifdef __cplusplus
extern "C" {
#endif
#if !defined(CLINTERACTIVE_H)
#define CLINTERACTIVE_H
#include <setjmp.h>
/*----------------------------------------------------------------------*/
#define clInteractive(a)    setjmp(__cl_env__);clStartInteractive(&__cl_env__,(a));
jmp_buf __cl_env__;
#endif
#ifdef __cplusplus
	   }
#endif
