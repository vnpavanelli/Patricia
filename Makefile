all: main

main: main.cpp patricia.cpp patricia.h
	g++ -ggdb -o $@ main.cpp patricia.cpp
