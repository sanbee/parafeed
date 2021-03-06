#-*-makefile-*-
include ../makefile.inc
#----------------------------------------------------------------
#
# The 'ranlib' program to be run, if it exists
#
RANLIB=/bin/ranlib
#
LEX = flex
YACC=bison -y -p calc_
#CC=/usr/local/compilers/7.2/gcc_3.2.3/bin/gcc

libcalc.a: calc.o calcinit.o
	ar r libcalc.a calc.o calcinit.o
	if [ -f $(RANLIB) ]; then $(RANLIB) libcalc.a; fi

y.tab.c: calc.y lex.yy.c yyerror.c
	$(YACC) calc.y

calc.o: y.tab.c calc.h
	$(CC) $(CFLAGS) -c y.tab.c -o calc.o

lex.yy.c: lex.l
	$(LEX) lex.l

y.tab.h: calc.y
	$(YACC) -d calc.y

calcinit.o:calcinit.c symboltable.h y.tab.h
	$(CC) $(CFLAGS) -c calcinit.c

clean: 
	\rm -rf *.o *~ tst *.a
