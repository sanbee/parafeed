/* $Id: clgetFullVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <shell.h>
#include <cllib.h>

#ifdef __cplusplus
/* extern "C" { */
/* #endif */
/*------------------------------------------------------------------------
   Get the value associated with Key as one string.
------------------------------------------------------------------------*/
int dbgclgetFullValp(const string& Name, string& val)
{
  int n,i,len=0;
  char tmp[FILENAME_MAX];
  Symbol *S;
  S=SearchQSymb((char*)Name.c_str(),"Mixed[]");
  if (S == NULL)
    S = SearchVSymb((char *)Name.c_str(),cl_SymbTab);
  
  if (S != NULL) S->Class=CL_DBGCLASS;
  
  if ((n=clgetNVals((char *)Name.c_str()))>0)
    {
      val="";
      for (i=1;i<=n;i++)
	{
	  clgetSVal((char *)Name.c_str(),tmp,&i);
	  len += strlen(tmp)+1;
	}

      i=1; clgetSVal((char *)Name.c_str(),tmp,&i);
      val=tmp;

      for (i=2;i<=n;i++)
	{
	  val = val +",";
	  clgetSVal((char *)Name.c_str(),tmp,&i);
	  val = val + tmp;
	}
    }
  return n;
}
/* #ifdef __cplusplus */
/* 	   } */
#endif
