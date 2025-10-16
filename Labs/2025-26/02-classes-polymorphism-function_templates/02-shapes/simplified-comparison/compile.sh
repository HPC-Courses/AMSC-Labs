#!/usr/bin/bash
for file in `ls *.cpp`; do
	echo " "
	echo $file
	clang++ -std=c++20 -Wall $file
	 ./a.out;
done;
rm ./a.out
