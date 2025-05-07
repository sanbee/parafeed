/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* $Id: tst2.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <cl.h>
#include <clsh.h>
#include <clinteract.h>
#include <stdio.h>
/*
   Test program to test the embedded shell via the commandline library
*/
void UI();

int main(int argc, char **argv)
{
  UI();
}

void UI()
{
  char **argv;
  int argc,i,j=0,N;
  float f,fa[3];
  char str[128]="";
  string strp;
  bool b=false;
  
  argv = (char **)malloc(2*sizeof(char **));
  argv[0]=(char *)malloc(20);
  strcpy(argv[0],"test2");
  argc=0;

  BeginCL(argc,argv);
  clInteractive(1);
  {
    i=1;clgetBVal("bool",&b,&i);
    i=1;clgetIVal("int",&j,&i);
    i=1;clgetFVal("float",&f,&i);
    i=1;clgetSValp("string",strp,i);
    N=3;N=clgetNFVal("farray",fa,&N);
    VString options;
    options.resize(3);
    options[0]="one"; options[1]="two"; options[2]="three";
    clSetOptions("string",options);
  }
  EndCL();
  fprintf(stderr,"Bool = %d \n",b);
  fprintf(stderr,"Float = %f   Int = %d \n",f,j);
  //  fprintf(stderr,"String = %s \n",str);
  cerr << "String = " << strp << endl;
  fprintf(stderr,"Float Array = ");
  for (i=0;i<N;i++) fprintf(stderr,"%f ",fa[i]);
  fprintf(stderr,"\n");
}
