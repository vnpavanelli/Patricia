all: main teste.png

main: main.cpp patricia.cpp patricia.h
	g++ -ggdb -o $@ main.cpp patricia.cpp

teste.png: teste.dot
	dot -Tpng teste.dot -o teste.png
