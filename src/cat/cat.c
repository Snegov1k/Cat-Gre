#include "cat_lib.h"

int main(int argc, char *argv[]) {
  char flags[7];

  parse_flags(flags, argc, argv);

  if (optind == argc) {
    print_file("-", flags);
  }

  for (int i = optind; i < argc; i++) {
    print_file(argv[i], flags);
  }

  return 0;
}
