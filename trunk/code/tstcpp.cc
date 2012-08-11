/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
#include <vector>
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
  vector<float> fv(10);
  string str;
  bool b=false;
  //
  // Change cl-shell prompt
  //  
  argv = (char **)malloc(2*sizeof(char **));
  argv[0]=(char *)malloc(20);
  strcpy(argv[0],"test2");
  argc=0;

  BeginCL(argc,argv);
  clInteractive(1);
  {
    SMap watchPoints; vector<string> exposedKeys;
    ClearMap(watchPoints);
    exposedKeys.resize(0);
    exposedKeys[0]="int";
    watchPoints["1"]=exposedKeys;
    i=1;clgetBValp("bool",b,i,watchPoints);
    i=1;clgetIValp("int",j,i);
    i=1;clgetFValp("float",f,i);
    i=1;clgetSValp("string",str,i);
    N=3;N=clgetNFValp("farray",fv,N);
    VString options;
    options.resize(3);
    options[0]="one"; options[1]="two"; options[2]="three";
    clSetOptions("string",options);
  }
  EndCL();
  cerr << "  Bool        = " << b << endl;
  cerr << "  Float       = " << f << endl;
  cerr << "  Int         = " << j << endl;
  cerr << "  String      = " << str << endl;
  cerr << "  Float Array = ";
  for (vector<float>::const_iterator i=fv.begin();i!=fv.end();i++) 
    cerr << *i << " ";cerr << endl;
}
