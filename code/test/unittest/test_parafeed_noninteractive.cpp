#include <unittest/ParafeedTest.h>
//
// ---------------------------------------------------------------------------------
//
TEST_F(ParafeedTest, ParsesClgetValpParametersCorrectly)
{
  std::vector<std::string> args=makeCanonicalArgs("","help=noprompt");
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  {
    clInteractive(0);

    canonicalTest();
    //
    // The following keys should be of the CL_DBGCLASS class and
    // CL_DBG_ON==false.
    //
    Symbol *S;
    S=SearchVSymb("dbgint");
    EXPECT_EQ(!CL_DBG_ON && S->Class==CL_DBGCLASS, true);

    S=SearchVSymb("dbgfullval");
    EXPECT_EQ(!CL_DBG_ON && S->Class==CL_DBGCLASS, true);

  }
  EndCL();
  FreeArgv(argc, argv);
}

TEST_F(ParafeedTest, TextColouringHandlesMissingTermEnvironment)
{
  const char* originalTerm = std::getenv("TERM");
  std::string originalTermValue = originalTerm ? originalTerm : "";

  unsetenv("TERM");

  std::string startSeq;
  std::string endSeq;
  EXPECT_NO_THROW(clTextColouring("keyword", CL_HIDDENKEYWORD, startSeq, endSeq));

  if (originalTerm)
    setenv("TERM", originalTermValue.c_str(), 1);
  else
    unsetenv("TERM");
}

// below tests can be included when parafeed throws on these wrong usage.

TEST_F(ParafeedTest, PrintsErrorOnUnknownParameter)
{
  std::vector<std::string> args =
    {
     "test2",
     "help=noprompt",
     "bool=true"
     // "unknownparam=badvalue"  // This will add the named parameter to the symbol table.
     // The library allows user-defined symbols.
    };
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  clInteractive(0);

  // Try accessing only the known parameter
  bool b = false;
  int i = 1;
  clgetValp("bool", b, i);

  std::string unused;
  i = 1;
  clgetValp("unknownparam", unused, i);
  EndCL();

  FreeArgv(argc, argv);
}



/*TEST_F(ParafeedTest, MissingRequiredParam) {
  std::vector<std::string> args = {
  "test2",
  "help=noprompt",
  "bool=true",
  "oneint=5",
  "string=showstrarr",
  // missing "strarr="
  "farray=1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0"
  };
  auto [argc, argv] = MakeArgv(args);

  ::testing::internal::CaptureStdout();

  BeginCL(argc, argv);
  clInteractive(0);

  bool b = false;
  int oneint = 0;
  std::string s;
  std::vector<std::string> strarr;
  std::vector<float> farray(10);
  int i = 1;

  clgetValp("bool", b, i);
  clgetValp("oneint", oneint, i);
  clgetValp("string", s, i);

  // attempt to access the missing "strarr"
  int index = 0;
  clgetValp("strarr", strarr, index);  // Should produce error

  EndCL();

  std::string output = ::testing::internal::GetCapturedStdout();
  FreeArgv(argc, argv);


  EXPECT_NE(output.find("###Error"), std::string::npos)
  << "Expected error message for missing 'strarr' not found in stdout:\n" << output;
  }
*/

TEST_F(ParafeedTest, WrongDataType)
{
  std::vector<std::string> args =
    {
     "test2",
     "help=noprompt",
     "bool=not_true",               // Wrong data type
     "oneint=not_int",              // Wrong data type
     "string=showstrarr",
     "strarr=foo,bar",
     "farray=1,x,3,4,5,6,7,8,9,10" // One wrong data type
    };
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  //    clInteractive(0);

  bool b = false;
  int oneint = 0;
  std::string s;
  std::vector<std::string> strarr;
  std::vector<float> farray(10);
  int i = 1;

  EXPECT_THROW(clgetValp("bool", b, i),clError);

  // Should fail due to wrong type
  EXPECT_THROW(clgetValp("oneint", oneint, i),clError);

  clgetValp("string", s, i);
  int idx = 0;
  clgetValp("strarr", strarr, idx);
  int N = 10;

  EXPECT_THROW(clgetValp("farray", farray, N),clError);

  EndCL();

  FreeArgv(argc, argv);
}

//
//--------------------------------------------------------------------
// This interactive test after non-interactive ones above tests if the
// internal state of the library is properly reset (in clCleanUp()).
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
  sendCmd("bool=true\noneint=100\ninp\ngo\n");

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

  EndCL();

  // Expect the value as set interactively vis sendCmd()
  EXPECT_EQ(oneint,100);

  FreeArgv(argc, argv);
}

//
//--------------------------------------------------------------------
// Test help=params mode
//
TEST_F(ParafeedTest, ParamsMode)
{
  std::vector<std::string> args =
    {
     "test2",
     "help=params"
    };
  auto [argc, argv] = MakeArgv(args);

  ::testing::internal::CaptureStdout();

  BeginCL(argc, argv);

  clInteractive(0);

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

  EndCL();
  std::string expected_output=
    "%%N:test2\n"
    "%%P:bool:bool:0:\n"
    "%%P:oneint:int:0:\n"
    "%%P:string:string::\n"
    "%%P:strarr:string[]::\n"
    "%%P:farray:float[10]:0,0,0,0,0,0,0,0,0,0:\n";

  std::string output = ::testing::internal::GetCapturedStdout();
  EXPECT_EQ(output,expected_output);

  FreeArgv(argc, argv);
}
//
//--------------------------------------------------------------------
// Test help=def[,Name] mode
//
TEST_F(ParafeedTest, DefMode)
{
  std::vector<std::string> args =
    {
     "test2",
     "help=def"
    };
  auto [argc, argv] = MakeArgv(args);

  // help=def will look for test2.def file to load the parameters
  // and throw an exception since test2.def does not exist
  EXPECT_THROW(BeginCL(argc, argv),clError);

  clInteractive(0);

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

  EndCL();

  FreeArgv(argc, argv);
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, DefModeWithDefFile)
{
  std::string defFile0="test2.def", defFile1="tt.def";
  std::vector<std::string> args;

  cerr << "[INFO]: With help=def," << defFile0
       << " BeginCL() will look for test2.def "
       << "file to load the parameters and find it."
       << endl;

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());

  args=makeCanonicalArgs(defFile0,"help=def,"+defFile0);

  for(auto s : args) cerr << s << " "; cerr << endl;
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  FreeArgv(argc, argv);

  canonicalTest();

  EndCL();

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, DefModeWithoutDefFileError)
{
  std::string defFile0="test2.def", defFile1="tt.def";
  std::vector<std::string> args;

  cerr << "[INFO]: With help=def"
       << " by default BeginCL() will look for " << defFile0
       << "file to load the parameters and find it."
       << endl;

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
  //
  // With help="def" by default BeginCL() will look for
  // test2.def file to load the parameters and find it.
  //
  args=makeCanonicalArgs(defFile0,"help=def");

  for(auto s : args) cerr << s << " "; cerr << endl;
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  FreeArgv(argc, argv);

  canonicalTest();

  EndCL();

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, DefModeWithDefFileError)
{
  std::string defFile0="test2.def", defFile1="tt.def";
  std::vector<std::string> args;

  cerr << "[INFO]: With help=def"
       << " by default BeginCL() will look for " << defFile0
       << " file to load the parameters and not find it (since, "
       << defFile1 << " is saved)"
       << " and throw an exception."
       << endl;

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
  //
  // With help="def" BeginCL() will look for
  // test2.def file to load the parameters and not find it,
  // and throw an exception
  //
  args=makeCanonicalArgs(defFile1,"help=def");

  for(auto s : args) cerr << s << " "; cerr << endl;
  auto [argc, argv] = MakeArgv(args);

  EXPECT_THROW(BeginCL(argc, argv),clError);
  FreeArgv(argc, argv);

  canonicalTest();

  EndCL();

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
}
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, DefModeWithWrongDefFile)
{
  std::string defFile0="test2.def", defFile1="tt.def";
  std::vector<std::string> args;

  cerr << "[INFO]: With help=def,"+defFile1
       << " BeginCL() will look for " << defFile1
       << " file to load the parameters and not find it (since "
       << defFile0 << " is saved)"
       << " and throw an exception."
       << endl;

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
  //
  // With help="def,+defFile1 BeginCL() will look for
  // tt.def file to load the parameters and not find it,
  // and throw an exception
  //
  args=makeCanonicalArgs(defFile0,"help=def,"+defFile1);

  for(auto s : args) cerr << s << " "; cerr << endl;
  auto [argc, argv] = MakeArgv(args);

  EXPECT_THROW(BeginCL(argc, argv),clError);
  FreeArgv(argc, argv);

  canonicalTest();

  EndCL();

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
}
