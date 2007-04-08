/* $Id: clThrowError.c,v 2.1 2001/09/11 05:59:21 sanjay Exp $ */
#include <stdio.h>
#include <cl.h>
#ifdef __cplusplus
#include <iostream.h>
#include <clError.h>
/*----------------------------------------------------------------------*/
void clDefaultErrorHandler()
{
  char *Throes = "###Fatal: Uncaught exception of type ",
    *LastWords = "###Fatal: Exiting the application.";

  try
    {
      throw;
    }
  catch(clError& x)
    {
      x << Throes << "clError" << endl;
      x << "###Fatal: The exceptional message was:" << endl;
      x << "\t\"" <<  x.what() << "\"" << endl;
      x << LastWords << endl;
    }
  catch (...)
    {
      clError x;
      x << Throes << "UNKNOWN" << endl;
      x << LastWords << endl;
    }
  exit(-1);
}


#endif
