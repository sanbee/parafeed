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
#if !defined(ERROROBJ_H)
#define      ERROROBJ_H
#include <string>
#include <iostream>

using namespace std;

class ErrorObj{
 public:
  enum {Informational=100,Recoverable,Severe,Fatal};
  ErrorObj() {Id.resize(0);Msg.resize(0);Src.resize(0);Message.resize(0);};
  ErrorObj(const char *m,const char *i,int l=0);
  ~ErrorObj() {};
  
  void SetSource(const char *m=0);
  const char *Source()               {return Src.c_str();}
  int Severity()                     {return Level;}
  const char *what();

  ostream &operator<<(const char *m) {return cerr << m;}
  ostream &operator<<(ErrorObj &E)   {return cerr << E;}
  friend ostream &operator<<(ostream& o,const ErrorObj&);

 private:
  string Id,Msg,Src,Message;
  int Level;
};

ostream &operator<<(ostream& o, const ErrorObj &E);

#endif
