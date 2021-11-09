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
#----------------------------------------------------------------
#
# The 'ranlib' program to be run, if it exists
#
RANLIB=/bin/ranlib
#
PREFIX   = calc_
LEX      = flex -P$(PREFIX)
YACC     = bison -y -p $(PREFIX)
ARCH     = $(GOSDIR)
OPTFLAGS = -g
DEFINES  = -Wall
INCL     = -I. 
CFLAGS   = -fPIC $(OPTFLAGS) $(DEFINES) $(INCL)
CC       = gcc
#CC=/usr/local/compilers/7.2/gcc_3.2.3/bin/gcc

libcalc.a: calc.o calcinit.o lex.o
	ar r libcalc.a  calc.o calcinit.o  lex.o
	if [ -f $(RANLIB) ]; then $(RANLIB) libcalc.a; fi

y.tab.c: calc.y lex.calc_.c
	$(YACC) calc.y

calc.o: y.tab.c calc.h
	$(CC) $(CFLAGS) -c y.tab.c -o calc.o

lex.o: lex.calc_.c
	$(CC) $(CFLAGS) -c lex.calc_.c -o lex.o

lex.calc_.c: lex.l y.tab.h
	$(LEX) lex.l

y.tab.h: calc.y
	$(YACC) -d calc.y

calcinit.o:calcinit.c symboltable.h y.tab.h
	$(CC) $(CFLAGS) -c calcinit.c

clean: 
	\rm -rf y.tab.h y.tab.c *.o *~ tst *.a lex.$(PREFIX).c
