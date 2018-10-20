#include "patricia.h"
#include <iostream>
#include <sstream>
#include <memory>

unsigned int Patricia::contador = 0;

void Patricia::Insere(const std::string &chave, const std::string &conteudo) {
    return Patricia::Insere(PayLoad(chave, conteudo));
}

char Patricia::AchaChar(const std::string &in, unsigned int nivel) {
    const char *p = in.c_str();
    char r = *(p+nivel);
    if (r == '\0') return '{';
    return r;
}


void Patricia::Insere(const PayLoad &pay) {
    std::cout << "Inserindo PayLoad: chave=" << pay.chave << std::endl;

    /* Se a raiz é nula podemos inserir uma folha nela e termina */
    if (raiz.get() == nullptr) {
        std::cout << " -> Raiz é nula!" << std::endl;
        auto node = std::make_shared<NodeFolha>(pay);
        std::cout << " -> Node: folha=" << node->isFolha() << " interno=" << node->isInterno() << std::endl;
        raiz = std::static_pointer_cast<Node>(node);
        return;
    } else {
        std::cout << " -> Raiz existe!" << std::endl;
    }

    /* Caso contrario precisamos examinar a raiz */
    std::cout << " -> Raiz: folha=" << raiz->isFolha() << " interno=" << raiz->isInterno() << std::endl;

    /* Se a raiz é uma simples folha, inserimos um no interno dividindo entra a folha e no que vamos criar */
    if (raiz->isFolha()) {
        /* A raiz contem o proprio nó que queremos inserir? */
        auto node = std::static_pointer_cast<NodeFolha>(raiz);
        if (node->payload->chave == pay.chave) {
            std::cout << " -> Chave do payload já esta na raiz, encerrando" << std::endl;
            return;
        }


        std::cout << " -> Raiz é folha, criando nó interno" << std::endl;
        /* Cria os nos interno e o no novo a ser inserido */
        auto node_novo = std::make_shared<NodeFolha>(pay);
        auto node_interno = std::make_shared<NodeInterno>();

        /* Acha o nivel que as strings divergem e o caractere de cada uma */
        auto nivel = AchaNivel(pay.chave, node->Chave());
        const char p_novo = AchaChar(pay.chave, nivel);
        const char p_node = AchaChar(node->Chave(), nivel);

        /* Preparamos o no interno, com o nivel e as duas folhas */
        std::cout << " -> Ponteiros: novo = '" << p_novo << "' node='" << p_node << "'" << std::endl;
        node_interno->nivel = nivel;
        node_interno->prefixo = std::string(pay.chave, 0, nivel);
        node_interno->ponteiros[p_novo - 'a'] = node_novo;
        node_interno->ponteiros[p_node - 'a'] = node;

        /* Apontamos a raiz para o no interno */
        raiz = node_interno;
        return;
    }

    /* Se a raiz é um no interno precisamos checar se ele contem o prefixo da chave */
    if (raiz->isInterno()) {
        auto node = std::static_pointer_cast<NodeFolha>(raiz);

        /* Se ele não contem, vamos inserir um novo nó interno para divergir */
        if (!ComecaCom(pay.chave, node->Chave())) {
            std::cout << " -> Raiz não contem prefixo da arvore" << std::endl;

            /* Cria os nos interno e o no novo a ser inserido */
            auto node_novo = std::make_shared<NodeFolha>(pay);
            auto node_interno = std::make_shared<NodeInterno>();

            /* Acha o nivel que as strings divergem e o caractere de cada uma */
            auto nivel = AchaNivel(pay.chave, node->Chave());
            const char p_novo = AchaChar(pay.chave, nivel);
            const char p_node = AchaChar(node->Chave(), nivel);

            /* Preparamos o no interno, com o nivel e as duas folhas */
            std::cout << " -> Ponteiros: novo = '" << p_novo << "' node='" << p_node << "'" << std::endl;
            node_interno->nivel = nivel;
            node_interno->prefixo = std::string(pay.chave, 0, nivel);
            node_interno->ponteiros[p_novo - 'a'] = node_novo;
            node_interno->ponteiros[p_node - 'a'] = node;

            /* Apontamos a raiz para o no interno */
            raiz = node_interno;
            return;
        }
    }


    auto aux = Busca(pay.chave);
    if (aux->achou) {
        std::cout << " -> Chave já existe na árvore!" << std::endl;
        return;
    }
    std::cout << " -> Inserindo chave entre dois nós" << std::endl;
    if (aux->p != nullptr && aux->p->get() != nullptr) aux->p->get()->print();
    if (aux->q && aux->q->get()) aux->q->get()->print();

    if (aux->q && !aux->p) {
        aux->p = &raiz;
    }

    if (aux->q && !aux->q->get()) {
        // Existe ponteiro em Q mas ele eh nulo
        auto node_novo = std::make_shared<NodeFolha>(pay);
        (*aux->q) = node_novo;
        return;
    }


        auto node_novo = std::make_shared<NodeFolha>(pay);
        auto node_interno = std::make_shared<NodeInterno>();
        auto node = (NodeFolha*) aux->q->get();
        auto node_ponteiros = (NodeInterno*) aux->p->get();
        auto nivel = AchaNivel(pay.chave, node->Chave());
        const char p_novo = AchaChar(pay.chave, nivel);
        const char p_node = AchaChar(node->Chave(), nivel);

        const char p_ptr = AchaChar(node->Chave(), node_ponteiros->nivel);
        node_interno->nivel = nivel;
        node_interno->prefixo = std::string(pay.chave, 0, nivel);
        std::cout << " -> Ponteiros: novo = '" << p_novo << "' node='" << p_node << "' prefixo='" << node_interno->prefixo << "'" << std::endl;
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
        for (char i='a'; i <= '{'; i++) {
            if (tmp->ponteiros[i-'a']) {
                if (virgula) definicoes << " | ";
                 else virgula = true;
                definicoes << "<f" << (i<='z'?i:'9') << "> " << (i<='z'?i:' ');
                ligacoes << "no" << tmp->id << ":f" << (i<='z'?i:'9') << " -> no" << tmp->ponteiros[i-'a']->id << (tmp->ponteiros[i-'a']->isFolha() ? "" : ":f0") << ";" << std::endl;
            }
        }
        definicoes << "}}\"];" << std::endl;

        for (char i='a'; i <= '{'; i++) {
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
    std::cout << "Raiz:" << std::endl;
    raiz->print();
    if (raiz->isFolha()) {
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
    no->print(identacao);
    if (no->isInterno()) {
        auto tmp = std::static_pointer_cast<NodeInterno>(no);
        for (char i='a'; i <= '{'; i++) {
            if (tmp->ponteiros[i-'a']) {
                std::cout << itmp << "-> " << i << std::endl;
                ListaAux(tmp->ponteiros[i-'a'], identacao+2);
            }
        }
    }
    return;
}

bool Patricia::Remove (const std::string& chave) {
    std::cout << "Remove chave=" << chave << std::endl;
    auto r = Busca(chave);
    if (!r->achou) return false;
    std::cout << "Remove chave encontrada: " << chave << std::endl;
    std::cout << "O:" << std::endl; if (r->o) r->o->get()->print(2);
    std::cout << "P:" << std::endl; if (r->p) r->p->get()->print(2);
    std::cout << "Q:" << std::endl; if (r->q) r->q->get()->print(2);

    /* Checar se temos um nó p, se tiver ele deve ser do tipo interno */
    if (r->p && r->p->get()->isInterno()) {
        std::cout << "Remove: temos um nó P:" << std::endl;
        r->p->get()->print();
        auto tmp = std::static_pointer_cast<NodeInterno>(*(r->p));
        unsigned int nivel = tmp->nivel;
        char letra = chave.c_str()[nivel];
        tmp->ponteiros[letra - 'a'] = nullptr;

        /* Temos um nó O */
        if (tmp->NumFilhos() == 1 && r->o && r->o->get()->isInterno()) {
            auto O = std::static_pointer_cast<NodeInterno>(*(r->o));
            std::shared_ptr<Node> *tmp2 = nullptr;
            for (char i='a'; i <= '{'; i++) {
                if (O->ponteiros[i]) {
                    tmp2 = &O->ponteiros[i];
                    break;
                }
            }
            if (tmp2 != nullptr) {
                /* Devemos ligar o nó O até o filho único tmp2 */
                char letra2 = tmp2->get()->Chave().c_str()[O->nivel];
                O->ponteiros[letra2 - 'a'] = (*tmp2);
            }
        }
    }
}

std::shared_ptr<RetornoBusca> Patricia::Busca(const std::string& chave) {
    std::cout << "Listando arvore antes da busca: " << std::endl;
    Lista();
    std::cout << "Busca: chave=" << chave << std::endl;
    auto r = std::make_shared<RetornoBusca>();
    //r->q = &this->raiz;
    r->achou = false;
    if (!raiz) {
        std::cout << "Busca retornando, raiz vazia" << std::endl;
        return r;
    }
    BuscaAuxiliar(chave, &raiz, r);
    return r;
}

void Patricia::BuscaAuxiliar(const std::string& chave, std::shared_ptr<Node>* no, std::shared_ptr<RetornoBusca> r) {
     r->o = r->p;
     r->p = r->q;
     r->q = no;
     if (!(*no)) {
//         r->q = nullptr;
         std::cout << " Nó nulo!" << std::endl;
         return;
     }
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
        std::cout << " Nó (" << no->get()->id << ") é nó interno" << std::endl;
        auto tmp = std::static_pointer_cast<NodeInterno>(*no);
        if (!ComecaCom(chave, tmp->prefixo)) {
            std::cout << "   -> Prefixo=" << tmp->prefixo << " chave=" << chave << std::endl;
            // A chave não pode estar dentro do no interno
            return;
        }
        char letra = chave.c_str()[tmp->nivel];
        std::cout << " -> nivel=" << tmp->nivel << " letra=" << letra << " ponteiro=" << letra-'a' << std::endl;
        auto no2 = &tmp->ponteiros[letra-'a'];
        BuscaAuxiliar(chave, no2, r);
        return;
    }
    return;
}

void NodeFolha::Lista(void) {
    print();
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

bool Patricia::ComecaCom (const std::string& s1, const std::string& pre) {
    const char *p1 = s1.c_str(), *p2 = pre.c_str();
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        p1++; p2++;
    }
    if (*p2 == '\0' && *p1 != '\0') return true;
    return false;
}

Node::Node() {
    this->id = Patricia::contador++;
}

void NodeFolha::print(unsigned int identacao) {
    std::string itmp(" ", identacao);
    std::cout << itmp <<  "==> NodeFolha ID=" << id << " Chave=" << payload->chave << std::endl;
}

void NodeInterno::print(unsigned int identacao) {
    std::string itmp(" ", identacao);
    std::cout << itmp << "==> NodeInterno ID=" << id << " Prefixo=" << prefixo << std::endl;
}

unsigned int NodeInterno::NumFilhos(void) {
    unsigned int r = 0;
    for (char i = 'a'; i <= '{'; i++) {
        if (ponteiros[i - 'a']) r++;
    }
    return r;
}
