#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void parse_flags(char *flags, int argc, char *argv[]);

void print_file(char *name_of_file, char *flags);

void print_symbol(char symbol, char *flags, char *previous_symbol,
                  int *index_of_string, int *empty_line_printed);

int should_skip_printing(char symbol, int *empty_line_printed, char *flags);

int has_flag(char *flags, char flag);

void handle_unprintable_symbols(char *symbol, int delete_character);

void handle_t_flag(char *symbol, int delete_character);

void check_hidden_symbols(int is_unprintable, char *symbol,
                          int delete_character);

#endif
