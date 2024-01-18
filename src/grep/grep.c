#include "grep_lib.h"

int main(int argc, char *argv[]) {
  char patterns[MAX_ROWS][MAX_COLUMNS] = {0};
  int length_patterns = 0;

  char files[MAX_ROWS][MAX_COLUMNS] = {0};
  int length_files = 0;

  char complex_regex[MAX_SIZE] = {0};
  int length_complex_regex = 0;

  Flags flags = {0};

  parse_flags(&flags, argc, argv, patterns, &length_patterns, files,
              &length_files);
  int regex_flag = flags.i_flag ? REG_ICASE : REG_EXTENDED;

  join_patterns(flags, argv, files, patterns, &length_patterns);

  process_files(argc, argv, complex_regex, &length_complex_regex, patterns,
                length_patterns, flags, regex_flag);

  return 0;
}
