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
#include <ErrorObj.h>

ErrorObj::ErrorObj(const ErrorObj& that)
{
  Id = that.Id; Msg=that.Msg;Level=that.Level;
};

void ErrorObj::SetSource(const char *s)
{
  if (s) {Src = s;}
}

const char* ErrorObj::what()
{
  Message=Id+": "+Msg;
  return Message.c_str();
}

ostream &operator<<(ostream& o, const ErrorObj &E)
{
  if (E.Id.size() > 0)   o << E.Id;
  if (E.Msg.size() > 0)  o <<": "<< E.Msg;

  return o;
}
