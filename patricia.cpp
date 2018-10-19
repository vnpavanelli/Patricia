#include "patricia.h"
#include <iostream>
#include <sstream>
#include <memory>

unsigned int Patricia::contador = 0;

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
        node_interno->prefixo = std::string(pay.chave, 0, nivel);
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
    if (aux->p != nullptr && aux->p->get() != nullptr) std::cout << "    -> nó p(" << aux->p->get()->id << ") -> folha=" << aux->p->get()->isFolha() << " interno=" << aux->p->get()->isInterno() << std::endl;
    if (aux->q && aux->q->get()) std::cout << "    -> nó q(" << aux->q->get()->id << ") -> folha=" << aux->q->get()->isFolha() << " interno=" << aux->q->get()->isInterno() << std::endl;


        auto node_novo = std::make_shared<NodeFolha>(pay);
        auto node_interno = std::make_shared<NodeInterno>();
        auto node = (NodeFolha*) aux->q->get();
        auto node_ponteiros = (NodeInterno*) aux->p->get();
        auto nivel = AchaNivel(pay.chave, node->payload->chave);
        const char p_novo = pay.chave.c_str()[nivel];
        const char p_node = node->payload->chave.c_str()[nivel];

        const char p_ptr = node->payload->chave.c_str()[node_ponteiros->nivel];
        std::cout << " -> Ponteiros: novo = '" << p_novo << "' node='" << p_node << "'" << std::endl;
        node_interno->nivel = nivel;
        node_interno->prefixo = std::string(pay.chave, 0, nivel);
        node_interno->ponteiros[p_novo - 'a'] = node_novo;
        node_interno->ponteiros[p_node - 'a'] = *(aux->q);
        node_ponteiros->ponteiros[p_ptr - 'a'] = node_interno;


    return;

}

std::string Patricia::GeraDot(void) {
    std::stringstream definicoes, ligacoes;
    GeraDotAux(definicoes, ligacoes, raiz, 0, ' ');

    std::stringstream tmp;
    tmp << "digraph Teste {" << std::endl << " node [shape=record];" << std::endl;
    tmp << definicoes.str();
    tmp << ligacoes.str();
    tmp << "}" << std::endl;
    return tmp.str();
}

void Patricia::GeraDotAux(std::stringstream& definicoes, std::stringstream& ligacoes, std::shared_ptr<Node> no, unsigned int pai, char pai_char) {
    if (!no) return;
    if (no->isFolha()) {
        NodeFolha* tmp = (NodeFolha*) no.get();
        definicoes << "no" << tmp->id << " [shape=ellipse, label=\"" << tmp->payload->chave << "\"];" << std::endl;
    }
    if (no->isInterno()) {
        NodeInterno* tmp = (NodeInterno*) no.get();
        definicoes << "no" << tmp->id << " [label=\"{<f0> " << tmp->nivel << "| <f1> " << tmp->prefixo <<  "| {";
        bool virgula = false;
        for (char i='a'; i <= 'z'; i++) {
            if (tmp->ponteiros[i-'a']) {
                if (virgula) definicoes << " | ";
                 else virgula = true;
                definicoes << "<f" << i << "> " << i;
                ligacoes << "no" << tmp->id << ":f" << i << " -> no" << tmp->ponteiros[i-'a']->id << (tmp->ponteiros[i-'a']->isFolha() ? "" : ":f0") << ";" << std::endl;
            }
        }
        definicoes << "}}\"];" << std::endl;

        for (char i='a'; i <= 'z'; i++) {
            if (tmp->ponteiros[i-'a']) {
                GeraDotAux(definicoes, ligacoes, tmp->ponteiros[i-'a'], tmp->id, i);
            }
        }
 
    }
}

void Patricia::Lista(void) {
    if (raiz.get() == nullptr) {
        std::cout << "Arvore vazia!" << std::endl;
        return;
    }
    std::cout << "Raiz(" << raiz->id << "): folha=" << raiz->isFolha() << " interno=" << raiz->isInterno() << std::endl;
    if (raiz->isFolha()) {
        std::cout << "Arvore só contem uma folha!" << std::endl;
        raiz->Lista();
        return;
    }
    unsigned int identacao = 0;
    std::cout << "Arvore contem nos internos!" << std::endl;
    Patricia::ListaAux(raiz, identacao);
    return;
}

void Patricia::ListaAux(std::shared_ptr<Node> no, unsigned int identacao) {
    std::string itmp(" ", identacao);
    if (no.get() == nullptr) {
        std::cout << itmp << "-> NULL" << std::endl;
        return;
    }
    std::cout << itmp << "-> Nó " << (no->isFolha() ? "folha" : "interno") << " (" << no->id << ")" << std::endl;
    if (no->isFolha()) {
        auto tmp = std::static_pointer_cast<NodeFolha>(no);
        std::cout << itmp <<  "  => chave=" << tmp->payload->chave << " conteudo=" << tmp->payload->conteudo << std::endl;
        return;
    }
    if (no->isInterno()) {
        auto tmp = std::static_pointer_cast<NodeInterno>(no);
        std::cout << "  => nivel=" << tmp->nivel << std::endl;
        for (char i='a'; i <= 'z'; i++) {
            if (tmp->ponteiros[i-'a']) {
                std::cout << itmp << "  => " << i << std::endl;
                ListaAux(tmp->ponteiros[i-'a'], identacao+2);
            }
        }
    }
    return;
}

std::shared_ptr<RetornoBusca> Patricia::Busca(const std::string& chave) {
    std::cout << "Busca: chave=" << chave << std::endl;
    auto r = std::make_shared<RetornoBusca>();
    r->q = &this->raiz;
    r->achou = false;
    if (r->q->get() == nullptr) {
        std::cout << "Busca retornando, raiz vazia" << std::endl;
        return r;
    }
    BuscaAuxiliar(chave, &raiz, r);
    return r;
}

void Patricia::BuscaAuxiliar(const std::string& chave, std::shared_ptr<Node>* no, std::shared_ptr<RetornoBusca> r) {
     r->p = r->q;
     r->q = no;
     if (no->get()->isFolha()) {
        std::cout << " Nó é folha" << std::endl;
        auto tmp = std::static_pointer_cast<NodeFolha>(*no);
       if (tmp->payload->chave == chave) {
            std::cout << " Nó contém chave" << std::endl;
            r->payload = tmp->payload;
            r->achou = true;
            return;
        }
        return;
    }
    if (no->get()->isInterno()) {
        std::cout << " Nó é nó interno" << std::endl;
        auto tmp = std::static_pointer_cast<NodeInterno>(*no);
        char letra = chave.c_str()[tmp->nivel];
        std::cout << " -> nivel=" << tmp->nivel << " letra=" << letra << " ponteiro=" << letra-'a' << std::endl;
        auto no2 = &tmp->ponteiros[letra-'a'];
        BuscaAuxiliar(chave, no2, r);
        return;
    }
    return;
}

void NodeFolha::Lista(void) {
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

Node::Node() {
    this->id = Patricia::contador++;
}
