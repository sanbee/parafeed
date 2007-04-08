/* $Id: AddQKey.c,v 2.1 2002/04/11 20:26:58 sbhatnag Exp $ */
#include <cl.h>
#include <shell.h>
#include "shell.tab.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   This adds a symbol with name Name, of Type <Type> to the list <Tab>
   with it's head and tail pointed to by <Head> and <Tail> resp.
 
   If a symbol with the given name already exist, this will only
   modify it's Type.  Only if the given <Type> carries more info than
   already exist will it modify the existing Type (by adding the extra
   info).  If the existing Type==UNKNOWN, then any info is new (or
   same), except that it will check if the last character of the
   existing Type was ']'. If it was, it will assume that the existing
   Type was UNKNOWN[] and will only replace the word "UNKNOWN" with
   the new info.

   If the name symbol does not exist, it will create one 
------------------------------------------------------------------------*/
Symbol *AddQKey(char *Name, char *Type, 
		Symbol **Head, Symbol **Tail)
{
  //  Symbol *s = SearchVSymb(Name,*Head);
  Symbol *s = SearchVSymbFullMatch(Name,*Head);
  if (s == NULL)
    {
      s = AllocVSymb(1);
      s->Type = (char *)getmem(strlen(Type)+1,"AddQKey");
      s->Name = (char *)getmem(strlen(Name)+1,"AddQKey");
      strcpy(s->Type,Type);
      strcpy(s->Name,Name);
      
      AddVNode(s,Head,Tail);
    }
  else
    {
      if (s->Type == NULL)
	{
	  s->Type = (char *)getmem(strlen(Type)+1,"AddQKey");
	  strcpy(s->Type,Type);
	}
      else
	if (strcmp(s->Type,Type))
	  {
	    if (!strncmp(s->Type,"Mixed",7))
	      {
		int j=strlen(s->Type)-1;
		char tt[8]={'\0'};

		if (s->Type[j]==']')
		  {
		    while (s->Type[j] != '[') j--;
		    strcpy(tt,&(s->Type[j]));j=strlen(tt);
		  }
		
		s->Type = (char *)realloc(s->Type,strlen(Type)+1+j);
		strcpy(s->Type,Type);
		if (j) strcat(s->Type,tt);
	      }
	    else 
	      if ((Type[strlen(Type)-1]==']') &&
		  (s->Type[strlen(s->Type)-1] != ']'))
		{
		  char *t=Type;
		  s->Type = (char *)realloc(s->Type,strlen(s->Type)+3);
		  while (*t != '[') t++;
		  strcat(s->Type,t);
		}
	  }
    }
  return s;
}
#ifdef __cplusplus
	   }
#endif
