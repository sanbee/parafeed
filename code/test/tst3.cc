/* $Id: tst3.c,v 2.0 1998/11/11 07:13:05 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
#include <shell.h>
#include <rl_interface.h>

/*
  Test program to test the embedded shell via the commandline library
  */
void UI();
extern unsigned short cl_RegistrationMode;

int main(int argc, char **argv)
{
    char val[9];
    int i=-1;
    
  BeginCL(argc,argv);

    cl_RegistrationMode=0;
    if ((i = clgetSVal("help",val,&i))!=CL_FAIL)
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
    
    if (i==CL_FAIL)  startShell();
    else  if (!cl_NoPrompt) startShell();
    
    if (cl_DOCLEANUP) clCleanUp();
    
#ifdef GNUREADLINE
    /* Put the history in the history file */
    //    save_hist("GHIST",CL_HIST_DEFAULT);
#endif
    return 0;  
}

void UI()
{
}
