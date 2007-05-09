#ifdef __cpluscplus
extern "C" {
#endif

char*  dupstr (char *s);
char*  rl_command_generator(const char *text,int state);
char** command_completor(const char *text, int start, int end);
void   initialize_readline();
void   mkfilename(char *out,char *envvar,char *name,char *type);
void   save_hist(char *EnvVar, char *Default);
void   limit_hist(char *EnvVar, int Default);
void   load_hist(char *EnvVar, char *Default);

#ifdef __cpluscplus
}
#endif
