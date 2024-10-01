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
/* $Id: support.c,v 2.3 1999/01/25 04:03:33 sanjay Exp sanjay $ */
#include <iosupport.h>
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vs)
{
  for(auto x : vs) os << x << ",";
  return os;
}
template std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs);
template std::ostream& operator<<(std::ostream& os, const std::vector<int>& vs);
template std::ostream& operator<<(std::ostream& os, const std::vector<float>& vs);
template std::ostream& operator<<(std::ostream& os, const std::vector<bool>& vs);

// It will be nice to make the above template code and instantiations work.

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs)
{
  for(auto x : vs) os << x << ",";
  return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<float>& vs)
{
  for(auto x : vs) os << x << ",";
  return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vs)
{
  for(auto x : vs) os << x << ",";
  return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<bool>& vs)
{
  for(auto x : vs) os << x << ",";
  return os;
}

