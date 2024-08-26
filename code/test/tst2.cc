/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
//#include <clgetValp.h>
/*
   Test program to test the embedded shell via the commandline library
*/
void UI();

int main(int argc, char **argv)
{
  UI();
}

void UI()
{
  char **argv;
  int argc,i,j=0,N;
  float f=0;
  vector<float> fa(3);
  char str[128]="";
  string strp;
  bool b=false;
  
  argv = (char **)malloc(2*sizeof(char **));
  argv[0]=(char *)malloc(20);
  strcpy(argv[0],"test2");
  argc=0;

  BeginCL(argc,argv);
  clInteractive(1);
  {
    i=1;clgetBValp("bool",b,i);
    i=1;clgetIValp("int",j,i);
    i=1;clgetFValp("float",f,i);
    i=1;clgetSValp("string",strp,i);
    N=3;N=clgetNFValp("farray",fa,N);
    VString options;
    options.resize(3);
    options[0]="one"; options[1]="two"; options[2]="three";
    clSetOptions("string",options);
  }
  EndCL();
  fprintf(stderr,"Bool = %d \n",b);
  fprintf(stderr,"Float = %f   Int = %d \n",f,j);
  //  fprintf(stderr,"String = %s \n",str);
  cerr << "String = " << strp << endl;
  fprintf(stderr,"Float Array = ");
  for (i=0;i<N;i++) fprintf(stderr,"%f ",fa[i]);
  fprintf(stderr,"\n");
}
