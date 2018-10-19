all: main teste.png

main: main.cpp patricia.cpp patricia.h
	g++ -ggdb -o $@ main.cpp patricia.cpp

teste.png: teste.dot
	unflatten teste.dot | dot -o teste.png
