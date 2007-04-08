/* $Id: TransferTab.c,v 2.2 1999/06/16 16:49:22 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void TransferTab(Symbol **Target, Symbol **TTail, Symbol *Source,int AddVar)
{
  Symbol *T,*S;
  unsigned int i=COPYNAME|COPYCLASS|COPYVAL;

  for (S=Source;S;S=S->Next)
    {
      //      if ((T = SearchVSymb(S->Name,*Target))==NULL)
      if ((T = SearchVSymbFullMatch(S->Name,*Target))==NULL)
	{
	  T = AllocVSymb(1);
	  CopyVSymb(T,S,i);
	  AddVNode(T,Target,TTail);
	}
      else if (AddVar)  CopyVSymb(T,S,i);

      CopyVSymb(T,S,COPYTYPE);
	  
    }
}
#ifdef __cplusplus
	   }
#endif
