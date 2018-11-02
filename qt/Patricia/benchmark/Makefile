all: benchmark tabela

benchmark: benchmark.cpp ../patricia.cpp ../patricia.h ../memory.cpp
	g++ -Os -ggdb3 -o benchmark -I.. benchmark.cpp ../patricia.cpp ../memory.cpp

tabela: tabela.cpp
	g++ -o tabela tabela.cpp

x: x.cpp
	g++ -ggdb -I.. -o x x.cpp ../patricia.cpp
