/* $Id: clinteract.h,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#if !defined(CLINTERACTIVE_H)
#define CLINTERACTIVE_H
#include <setjmp.h>
/*----------------------------------------------------------------------*/
#define clInteractive(a)    setjmp(__cl_env__);clStartInteractive(&__cl_env__,(a));
jmp_buf __cl_env__;
#endif
#ifdef __cplusplus
	   }
#endif
