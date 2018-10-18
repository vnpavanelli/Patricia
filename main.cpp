#include <iostream>
#include "patricia.h"

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
    return 0;
}
