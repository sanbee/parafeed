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
/* $Id: clgetConfigFile.c,v 2.1 1999/03/19 04:29:49 sanjay Exp sanjay $ */
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <cllib.h>
#include <libgen.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
int clgetConfigFile(char *FullName, char *Name)
{
  char *s=NULL;

  if (Name == NULL) return -1;
  if ((s=(char *)getenv(CL_CONFIGENV)) == NULL) s=(char *)getenv(CL_DOCPATH);
  if (s == NULL)  strcpy(FullName,"./");
  else strcpy(FullName,s);

  strcat(FullName,"/");

  strcat(FullName,basename(Name)); 
  return 1;
}
#ifdef __cplusplus
	   }
#endif
