#!/bin/zsh

GREP_EXECUTABLE="./s21_grep"
ORIGINAL_GREP="grep"
add_flags=( -v -c -l -n -h -s -vc -vl -vn -vh -vs -cl -cn -ch -cs -ln -lh -ls -nh -ns -hs)
flags=(-o -iv -vc -cli -nh -ic -ovl -nl -vs)
flags2=(-vc -clv -ns -ic -ovl -nl -vsl)
words=(a a*b [a-f8] [0-9][0-9])
words2=(-s g*f [a-fA-F] qvazaaaaar)
f_test_list=(grep_tests/regexes grep_tests/regexes_1)

VALGRIND_CMD="valgrind -q --leak-check=full --show-leak-kinds=all --tool=memcheck "

RED='\033[0;31m' 
GREEN='\033[0;32m' 
NC='\033[0m'

SUCCESS="${GREEN}SUCCESS${NC}"
FAILED="${RED}FAILED${NC}"

list_input_files=('grep_tests/examples/*' grep_tests/examples/Example10-XML.txt grep_tests/examples//test_case_cat.txt )

for input_files in "${list_input_files[@]}"; do

 for f_test in "${f_test_list[@]}"; do


        # $($VALGRIND_CMD "$GREP_EXECUTABLE" -f "$f_test" $input_files > temp_err) 
        s21_grep_output=$("$GREP_EXECUTABLE" -f "$f_test" $input_files)
        original_grep_output=$("$ORIGINAL_GREP" -f "$f_test" $input_files)

        s21_grep_output_file=$(mktemp)
        original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 

        if [ "$s21_grep_output" == "$original_grep_output" ]; then
          result=$SUCCESS
        else
          result=$FAILED
        fi
        
        echo -e "GREP: $input_files: Test case with -f flag, no flags and file $f_test: $result"

        rm -f "$s21_grep_output_file" "$original_grep_output_file"
    done

    for word in "${words[@]}"; do
      for word2 in "${words2[@]}"; do


        # $($VALGRIND_CMD $GREP_EXECUTABLE -e $word -e $word2 $input_files > temp_err 2>&1) 
        s21_grep_output=$("$GREP_EXECUTABLE"  -e "$word" -e "$word2" $input_files)
        original_grep_output=$("$ORIGINAL_GREP" -e "$word" -e "$word2" $input_files)

        s21_grep_output_file=$(mktemp)
        original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 

        if [ "$s21_grep_output" == "$original_grep_output" ]; then
          result=$SUCCESS
        else
          result=$FAILED
        fi

        echo -e "GREP: $input_files: Test case with -e flag, no flags and words $word,$word2: $result"

        rm -f "$s21_grep_output_file" "$original_grep_output_file"
      done
    done



  for word in "${words[@]}"; do


    # $($VALGRIND_CMD $GREP_EXECUTABLE $word $input_files > temp_err 2>&1) 
    s21_grep_output=$("$GREP_EXECUTABLE" "$word" $input_files)
    original_grep_output=$("$ORIGINAL_GREP" "$word" $input_files)

    s21_grep_output_file=$(mktemp)
    original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 

    if [ "$s21_grep_output" == "$original_grep_output" ]; then
      result=$SUCCESS
    else
      result=$FAILED
    fi

    echo -e "GREP: $input_files: Test case with no flag and word $word: $result"

    rm -f "$s21_grep_output_file" "$original_grep_output_file"
  done

  for add_flag in "${add_flags[@]}"; do
      for f_test in "${f_test_list[@]}"; do


        # $($VALGRIND_CMD $GREP_EXECUTABLE $add_flag -f $f_test $input_files > temp_err 2>&1) 
        s21_grep_output=$("$GREP_EXECUTABLE" $add_flag -f "$f_test" $input_files)
        original_grep_output=$("$ORIGINAL_GREP" $add_flag  -f "$f_test" $input_files)

        s21_grep_output_file=$(mktemp)
        original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 

        if [ "$s21_grep_output" == "$original_grep_output" ]; then
          result=$SUCCESS
        else
          result=$FAILED
        fi

        echo -e "GREP: $input_files: Test case with -f flag, $add_flag and file $f_test: $result"

        rm -f "$s21_grep_output_file" "$original_grep_output_file"
    done

    for word in "${words[@]}"; do
      for word2 in "${words2[@]}"; do


        # $($VALGRIND_CMD $GREP_EXECUTABLE $add_flag -e $word -e $word2 $input_files > temp_err 2>&1) 
        s21_grep_output=$("$GREP_EXECUTABLE" $add_flag  -e "$word" -e "$word2" $input_files)
        original_grep_output=$("$ORIGINAL_GREP" $add_flag -e "$word" -e "$word2" $input_files)

        s21_grep_output_file=$(mktemp)
        original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 

        if [ "$s21_grep_output" == "$original_grep_output" ]; then
          result=$SUCCESS
        else
          result=$FAILED
        fi

        echo -e "GREP: $input_files: Test case with -e flag, $add_flag and words $word,$word2: $result"

        rm -f "$s21_grep_output_file" "$original_grep_output_file"
      done
    done
  done
  
  for flag in "${flags[@]}"; do
    for flag2 in "${flags2[@]}"; do
      for word in "${words[@]}"; do


        # $($VALGRIND_CMD $GREP_EXECUTABLE $flag2 $flag $word $input_files > temp_err 2>&1) 
        s21_grep_output=$("$GREP_EXECUTABLE" "$flag2" "$flag" "$word"  $input_files)
        original_grep_output=$("$ORIGINAL_GREP" "$flag2" "$flag" "$word" $input_files)

        s21_grep_output_file=$(mktemp)
        original_grep_output_file=$(mktemp)

        echo "$s21_grep_output" > "$s21_grep_output_file"  
        echo "$original_gpre_output" > "$original_grep_output_file" 


        if [ "$s21_grep_output" == "$original_grep_output" ]; then
          result=$SUCCESS
        else
          result=$FAILED
        fi

        echo -e "GREP: $input_files: Test case with flag $flag + flag $flag2 and word $word: $result"

        rm -f "$s21_grep_output_file" "$original_grep_output_file"
      done
    done
  done
done
