extern int fork(void);
extern int print(int num);
extern int yield(void);
extern int exit(void);
extern int exec(const unsigned char *elf);
extern int twait(int tid);
extern int prints(const char *s);
extern int read(char *s);
