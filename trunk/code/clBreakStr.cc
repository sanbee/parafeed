#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cl.h>
//------------------------------------------------------------------
//    Break a string of the type <Name>=<Value> into Name and Value.
//------------------------------------------------------------------
int clBreakStr(const string& str, string& Name, string& val)
{
  if (str.length() == 0) return 0;
  char *t,*off;
  char *cstr=(char *)str.c_str();

  Name="";
  val="";
  int n;
  n = str.find_first_of('=',0);
  string valstr=str.substr(n);
  off=(char *)valstr.c_str();
  off++;
  if ((t=strtok(cstr,"="))!=NULL)
    {
      stripwhite(t);
      Name=t;
    }

  
  if (off)
    {
      stripwhite(off);
      val=off;
    }

  return 1;
}
