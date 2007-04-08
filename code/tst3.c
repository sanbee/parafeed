/* $Id: tst3.c,v 2.0 1998/11/11 07:13:05 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
/*
  Test program to test the embedded shell via the commandline library
  */
void UI();
extern unsigned short cl_RegistrationMode;

void main(int argc, char **argv)
{
    char val[9];
    int i=-1;
    
  BeginCL(argc,argv);

    cl_RegistrationMode=0;
    if ((i = clgetSVal("help",val,&i))!=FAIL)
      DeleteVar("help",&cl_SymbTab,&cl_TabTail);
    
    
    if (!strcmp(val,"doc")) 
      {
	if (!cl_SymbLoaded) clLoadSymb();
	DeleteVar("help",&cl_SymbTab,&cl_TabTail);
	doprintdoc(val);
      }
    else if (!strcmp(val,"explain"))
      {
	doexplain(NULL);
	exit(0);
      }
    
    if (i==FAIL)  startShell();
    else  if (!cl_NoPrompt) startShell();
    
    if (cl_DOCLEANUP) clCleanUp();
    
#ifdef GNUREADLINE
    /* Put the history in the history file */
    save_hist("GHIST",HIST_DEFAULT);
#endif

  
}

void UI()
{
}
