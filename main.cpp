#include <iostream>
#include "patricia.h"
#include <fstream>

int main (int ac, char **av) {
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
