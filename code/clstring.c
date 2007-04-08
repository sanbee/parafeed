/* $Id: clstring.c,v 2.0 1998/11/11 07:13:02 sanjay Exp $ */
#include <string.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
  Given a string, this will return the number of tokens seperated by
  the seperator string sep.  Currently, only the sep[0] is checked for.

  This will recognize strings enclosed in double quotes ('"') and
  characters 'escaped' using char. esc.
------------------------------------------------------------------------*/
int ntok(char *str, char *sep,char esc)
{
  int i,N=strlen(str),count=0;
  int STRING=0;

  for (i=0;i<N;i++)
    {
      if (str[i]=='"' && str[i-1] != esc)
	{if (STRING)  STRING=0;	else STRING=1;}
      if (!STRING && str[i]==sep[0] && (str[i-1] != esc)) count++;
    }
  if (STRING && str[i]=='"' && str[i-1] != esc) STRING=0;
  if (i>=N && STRING) 
    {
      fprintf(stderr,"###Error: Missing \'\"\'\n");
      return -1;
    }
  return count+1;
}
/*------------------------------------------------------------------------
  Given a null terminated string, this will write the string on the file 
  pointed to by fd.  Special characters ',','"' and esc are escaped 
  using the char. esc.
------------------------------------------------------------------------*/
int tokenize(FILE *fd, char *str, char *sep,char esc)
{
  while(*str) 
    {
      if (!strncmp(str,"/*",2))
	  {
	    while(*str && strncmp(str,"*/",2)) fputc(*str++,fd);
	    fputc(*str++,fd);
	  }
      if(*str=='"' || *str==',' || *str == '[' || *str == ']' ||
	 *str == '$' || *str==esc) fputc(esc,fd);
      fputc(*str++,fd);
    }
  return 1;
}
/*------------------------------------------------------------------------
  Same functionality as strtok, except that this will be sensitive to
  'escaped' characters.  The return tokens will not have the escape
  character esc when it is used to 'escape' the following character.
------------------------------------------------------------------------*/
char *clstrtok(char *str, char *sep,char esc)
{
 static int i,len,prev;
 int STRING=0;
 static char *t;
 if (str) {t=str;i=prev=0;len=strlen(str);}
 else {prev=i+1;i++;}

 while (i <= len)
   {
     if (!strncmp(&t[i],"/*",2))
       {
	 while(i < len)
	   if (!strncmp(&t[i++],"*/",2)) 
	     {i++;break;}
       }
     if (!STRING && t[i] == esc) 
       {
	 int j=i+1;
	 while (j<=len) {t[j-1]=t[j];j++;}
	 i++;
       }
     else if (!STRING && t[i]==sep[0]) break;
     else
       {
	 if (t[i]=='"')     
	   {
	     int j=i+1;
	     while (j<=len) {t[j-1]=t[j];j++;}

	     if (STRING) {STRING = 0;break;}
	     else STRING = 1;
	   }
	 i++;
       }
   }
 if (i>=len) i--;
 if (prev >= len) return NULL;
 else t[i]='\0';
 return &t[prev];
} 

#if defined(TESTBED)
main(int argc, char **argv)
{
  char str[128], *c;
  int n,i=0;

  gets(str);
  n = ntok(str,",",'\\');
  printf("No. of tokens detected = %d\n",n);
  c = clstrtok(str,",",'\\');
  while(c)
    {
      fprintf(stdout,"Tokenized token[%d] = ",i);
      tokenize(stdout,c,",",'\\');
      fprintf(stdout,"\nThe raw token[%d] = %s\n",i,c);
      c=clstrtok(NULL,",",'\\');
      i++;
    }
}
#endif
#ifdef __cplusplus
}
#endif
