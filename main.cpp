#include <iostream>
#include "patricia.h"
#include <fstream>

void teste(char *);

void teste(char *arquivo) {
    std::cout << "Iniciando teste, abrindo arquivo: " << arquivo << std::endl;
    Patricia p;
    char buffer[256];
    std::ifstream arq(arquivo, std::ifstream::in);
    if (!arq.is_open()) {
        std::cout << "Erro abrindo arquivo!" << std::endl;
        return;
    }
    std::string prefixo("__");
    while (arq.getline(buffer, 255)) {
        std::cout << "Inserindo: " << buffer << std::endl;
        PayLoad tmp(buffer, prefixo + buffer);
        p.Insere(tmp);
    }
    std::ofstream dot("teste.dot", std::ofstream::out);
    dot << p.GeraDot();
    dot.close();
    return;
}

int main (int ac, char **av) {
    if (ac > 1) {
        teste(av[1]);
        return 0;
    }
    Patricia p;
    PayLoad pay("teste", "1234teste");
    PayLoad pay2("taste", "1234taste");
    PayLoad pay3("testa", "1234testa");
    p.Insere(pay);
    p.Insere(pay2);
    std::cout << std::endl << std::endl << " Listando:" << std::endl;
    p.Lista();

    p.Insere(pay3);
    std::cout << std::endl << std::endl << " Listando:" << std::endl;
    p.Lista();
    std::cout << std::endl << std::endl << " Buscando:" << std::endl;
    auto r = p.Busca("teste");

    std::ofstream dot("teste.dot", std::ofstream::out);
    dot << p.GeraDot();
    dot.close();
    return 0;
}
