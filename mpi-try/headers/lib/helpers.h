/* mpi-try/lib/helpers.c */
int rand_int(int range, int min);
void prI(char *str, int i);
void pri(int i);
void prs(char *s);
void pre(void);
void prfile(char *fname);
void get_log_fname(char *fname, int rank);
void get_draw_fname(char *fname, int iteration);
void append_file_to_other_file(char* fname, FILE* target_f);
void get_fname(char* fname, int rank);
