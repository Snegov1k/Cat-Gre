#include "cat_lib.h"

void parse_flags(char *flags, int argc, char *argv[]) {
  int opt;
  int index_to_append = 0;

  static struct option long_flags[] = {{"number-nonblank", 0, 0, 'b'},
                                       {"number", 0, 0, 'n'},
                                       {"squeeze-blank", 0, 0, 's'},
                                       {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "beEnstT", long_flags, NULL)) != -1) {
    if (opt == '?') {
      printf("s21_cat: Нет такого флага или католога\n");
      exit(1);
    }

    flags[index_to_append++] = opt;
  }

  flags[index_to_append] = '\0';
}

void print_file(char *name_of_file, char *flags) {
  FILE *file =
      strcmp(name_of_file, "-") == 0 ? stdin : fopen(name_of_file, "r");

  if (file != NULL) {
    char symbol;
    char previous_symbol = '\n';
    int index_of_string = 0;
    int empty_line_printed = 0;

    while ((symbol = fgetc(file)) != EOF) {
      print_symbol(symbol, flags, &previous_symbol, &index_of_string,
                   &empty_line_printed);
    }

    if (file != stdin) fclose(file);
  } else {
    printf("s21_cat: не удалось открыть файл %s\n", name_of_file);
  }
}

void print_symbol(char symbol, char *flags, char *previous_symbol,
                  int *index_of_string, int *empty_line_printed) {
  if (!(should_skip_printing(symbol, empty_line_printed, flags))) {
    int delete_character = 0177;

    *empty_line_printed = (*previous_symbol == '\n' && symbol == '\n') ? 1 : 0;

    int flag_b =
        (has_flag(flags, 'b') && symbol != '\n' && *previous_symbol == '\n');
    int flag_n = (has_flag(flags, 'n') && !has_flag(flags, 'b') &&
                  *previous_symbol == '\n');

    if (flag_b || flag_n) {
      (*index_of_string)++;
      printf("%6d\t", *index_of_string);
    }

    if (has_flag(flags, 'e') || has_flag(flags, 'E')) {
      if (!has_flag(flags, 'E')) {
        handle_unprintable_symbols(&symbol, delete_character);
      }

      if (symbol == '\n') {
        printf("$");
      }
    }

    if (has_flag(flags, 't') && !has_flag(flags, 'T')) {
      handle_t_flag(&symbol, delete_character);
    } else if (has_flag(flags, 'T') && !has_flag(flags, 't')) {
      printf("^");
      symbol += 64;
    }

    fputc(symbol, stdout);
  }

  *previous_symbol = symbol;
}

int should_skip_printing(char symbol, int *empty_line_printed, char *flags) {
  return (symbol == '\n' && *empty_line_printed && has_flag(flags, 's'));
}

int has_flag(char *flags, char flag) { return (strchr(flags, flag) != NULL); }

void handle_unprintable_symbols(char *symbol, int delete_character) {
  if (*symbol < 0 || (*symbol <= 31 && *symbol != '\n' && *symbol != '\t') ||
      *symbol == delete_character) {
    printf("^");
    *symbol = (*symbol == delete_character) ? '?' : *symbol + 64;
  }
}

void handle_t_flag(char *symbol, int delete_character) {
  int is_unprintable = *symbol >= 0 && *symbol <= 31 && *symbol != '\n';
  check_hidden_symbols(is_unprintable, symbol, delete_character);
}

void check_hidden_symbols(int is_unprintable, char *symbol,
                          int delete_character) {
  if (is_unprintable || *symbol == delete_character) {
    printf("^");
    *symbol = (*symbol == delete_character) ? '?' : *symbol + 64;
  }
}