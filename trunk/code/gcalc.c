#include <stdio.h>
#include <string.h>
#include <cl.h>
extern "C" {
#include <calc.h>
	   }

main(int argc, char *argv[])
{
  char *s=NULL;
  double ans;

  while ((s=readline(">"))!=NULL)
    {
      if (s)
	{
	  stripwhite(s);
	  if (s) add_history(s);
	}
      if (!strncmp(s,"quit",4)) exit(0);
      else if (calc(s,&ans)==0)
	printf("%24.12e\n",ans);
      if (s) free(s);
    }
}
