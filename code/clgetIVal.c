/* $Id: clgetIVal.c,v 2.1 1999/03/23 15:01:23 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#include <support.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an integer
------------------------------------------------------------------------*/
int clgetIVal(char *Name, int *val, int *n)
{
  Symbol *S;
  double d;
  int N;
HANDLE_EXCEPTIONS(
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);  
  else
    S=SearchQSymb(Name,"int");
  if ((N=clparseVal(S,n,&d))>0) *val = (int)d;
  return N;
  );
}
int clgetIValp(const string& Name, int& val, int& n)
{
  Symbol *S;
  double d;
  int N;
HANDLE_EXCEPTIONS(
		  if (n < 0)
		    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);  
		  else
		    S=SearchQSymb((char *)Name.c_str(),"int");
		  setAutoIDefaults(S,val);
		  if ((N=clparseVal(S,&n,&d))>0) val = (int)d;
		  return N;
		  );
}
#ifdef __cplusplus
	   }
#endif
