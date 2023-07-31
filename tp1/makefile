main:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o tp1 *.c
	./tp1
	leaks --atExit --  ./tp1
	rm tp1