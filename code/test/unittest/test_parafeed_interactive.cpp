#include <unittest/ParafeedTest.h>
//
//--------------------------------------------------------------------
// This does not yet work as expected.
//
TEST_F(ParafeedTest, Interactive) {
    std::vector<std::string> args = {
        "test2",
        //"help=noprompt",
        "bool=1",               // Wrong data type
        "oneint=1",              // Wrong data type
        "string=showstrarr",
        "strarr=foo,bar",
        "farray=1,3,4,5,6,7,8,9,10" // One wrong data type
    };
    auto [argc, argv] = MakeArgv(args);

    clCleanUp();
    sendCmd("bool=true\noneint=100\ninp\ngo\n");
    //sendCmd("go\n");
    BeginCL(argc, argv);
    clInteractive(1);

    bool b = false;
    int oneint = 0;
    std::string s;
    std::vector<std::string> strarr;
    std::vector<float> farray(10);
    int i = 1;

    //    EXPECT_THROW(clgetValp("bool", b, i),clError);
    clgetValp("bool", b, i);
    
    // Should fail due to wrong type 
    clgetValp("oneint", oneint, i);
    
    clgetValp("string", s, i);
    int idx = 0;
    clgetValp("strarr", strarr, idx);
    int N = 10;

    clgetValp("farray", farray, N);
    
    EndCL();

    EXPECT_EQ(oneint,100);

    FreeArgv(argc, argv);
}
