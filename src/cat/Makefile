CFLAGS = -Wall -Werror -Wextra -std=c11

CAT = s21_cat

CAT_SRC = cat.c cat_lib.c

all: rebuild

rebuild: clean s21_cat

s21_cat: $(CAT_SRC)
	gcc $(CFLAGS) $(CAT_SRC) -o $(CAT) 

test:
	make rebuild	
	sh cat_tests/test_func_cat.sh

clean:
	rm -rf $(CAT) 
