#include "patricia.h"
#include <iostream>
#include <memory>


void Patricia::Insere(const std::string &chave, const std::string &conteudo) {
    return Patricia::Insere(PayLoad(chave, conteudo));
}


void Patricia::Insere(const PayLoad &pay) {
    std::cout << "Inserindo PayLoad: chave=" << pay.chave << std::endl;
    if (raiz.get() == nullptr) {
        std::cout << " -> Raiz é nula!" << std::endl;
        auto node = std::make_shared<NodeFolha>(pay);
        std::cout << " -> Node: folha=" << node->isFolha() << " interno=" << node->isInterno() << std::endl;
        raiz = std::static_pointer_cast<Node>(node);
    } else {
        std::cout << " -> Raiz existe!" << std::endl;
    }
    std::cout << " -> Raiz: folha=" << raiz->isFolha() << " interno=" << raiz->isInterno() << std::endl;
    if (raiz->isFolha()) {
        auto node = std::static_pointer_cast<NodeFolha>(raiz);
        if (node->payload->chave == pay.chave) {
            std::cout << " -> Chave do payload já esta na raiz, encerrando" << std::endl;
            return;
        }
        std::cout << " -> Raiz é folha, criando nó interno" << std::endl;
        auto node_novo = std::make_shared<NodeFolha>(pay);
        auto node_interno = std::make_shared<NodeInterno>();
        auto nivel = AchaNivel(pay.chave, node->payload->chave);
        const char p_novo = pay.chave.c_str()[nivel];
        const char p_node = node->payload->chave.c_str()[nivel];
        std::cout << " -> Ponteiros: novo = '" << p_novo << "' node='" << p_node << "'" << std::endl;
        node_interno->nivel = nivel;
        node_interno->ponteiros[p_novo - 'a'] = node_novo;
        node_interno->ponteiros[p_node - 'a'] = node;
        raiz = node_interno;
        return;
    }
    auto aux = Busca(pay.chave);
    if (aux->achou) {
        std::cout << " -> Chave já existe na árvore!" << std::endl;
        return;
    }
    std::cout << " -> Inserindo chave entre dois nós" << std::endl;
    if (aux->p != nullptr) std::cout << "    -> nó p -> folha=" << aux->p->isFolha() << " interno=" << aux->p->isInterno() << std::endl;
    if (aux->q != nullptr) std::cout << "    -> nó q -> folha=" << aux->q->isFolha() << " interno=" << aux->q->isInterno() << std::endl;
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

std::shared_ptr<Patricia::RetornoBusca> Patricia::Busca(const std::string& chave) {
    std::cout << "Busca: chave=" << chave << std::endl;
    auto r = std::make_shared<RetornoBusca>();
    r->q = raiz.get();
    r->achou = false;
    if (r->q == nullptr) {
        std::cout << "Busca retornando, raiz vazia" << std::endl;
        return r;
    }
    BuscaAuxiliar(chave, raiz, r);
    return r;
}

void Patricia::BuscaAuxiliar(const std::string& chave, std::shared_ptr<Node> no, std::shared_ptr<RetornoBusca> r) {
     r->p = r->q;
     r->q = no.get();
     if (no->isFolha()) {
        std::cout << " Nó é folha" << std::endl;
        auto tmp = std::static_pointer_cast<NodeFolha>(no);
       if (tmp->payload->chave == chave) {
            std::cout << " Nó contém chave" << std::endl;
            r->payload = tmp->payload;
            r->achou = true;
            return;
        }
        return;
    }
    if (no->isInterno()) {
        std::cout << " Nó é nó interno" << std::endl;
        auto tmp = std::static_pointer_cast<NodeInterno>(raiz);
        char letra = chave.c_str()[tmp->nivel];
        std::cout << " -> nivel=" << tmp->nivel << " letra=" << letra << " ponteiro=" << letra-'a' << std::endl;
        auto no2 = tmp->ponteiros[letra-'a'];
        BuscaAuxiliar(chave, no2, r);
        return;
    }
    return;
}

void Patricia::NodeFolha::Lista(void) {
    std::cout << "NoFolha: Payload: chave=" << this->payload->chave << " conteudo=" << this->payload->conteudo << std::endl;
    return;
}

unsigned int Patricia::AchaNivel (const std::string& k1, const std::string& k2) {
    const char *p0 = k1.c_str();
    const char *p1 = k1.c_str();
    const char *p2 = k2.c_str();
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        p1++; p2++;
    }
    unsigned int nivel = p1-p0;
    std::cout << " AchaNivel: k1=" << k1 << " k2=" << k2 << " nivel=" << nivel << std::endl;
    return (nivel);
}
