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
/* $Id: clstring.h,v 2.0 1998/11/11 07:22:14 sanjay Exp $ */
#ifndef CLSTRING_H
#define CLSTRING_H
#include <stdio.h>
#ifdef	__cplusplus
extern "C" {
#endif

  char*         clstrtok(char *, const char *,char);
  int           ntok(char *, const char *,char);
  int           tokenize(FILE *,char *, const char *,char);
  vector<string> clstrtokp(const string& input, char seperator,
			 char escape);
#ifdef	__cplusplus
}
#endif
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

#endif
