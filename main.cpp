#include <iostream>
#include "patricia.h"

int main (int ac, char **av) {
    Patricia p;
    PayLoad pay("teste", "1234");
    p.Insere(pay);
//    p.Insere(pay);
    p.Lista();
    return 0;
}
