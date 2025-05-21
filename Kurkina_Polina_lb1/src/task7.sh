#!/bin/bash

stat -c %s  $1
for iter in {1..101}; do
	echo "Iter is $iter"
	cp File.c  ./$1/"file_$iter".c  
	stat -c %s  $1
	
done
