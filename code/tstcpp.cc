/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
#include <vector>
#include <exception>
/*
   Test program to test the embedded shell via the commandline library
*/

void UI(bool restart, int argc, char **argv)
{
  int i,j=0,N;
  float f=0;
  vector<float> fv(10);
  VString strarr;
  string str;
  bool b=false, b1=false;
  //
  // Change cl-shell prompt
  //  
  // char **argv_l; int argc_l;
  // argv_l = (char **)malloc(2*sizeof(char **));
  // argv_l[0]=(char *)malloc(20);
  // strcpy(argv_l[0],"test2");
  // argc_l=0;

  if (!restart)
    {
      BeginCL(argc,argv);
      clInteractive(0);
    }
  else
    clRetry();
  try
    {
      {
	SMap watchPoints; VString exposedKeys;
	InitMap(watchPoints,exposedKeys);

	exposedKeys.push_back("bool1");
	watchPoints["1"]=exposedKeys;
	i=1;clgetBValp("bool",b,i,watchPoints);

	// Create a HIDENDSEEK type keyword (one which is hidden and
	// also hides another keyword).
	ClearMap(watchPoints); exposedKeys.resize(0); // Re-use watchPoints and exposedKeys
	exposedKeys.push_back("int");
	watchPoints["1"]=exposedKeys;
	i=1;clgetBValp("bool1",b1,i,watchPoints);

	i=1;clgetIValp("int",j,i);
	i=1;clgetFValp("float",f,i);
	i=1;clgetSValp("string",str,i);
	i=0;clgetNSValp("strarr",strarr,i);
	N=3;N=clgetNFValp("farray",fv,N);
	VString options;
	options.resize(3);
	options[0]="one"; options[1]="two"; options[2]="three";
	clSetOptions("string",options);
      }
      EndCL();
    }
  catch (clError x)
    {
      x << x << endl;
      clRetry();
    }
  cerr << "  Bool        = " << b << endl;
  cerr << "  Bool1       = " << b1 << endl;
  cerr << "  Float       = " << f << endl;
  cerr << "  Int         = " << j << endl;
  cerr << "  StrArr      = "; for(uint ii=0;ii<strarr.size();ii++) cerr << strarr[ii] << " "; cerr << endl;
  cerr << "  String      = " << str << endl;
  cerr << "  Float Array = ";
  for (vector<float>::const_iterator i=fv.begin();i!=fv.end();i++) 
    cerr << *i << " ";cerr << endl;
}

//
//-------------------------------------------------------------------------
//
#define RestartUI(Label)  {if(clIsInteractive()) {goto Label;}}

int main(int argc, char **argv)
{
  bool restart=false;
 RENTER:
  try
    {
      UI(restart, argc, argv);
    }
  catch(clError& x)
    {
      x << x << " " << x.Severity() << endl;
      
      if (x.Severity() != ErrorObj::Fatal) restart=true;
    }
  if (restart) RestartUI(RENTER);
}

