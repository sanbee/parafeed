#include <gtest/gtest.h>
#include <cl.h>
#include <clgetValp.h>
#include <clsh.h>
#include <clinteract.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <readline/readline.h>
#include <cstdlib>
#include <fstream>

string cmd_g;
bool firstPass_g=true;
size_t cmd_pos_g=0;
//
//--------------------------------------------------------------------
// Factored out re-usable code into global functions
//
auto makeDefFile=[](std::vector<std::string>& args,
		    std::string defFile=std::string(),
		    std::string help=std::string())
{
  if (!defFile.empty())
    {
      std::ofstream ofs(defFile);
      for(int i=1;i<args.size();i++) ofs << args[i] << endl;
    }

  if (!help.empty()) args.push_back(help);
};
//
//--------------------------------------------------------------------
//
auto canonicalArgs=[]()
{
  std::vector<std::string> args
    {
     "test2",
     "bool=true",
     "bool1=false",
     "int=42",
     "dbgint=77",
     "float=2.71",
     "oneint=atan(1)*4/PI*100+23",
     "string=showstrarr",
     "strarr=val1,val2",
     "fullval=custom_value",
     "dbgfullval=debug_value",
     "farray=9.9,8.8,7.7"
    };

  return args;
};
//
//--------------------------------------------------------------------
//
auto makeCanonicalArgs=[](std::string defFile=std::string(),
			  std::string help=std::string())
{
  auto args=canonicalArgs();
  makeDefFile(args,defFile,help);

  return args;
};
//
//--------------------------------------------------------------------
//
auto FactoryCanonicalTest=[]()
{
  int i;

  // bool
  bool b = false;
  SMap watchPoints;
  VString exposedKeys = {"bool1"};
  watchPoints["1"] = exposedKeys;
  i = 1;
  clgetValp("bool", b, i, watchPoints);
  EXPECT_FALSE(b);

  // bool1
  bool b1 = true;
  ClearMap(watchPoints);
  exposedKeys = {"int"};
  watchPoints["0"] = exposedKeys;
  i = 1;
  clgetValp("bool1", b1, i, watchPoints);
  EXPECT_TRUE(b1);

  // int
  int intVal = 0;
  ClearMap(watchPoints);
  exposedKeys = {"float"};
  watchPoints["1"] = exposedKeys;
  i = 1;
  clgetValp("int", intVal, i, watchPoints);
  EXPECT_EQ(intVal, 0);

  // dbgint
  int dbgInt = 10;
  i = 1;
  cldbggetValp("dbgint", dbgInt, i);
  EXPECT_EQ(dbgInt, 10);

  // float
  float fVal = 10.96f;
  i = 1;
  clgetValp("float", fVal, i);
  EXPECT_FLOAT_EQ(fVal, 10.96f);

  // oneint
  int oneintVal = 3;
  i = 1;
  clgetValp("oneint", oneintVal, i);
  EXPECT_EQ(oneintVal, 3);

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

  // {
  //   char sstr[100];
  //   clgetSVal("string", sstr, &i, watchPoints);
  //   //EXPECT_EQ(sstr, "showstrarr");// DOES NOT WORK!
  //   EXPECT_EQ(sstr, str);

  //   clgetSVal("string", sstr, &i);
  //   //EXPECT_EQ(sstr, "showstrarr");// DOES NOT WORK!
  //   EXPECT_EQ(sstr, str);
  // }

  // fullval
  std::string fullVal;
  i = 0;
  clgetFullValp("fullval", fullVal);
  EXPECT_EQ(fullVal, "");

  // dbgfullval
  std::string dbgFullVal="This is dbg full val factory setting";
  i = 0;
  dbgclgetFullValp("dbgfullval", dbgFullVal);
  EXPECT_EQ(dbgFullVal, "This is dbg full val factory setting");

  // strarr
  VString strarr={"v1","v2"};
  i = 0;
  clgetValp("strarr", strarr, i);
  ASSERT_EQ(strarr.size(), 2u);
  EXPECT_EQ(strarr[0], "v1");
  EXPECT_EQ(strarr[1], "v2");

  // farray
  std::vector<float> fv={3.14,2*3.14,3*3.14};

  cerr << "########## " << fv.size() << endl;
  int N = 0;
  int count = clgetValp("farray", fv, N);
  EXPECT_EQ(count, 3);
  EXPECT_FLOAT_EQ(fv[0], 3.14f);
  EXPECT_FLOAT_EQ(fv[1], 2*3.14f);
  EXPECT_FLOAT_EQ(fv[2], 3*3.14f);
};
//
//--------------------------------------------------------------------
//
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

  {
    char sstr[100];
    clgetSVal("string", sstr, &i, watchPoints);
    //EXPECT_EQ(sstr, "showstrarr");// DOES NOT WORK!
    EXPECT_EQ(sstr, str);

    clgetSVal("string", sstr, &i);
    //EXPECT_EQ(sstr, "showstrarr");// DOES NOT WORK!
    EXPECT_EQ(sstr, str);
  }

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
  std::vector<float> fv;
  int N = 3;
  int count = clgetValp("farray", fv, N);
  EXPECT_EQ(count, 3);
  EXPECT_FLOAT_EQ(fv[0], 9.9f);
  EXPECT_FLOAT_EQ(fv[1], 8.8f);
  EXPECT_FLOAT_EQ(fv[2], 7.7f);
};
//
//--------------------------------------------------------------------
//
class ParafeedTest : public ::testing::Test
{
public:
  ~ParafeedTest()
  {
    clCleanUp();
    // Reset the input mechanism for the scanner/parser
    set_shell_input(backup_cl_shell_input_g);
    firstPass_g=true;
    cmd_pos_g=0;
  }
  //
  //------------------------------------------------------------------
  // Function that can supply the input to the scanner/parser from a
  // string (here the global cmd_g variable).
  static int test_shell_inp(char *buf, size_t max_size)
  {
    if (!firstPass_g || cmd_pos_g >= cmd_g.size()) return 0;

    const size_t n_to_copy = std::min(max_size, cmd_g.size() - cmd_pos_g);
    std::memcpy(buf, cmd_g.data() + cmd_pos_g, n_to_copy);
    cmd_pos_g += n_to_copy;
    if (cmd_pos_g >= cmd_g.size()) firstPass_g=false;

    // Show the buffer passed to the caller (shell.l::YYINPUT).
    cerr << "< ";
    for(uint i=0;i<n_to_copy;i++) cerr << buf[i];
    cerr << "---------------" << endl;

    return static_cast<int>(n_to_copy);
  }
  //
  //------------------------------------------------------------------
  // Set the string for the scanner in test_shell_inp().  Save the
  // current input function for the scanner before resetting the
  // scanner
  void sendCmd(const string& cmd)
  {
    cmd_g=cmd;
    firstPass_g=true;
    cmd_pos_g=0;
    backup_cl_shell_input_g=get_shell_input();
    set_shell_input(test_shell_inp);
  }

  std::function<int (char *, size_t)> backup_cl_shell_input_g;

protected:
  // C-style argv array
  std::vector<char*> argv_p;

  //
  // Code inspired from an example on Stackoverflow.  This does not
  // need explicit memory allocation, and therefore is leak-free.
  // However, the generated argc,argv pair is not available outside
  // the scope if the argv_p variable (which currently is the class
  // scope).
  //
  std::pair<int, char**> MakeArgv(std::vector<std::string>& string_args)
  {
    argv_p.resize(0);
    // .data() provides the char* pointer needed for C-style arrays
    // push these into the class data member, available for the life
    // of the class instance.
    for (auto& s : string_args)
#if __cplusplus == 199711L
      // Code specific to C++98
      argv_p.push_back((char *)(s.c_str()));
#else
      argv_p.push_back(s.data());
#endif

    // The following was in the original example, but seems not necessary
    //    argv.push_back(nullptr);

    return std::pair<int, char**>(static_cast<int>(argv_p.size()), argv_p.data());
  }

  // Dummy method.
  inline void FreeArgv(int argc, char **argv) {};

  //
  // The code below generates the argc,argv pair that is usable
  // outside the class scope, but it requires explicity freeing the
  // allocate memory without which there will be a memory leak.
  //
  // std::pair<int, char**> MakeArgv(const std::vector<std::string>& args)
  // {
  //   char** argv = new char*[args.size()];
  //   for (size_t i = 0; i < args.size(); ++i)
  //     {
  // 	argv[i] = new char[args[i].size() + 1];
  // 	std::strcpy(argv[i], args[i].c_str());
  //     }
  //   return {static_cast<int>(args.size()), argv};
  // }
  // void FreeArgv(int argc, char** argv)
  // {
  //   for (int i = 0; i < argc; ++i)
  //     {
  // 	delete[] argv[i];
  //     }
  //   delete[] argv;
  // }
};
