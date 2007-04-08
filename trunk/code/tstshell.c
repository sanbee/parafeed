#ifdef __cplusplus
  extern "C" {
#endif
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
#include <shell.h>

int sh_lex();
#ifdef __cplusplus
	     }
#endif

int main(void)
{
  while (1) sh_parse();
}
