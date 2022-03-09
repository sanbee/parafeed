#-*-makefile-*-
# Copyright (c) 2021 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#include $(GSRCROOT)/../Setup/Setup.$(GOSDIR)
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
CFLAGS        = -fPIC -std=c++11 $(OPTFLAGS) $(DEFINES) $(COMMONOPTS) $(INCL)
PREFIX        = sh_
#
# Compilers
#
C++           = g++ $(CFLAGS)
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

GENLIBS       = -L$(READLINEHOME)/lib -lm \
	        -lreadline -lhistory -ltermcap -lm
LIBS          = $(GENLIBS) 

PARAFEEDLIB   = libparafeed.a

RANLIB        = /usr/bin/ranlib 

SHSRC = CopySymb.cc AddCmd.cc AddNode.cc AddVar.cc AllocSymb.cc AddQKey.cc      \
	 DeleteCmd.cc DeleteVar.cc DeleteNode.cc DeleteTab.cc FreeSymb.cc       \
	 SearchNode.cc SearchSymb.cc lex.yy.cc shell.tab.cc TransferTab.cc      \
	 PrintVals.cc SetVar.cc SetPrompt.cc SetSymbTab.cc SetCmdTab.cc         \
	 yyerror.cc  minmatch.cc isBool.cc

CLSRC = ParseCmdLine.cc clMakeArgvFromFile.cc clparseVal.cc InstallSymb.cc clgetFVal.cc clgetIVal.cc \
	 clgetSVal.cc clgetFullVal.cc clgetNFVal.cc clgetNIVal.cc clgetNSVal.cc \
	 clgetNVals.cc cldbggetFVal.cc cldbggetIVal.cc cldbggetSVal.cc         \
	 cldbggetNFVal.cc cldbggetNIVal.cc cldbggetNSVal.cc cldbggetFullVal.cc\
	 clStartInteractive.cc clRestartShell.cc clfInteractive.cc clstring.cc \
	 clstrtok.cc clgetOptsList.cc clclearOptsList.cc clgetCommandLine.cc clgetOpt.cc   \
	 clstrtStream.cc clloadConfig.cc callbacks_awk.cc support.cc               \
	 clgetInputFile.cc clgetOutputFile.cc clgetNOpts.cc clCleanUp.cc       \
	 clReset.cc clgetConfigFile.cc clSaneExit.cc clThrowError.cc clRetry.cc \
	 clSigHandler.cc clIsInteractive.cc ErrorObj.cc rl_interface.cc        \
	 clSetOptions.cc clTextColouring.cc clgetBVal.cc cldbggetBVal.cc clgetNBVal.cc \
         cldbggetNBVal.cc clBreakStr.cc 

FCLSRC = ParseCmdLine.cc InstallSymb.cc AddQKey.cc clgetFVal.cc clgetIVal.cc   \
	  clgetSVal.cc clgetNFVal.cc clgetNIVal.cc clgetNSVal.cc clgetNVals.cc  \
	  clStartInteractive.cc clRestartShell.cc clfInteractive.cc clstring.cc\
	  clstrtok.cc cl.cc clgetBVal.cc clgetNBVal.cc

LIBSH = $(patsubst %.cc, %.o, $(SHSRC))
LIBCL = $(patsubst %.cc, %.o, $(CLSRC))

#-----------------------------------------------------------------------------
#
# The default target
#
#all: libs $(CALCHOME)/libcalc.a libcalc.a
all: libs

install: libs
	mkdir -p $(PUBLIBS) 
	mkdir -p $(PINCLUDE)
	cp libparafeed.a $(PUBLIBS)
	cp *.h $(PINCLUDE)

$(LIBSH): %.o:  %.cc 
	$(C++) -c $< -o $@
ParseCmdLine.o: ParseCmdLine.cc SymbTab.h

$(LIBCL): %.o:  %.cc 
	$(C++) -c $< -o $@


libs: libparafeed.a
#
# Shell related targets.
#
y.tab.c:shell.y
	$(YACC) -d shell.y	

y.tab.h:shell.y y.tab.c
	$(YACC) -d shell.y	

shell.tab.cc:y.tab.c
	cp y.tab.c shell.tab.cc

shell.tab.h:shell.tab.cc
	cp y.tab.h shell.tab.h

lex.yy.cc: shell.l shell.tab.h
	$(LEX) shell.l
#sed "s/#define yywrap $(PREFIX)wrap/\/\* #define yywrap $(PREFIX)wrap \*\//"< lex.$(PREFIX).c > lex.yy.c
	sed "s/#define yywrap $(PREFIX)wrap/#define yywrap libyywrap /"< lex.$(PREFIX).c > lex.yy.cc

callbacks_awk.cc: callbacks.cc cl.h shell.tab.h
	sed -e "s/__REPLACE_ME_WITH_AWK_PROG_NAME__/$(AWK)/" \
	callbacks.cc > callbacks_awk.cc
#-------------------------------------------------------------------
# Library targets
#
libshccl.a: $(LIBCL)
	ar r libshccl.a $(LIBCL)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libshccl.a; fi

libsh.a: $(LIBSH)
	ar r libsh.a $(LIBSH)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libsh.a; fi

libparafeed.a: y.tab.h shell.tab.h $(LIBCL) $(LIBSH) $(CALCHOME)/libcalc.a
	ar r libparafeed.a $(LIBSH) $(CALCHOME)/*.o $(LIBCL)
#ar -rcT libparafeed.a libsh.a calc/libcalc.a libshccl.a

libshfcl.a: $(FCLOBJS)
	ar r libshfcl.a $(FCLOBJS)
#	if [ -f $(RANLIB) ]; then $(RANLIB) libshfcl.a; fi

$(CALCHOME)/libcalc.a libcalc.a: 
	cd $(CALCHOME); $(MAKE) -f nocmake_makefiles/makefile libcalc.a

#-------------------------------------------------------------------
# Applicaion targets
#
gcalc: gcalc.c $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) -Icalc gcalc.c -o gcalc $(PARAFEEDLIB) $(LIBS)

tst: tst.c $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) tst.c -o tst $(PARAFEEDLIB) $(LIBS)

tst2: tst2.cc $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) tst2.cc -o tst2 $(PARAFEEDLIB) $(LIBS)

#tstcpp: tstcpp.cc libshccl.a libsh.a $(CALCHOME)/libcalc.a clsh.h
tstcpp: tstcpp.cc $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) tstcpp.cc -o tstcpp $(PARAFEEDLIB) $(LIBS)

tst3: tst3.cc $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) tst3.cc -o tst3 $(PARAFEEDLIB) $(LIBS)

tstshell:tstshell.c $(PARAFEEDLIB) clsh.h
	$(C++) $(INCL) tstshell.c -o tstshell $(PARAFEEDLIB) $(LIBS)

ftst: ftst.f $(PARAFEEDLIB)
	$(F77) $(INCL) ftst.f -o ftst libshccl.a $(PARAFEEDLIB) $(LIBS)

#-------------------------------------------------------------------
# Housekeeping targets
#
clean: 
	\rm -rf *.o *~ tst ftst tst2 tstcpp gcalc *.a *.so
	\rm -rf y.tab.c y.tab.cc y.tab.h lex.yy.cc shell.tab.c shell.tab.cc shell.tab.h lex.sh_.{cc,c} callbacks_awk.cc
	\rm -rf calc/y.tab.c calc/y.tab.h calc/lex.yy.c
	cd $(CALCHOME); $(MAKE) -f nocmake_makefiles/makefile clean
