/* $Id: minmatch.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <string.h>
#include <clshelldefs.h>
#include <cllib.h>
#define NN 50

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
Symbol *minmatch(char *str, Symbol *List)
{
  int i,m=0;
  Symbol *s,*l[NN];
  
  for (s=List;s;s=s->Next)
    if (!strncmp(str,s->Name,strlen(str))) 
      {
	if (m<NN) l[m++]=s;
	else
	  {fprintf(stderr,"###Error: Mem overflow in minmatch\n");exit(-1);}
	if (strlen(str) == strlen(s->Name)) break;
      }  
  if (m > 1)
    {
      //
      // If more than one min-matched names were found, see if there
      // is any that matches completely.  If a complete match is
      // found, return that symbol.  Else there is an ambiguity.
      //  
      for(int j=0;j<m;j++)
	if (strcmp(str,l[j]->Name)==0) return l[j];
      fprintf(stderr,"###Ambiguous min-match: ");
      for (i=0;i<m;i++) fprintf(stderr,"%s ",l[i]->Name);
      fprintf(stderr,"\n");
      return NULL;
    }
  if (m==1)
    {
      s=l[0];
      return s;
    }
  else
    return NULL;
}
#ifdef __cplusplus
	   }
#endif
