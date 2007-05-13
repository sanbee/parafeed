/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
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
#include <stdio.h>
#include <string.h>
int GetKeyOffset(char *Path, char *Task,char *Key)
{
  char Name[FILENAME_MAX],Line[80],Tok[4],FKey[128];
  int LineNo=0;
  FILE *fd;
  sprintf(Name,"%s/%s.doc",Path,Task);
  if ((fd=fopen(Name,"r")) == NULL) return -1;
  while (fgets(Line,80,fd)!=NULL)
    {
      sscanf(Line,"%s %s",Tok,FKey);
      if ((Tok[0] == '%') && (!strcmp(FKey,Key)))
	{
	  LineNo *= -1;
	  break;
	}
      LineNo++;
    }
  fclose(fd);
  if (LineNo < 0) return -1*LineNo+1;
  else return -1;
}
  
main()
{
  int N=GetKeyOffset("/home/sanjay/src/New/doc/appdoc","xtract","baselines");
  char Cmd[FILENAME_MAX];
  sprintf(Cmd,"more +%d /home/sanjay/src/New/doc/appdoc/xtract.doc",N);
  system(Cmd);
}
