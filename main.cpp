#include <iostream>
#include "patricia.h"
#include <fstream>

void teste(char *);

void teste(char *arquivo, std::string saida) {
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
    std::ofstream dot(saida, std::ofstream::out);
    dot << p.GeraDot();
    dot.close();
    return;
}

int main (int ac, char **av) {
    if (ac > 1) {
        teste(av[1], "teste.dot");
        return 0;
    }
    Patricia p;
    p.Insere({"amarelo", "_amarelo"});
    p.Insere({"azul", "_azul"});
//    std::cout << std::endl << std::endl << " Listando:" << std::endl;
//    p.Lista();

    p.Insere({"bege", "_bege"});
    std::cout << std::endl << std::endl << " Listando:" << std::endl;
    p.Lista();
//    std::cout << std::endl << std::endl << " Buscando:" << std::endl;
//    auto r = p.Busca("teste");
    {
        std::ofstream dot("antes.dot", std::ofstream::out);
        dot << p.GeraDot();
        dot.close();
    }

    std::cout << std::endl << "Removendo bege" << std::endl;
    p.Remove("bege");


    std::cout << std::endl << std::endl << " Listando:" << std::endl;
    p.Lista();
    return 0;
}
