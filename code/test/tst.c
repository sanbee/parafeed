/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* $Id: tst.c,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>

#ifdef GNUREADLINE
#include <readline/history.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
//extern Symbol    *cl_SymbTab,*cl_TabTail;
//extern CmdSymbol *cl_CmdTab;
//extern char      *cl_ProgName;
extern unsigned short CL_DBG_ON;

extern "C" void (*CL_INPUT)(char *, int *, int);
extern void inp_readline(char *, int *, int);
//CL_INPUT=&inp_readline;
#define MAXBUF     256
/*------------------------------------------------------------------------
 Saves the current setting of the various keywords to the given file
 If *f==NULL, save in ./<ProgName>.def
 -------------------------------------------------------------------------*/
int udosave(char *f)
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
	if ((t->Class==CL_APPLNCLASS))
	  //	    ||
	  //	    ((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	  {
	    fprintf(fd,"%-10.10s = ",t->Name);
	    PrintVals(fd,t);
	  }
      fclose(fd);
    }
  return 1;
}

#ifdef __cplusplus
}
#endif


/*
   Test program to test the embedded shell via the commandline library
*/
int main(int argc, char **argv)
{
  char *Inf=0,*Outf=0;
  char *ConfFile=0;
  int i=3,n=1,dbg=0;
  float f[3];
  int IList[10];

  ConfFile=(char *)getenv("TST");

  argc=BeginCL(argc, argv); /* Commandline lib. entry point */

  clloadConfig(ConfFile);   /* Load the initial config file */

  clCmdLineFirst();         /* Give precedence to the commandline
			       args. if present*/
  clAddCmd("ademo",
	   "Just a demo command!",
	   udosave);        /*Add a new shell command */


  clDeleteCmd("gob");       /*Delete the command "gob" from the 
			      shell table      */
  clInteractive(0);         /*Shell must be started when EndCL() is
			      called but after finishing interactive
			      session, the internal symbol tables of
			      the shell must be preserved */

#ifdef __cplusplus
  try
    {
#endif
      {
	char tmp[128];
	float ff;int k;
	i=1;dbgclgetFVal("fdbg",&ff,&i);
	i=1;dbgclgetIVal("idbg",&k,&i);
	clgetFullVal("in",&Inf);
	clgetFullVal("integ",&Inf);
	clgetSVal("out",tmp,&n);clloadBuf(&Outf,tmp,NULL);
	i=1;if (clgetNFVal("float",f,&i)<1) f[0]=0;
	i=3;if (clgetNFVal("multifloat",f,&i)<1) f[0]=0;
	i=1;if (dbgclgetIVal("dbgint",&dbg,&i)<1) dbg=0;
	i=1;
	while(dbgclgetIVal("ilist",&IList[i-1],&i)>0) i++;
	for (int j=0;j<i-1;j++) fprintf(stderr,"%d ",IList[j]);

	VString Options;
	Options.resize(3);
	Options[0]="one";Options[1]="two"; Options[2]="three";
	clSetOptions("out",Options);
      }
      EndCL();

      fprintf(stderr,"Unused commandline options:\n");
      for (i=0;i<argc;i++) fprintf(stderr,"\t%s\n",argv[i]);
      if (Inf) fprintf(stderr,"%s ",Inf);
      fprintf(stderr,"%E \n",f[0]);
#ifdef __cplusplus
    }
  catch (clError Err)
    {
      cerr << Err.what() << endl;
    }
#endif
  clRetry();   /*Restart the shell.  Since the internal symbol tables
		 are preserved, it *can* start */
}
