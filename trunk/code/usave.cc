/* $Id: callbacks.c,v 2.5 1999/07/12 09:49:34 sanjay Exp sanjay $ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cl.h>
#include <shell.h>
#include <shell.tab.h>

#ifdef GNUREADLINE
#include <readline/history.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern Symbol    *cl_SymbTab,*cl_TabTail;
extern CmdSymbol *cl_CmdTab;
extern char      *cl_ProgName;
extern unsigned short CL_DBG_ON;

#define MAXBUF     256
/*------------------------------------------------------------------------
 Saves the current setting of the various keywords to the given file
 If *f==NULL, save in ./<ProgName>.def
 -------------------------------------------------------------------------*/
int dosave(char *f)
{
  FILE *fd;
  char str[MAXBUF];
  
  fprintf(stderr,"At U's service! \n");

  stripwhite(f);
  if(f==NULL || strlen(f) == 0)
    {
      strcpy(str,cl_ProgName);
#ifdef GNUREADLINE
      str[strlen(cl_ProgName)-1]='\0';
#endif
      strcat(str,".def");
    }
  else strcpy(str,f);
  
  if ((fd=fopen(str,"w"))==NULL)
    {
      fprintf(stderr,"###Error: Error in opening file \"%s\"\n",str);
      return 2;
    }
  else
    {
      Symbol *t;
      
      for (t=cl_SymbTab;t;t=t->Next)
	if ((t->Class==CL_APPLNCLASS) ||
	    ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	  {
	    fprintf(fd,"%-10.10s = ",t->Name);
	    PrintVals(fd,t);
	  }
      fclose(fd);
    }
  return 1;
}
