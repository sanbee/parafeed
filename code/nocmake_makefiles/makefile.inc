#-*-makefile-*-
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
ARCH          = Linux
#OPTFLAGS      = -g -m64
OPTFLAGS      = -g
DEFINES       = -Wall -DHAVE_STRING_H -DGNUREADLINE
COMMONOPTS    = -Wall -I/usr/local/include -I./
PREFIX        = _sh
#
# Compilers
#
C++           = g++ $(CFLAGS)
CC            = gcc $(CFLAGS)
#F77           = /usr/local/compilers/9/gcc_3.2.3/bin/g77  
LEX           = flex -Ce -P$(PREFIX)
YACC          = bison -y -p $(PREFIX)
AWK           = awk
READLINEHOME  = /usr/local
#
# Locations where libaries and include files should be copied for
# public use
#
PUBHOME       = ~/SCI/linux_gnu
PUBQUALIFIER  =
PUBLIBS       = $(PUBHOME)/lib/$(PUBQUALIFIER)
PUBINCL       = $(PUBHOME)/include
