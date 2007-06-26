#-*-makefile-*-
include $(GSRCROOT)/Setup/Setup.$(GOSDIR)
#include $(GSRCROOT)/Setup/Setup.$(GOSDIR).personal
#
# The GNU readline library is not required unless the -DHAVE_STRING_H
# and -DGNUREADLINE options are switched on.  If they are switched on,
# the path to the home of GNU readline library should be set correctly.
#
# Set -DFORTRAN as a compiler switch for building libshfcl.a - the
# FORTRAN interface of the CommandLine lib.
# Read the document for the differences and limitations of the 
# FORTRAN interface.
#
# -O? optimization does not seem to work any more!  Not that it's
# required here, but the bug should be tracked down
#
#
# Compiler flags
#
ARCH          = $(GOSDIR)
OPTFLAGS      = -g
DEFINES       = -Wall -DHAVE_STRING_H -DGNUREADLINE
CFLAGS        = $(OPTFLAGS) $(DEFINES) $(COMMONOPTS)
PREFIX        = sh_
#
# Compilers
#
#C++           = g++ $(CFLAGS)
#C++           = /usr/local/compilers/9/gcc_3.2.3/bin/g++ $(CFLAGS)
#CC            = /usr/local/compilers/9/gcc_3.2.3/bin/gcc $(CFLAGS)
#F77           = /usr/local/compilers/9/gcc_3.2.3/bin/g77  
LEX           = flex -Ce -P$(PREFIX)
YACC          = bison -y -p $(PREFIX)
AWK           = awk
#
# Others...
#
READLINEHOME  = /usr/local
PUBLIBS       = $(GLIB)
PINCLUDE      = $(GINCLUDE)
CALCHOME      = ./calc

READLINEINCL  = $(READLINEHOME)/include
INCL          = -I. -I$(READLINEINCL)

GENLIBS       = -L$(READLINEHOME)/lib -lm -lfl \
	        -lreadline -lhistory -ltermcap -lm
LIBS          = $(GENLIBS) $(OFFLIBS)

RANLIB        = /usr/bin/ranlib 

SHOBJS = CopySymb.o AddCmd.o AddNode.o AddVar.o AllocSymb.o AddQKey.o      \
	 DeleteCmd.o DeleteVar.o DeleteNode.o DeleteTab.o FreeSymb.o       \
	 SearchNode.o SearchSymb.o lex.yy.o shell.tab.o TransferTab.o      \
	 PrintVals.o SetVar.o SetPrompt.o SetSymbTab.o SetCmdTab.o         \
	 yyerror.o  minmatch.o

CLOBJS = ParseCmdLine.o clparseVal.o InstallSymb.o clgetFVal.o clgetIVal.o \
	 clgetSVal.o clgetFullVal.o clgetNFVal.o clgetNIVal.o clgetNSVal.o \
	 clgetNVals.o cldbggetFVal.o cldbggetIVal.o cldbggetSVal.o         \
	 cldbggetNFVal.o cldbggetNIVal.o cldbggetNSVal.o cldbggetFullVal.o\
	 clStartInteractive.o clRestartShell.o clfInteractive.o clstring.o \
	 clgetOptsList.o clclearOptsList.o clgetCommandLine.o clgetOpt.o   \
	 clstrtStream.o clloadConfig.o callbacks.o support.o               \
	 clgetInputFile.o clgetOutputFile.o clgetNOpts.o clCleanUp.o       \
	 clReset.o clgetConfigFile.o clSaneExit.o clThrowError.o clRetry.o \
	 clSigHandler.o clIsInteractive.o ErrorObj.o rl_interface.o        \
	 clSetOptions.o clTextColouring.o

FCLOBJS = ParseCmdLine.o InstallSymb.o AddQKey.o clgetFVal.o clgetIVal.o   \
	  clgetSVal.o clgetNFVal.o clgetNIVal.o clgetNSVal.o clgetNVals.o  \
	  clStartInteractive.o clRestartShell.o clfInteractive.o clstring.o\
	  cl.o
#-----------------------------------------------------------------------------
#
# The default target
#
all: libs $(CALCHOME)/libcalc.a libcalc.a 

install: libs
	cp libshccl.a libsh.a $(CALCHOME)/libcalc.a $(PUBLIBS)
	cp *.h $(PINCLUDE)

libs: libshccl.a libsh.a $(CALCHOME)/libcalc.a
#
# Shell related targets.
#
y.tab.c:shell.y
	$(YACC) -d shell.y	

y.tab.h:shell.y y.tab.c
	$(YACC) -d shell.y	

shell.tab.c:y.tab.c
	cp y.tab.c shell.tab.c

shell.tab.h:shell.tab.c
	cp y.tab.h shell.tab.h

lex.yy.c: shell.l shell.tab.h
	$(LEX) shell.l
	sed "s/#define yywrap $(PREFIX)wrap/\/\* #define yywrap $(PREFIX)wrap \*\//"< lex.$(PREFIX).c > lex.yy.c

shell.tab.o: shell.tab.c
	$(C++) -c $(INCL) shell.tab.c

lex.yy.o: lex.yy.c
	$(CC) -c $(INCL) lex.yy.c

SetVar.o: SetVar.c shell.h
	$(C++) -c $(INCL) SetVar.c

SearchSymb.o: SearchSymb.c shell.h
	$(C++) -c $(INCL) SearchSymb.c

SearchNode.o: SearchNode.c shell.h
	$(C++) -c $(INCL) SearchNode.c

PrintVals.o: PrintVals.c shell.h
	$(C++) -c $(INCL) PrintVals.c

yyerror.o:shell.h yyerror.c
	$(C++) -c $(INCL) yyerror.c

FreeSymb.o: shell.h FreeSymb.c clshelldefs.h
	$(C++) -c $(INCL) FreeSymb.c

CopySymb.o: shell.h CopySymb.c clshelldefs.h
	$(C++) -c $(INCL) CopySymb.c

DeleteNode.o: shell.h DeleteNode.c clshelldefs.h
	$(C++) -c $(INCL) DeleteNode.c

DeleteVar.o: shell.h DeleteVar.c clshelldefs.h
	$(C++) -c $(INCL) DeleteVar.c

DeleteCmd.o: shell.h DeleteCmd.c clshelldefs.h
	$(C++) -c $(INCL) DeleteCmd.c

DeleteTab.o: shell.h DeleteTab.c clshelldefs.h
	$(C++) -c $(INCL) DeleteTab.c

AddNode.o: shell.h AddNode.c clshelldefs.h
	$(C++) -c $(INCL) AddNode.c

AddCmd.o: shell.h AddCmd.c clshelldefs.h
	$(C++) -c $(INCL) AddCmd.c

AddVar.o: shell.h AddVar.c clshelldefs.h
	$(C++) -c $(INCL) AddVar.c

AllocSymb.o: shell.h AllocSymb.c clshelldefs.h
	$(C++) -c $(INCL) AllocSymb.c

TransferTab.o:TransferTab.c clshelldefs.h cl.h
	$(C++) -c $(INCL) TransferTab.c

SetPrompt.o:SetPrompt.c clshelldefs.h cl.h
	$(C++) -c $(INCL) SetPrompt.c

SetSymbTab.o:SetSymbTab.c clshelldefs.h cl.h
	$(C++) -c $(INCL) SetSymbTab.c

SetCmdTab.o:SetCmdTab.c clshelldefs.h cl.h
	$(C++) -c $(INCL) SetCmdTab.c

minmatch.o:minmatch.c clshelldefs.h cl.h
	$(C++) -c $(INCL) minmatch.c

rl_interface.o: rl_interface.cc cllib.h
	$(C++) -c $(INCL) rl_interface.cc

clSetOptions.o: clSetOptions.cc clshelldefs.h
	$(C++) -c $(INCL) clSetOptions.cc

clTextColouring.o: clTextColouring.cc clshelldefs.h
	$(C++) -c $(INCL) clTextColouring.cc
#
# The CL library package related targets.  libshccl.a depends on 
# the shell library libsh.a
#
AddQKey.o: cl.h AddQKey.c clshelldefs.h
	$(C++) -c $(INCL) AddQKey.c

InstallSymb.o: cl.h InstallSymb.c SymbTab.h
	$(C++) -c $(INCL) InstallSymb.c

callbacks_awk.c: callbacks.c cl.h shell.tab.h
	sed -e "s/__REPLACE_ME_WITH_AWK_PROG_NAME__/$(AWK)/" \
	callbacks.c > callbacks_awk.c
callbacks.o: cl.h callbacks_awk.c shell.tab.h
	$(C++) -c $(INCL) callbacks_awk.c -o callbacks.o

ParseCmdLine.o:ParseCmdLine.c clshelldefs.h cl.h clconvert.h \
	clinteract.h shell.h clstring.h shell.tab.h
	$(C++) -c $(INCL) ParseCmdLine.c

clparseVal.o: clparseVal.c clshelldefs.h cl.h cllib.h	
	$(C++) -c $(INCL) clparseVal.c

clgetFVal.o:clgetFVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetFVal.c

cldbggetFVal.o:cldbggetFVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetFVal.c

clgetIVal.o:clgetIVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetIVal.c

cldbggetIVal.o:cldbggetIVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetIVal.c

clgetSVal.o:clgetSVal.cc clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetSVal.cc

cldbggetSVal.o:cldbggetSVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetSVal.c

clgetNSVal.o:clgetNSVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetNSVal.c

cldbggetNSVal.o:cldbggetNSVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetNSVal.c

clgetNFVal.o:clgetNFVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetNFVal.c

cldbggetNFVal.o:cldbggetNFVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetNFVal.c

clgetNIVal.o:clgetNIVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetNIVal.c

cldbggetNIVal.o:cldbggetNIVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetNIVal.c

clgetNVals.o:clgetNVals.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetNVals.c

cldbggetNVals.o:cldbggetNVals.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetNVals.c

clgetNOpts.o:clgetNOpts.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetNOpts.c

clgetFullVal.o:clgetFullVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetFullVal.c

cldbggetFullVal.o:cldbggetFullVal.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) cldbggetFullVal.c

clStartInteractive.o:clStartInteractive.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clStartInteractive.c

clRestartShell.o:clRestartShell.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clRestartShell.c

clgetOpt.o:clgetOpt.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetOpt.c

clgetOptsList.o:clgetOptsList.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetOptsList.c

clclearOptsList.o:clclearOptsList.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clclearOptsList.c

clgetCommandLine.o:clgetCommandLine.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetCommandLine.c 

clgetInputFile.o:clgetInputFile.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetInputFile.c 

clgetOutputFile.o:clgetOutputFile.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetOutputFile.c 

clstrtStream.o:clstrtStream.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clstrtStream.c 

clloadConfig.o:clloadConfig.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clloadConfig.c

clgetConfigFile.o:clgetConfigFile.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clgetConfigFile.c

clCleanUp.o:clCleanUp.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clCleanUp.c

clReset.o:clReset.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clReset.c

clRetry.o:clRetry.c clshelldefs.h cl.h cllib.h
	$(C++) -c $(INCL) clRetry.c

support.o:support.c 
	$(C++) -c $(INCL) support.c

clstring.o:clstring.c 
	$(C++) -c $(INCL) clstring.c

clfInteractive.o:clfInteractive.c
	$(C++) -c $(INCL) clfInteractive.c

clSaneExit.o:clSaneExit.c
	$(C++) -c $(INCL) clSaneExit.c

clThrowError.o:clThrowError.c
	$(C++) -c $(INCL) clThrowError.c

clSigHandler.o:clSigHandler.c
	$(C++) -c $(INCL) clSigHandler.c

clErrorObj.o:clErrorObj.cc
	$(C++) -c $(INCL) clErrorObj.cc

ErrorObj.o: ErrorObj.cc
	$(C++) -c $(INCL) ErrorObj.cc

cl.o: cl.f cl.inc
	$(F77) -c $(INCL) cl.f

clIsInteractive.o: clIsInteractive.cc
	$(C++) -c $(INCL) clIsInteractive.cc
libshccl.a: $(CLOBJS)
	ar r libshccl.a $(CLOBJS)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libshccl.a; fi

libsh.a: $(SHOBJS)
	ar r libsh.a $(SHOBJS)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libsh.a; fi

libshfcl.a: $(FCLOBJS)
	ar r libshfcl.a $(FCLOBJS)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libshfcl.a; fi

$(CALCHOME)/libcalc.a libcalc.a: 
	cd $(CALCHOME); $(MAKE) -f makefile.calc.generic libcalc.a

gcalc: gcalc.c libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
	$(C++) $(INCL) -Icalc gcalc.c -o gcalc libshccl.a \
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

tst: tst.c libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
	$(C++) $(INCL) tst.c -o tst libshccl.a \
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

tst2: tst2.c libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
	$(C++) $(INCL) tst2.c -o tst2 libshccl.a \
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

tst3: tst3.c libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
	$(C++) $(INCL) tst3.c -o tst3 libshccl.a \
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

tstshell:tstshell.c libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
	$(C++) $(INCL) tstshell.c -o tstshell libshccl.a \
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

ftst: ftst.f libshfcl.a libsh.a $(CALCHOME)/libcalc.a
	$(F77) $(INCL) ftst.f -o ftst libshccl.a libshfcl.a\
	$(CALCHOME)/libcalc.a libsh.a $(LIBS)

clean: 
	\rm -rf *.o *~ tst ftst tst2 *.a
	cd $(CALCHOME); $(MAKE) -f makefile.calc.generic clean
