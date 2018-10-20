all: main teste.png

main: main.cpp patricia.cpp patricia.h
	g++ -ggdb -o $@ main.cpp patricia.cpp

%.png: %.dot
	unflatten $< | dot -o $@
