#!/bin/bash
find -L $HOME -samefile "$1" > task3_output.txt
count=$(wc -l task3_output.txt)
echo "count = $count"
cat "task3_output.txt"

