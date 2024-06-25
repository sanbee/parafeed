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
/* $Id: support.c,v 2.3 1999/01/25 04:03:33 sanjay Exp sanjay $ */
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cl.h>
#include <clGlobals.h>
#include <sstream>
#include <clstring.h>
#include <setAutoDefaults.h>

#include <regex>
  /*---------------------------------------------------------------------------*/
  std::string makeProgName(const std::string& name,
			   const std::string& def)
  {
    std::string out=def;
#ifdef GNUREADLINE
    out=name.substr(0,name.size()-1);
#else
    out=name;
#endif
    return out;
  }
#ifdef __cplusplus
extern "C" {
#endif
  /*---------------------------------------------------------------------------*/
  string clMakeDefaultsFilename(int complement)
  {
    string fname;
//     char out[FILENAME_MAX+2]="./";
// #ifdef GNUREADLINE
//     strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
// #else
//     strcat(out,cl_ProgName);
// #endif
    fname = makeProgName(std::string(cl_ProgName),std::string("./"));
    fname = fname + ".def";
    if (complement) fname = fname + "!";
    return fname;
  }
  /*----------------------------------------------------------------------
    Puts a NULL character after the first alpha-numeric character 
    that it finds in buf, starting from the back.
    ----------------------------------------------------------------------*/
  void cltruncateFromBack(char *buf)
  {
    int n;
    n=strlen(buf)-1;
    
    while(((buf[n] == '\t') || (buf[n] == ' ') || (buf[n] == '\n')) && (n >= 0))
      n--;
    n++;
    if (n>0) buf[n] = '\0';
  }
  /*----------------------------------------------------------------------
    Loads the source in the target buffer.  The mem. for target
    is allocated inside by malloc.  If source==NULL, the source
    is filled with default given by def.
    ----------------------------------------------------------------------*/
  void clloadBuf(char **target, char *source, char *def)
  {
    int n;
    n = strlen(source);
    if (n<=0) 
      {
	if (def != NULL) 
	  {
	    n = strlen(def);
	    if (*target != NULL) free(*target);
	    *target = (char *)getmem(sizeof(char)*(n+1),"loadBuf");
	    strcpy(*target,def);
	  }
	else
	  {
	    if (*target != NULL) free(*target);
	    *target = NULL;
	  }
      }
    else
      {
	if (*target != NULL) free(*target);
	*target = (char *)getmem(sizeof(char)*(n+1),"loadBuf");
	strcpy(*target,source);
      }
    source[0]='\0';
  }
  /*----------------------------------------------------------------------
    Strip any leading white spaces (' ',TAB).  This is useful while 
    reading strings typed by humans.
    ----------------------------------------------------------------------*/
  void stripwhitep (std::string& str)
  {
    string tmp(str);
    auto beg=tmp.begin(),
      end=tmp.end();

    for(;beg != end; beg++)
      if (!std::isspace(*beg)) break;

    for(;end != beg;end--)
      if (!std::isspace(*end)) break;

    str = std::string(beg,end);
  }

  void stripwhite (char *string)
  {
    int i = 0;
    
    if (string!=NULL)
      {
	while (string[i] == ' ' || string[i]=='\t') i++;
	if (i)
	  {
	    std::string tmp=(string+i);
	    strcpy(string,tmp.c_str());
	  }
	//	if (i) strcpy (string, string + i);
	i = strlen (string) - 1;
	while (i > 0 && (string[i]==' '||string[i]=='\t')) i--;
	string[++i] = '\0';
      }
  }
  /*----------------------------------------------------------------------
    Break a string of the type <Name>=<Value> into Name and Value.
    ----------------------------------------------------------------------*/
  int BreakStrp(const std::string& str,
		std::string& Name, std::string& val)
  {
    auto i=str.begin();
    for(i=str.begin(); i != str.end(); i++)
      if (*i == '=') break;
    
    if (str.begin() != i)
      Name = std::string(str.begin(),i);

    if (i != str.end())
      {
	i++;
	val = std::string(i,str.end());
      }
    return 1;
  }
  int BreakStr(char *str, char **Name, char **val)
  {
    char *t,*off;
    
    if ((off = strchr(str,'='))) off++;
    
    if ((t=strtok(str,"="))!=NULL)
      {
	stripwhite(t);
	*Name = (char *)getmem(strlen(t)+1,"BreakStr");
	strcpy(*Name,t);
      }
    else *Name = NULL;
    if (off)
      {
	stripwhite(off);
	*val = (char *)getmem(strlen(off)+1,"BreakStr");
	strcpy(*val,off);
      }
    else *val = NULL;
    return 1;
  }
  /*----------------------------------------------------------------------
    Redirect the stdout and stderr to files given by out and err.  This is
    useful when wanting to put an application in the background.
    ----------------------------------------------------------------------*/
  int redirect(char *out, char *err)
  {
    int Same=0,OUT=1, ERR=1;
    int fd,fd1;
    
    if (out == NULL) OUT = 0;
    if (err == NULL) ERR = 0;
    
    if (OUT && ERR) 
      {if (!strcmp(out,err)) Same=1; else Same=0;}
    else
      Same=0;
    
    if (OUT)
      {
	if ((fd=creat(out,0644))<0)
	  {
	    perror(out);
	    clThrowUp("In STDOUT redirection.","###Error",
		      CL_SEVERE);
	  }
	close(1);
	dup(fd);
      }
    
    if (ERR)
      {
	if (!Same)
	  {
	    if ((fd1=creat(err,0644))<0)
	      {
		perror(out);
		/*
		  if (clThrowUp("In STDERR redirection.","###Error",
		  CL_SEVERE)==CL_SEVERE)
		  exit(-1);
		*/
		clThrowUp("In STDERR redirection.","###Error",
			  CL_SEVERE);
	      }
	    close(2);
	    dup(fd1);
	  }
	else
	  {
	    close(2);
	    dup(fd);
	  }
      }
    return 1;
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetIValp() call.
    ----------------------------------------------------------------------*/
  void setAutoIDefaults(Symbol *S, const int& val)
  {setAutoDefaults(S,val);}
  void setAutoBDefaults(Symbol *S, const bool& val)
  {setAutoDefaults(S,val);}
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetFValp() call.
    ----------------------------------------------------------------------*/
  void setAutoFDefaults(Symbol *S, const float& val)
  {setAutoDefaults(S,val);}
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetSValp() call.
    ----------------------------------------------------------------------*/
  void setAutoSDefaults(Symbol *S, const string& val, const int fullVal)
  {
    if (fullVal)
      {
	//char *valstr; valstr=(char *)val.c_str();
	vector<string> tokens;
	tokens=clstrtokp(val,',',CL_ESC);

	unsigned ntok=tokens.size();
	if (cl_RegistrationMode==1)
	  {
	    S->DefaultVal.resize(ntok);
	    for(unsigned i=0;i<ntok;i++)
	      S->DefaultVal[i]=tokens[i];
	  }
      }
    else
      {
    	setAutoDefaults(S,val);
      }
  }
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetNIValp() call.
    ----------------------------------------------------------------------*/
  void setAutoNIDefaults(Symbol *S, const vector<int>& val)
  {setAutoDefaults(S,val);}
  /*----------------------------------------------------------------------*/
  void setAutoNBDefaults(Symbol *S, const vector<bool>& val)
  {setAutoDefaults(S,val);}
  /*----------------------------------------------------------------------
    Set the defaults value of the given symbol to the value of the 
    in which the user value is returned to the application layer via
    clgetNFValp() call.
    ----------------------------------------------------------------------*/
  void setAutoNFDefaults(Symbol *S, const vector<float>& val)
  {setAutoDefaults(S,val);}
#ifdef __cplusplus
}
#endif
/*----------------------------------------------------------------------*/
std::vector<std::string> stokenize(const std::string& in, const std::regex& rgx)
{
  //std::regex rgx("\\s+");
  std::sregex_token_iterator iter(in.begin(),
				  in.end(),
				  rgx,
				  -1);
  std::sregex_token_iterator end;
  std::vector<string> out;
  for ( ; iter != end; ++iter)
    out.push_back(*iter);
  return out;
}
/*----------------------------------------------------------------------*/
// Recursively show exposed keys.  Printing is done using the supplied
// printer.
void showExposedKeys(Symbol* t, const bool& showAll,
		     std::function<void(FILE*, Symbol*)> printer
		     )
{
  vector<string> mapVal;
  //checkVal(t,mapVal);
  if (!t->smap.empty())
    {
      // Iterate over all entires in the smap for the symbol
      for(auto smap : t->smap)
	{
	  //	mapVal = (t->smap.begin())->second;
	  mapVal = smap.second;
	  // Iterate over all keys in the smap for the symbol
	  for (auto key : mapVal)
	    {
	      Symbol *S;
	      S=SearchVSymb((char *)key.c_str(),cl_SymbTab);
	      if (S==NULL) break;
	      //S=SearchVSymb(iarg.c_str(),cl_SymbTab);
	      if (((S->Exposed || showAll) && (S->Class == CL_APPLNCLASS)) ||
		  (((S->Class == CL_DBGCLASS) && (CL_DBG_ON))))
		{
		  printer(stderr,S);
		}
	      // Recusively show watched keys, if exposed by the current
	      // value of the parent key or if showAll==True.
	      showExposedKeys(S,showAll,printer);
	    }
	}
    }
}
/*----------------------------------------------------------------------*/
// A generic function for display keys as listed in arg, using the
// supplied printer function.
int showKeys(char *arg,
	     std::function<void(FILE*, Symbol*)> printer)
{
  Symbol *t;
  //
  // First ensure that the key-exposure algorithm is executed (it
  // can be run anywhere any number of times).
  //
  for (t=cl_SymbTab;t;t=t->Next) exposeKeys(t);
  
  std::vector<std::string> sv;
  if (arg) sv = stokenize(string(arg), std::regex("\\s+"));
  //    
  // Now print the viewable keywords. The code below is a little
  // state-machine (just about at the level that the author can code
  // by-hand).
  //

  // Lambda function to throw an exception if SearchVSymb returns
  // NULL. If checkExposure=true, throw the exception if the symbol is
  // found but is not exposed. If the exception is thrown the return
  // value is also NULL.
  auto SearchVSymb_l = [](const string& name,
			  Symbol *tab,
			  const bool checkExposure=false) -> Symbol*
		       {
			 Symbol *t=NULL;
			 if (((t=SearchVSymb(name.c_str(),cl_SymbTab))==NULL) ||
			     ((!t->Exposed) && checkExposure))
			   {
			     t=NULL;
			     string mesg = "Key (\""+name+"\") not found";
			     if (checkExposure) mesg = mesg+" or is not currently exposed";
			     clThrowUp(mesg.c_str(),"###Infomational",CL_INFORMATIONAL);
			   }
			 return t;
		       };

  if (sv.size()==0)//arg == NULL)
    for (t=cl_SymbTab;t;t=t->Next)
      {
	if ((t->Exposed) && 
	    ((t->Class==CL_APPLNCLASS) || 
	     ((t->Class==CL_DBGCLASS) && (CL_DBG_ON))))
	  {
	    printer(stderr,t);
	  }
      }
  // Single argument.  It can be "-a" or name of a key. 
  else if (sv.size()==1)
    {
      if (sv[0]=="-a") // Apply -a on all keys
	for (t=cl_SymbTab;t;t=t->Next)
	  {
	    if ((t->Class==CL_APPLNCLASS) || 
		((t->Class==CL_DBGCLASS) && (CL_DBG_ON)))
	      {
		printer(stderr,t);
	      }
	  }
      else // Print the single given key
	{
	  if (t=SearchVSymb_l(sv[0],cl_SymbTab,true))
	    printer(stderr,t);
	}
    }
  // Multiple arguments.  E.g. "-a name1 name2 ..."
  //
  // An internal option to print the tree of exposed keys associated
  // with the symbol t
  else if (sv[0]=="-t") 
    {
      sv.erase(sv.begin());
      for(auto iarg : sv)
	{
	  // Recusrively show keys associated with the root key that
	  // are exposed with its current setting.
	  if (t=SearchVSymb_l(iarg,cl_SymbTab,false))
	    showExposedKeys(t,false,printer);
	}
    }
  else
    {
      bool showAll=false;
      if (sv[0]=="-a")
	{
	  showAll=true;
	  sv.erase(sv.begin());
	}
      //	t=SearchVSymb(sv[0].c_str(),cl_SymbTab);
      for(auto iarg : sv)
	{
	  if ((t=SearchVSymb_l(iarg,cl_SymbTab,false))!=NULL)
	    {
	      printer(stderr,t);
	      // Recusrively show keys associated with the root key that
	      // are exposed with its current setting.
	      showExposedKeys(t,showAll,printer);
	    }
	}
    }
  return 1;
}
/*----------------------------------------------------------------------*/
bool checkVal(Symbol* t, vector<string>& mapVal)
{
  bool found=false;
  SMap::iterator loc;
  try
    {
      if (ISSET(t->Attributes,CL_BOOLTYPE))
	{
	  //
	  // This is a BOOLTYPE keyword.  Check for logical
	  // true/false.  String comparision is not enough
	  // (e.g. string "0" and "no" are both logical false).
	  //
	  for(loc=t->smap.begin(); loc!=t->smap.end(); loc++)
	    {
	      bool logicalKey = clIsTrue((*loc).first.c_str());
	      // Return true if t->Val[0] string maps to the logical value in logicalKey.
	      // E.g., t->Val0]="0" and logicalKey=False will return true.
	      // bool symbolBoolVal = clBoolCmp(t->Val[0],logicalKey);
	      // if ((found = (symbolBoolVal==logicalKey))) break;
	      if ((found = clBoolCmp(t->Val[0],logicalKey))) break;
	    }
	}
      else
	{
	  //
	  // For all other types, check by string comparision only.
	  //
	  if (t->Val.size() > 0)
	    {
	      loc = t->smap.find(string(t->Val[0]));
	      found = (loc != t->smap.end());
	    }
	}
      if (found) mapVal=(*loc).second;
    }
  catch (clError &x)
    {
      x << x.what() << endl;
    }
  return found;
}
/*----------------------------------------------------------------------*/
int exposeKeys(Symbol *t)
{
  Symbol *S;
  int exposedSomething=0;
  /*    char *name = t->Name;*/
  
  if ((t->smap.size() > 0))
    {
      //
      // Irrespective of the current value of the symbol, hide all
      // keys on which a watch was set by this symbol
      //
      if (ISSET(t->Attributes,CL_HIDDENKEYWORD))
	SETBIT(t->Attributes,CL_HIDENSEEKKEYWORD);
      else
	SETBIT(t->Attributes,CL_HIDINGKEYWORD);
      for(SMap::iterator i=t->smap.begin(); i != t->smap.end(); i++)
	{
	  vector<string> sv=(*i).second;
	  for(unsigned int j=0;j<sv.size();j++)
	    {
	      if ((S=SearchVSymb((char *)sv[j].c_str(),cl_SymbTab))==NULL)
		{
		  string mesg = "Programmer error: Watch key \"" + sv[j] + "\" not found.";
		  clThrowUp(mesg.c_str(),"###Fatal ",CL_FATAL);
		}
	      S->Exposed=0;
	      SETBIT(S->Attributes,CL_HIDDENKEYWORD);
	    }
	}
      //
      // Now set for display those watched-keys which are exposed by
      // the current setting of this symbol. If the current symbol
      // (t) is not exposed itself, the keys are that watching
      // remain unexposed. For now, the "current setting" is only
      // the first value (i.e. ignores other possible comma
      // seperated values).
      //
      
      if ((t->NVals > 0) && (t->Exposed==1))
	{
	  // SMap::iterator loc = (t->smap.find(string(t->Val[0])));
	  // if (loc != t->smap.end())
	  vector<string> mapVal;
	  if (checkVal(t,mapVal))
	    {
	      //		vector<string> sv=(*loc).second;
	      vector<string> sv=mapVal;
	      for(unsigned int j=0;j<sv.size();j++)
		{
		  S=SearchVSymb((char*)sv[j].c_str(),cl_SymbTab);
		  S->Exposed=1;
		  exposedSomething=1;
		  //
		  // Recursively expose the keys.
		  //
		  exposeKeys(S);
		  //
		  // Remove the CL_KEYWORD attribute if a keyword is
		  // hidden due to a setting of another keyword which 
		  // is made into a shell constant (by the .config
		  // file).  Such a keyword should not be colour coded
		  // (since the other keyword which will hide/expose
		  // this keyword is not exposed itself...hence user
		  // has really no control on the exposure of these
		  // keywords).
		  // 
		  if (t->Class==CL_USERCLASS) 
		    {
		      RESETBIT(S->Attributes,CL_KEYWORD);
		    }
		}
	    }
	}
    }
  return exposedSomething;
}
/*----------------------------------------------------------------------*/
int loadDefaults(int complement)
{
  FILE *fd;
    
  /*
    First, do a complimentary load from the .def file available
    locally.
  */
    
//   char out[FILENAME_MAX+2]="./", *t;
// #ifdef GNUREADLINE
//   strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
// #else
//   strcat(out,cl_ProgName);
// #endif
  // strcat(out,".def");

  std::string out=makeProgName(std::string(cl_ProgName),std::string("./"))+std::string(".def");
    
  if ((fd = fopen((const char*)out.c_str(),"r")) != NULL)
    {
      fclose(fd);
      if (complement) out+=std::string("!");//strcat(out.c_str(),"!");  /* Perform a complimentery load */
      doload((char*)out.c_str());
    }
    
  /*
    If CL_DEFAULTSENV env. var. is set, look for a .def file
    there and if found, do a complimentary load from there too.
  */

  char *t;
  t=(char *)getenv(CL_DEFAULTSENV);
  //  if (t && strlen(t)) {strncpy(out,t,FILENAME_MAX);strcat(out,"/");}
  if (t && strlen(t))
    {
      out=std::string(t)+std::string("/");
      // strncpy(out,t,FILENAME_MAX);
      // strcat(out,"/");
    }
// #ifdef GNUREADLINE
//   strncat(out,cl_ProgName,strlen(cl_ProgName)-1);
// #else
//   strcat(out,cl_ProgName);
// #endif
//   strcat(out,".def");

  out=out+makeProgName(std::string(cl_ProgName),std::string())+std::string(".def");
    
  if ((fd = fopen((char *)out.c_str(),"r")) != NULL)
    {
      fclose(fd);  
      //	if (complement) 
      out+=std::string("!");
      //strcat(out,"!");  /* Perform a complimentery load */
      doload((char *)out.c_str());
    }
    
  /*
    Final effect will be that first any commandline setting will be
    set.  Next, the local .def file will be hououred for all those
    keys which still remain unset.  Next, .def file from CL_DEFAULTSENV
    area will he honoured for those keys which continue to remain
    unset.  */
    
  return 1;
}
