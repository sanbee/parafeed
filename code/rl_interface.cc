#ifdef GNUREADLINE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <cl.h>
#include <cllib.h> // Get the extern definitions for cl_SymbTab
#include <string>
#include <vector>
#include <support.h>
#include <clGlobals.h>
#ifdef __cplusplus
extern "C" {
#endif
  //------------------------------------------------------------------------
  // Use getmem to make a copy of a string.  getmem uses malloc to
  // make a copy.
  //
  char *dupstr (char *s)
  {
    char *r;
    
    r = (char *)getmem(sizeof(char *)*(strlen (s) + 1),"dupstr");
    strcpy (r, s);
    return (r);
  }
  //------------------------------------------------------------------------
  // CL Keyword generator function.
  // 
  char* rl_keyword_generator(const char *text,int state)
  {
    static Symbol *S;
    if (state == 0) S = cl_SymbTab;
    
    char *tmp=NULL;
    int len=strlen(text),m,exposed,showdbg=1;
    while((S != NULL))
      {
	//
	// Set/unset any keys watched by this symbol.  Only exposed
	// keys are then available for completion.
	//
	exposeKeys(S);
	m=strncmp (S->Name, text, len);
	tmp = S->Name;
	exposed=S->Exposed;
	if (S->Class==CL_DBGCLASS) if (CL_DBG_ON) showdbg=1; else showdbg=0;

	S = S->Next;
	if ((m==0) && (exposed == 1) && showdbg) return dupstr(tmp);
      }
    return NULL;
  }
  //------------------------------------------------------------------------
  // CL completor.  Completes keywords first.  And then switches to
  // filename completion.
  //
  char **cl_command_completor(const char *text, int start, int end)
  {
    char **matches;
    matches = (char **)NULL;
     
    if (start > 0) 
      matches=rl_completion_matches(text,rl_filename_completion_function);
    else
      matches = rl_completion_matches (text, rl_keyword_generator);
    
    return (matches);
  }
  //------------------------------------------------------------------------
  // Initialize the readline lib.  Supplies cl_command_completor as the 
  // completion function.
  //
  void initialize_readline()
  {
    /* Allow conditional parsing of the ~/.inputrc file. */
    //    rl_readline_name = "FileMan";
    
    /* Tell the completer that we want a crack first. */
    rl_attempted_completion_function = cl_command_completor;
  }
  //------------------------------------------------------------------------

#ifdef GNUREADLINE
/*#include <readline/history.h>*/
/************************************************************************/
/*----------------------------------------------------------------------*/
void mkfilename(char *out,char *envvar,char *name,char *type)
{
#ifdef vms
  if(envvar && *envvar)sprintf(out,"%s:%s%s",envvar,name,type);
  else       sprintf(out,"%s%s",name,type);
#else
  char *s;
  if(envvar && *envvar){
    s = (char *)getenv(envvar);
    if(s == NULL)
      fprintf(stderr,"Unable to find environment variable %s.",envvar);
    else 
      sprintf(out,"%s/%s%s",s,name,type);
  }else sprintf(out,"%s%s",name,type);
#endif
}
/************************************************************************/
void save_hist(char *EnvVar, char *Default)
{
  char *HistFile;
  char hfile[FILENAME_MAX];
  if ((HistFile = (char *)getenv(EnvVar)) == NULL)
    HistFile = Default;
  mkfilename(hfile,"HOME",HistFile,"\0");
  write_history(hfile);
}
/************************************************************************/
void limit_hist(char *EnvVar, int Default)
{
  char *NHist;
  int n;
  
  if ((NHist = (char *)getenv(EnvVar)) == NULL)
    n = Default;
  else
    sscanf(NHist, "%d",&n);
  
  stifle_history(n);
}
/************************************************************************/
void load_hist(char *EnvVar, char *Default)
{
  char *HistFile;
  char hfile[FILENAME_MAX];
  static unsigned int Loaded=0;

  if (!Loaded)
    {
      if ((HistFile = (char *)getenv(EnvVar)) == NULL)
	HistFile = Default;
  
      mkfilename(hfile,"HOME",HistFile,"\0");
      read_history(hfile);
      Loaded = !Loaded;
    }
}
/************************************************************************/
/*
void list_hist()
{
  int i;
  register HIST_ENTRY **list = history_list();
  
  if (list)
    for (i = 0; list[i]; i++)
      fprintf (stdout, "%d: %s\n",
	       i + history_base, list[i]->line);
}
*/
#endif



#ifdef __cplusplus
}
#endif

#endif 
