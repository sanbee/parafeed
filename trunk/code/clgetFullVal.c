/* $Id: clgetFullVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Get the value associated with Key as one string.
------------------------------------------------------------------------*/
int clgetFullVal(char *Name, char **val)
{
  int n,i,len=0;
  char tmp[FILENAME_MAX];

  if (!*val) free(*val); *val=NULL;
  if ((n=clgetNVals(Name))>0)
    {
      for (i=1;i<=n;i++)
	{
	  clgetSVal(Name,tmp,&i);
	  len += strlen(tmp)+1;
	}

      *val = (char *)getmem(len,"clgetFullVal");
      for (i=0;i<len;i++) (*val)[i]=' ';
      i=1; clgetSVal(Name,tmp,&i);
      strcpy(*val,tmp);

      for (i=2;i<=n;i++)
	{
	  strcat(*val,",");
	  clgetSVal(Name,tmp,&i);
	  strcat(*val,tmp);
	}
      *(*val+strlen(*val))='\0';
    }
  return n;
}
int clgetFullValp(const string& Name, string& val)
{
  int n,i,len=0;
  char tmp[FILENAME_MAX];
  Symbol *S;

  S=SearchQSymb((char*)Name.c_str(),"Mixed[]");
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
#ifdef __cplusplus
	   }
#endif
