#include <gtest/gtest.h>
#include <cl.h>
#include <clgetValp.h>
#include <clsh.h>
#include <clinteract.h>
#include <string>
#include <vector>

class ParafeedTest : public ::testing::Test {
protected:
    std::pair<int, char**> MakeArgv(const std::vector<std::string>& args) {
        char** argv = new char*[args.size()];
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = new char[args[i].size() + 1];
            std::strcpy(argv[i], args[i].c_str());
        }
        return {static_cast<int>(args.size()), argv};
    }

    void FreeArgv(int argc, char** argv) {
        for (int i = 0; i < argc; ++i) {
            delete[] argv[i];
        }
        delete[] argv;
    }
};

TEST_F(ParafeedTest, ParsesClgetValpParametersCorrectly) {
    std::vector<std::string> args = {
        "test2",
        "help=noprompt",
        "bool=true",
        "bool1=false",
        "int=42",
        "dbgint=77",
        "float=2.71",
        "oneint=123",
        "string=showstrarr",
        "strarr=val1,val2",
        "fullval=custom_value",
        "dbgfullval=debug_value",
        "farray=9.9,8.8,7.7"
    };
    auto [argc, argv] = MakeArgv(args);

    BeginCL(argc, argv);
    clInteractive(0);

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

    EndCL();
    FreeArgv(argc, argv);
}

// below tests can be included when parafeed throws on these wrong usage.
/*
TEST_F(ParafeedTest, PrintsErrorOnUnknownParameter) {
    std::vector<std::string> args = {
        "test2",
        "help=noprompt",
        "bool=true",
        "unknownparam=badvalue"  // This should cause an error message
    };
    auto [argc, argv] = MakeArgv(args);

    ::testing::internal::CaptureStdout();  // Begin capturing stderr

    BeginCL(argc, argv);
    clInteractive(0);

    // Try accessing only the known parameter
    bool b = false;
    int i = 1;
    clgetValp("bool", b, i);

    std::string unused;
    i = 1;
    clgetSValp("unknownparam", unused, i);

    EndCL();

    std::string errOutput = ::testing::internal::GetCapturedStdout();
    std::cerr << "Captured stderr: \n" << errOutput << std::endl;

    FreeArgv(argc, argv);

    // Check that the error message contains the expected output
    EXPECT_NE(errOutput.find("undefined symbol"), std::string::npos)
        << "Expected error message about undefined symbol not found in stdout:\n" << errOutput;


}
*/


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


TEST_F(ParafeedTest, WrongDataType) {
    std::vector<std::string> args = {
        "test2",
        "help=noprompt",
        "bool=true",
        "oneint=not_int",                // Wrong data type
        "string=showstrarr",
        "strarr=foo,bar",
        "farray=1,2,3,4,5,6,7,8,9,10"
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
    
    // Should fail due to wrong type 
    clgetValp("oneint", oneint, i);

    clgetValp("string", s, i);
    int idx = 0;
    clgetValp("strarr", strarr, idx);
    int N = 10;
    clgetValp("farray", farray, N);

    EndCL();

    std::string output = ::testing::internal::GetCapturedStdout();
    FreeArgv(argc, argv);


    EXPECT_NE(output.find("###Error"), std::string::npos)
        << "Expected error for wrong data type for 'oneint' not found:\n" << output;

}*/


