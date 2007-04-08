typedef union {
  double val;
  Calc_Symbol *symb;
} YYSTYPE;
#define	NUMBER	257
#define	BUILTIN	258
#define	VAR	259
#define	UNDEF	260
#define	BUILTIN2	261
#define	MINUS	262


extern YYSTYPE calc_lval;
