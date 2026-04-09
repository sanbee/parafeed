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

string cmd_g;
bool firstPass=true;
size_t cmd_pos_g=0;
class ParafeedTest : public ::testing::Test
{
public:
  ~ParafeedTest()
  {
    clCleanUp();
    // Reset the input mechanism for the scanner/parser
    set_shell_input(backup_cl_shell_input_g);
    firstPass=true;
    cmd_pos_g=0;
  }

  //------------------------------------------------------------------
  // Function that can supply the input to the scanner/parser from a
  // string (here the global cmd_g variable).
  static int test_shell_inp(char *buf, size_t max_size)
  {
    if (!firstPass || cmd_pos_g >= cmd_g.size()) return 0;

    const size_t n_to_copy = std::min(max_size, cmd_g.size() - cmd_pos_g);
    std::memcpy(buf, cmd_g.data() + cmd_pos_g, n_to_copy);
    cmd_pos_g += n_to_copy;
    if (cmd_pos_g >= cmd_g.size()) firstPass=false;
    return static_cast<int>(n_to_copy);
  }

  //------------------------------------------------------------------
  // Set the string for the scanner in test_shell_inp().  Save the
  // current input function for the scanner before resetting the
  // scanner
  void sendCmd(const string& cmd)
  {
    cmd_g=cmd;
    firstPass=true;
    cmd_pos_g=0;
    backup_cl_shell_input_g=get_shell_input();
    set_shell_input(test_shell_inp);
  }

  std::function<int (char *, size_t)> backup_cl_shell_input_g;

protected:
  std::pair<int, char**> MakeArgv(const std::vector<std::string>& args)
  {
    char** argv = new char*[args.size()];
    for (size_t i = 0; i < args.size(); ++i)
      {
	argv[i] = new char[args[i].size() + 1];
	std::strcpy(argv[i], args[i].c_str());
      }
    return {static_cast<int>(args.size()), argv};
  }
  void FreeArgv(int argc, char** argv)
  {
    for (int i = 0; i < argc; ++i)
      {
	delete[] argv[i];
      }
    delete[] argv;
  }
};
