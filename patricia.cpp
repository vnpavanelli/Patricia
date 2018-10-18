#include "patricia.h"
#include <iostream>
#include <memory>


void Patricia::Insere(const std::string &chave, const std::string &conteudo) {
    return Patricia::Insere(PayLoad(chave, conteudo));
}


void Patricia::Insere(const PayLoad &pay) {
    if (raiz.get() == nullptr) {
        std::cout << "Raiz é nula!" << std::endl;
        auto node = std::make_shared<NodeFolha>(pay);
        std::cout << "Node: folha=" << node->isFolha() << " interno=" << node->isInterno() << std::endl;
        raiz = std::static_pointer_cast<Node>(node);
    } else {
        std::cout << "Raiz existe!" << std::endl;
    }
    std::cout << "Raiz: folha=" << raiz->isFolha() << " interno=" << raiz->isInterno() << std::endl;
    return;

}

void Patricia::Lista(void) {
    if (raiz.get() == nullptr) {
        std::cout << "Arvore vazia!" << std::endl;
        return;
    }
    std::cout << "Raiz: folha=" << raiz->isFolha() << " interno=" << raiz->isInterno() << std::endl;
    if (raiz->isFolha()) {
        std::cout << "Arvore só contem uma folha!" << std::endl;
        raiz->Lista();
        return;
    }
    std::cout << "Arvore contem nos internos!" << std::endl;
    return;
}

std::shared_ptr<PayLoad> Patricia::Busca(const std::string& chave, AuxInsere *i) {
    auto r = std::make_shared<PayLoad>();
    return r;
}

void Patricia::NodeFolha::Lista(void) {
    std::cout << "NoFolha: Payload: chave=" << this->payload->chave << " conteudo=" << this->payload->conteudo << std::endl;
    return;
}
