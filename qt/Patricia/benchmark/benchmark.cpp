#include <iostream>
#include <memory.h>
#include <patricia.h>
#include <string.h>
#include <map>
#include <string>

using namespace std;

void patricia(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando patricia" << endl;
	int mem_antes = mem_usage();
	Patricia p;
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		p.Insere(buffer, buffer);
		if (++i % 1000 == 0) {
			int mem_agora = mem_usage();
			cout << i << " " << mem_agora-mem_antes << endl;

		}
	}

}

void stdmap(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando mapa" << endl;
	int mem_antes = mem_usage();
	map<string,string> mapa;
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		mapa[buffer] = buffer;
		if (++i % 1000 == 0) {
			int mem_agora = mem_usage();
			cout << i << " " << mem_agora-mem_antes << endl;

		}
	}

}



int main (int ac, char **av) {
	if (ac < 3) {
		cout << "Para usar entre com o nome do dicionario:" << endl;
		cout << av[0] << " <dicionario> <tipo>" << endl;
		cout << "Tipos: mapa, patricia" << endl;
		return -1;
	}
	cout << "Carregando dicionario..." << endl;
	FILE *fp = fopen(av[1], "r");
	if (!fp) {
		cout << "Arquivo " << av[1] << " não pode ser aberto!" << endl;
		return -2;
	}
	if (!strcmp(av[2], "mapa")) stdmap(fp);
	 else patricia(fp);
	return 0;
}
