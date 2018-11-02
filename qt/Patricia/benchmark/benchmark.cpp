#include <iostream>
#include <memory.h>
#include <patricia.h>
#include <string.h>
#include <map>
#include <string>
#include <chrono>
#include <vector>
#include <unistd.h>

using namespace std;

const useconds_t espera = 0; //10e5;

/* Checa uso de memoria pela Patricia */
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

/* Checa uso de memoria pelo std::map */
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

/* Checa tempo de insercao pela Patricia */
void patricia_insere(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando patricia" << endl;
	Patricia p;
	std::chrono::duration<double> elapsed;
	auto start = std::chrono::system_clock::now();
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		p.Insere(buffer, buffer);
		if (++i % 1000 == 0) {
			auto end = std::chrono::system_clock::now();
			elapsed = end-start;
			cout << i << " " << elapsed.count() << endl;
			start = std::chrono::system_clock::now();

		}
	}

}

/* Checa tempo de remocao pela Patricia */
void patricia_remove(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando patricia" << endl;
	vector<string> vetor;
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		vetor.push_back(buffer);
	}
	fseek(in, 0, SEEK_SET);
	for (i=10000;i<vetor.size();i+=10000) {
		{
		Patricia p;
		for (int j=0;j<i;j++) p.Insere(vetor[j], vetor[j]);
		auto start = std::chrono::system_clock::now();
		for (int j=0;j<1000;j++) p.Remove(vetor[j]);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end-start;
		cout << i << " " << elapsed.count() << endl;
		}
		usleep(espera);
	}
}



/* Checa tempo de insercao pelo std::map */
void stdmap_insere(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando patricia" << endl;
	map<string,string> mapa;
	Patricia p;
	std::chrono::duration<double> elapsed;
	auto start = std::chrono::system_clock::now();
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		mapa[buffer] = buffer;
		if (++i % 1000 == 0) {
			auto end = std::chrono::system_clock::now();
			elapsed = end-start;
			cout << i << " " << elapsed.count() << endl;
			start = std::chrono::system_clock::now();

		}
	}
}

/* Checa tempo de remocao pelo std::map */
void stdmap_remove(FILE* in) {
	int i = 0;
	char buffer[128];
	cout << "Iniciando patricia" << endl;
	vector<string> vetor;
	while (!feof(in)) {
		fgets(buffer, 127, in);
		if (strlen(buffer)) buffer[strlen(buffer)-1] = '\0';
		vetor.push_back(buffer);
	}
	fseek(in, 0, SEEK_SET);
	for (i=10000;i<vetor.size();i+=10000) {
		map<string,string> mapa;
		for (int j=0;j<i;j++) mapa[vetor[j]] = vetor[j];
		auto start = std::chrono::system_clock::now();
		for (int j=0;j<1000;j++) mapa.erase(vetor[j]);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end-start;
		cout << i << " " << elapsed.count() << endl;
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
	if (!strcmp(av[2], "memmap")) stdmap(fp);
	 else if (!strcmp(av[2], "mempat")) patricia(fp);
	 else if (!strcmp(av[2], "inspat")) patricia_insere(fp);
	 else if (!strcmp(av[2], "insmap")) stdmap_insere(fp);
	 else if (!strcmp(av[2], "rempat")) patricia_remove(fp);
	 else if (!strcmp(av[2], "remmap")) stdmap_remove(fp);
	return 0;
}
