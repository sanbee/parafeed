/* $Id: clsh.h,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#ifndef CLSHL_H
#define CLSHL_H
#include <cllib.h>

#ifdef	__cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
#define clDeleteCmd(a)   DeleteCmd((a),&cl_CmdTab,&cl_CmdTail);
#define clAddCmd(a,d,f)    AddCmd((a),(d),(f),&cl_CmdTab,&cl_CmdTail);
#ifdef	__cplusplus
}
#endif

#endif
