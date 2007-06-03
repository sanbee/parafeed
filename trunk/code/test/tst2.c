/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
/*
   Test program to test the embedded shell via the commandline library
*/
void UI();

void main(int argc, char **argv)
{
  UI();
}

void UI()
{
  char **argv;
  int argc,i,j,N;
  float f,fa[3];
  char str[128];
  
  argv = (char **)malloc(2*sizeof(char **));
  argv[0]=(char *)malloc(20);
  strcpy(argv[0],"test2");
  argc=0;

  BeginCL(argc,argv);
  clInteractive(1);
  {
    i=1;clgetIVal("int",&j,&i);
    i=1;clgetFVal("float",&f,&i);
    i=1;clgetSVal("string",str,&i);
    N=3;N=clgetNFVal("farray",fa,&N);
  }
  EndCL();
  fprintf(stderr,"Float = %f   Int = %d \n",f,j);
  fprintf(stderr,"String = %s \n",str);
  fprintf(stderr,"Float Array = ");
  for (i=0;i<N;i++) fprintf(stderr,"%f ",fa[i]);
  fprintf(stderr,"\n");
}
