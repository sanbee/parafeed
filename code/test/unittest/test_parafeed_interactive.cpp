#include <unittest/ParafeedTest.h>
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, Interactive)
{
  std::vector<std::string> args =
    {
     "test2",
     //"help=noprompt",
     "bool=1",
     "oneint=x",
     "string=showstrarr",
     "strarr=foo,bar",
     "farray=1,3,4,5,6,7,8,9,10"
    };
  auto [argc, argv] = MakeArgv(args);

  // sendCmd() sets the parser input stream to be the given string.
  // The parser scans this string in the interactive shell (started
  // in the EndCL() call below).
  sendCmd("bool=true\n oneint=100\n fullval=this is full val\n inp\n go\n");

  BeginCL(argc, argv);
  clInteractive(1);

  bool b = false;
  int oneint = 0;
  int N = 10;
  int i = 1;

  std::string s;
  std::vector<std::string> strarr;
  std::vector<float> farray(N);
  string fullVal="this is the default value";

  i=1;clgetValp("bool", b, i);

  i=1;

  //
  // The setting from argv is used in the first pass
  // (a.k.a. "registeration pass").  Without the "help=noprompt"
  // detected in this first pass, the EndCL() call starts the
  // interactive shell, which triggers the second pass on the "go"
  // command (setjmp() called in EndCL() to restart execution from
  // the location of the clInteractive() call).  In this second pass
  // the interactive settings (here, vis the sendCmd() call)
  // replaces the values which are then available in the clgetValp()
  // calls.
  //
  if (cl_Pass == 0) // cl_Pass is a global parafeed lib. control
    // variable
    EXPECT_THROW(clgetValp("oneint", oneint, i),clError);
  else
    clgetValp("oneint", oneint, i);

  i=1;clgetValp("string", s, i);

  i=0;clgetValp("strarr", strarr, i);

  clgetValp("farray", farray, N);

  i=0;clgetFullValp("fullval",fullVal);

  EndCL();

  // Expect the value as set interactively vis sendCmd()
  EXPECT_EQ(oneint,100);
  EXPECT_EQ(fullVal,"this is full val");

  FreeArgv(argc, argv);
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, InteractiveWrongType)
{
  std::vector<std::string> args =
    {
     "test2",
     //"help=noprompt",
     "bool=1",
     "oneint=1",
     "string=showstrarr",
     "strarr=foo,bar",
     "farray=1,3,4,5,6,7,8,9,10"
    };
  auto [argc, argv] = MakeArgv(args);

  // Interactively set the wrong type for bool
  sendCmd("oneint=100\n bool=xtrue\n inp\n go\n");

  BeginCL(argc, argv);
  clInteractive(1);

  bool b = false;
  int oneint = 0;
  int N = 10;
  int i = 1;
  std::string s;
  std::vector<std::string> strarr;
  std::vector<float> farray(N);

  i=1;clgetValp("bool", b, i);

  i=1;clgetValp("oneint", oneint, i);

  i=1;clgetValp("string", s, i);

  i=0;clgetValp("strarr", strarr, i);

  clgetValp("farray", farray, N);

  // Exception thrown in the interactive shell due to illegal
  // keyword setting (bool=xtrue) via sendCmd()
  EXPECT_THROW(EndCL(),clError);

  FreeArgv(argc, argv);
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, InteractiveComplementaryLoad)
{
  std::string defFile="tt.def";
  std::vector<std::string>   args=canonicalArgs();

  //  std::replace(args.begin(), args.end(), +"strarr=val1,val2", +"strarr=");

  makeDefFile(args,defFile,"",true);

  // Interactively set the wrong type for bool
  sendCmd("strarr=blah1,blah2\n load "+defFile+"!\n inp\n go\n");
 
  auto [argc, argv] = MakeArgv(args);
  std::vector<std::string> strarr;
  BeginCL(argc, argv);
  clInteractive(1);
  int i=0;clgetValp("strarr", strarr, i);
  //  for(auto s : strarr) cout << s << endl;
  EndCL();

  ASSERT_EQ(strarr.size(), 2u);
  EXPECT_EQ(strarr[0], "blah1");
  EXPECT_EQ(strarr[1], "blah2");

  std::remove(defFile.c_str());
}
