/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
#include <vector>
#include <exception>
#include <clgetValp.h>
/*
   Test program to test the embedded shell via the commandline library
*/

void UI(bool restart, int argc, char **argv)
{
  int i,j=0,dj=11,oi=0,N;
  float f=0;
  vector<float> fv(10);
  VString strarr;
  string str;
  string fullVal="this is the default value";
  string dbgfullVal="this is the dbg default value";
  bool b=false, b1=false;
  //
  // Change cl-shell prompt
  //  
  // char **argv_l; int argc_l;
  // argv_l = (char **)malloc(2*sizeof(char **));
  // argv_l[0]=(char *)malloc(20);
  // strcpy(argv_l[0],"test2");
  // argc_l=0;

  // Set the application to be non-interactive by default.
  // The setting can be changed via the help=prompt keyword.
  //  clSetPrompt(false);
  clSetPrompt(true);

  if (!restart)
    {
      BeginCL(argc,argv);
      clInteractive(0);
    }
  else
    clRetry();
  try
    {
      //clCmdLineFirst();
      {
	SMap watchPoints; VString exposedKeys;
	InitMap(watchPoints,exposedKeys);

	exposedKeys.push_back("bool1");
	watchPoints["1"]=exposedKeys;
	i=1;clgetValp("bool",b,i,watchPoints); // Equivalent to clgetBValp()

	// Create a HIDENDSEEK type keyword (one which is hidden and
	// also hides another keyword).
	ClearMap(watchPoints); exposedKeys.resize(0); // Re-use watchPoints and exposedKeys
	exposedKeys.push_back("int");
	watchPoints["0"]=exposedKeys;
	i=1;clgetValp("bool1",b1,i,watchPoints); // Equivalent to clgetBValp()

	ClearMap(watchPoints); exposedKeys.resize(0); // Re-use watchPoints and exposedKeys
	exposedKeys.push_back("float");
	watchPoints["1"]=exposedKeys;
	i=1;clgetValp("int",j,i,watchPoints); // Equivalent to clgetIValp()
	i=1;dbgclgetValp("dbgint",dj,i);

	i=1;clgetValp("float",f,i); // Equivalent to clgetIValp()

	i=1;clgetValp("oneint",oi,i); // Equivalent to clgetIValp()

	ClearMap(watchPoints);
	// Re-use watchPoints and exposedKeys
	exposedKeys.resize(0);exposedKeys.push_back("strarr"); watchPoints["showstrarr"]=exposedKeys;
	exposedKeys.resize(0);exposedKeys.push_back("fullval"); watchPoints["showfullval"]=exposedKeys;

	str="showstrarr";
	i=1;clgetSValp("string",str,i,watchPoints);

	// Add another watchpoint for the same keyword that exposes a
	// differet set of exposed keys.  As an example,
	// string=testarr will expose the keyworkd bool1.
	// ClearMap(watchPoints);
	// exposedKeys.resize(0);
	// exposedKeys.push_back("bool1");
	// watchPoints["testarr"]=exposedKeys;
	// i=1;clgetSValp("string",str,i,watchPoints);
	
	clSetOptions("string",{"one","two","three","showstrarr","showfullval"});

	i=0;clgetFullValp("fullval",fullVal);
	i=0;dbgclgetFullValp("dbgfullval",dbgfullVal);
	i=0;clgetNSValp("strarr",strarr,i);
	N=3;N=clgetNValp("farray",fv,N); // Equivalent to clgetNFValp()
      }
      EndCL();
    }
  catch (clError x)
    {
      x << x << endl;
      clRetry();
    }
  cerr << "  bool        = " << b << endl;
  cerr << "  bool1       = " << b1 << endl;
  cerr << "  float       = " << f << endl;
  cerr << "  dbgint      = " << dj << endl;
  cerr << "  int         = " << j << endl;
  cerr << "  oneint      = " << oi << endl;

  cerr << "  string      = " << str << endl;
  cerr << "  strarr      = "; for(auto s : strarr) cerr << s << " "; cerr << endl;
  cerr << "  fullval     = " << fullVal << endl;
  cerr << "  farray      = "; for(auto f : fv) cerr << f << " "; cerr << endl;
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
      x << x << " " << endl;//x.Severity() << endl;
      
      if (x.Severity() != ErrorObj::Fatal) restart=true;
    }
  if (restart) RestartUI(RENTER);
}

