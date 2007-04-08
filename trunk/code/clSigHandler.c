/* $Id: clSigHandler.c,v 2.0 1998/11/11 07:12:54 sanjay Exp $ */
#include <signal.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*sighandler_t)(int);
/*----------------------------------------------------------------------*/
void clSigHandler(int SigNum)
{
  switch (SigNum)
    {
    case SIGINT:
      fprintf(stderr,"Got ^C\n");
    }
}

#ifdef __cplusplus
	   }
#endif
