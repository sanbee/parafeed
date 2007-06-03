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
#ifdef GNUREADLINE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <cl.h>
#include <cllib.h> // Get the extern definitions for cl_SymbTab
#include <shell.h>
#include <string>
#include <vector>
#include <support.h>
#include <clGlobals.h>
#include <clhashdefines.h>
#ifdef __cplusplus
extern "C" {
#endif
  //------------------------------------------------------------------------
  // Use getmem to make a copy of a string.  getmem uses malloc to
  // make a copy.  Readline uses free to release the memory.
  //
  char *dupstr (char *s)
  {
    char *r;
    
    r = (char *)getmem(sizeof(char *)*(strlen (s) + 1),"dupstr");
    return strcpy (r, s);
    //    return (r);
  }
  //------------------------------------------------------------------------
  Symbol* rl_isKeyword(char *s)
  {
    string tmp;
    int len=0;
    while( (s[len] != '=') && (s[len] != ' ') ) len++;
    tmp.resize(len);
    int j=0;
    for(int i=0;i<len;i++)
      tmp[j++]=s[i];
    tmp[j]=(char)NULL;
    Symbol *S=NULL;
    if (tmp.size() > 0) S=SearchVSymb((char *)tmp.c_str(),cl_SymbTab);
    return S;
  }
  //------------------------------------------------------------------------
  CmdSymbol* rl_isCmd(char *s)
  {
    string tmp;
    int len=0;
    while( (s[len] != '=') && (s[len] != ' ') ) len++;
    tmp.resize(len);
    int j=0;
    for(int i=0;i<len;i++)
      tmp[j++]=s[i];
    tmp[j]=(char)NULL;
    CmdSymbol *S=NULL;
    if (tmp.size() > 0) S=SearchCSymb((char *)tmp.c_str(),sh_CmdTab);
    return S;
  }
  //------------------------------------------------------------------------
  // CL command generator function.
  // 
  char* rl_cmd_generator(const char *text,int state)
  {
    static CmdSymbol *C=NULL;
    char *tmp=NULL;
    int m,len=strlen(text);
    if (state == 0) {C=sh_CmdTab;}

    while(C != NULL)
      {
	tmp = C->Name;
	m=strncmp (C->Name, text, len);
	C=C->Next;
	if (m==0) return dupstr(tmp);
      }
    return NULL;
  }
  //------------------------------------------------------------------------
  // CL Keyword generator function.
  // 
  char* rl_keyword_generator(const char *text,int state)
  {
    static Symbol *S;
    char *tmp=NULL;
    int m,len=strlen(text);
    if (state == 0) S = cl_SymbTab;
    //
    // First exhaust keyword list...
    //    
    while((S != NULL))
      {
	int exposed,showdbg=1,viewable;
	//
	// Set/unset any keys watched by this symbol.  Only exposed
	// keys are then available for completion.
	//
	exposeKeys(S);
	m=strncmp (S->Name, text, len);
	tmp = S->Name;
	exposed=S->Exposed;
	if (S->Class==CL_DBGCLASS) if (CL_DBG_ON) showdbg=1; else showdbg=0;
	viewable = ((m==0) && (exposed==1) && showdbg && (S->Class!=CL_USERCLASS));
        
	S = S->Next;
	if (viewable) return dupstr(tmp);
      }
    return NULL;
  }
  //------------------------------------------------------------------------
  // CL Keyword and command generator function.
  // 
  char* rl_keyword_cmd_generator(const char *text,int state)
  {
    static Symbol *S;
    static CmdSymbol *C;
    char *tmp=NULL;
    int m,len=strlen(text);
    if (state == 0) {S = cl_SymbTab; C=sh_CmdTab;}
    //
    // First exhaust keyword list...
    //    
    while((S != NULL))
      {
	int exposed,showdbg=1,viewable;
	//
	// Set/unset any keys watched by this symbol.  Only exposed
	// keys are then available for completion.
	//
	exposeKeys(S);
	m=strncmp (S->Name, text, len);
	tmp = S->Name;
	exposed=S->Exposed;
	if (S->Class==CL_DBGCLASS) if (CL_DBG_ON) showdbg=1; else showdbg=0;
	viewable = ((m==0) && (exposed==1) && showdbg && (S->Class!=CL_USERCLASS));
        
	S = S->Next;
	if (viewable) return dupstr(tmp);
      }
    //
    // ...then shell command names.
    //
    while(C != NULL)
      {
	tmp = C->Name;
	m=strncmp (C->Name, text, len);
	C=C->Next;
	if (m==0) return dupstr(tmp);
      }
    return NULL;
  }
  //------------------------------------------------------------------------
  // Generate a context sensitive list of options.
  //
  char *rl_options_generator(const char *text, int state)
  {
    static VString options;
    static int which;
    static int isKeyWord=0;
    if (state==0) 
      {
	Symbol *S=rl_isKeyword((char *)rl_line_buffer);
	if (S != NULL) {options = S->Options; isKeyWord=1;};
	which=0;
      }
    int n=options.size(),len=strlen(text);
    while(which < n)
      {
	which++;
	int m;
	if ((m=strncmp((char *)(options[which-1].c_str()), text, len))==0)
	  return dupstr((char *)(options[which-1].c_str()));
      }
    return NULL;
  }
  //------------------------------------------------------------------------
  // CL completor.  Completes keywords first. Followed by options completion 
  // (if options are available).  And then switches to filename completion.
  //
  // The pseudo code for the completion logic is as follows:
  //
  // if in the initialization state
  //   check if the rl_line_buffer matches a CL keyword.
  //   if ((CL Keyword found) and (the last char. in the rl_line_buffer == ' ')
  //       and (there is no '=' sign in the rl_line_buffer))
  //      replace the last char in rl_line_buffer with '=';
  //
  //   if (CL Keyword found)
  //     if the associated Symbol has the list of options available
  //         attempt options completion
  //     else
  //         attempt filename completion
  //   else
  //     attempt keyword completion
  // else
  //   attempt keyword completion
  //
  char **cl_completor(const char *text, int start, int end)
  {
    char **matches;
    matches = (char **)NULL;
    char *rlLine=(char *)rl_line_buffer;

    if (start > 0) 
      {
	Symbol *S=rl_isKeyword(rlLine);
	CmdSymbol *C=rl_isCmd(rlLine);
	if ((S != NULL ) && 
	    (rl_line_buffer[rl_point-1] == ' ') && 
	    (strchr(rl_line_buffer,'=') == NULL))
	  rl_line_buffer[rl_point-1] = '=';
	if (S != NULL)
	  {
	    if (S->Options.size() > 0)
	      matches = rl_completion_matches(text,rl_options_generator);
	    else if (ISSET(S->Attributes,CL_STRINGTYPE))
	      matches=rl_completion_matches(text,rl_filename_completion_function);
	    else   
	      rl_attempted_completion_over = 1;
	  }
	else if (C != NULL)
	  {
	    if (ISSET(C->Attributes,CL_ARG_FILENAME))
	      matches = rl_completion_matches(text,rl_filename_completion_function);
	    else if (ISSET(C->Attributes,CL_ARG_KEYWORD))
	      matches = rl_completion_matches(text,rl_keyword_generator);
	    else   
	      rl_attempted_completion_over = 1;
	  }
	else
	  matches = rl_completion_matches (text, rl_keyword_cmd_generator);
      }
    else
      matches = rl_completion_matches (text, rl_keyword_cmd_generator);

    return (matches);
  }
  //------------------------------------------------------------------------
  // Initialize the readline lib.  Supplies cl_completor as the 
  // completion function.
  //
  // The rl_readline_name bit has been copied from the example in the
  // GNU Readline documentation without much understanding of it's use or 
  // implications (SB ;-)).
  //
  void initialize_readline()
  {
    /* Allow conditional parsing of the ~/.inputrc file. */
    rl_readline_name = cl_ProgName;
    
    /* Tell the completer that we want a crack first. */
    rl_attempted_completion_function = cl_completor;
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
