/* $Id: clgetSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
int clgetSVal(char *Name, char *val, int *n)
{
  Symbol *S;
  unsigned int N;
  char *buf,*c;
  
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,"string");
  N = _ABS(*n);

  if (S!=NULL) 
    {
      if (N <= S->NVals) 
	{
	  buf = S->Val[N-1];
	  while (*buf == ' ') buf++;
	  strncpy(val,buf,strlen(buf)+1);
	  if ((c=strstr(buf,"\\\""))) 
	    while (*c) *c = *(++c);
	  return strlen(buf);
	}
      else 
	return FAIL;
    }
  else
    return FAIL;
}
int clgetSValp(const string& Name, string& val, int& n)
{
  Symbol *S;
  unsigned int N;
  char *buf,*c;
  
  if (n < 0)
    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);
  else
    S=SearchQSymb((char *)Name.c_str(),"string");
  N = _ABS(n);

  if (S!=NULL) 
    {
      if (N <= S->NVals) 
	{
	  buf = S->Val[N-1];
	  while (*buf == ' ') buf++;
	  val = val + buf;
	  //	  strncpy(val,buf,strlen(buf)+1);
	  if ((c=strstr(buf,"\\\""))) 
	    while (*c) *c = *(++c);
	  return strlen(buf);
	}
      else 
	return FAIL;
    }
  else
    return FAIL;
}
#ifdef __cplusplus
	   }
#endif
