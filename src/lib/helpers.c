
int rand_int(int range, int min) {
  return rand() % range + min;
}

// *int[] fill_rand(int[] *arr, int range, int min) {



// output ----------------------------------------------------------------------

void prI(char* str, int i) {
  printf("%s: %d\n", str, i);
}

void pri(int i) {
  printf("%d\n", i);
}

void _prs(char* s) {
  printf("%s\n",s);
}

void pre() {
  printf("\n");
}


void prfile(char* fname) {
  int c;
  FILE *file;
  file = fopen(fname, "r");
  if (file) {
      while ((c = getc(file)) != EOF)
          putchar(c);
      fclose(file);
  }
}
void append_file_to_other_file(char* fname, FILE* target_f) {
  int c;
  FILE *file;
  file = fopen(fname, "r");
  if (file) {
      while ((c = getc(file)) != EOF)
          fputc(c, target_f);
      fclose(file);
  }
}

void get_log_fname(char* fname, int rank) {
  sprintf(fname, "log/%d", rank);
}

void get_draw_fname(char* fname, int rank, int iteration) {
  sprintf(fname, "draw/data/%d-%d", iteration, rank);
}

void get_fname(char* fname, int rank) {
  sprintf(fname, "out/%d", rank);
}
