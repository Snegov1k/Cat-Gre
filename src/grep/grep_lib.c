#include "grep_lib.h"

void parse_flags(Flags *flags, int argc, char *argv[],
                 char patterns[MAX_ROWS][MAX_COLUMNS], int *index_patterns,
                 char files[MAX_ROWS][MAX_COLUMNS], int *index_files) {
  int opt;
  int index_next_file = 2;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        flags->e_flag = 1;
        strncpy(&patterns[(*index_patterns)++][0], optarg, MAX_COLUMNS);
        index_next_file = 1;
        break;
      case 'i':
        flags->i_flag = 1;
        break;
      case 'v':
        flags->v_flag = 1;
        break;
      case 'c':
        flags->c_flag = 1;
        break;
      case 'l':
        flags->l_flag = 1;
        break;
      case 'n':
        flags->n_flag = 1;
        break;
      case 'h':
        flags->h_flag = 1;
        break;
      case 's':
        flags->s_flag = 1;
        break;
      case 'f':
        flags->f_flag = 1;
        strncpy(&files[(*index_files)++][0], optarg, MAX_COLUMNS);
        index_next_file = 1;
        break;
      case 'o':
        flags->o_flag = 1;
        break;
      case '?':
        printf("%s: Нет такого флага\n", argv[0]);
        exit(1);
        break;
    }
  }

  if (optind + index_next_file < argc && !flags->h_flag) {
    flags->print_file_names = 1;
  }
}

void join_patterns(Flags flags, char *argv[], char files[MAX_ROWS][MAX_COLUMNS],
                   char patterns[MAX_ROWS][MAX_COLUMNS], int *length_patterns) {
  if (flags.f_flag) {
    add_new_patterns(argv, files, patterns, length_patterns, flags);
  }

  if (!flags.e_flag && !flags.f_flag) {
    char *pattern = argv[optind++];
    strncpy(patterns[0], pattern, MAX_COLUMNS);
    *length_patterns = strlen(pattern);
  }
}

void add_new_patterns(char *argv[], char files[MAX_ROWS][MAX_COLUMNS],
                      char patterns[MAX_ROWS][MAX_COLUMNS],
                      int *length_patterns, Flags flags) {
  for (int i = 0; i < (int)strlen(files[i]); i++) {
    FILE *file = fopen(files[i], "r");

    if (file == NULL) {
      if (!flags.s_flag)
        printf("%s: не удалось открыть файл %s\n", argv[0], files[i]);
      continue;
    }

    char buffer[MAX_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      if (strlen(buffer) > 1) {
        strncpy(patterns[(*length_patterns)++], buffer, MAX_COLUMNS);
      }
    }

    fclose(file);
  }
}

void make_complex_regex(char complex_regex[MAX_SIZE], int *length_complex_regex,
                        char patterns[MAX_ROWS][MAX_COLUMNS],
                        int length_patterns, int regex_flag) {
  for (int i = 0; i < length_patterns; i++) {
    regex_t temp_regex;
    if (i != 0) {
      complex_regex[(*length_complex_regex)++] = '|';
    }

    if (patterns[i][strlen(patterns[i]) - 1] == '\n') {
      patterns[i][strlen(patterns[i]) - 1] = '\0';
    }

    if (regcomp(&temp_regex, patterns[i], regex_flag) != 0) {
      add_brackets_to_pattern(patterns[i]);
    }

    strncpy(complex_regex + *length_complex_regex, patterns[i],
            MAX_SIZE - *length_complex_regex);
    *length_complex_regex += strlen(patterns[i]);

    regfree(&temp_regex);
  }
}

void add_brackets_to_pattern(char *buffer) {
  if (strlen(buffer) > 1 && buffer[0] != '[') {
    if (buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }

    char new_pattern[MAX_SIZE];
    snprintf(new_pattern, sizeof(new_pattern), "[%s]", buffer);
    strncpy(buffer, new_pattern, strlen(new_pattern));
  }
}

void process_files(int argc, char *argv[], char complex_regex[MAX_SIZE],
                   int *length_complex_regex,
                   char patterns[MAX_ROWS][MAX_COLUMNS], int length_patterns,
                   Flags flags, int regex_flag) {
  regex_t regex;
  make_regcomp(&regex, complex_regex, length_complex_regex, patterns,
               length_patterns, flags, regex_flag);

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");

    if (file == NULL) {
      if (!flags.s_flag)
        printf("%s: не удалось открыть файл %s\n", argv[0], argv[i]);
      continue;
    }

    int lines_count = 0;
    int found_count = 0;

    char buffer[MAX_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      if (strlen(buffer) >= MAX_SIZE) buffer[MAX_SIZE - 1] = '\0';

      if (print_line(&regex, buffer, argv[i], &lines_count, &found_count,
                     flags))
        break;
    }

    if (flags.c_flag) {
      if (flags.print_file_names) printf("%s:", argv[i]);
      printf("%d\n", found_count);
    }

    if (flags.l_flag && found_count != 0) {
      printf("%s\n", argv[i]);
    }

    fclose(file);
  }
  regfree(&regex);
}

void make_regcomp(regex_t *regex, char complex_regex[MAX_SIZE],
                  int *length_complex_regex,
                  char patterns[MAX_ROWS][MAX_COLUMNS], int length_patterns,
                  Flags flags, int regex_flag) {
  if (flags.f_flag || flags.e_flag) {
    make_complex_regex(complex_regex, length_complex_regex, patterns,
                       length_patterns, regex_flag);
    regcomp(regex, complex_regex, regex_flag);
  } else {
    regcomp(regex, patterns[0], regex_flag);
  }
}

int print_line(regex_t *regex, char *buffer, char *filename, int *lines_count,
               int *found_count, Flags flags) {
  (*lines_count)++;

  int regex_result = regexec(regex, buffer, 0, NULL, 0) == 0;
  if (regex_result != flags.v_flag) {
    (*found_count)++;

    if (flags.l_flag) {
      return 1;
    } else if (flags.c_flag) {
      return 0;
    } else if (flags.o_flag) {
      regmatch_t matches[1];
      char *buffer_copy = buffer;

      if (regexec(regex, buffer_copy, 1, matches, 0) == 0) {
        if (flags.print_file_names) printf("%s:", filename);
        if (flags.n_flag) printf("%d:", *lines_count);
      }

      while (regexec(regex, buffer_copy, 1, matches, 0) == 0) {
        int m_start = matches[0].rm_so;
        int m_end = matches[0].rm_eo;

        printf("%.*s\n", m_end - m_start, buffer_copy + m_start);
        buffer_copy += m_end;
      }
    } else {
      if (flags.print_file_names) printf("%s:", filename);
      if (flags.n_flag) printf("%d:", *lines_count);

      printf("%s", buffer);
      if (*(buffer + strlen(buffer) - 1) != '\n' &&
          *(buffer + strlen(buffer) - 1) != '\r')
        printf("\n");
    }
  }

  return 0;
}
