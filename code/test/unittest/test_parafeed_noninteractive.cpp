#include <unittest/ParafeedTest.h>
#include <cstdlib>
#include <fstream>
//
// ---------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------
// Factored out re-usable code into global functions
//
auto canonicalArgs=[](std::string defFile=std::string(),
		      std::string help=std::string(),
		      bool writeDefFile=false)
 {
  std::vector<std::string> args
    {
     "test2",
     "bool=true",
     "bool1=false",
     "int=42",
     "dbgint=77",
     "float=2.71",
     "oneint=100+23",
     "string=showstrarr",
     "strarr=val1,val2",
     "fullval=custom_value",
     "dbgfullval=debug_value",
     "farray=9.9,8.8,7.7"
    };
  if (writeDefFile && !defFile.empty())
    {
      std::ofstream ofs(defFile);
      for(int i=1;i<args.size();i++) ofs << args[i] << endl;
    }

  if (!help.empty()) args.push_back(help);

  return args;
 };

//--------------------------------------------------------------------
auto canonicalTest=[]()
 {
  int i;

  // bool
  bool b = false;
  SMap watchPoints;
  VString exposedKeys = {"bool1"};
  watchPoints["1"] = exposedKeys;
  i = 1;
  clgetValp("bool", b, i, watchPoints);
  EXPECT_TRUE(b);

  // bool1
  bool b1 = true;
  ClearMap(watchPoints);
  exposedKeys = {"int"};
  watchPoints["0"] = exposedKeys;
  i = 1;
  clgetValp("bool1", b1, i, watchPoints);
  EXPECT_FALSE(b1);

  // int
  int intVal = 0;
  ClearMap(watchPoints);
  exposedKeys = {"float"};
  watchPoints["1"] = exposedKeys;
  i = 1;
  clgetValp("int", intVal, i, watchPoints);
  EXPECT_EQ(intVal, 42);

  // dbgint
  int dbgInt = 0;
  i = 1;
  cldbggetValp("dbgint", dbgInt, i);
  EXPECT_EQ(dbgInt, 77);

  // float
  float fVal = 0.0f;
  i = 1;
  clgetValp("float", fVal, i);
  EXPECT_FLOAT_EQ(fVal, 2.71f);

  // oneint
  int oneintVal = 0;
  i = 1;
  clgetValp("oneint", oneintVal, i);
  EXPECT_EQ(oneintVal, 123);

  // string
  std::string str;
  ClearMap(watchPoints);
  exposedKeys = {"strarr"};
  watchPoints["showstrarr"] = exposedKeys;
  exposedKeys = {"fullval"};
  watchPoints["showfullval"] = exposedKeys;
  str = "showstrarr";
  i = 1;
  clgetValp("string", str, i, watchPoints);
  EXPECT_EQ(str, "showstrarr");

  // fullval
  std::string fullVal;
  i = 0;
  clgetFullValp("fullval", fullVal);
  EXPECT_EQ(fullVal, "custom_value");

  // dbgfullval
  std::string dbgFullVal;
  i = 0;
  dbgclgetFullValp("dbgfullval", dbgFullVal);
  EXPECT_EQ(dbgFullVal, "debug_value");

  // strarr
  VString strarr;
  i = 0;
  clgetValp("strarr", strarr, i);
  ASSERT_EQ(strarr.size(), 2u);
  EXPECT_EQ(strarr[0], "val1");
  EXPECT_EQ(strarr[1], "val2");

  // farray
  std::vector<float> fv(3);
  int N = 3;
  int count = clgetValp("farray", fv, N);
  EXPECT_EQ(count, 3);
  EXPECT_FLOAT_EQ(fv[0], 9.9f);
  EXPECT_FLOAT_EQ(fv[1], 8.8f);
  EXPECT_FLOAT_EQ(fv[2], 7.7f);

  //  EndCL();

 };
//
//--------------------------------------------------------------------
//
TEST_F(ParafeedTest, ParsesClgetValpParametersCorrectly)
{
  std::vector<std::string> args=canonicalArgs("","help=noprompt",false);
  auto [argc, argv] = MakeArgv(args);

  BeginCL(argc, argv);
  {
    clInteractive(0);

    canonicalTest();
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
  std::string expected_output="%%N:test2\n%%P:bool:bool:0:\n%%P:oneint:int:0:\n%%P:string:string::\n%%P:strarr:string[]::\n%%P:farray:float[10]:0,0,0,0,0,0,0,0,0,0:\n";

  std::string output = ::testing::internal::GetCapturedStdout();
  EXPECT_EQ(output,expected_output);
  //  cout << output << endl;


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
  clCleanUp();

  cout << "[INFO]: With help=def," << defFile0
       << " BeginCL() will look for test2.def "
       << "file to load the parameters and find it."
       << endl;
  args=canonicalArgs(defFile0,"help=def,"+defFile0,true);

  for(auto s : args) cout << s << " "; cout << endl;
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
  clCleanUp();

  cout << "[INFO]: With help=def"
       << " by default BeginCL() will look for test2.def "
       << "file to load the parameters and find it."
       << endl;
  // With help="def" by default BeginCL() will look for
  // test2.def file to load the parameters and find it.
  args=canonicalArgs(defFile0,"help=def",true);

  for(auto s : args) cout << s << " "; cout << endl;
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
  clCleanUp();

  cout << "[INFO]: With help=def"
       << " by default BeginCL() will look for " << defFile0
       << " file to load the parameters and not find it,"
       << " and throw an exception."
       << endl;
  // With help="def" BeginCL() will look for
  // test2.def file to load the parameters and not find it,
  // and throw an exception
  args=canonicalArgs(defFile0,"help=def",false);

  for(auto s : args) cout << s << " "; cout << endl;
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
  clCleanUp();

  cout << "[INFO]: With help=def,"+defFile1
       << " BeginCL() will look for " << defFile1
       << " file to load the parameters and not find it,"
       << " and throw an exception."
       << endl;
  // With help="def,+defFile1 BeginCL() will look for
  // tt.def file to load the parameters and not find it,
  // and throw an exception
  args=canonicalArgs(defFile0,"help=def,"+defFile1,false);

  for(auto s : args) cout << s << " "; cout << endl;
  auto [argc, argv] = MakeArgv(args);

  EXPECT_THROW(BeginCL(argc, argv),clError);
  FreeArgv(argc, argv);

  canonicalTest();

  EndCL();

  std::remove(defFile0.c_str());
  std::remove(defFile1.c_str());
}
