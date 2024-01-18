CFLAGS = -Wall -Werror -Wextra

GREP = s21_grep

GREP_SRC = grep.c grep_lib.c

all: rebuild

rebuild: clean s21_grep

s21_grep: $(GREP_SRC)
	gcc $(CFLAGS) $(GREP_SRC) -o $(GREP) 

test:
	make rebuild
	sh grep_tests/grep_test.sh

clean:
	rm -rf $(GREP)
