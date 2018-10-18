all: main

main: main.cpp patricia.h patricia.cpp
	g++ -o $@ $^
