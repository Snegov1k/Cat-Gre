#ifndef GREP_LIB_H
#define GREP_LIB_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define MAX_ROWS 256
#define MAX_COLUMNS 256

typedef struct flags {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int print_file_names;
  int s_flag;
  int f_flag;
  int o_flag;
} Flags;

void parse_flags(Flags *flags, int argc, char *argv[],
                 char patterns[MAX_ROWS][MAX_COLUMNS], int *index_patterns,
                 char files[MAX_ROWS][MAX_COLUMNS], int *index_files);

void join_patterns(Flags flags, char *argv[], char files[MAX_ROWS][MAX_COLUMNS],
                   char patterns[MAX_ROWS][MAX_COLUMNS], int *length_patterns);

void add_new_patterns(char *argv[], char files[MAX_ROWS][MAX_COLUMNS],
                      char patterns[MAX_ROWS][MAX_COLUMNS],
                      int *length_patterns, Flags flags);

void make_complex_regex(char complex_regex[MAX_SIZE], int *length_complex_regex,
                        char patterns[MAX_ROWS][MAX_COLUMNS],
                        int length_patterns, int regex_flag);

void add_brackets_to_pattern(char *buffer);

void process_files(int argc, char *argv[], char complex_regex[MAX_SIZE],
                   int *length_complex_regex,
                   char patterns[MAX_ROWS][MAX_COLUMNS], int length_patterns,
                   Flags flags, int regex_flag);

void make_regcomp(regex_t *regex, char complex_regex[MAX_SIZE],
                  int *length_complex_regex,
                  char patterns[MAX_ROWS][MAX_COLUMNS], int length_patterns,
                  Flags flags, int regex_flag);

int print_line(regex_t *regex, char *buffer, char *filename, int *lines_count,
               int *found_count, Flags flags);

#endif
