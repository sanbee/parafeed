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
