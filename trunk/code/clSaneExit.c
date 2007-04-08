/* $Id: clSaneExit.c,v 2.0 1998/11/11 07:12:54 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
void clSaneExit(int Mode)
{
  doquit("save");
  if (Mode<0) exit(Mode);
}
#ifdef __cplusplus
	   }
#endif
