#include "patricia.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <cstring>

/* Contador para o ID de cada nó */
unsigned int Patricia::contador = 0;
//std::map<unsigned int, NodePtr> Patricia::mapa;

/* Insere deve receber um PayLoad, se receber duas strings, converter e chamar a função correta */
void Patricia::Insere(const std::string &chave, const std::string &conteudo) {
    return Patricia::Insere(chave, PayLoad(conteudo));
}

/* Função que acha o caractere no nível da string
 * Deve retornar '{' no caso do fim da string, para funcionar corretamente
 */
char Patricia::AchaChar(const std::string &in, unsigned int nivel) {
    const char r = in.c_str()[nivel];
    return r;
}


/*
 * Cria um novo nó interno, inserindo o node_inferior e o node_novo nele.
 * Linka o node_superior para o nó interno
 */
void Patricia::InsereAux(NodePtr *node_superior, NodePtr node_inferior, NodePtr node_novo) {

    /* Salva as chaves */
    const std::string& chave_novo = node_novo->chave;
    const std::string& chave_inferior = node_inferior->chave;

    /* Acha o nivel que as strings divergem e o caractere de cada uma */
    const  unsigned int nivel = AchaNivel(chave_inferior, chave_novo);
    const char p_novo = AchaChar(chave_novo, nivel);
    const char p_inferior = AchaChar(chave_inferior, nivel);

    /* Cria o nó interno */
    auto node_interno = new NodeInterno(std::string(chave_novo, 0, nivel), nivel);

    /* Preparamos o no interno, com o nivel e as duas folhas */
//    node_interno->nivel = nivel;
//    node_interno->chave = std::string(chave_novo, 0, nivel);
    node_interno->ponteiros[Traducao::Direta[(uint) p_novo]] = node_novo;
    node_interno->ponteiros[Traducao::Direta[(uint) p_inferior]] = node_inferior;

    /* Apontamos o nó superior para o novo nó interno */
    (*node_superior) = node_interno;
    return;
}

/* Insere o PayLoad na árvore */
void Patricia::Insere(const std::string& chave, const PayLoad &pay) {
    /* Cria um nó novo contendo o payload */
    auto node_novo = new NodeFolha(chave, pay);
//    Patricia::mapa[node_novo->id] = (NodePtr) node_novo;
//    auto node_novo = std::static_pointer_cast<Node>(std::make_shared<NodeFolha>(pay));

    /* Se a raiz é nula podemos inserir uma folha nela e termina */
    if (raiz == nullptr) {
        raiz = node_novo;
        return;
    }

    /* Se a raiz é uma simples folha, inserimos um no interno dividindo entra a folha e no que vamos criar */
    if (raiz->isFolha()) {
        if (raiz->Chave() == chave) return;
        InsereAux(&raiz, raiz, node_novo);
        return;
    }

    /* Se a raiz é um no interno precisamos checar se ele contem o prefixo da chave */
    if (raiz->isInterno() && !ComecaCom(chave, raiz->Chave())) {
        InsereAux(&raiz, raiz, node_novo);
        return;
    }

    /* Procura pela chave na árvore */
    auto aux = Busca(chave);

    /* Se achou termina */
    if (aux.achou) {
//        Patricia::mapa.erase(node_novo->id);
        delete node_novo;
        return;
    }

    /* Caso tenha chegado em um nó nulo, insira a chave neste nó */
    if ((aux.q != nullptr) && (*(aux.q) == nullptr)) {
        *(aux.q) = node_novo;
        return;
    }

    /* Se não, prossiga criando um nó interno e inserindo ela neste nó */
    InsereAux(aux.q, *(aux.q), node_novo);
    return;
}

/* Gera um arquivo DOT para a árvore */
std::string Patricia::GeraDot(void) const {
    std::stringstream definicoes, ligacoes;
    GeraDotAux(definicoes, ligacoes, raiz);

    std::stringstream tmp;
    tmp << "digraph Teste {" << std::endl << " node [shape=record];" << std::endl;
    tmp << "raiz [shape=doublecircle,label=\"RAIZ\"];" << std::endl;
    tmp << definicoes.str();
    if (raiz) tmp << "raiz->no" << raiz->id << ";" << std::endl;
    tmp << ligacoes.str();
    tmp << "}" << std::endl;
    return tmp.str();
}

/* Funcao recursiva para gerar o DOT */
void Patricia::GeraDotAux(std::stringstream& definicoes, std::stringstream& ligacoes, NodePtr no) const {
    if (no == nullptr) return;
    if (no->isFolha()) {
        NodeFolha* tmp = (NodeFolha*) no;
        definicoes << "no" << tmp->id << " [shape=ellipse, label=\"" << tmp->chave << "\"];" << std::endl;
        return;
    }
    if (no->isInterno()) {
        NodeInterno* tmp = (NodeInterno*) no;
        definicoes << "no" << tmp->id << " [label=\"{<f0> " << tmp->nivel << "| <f1> " << tmp->chave <<  "| {";
        bool virgula = false;

        for (int i=0; i < NUMARY; i++) {
            if (tmp->ponteiros[i]) {
                if (virgula) definicoes << " | ";
                 else virgula = true;
                definicoes << "<f" << Traducao::Reversa2[i] << "> " << Traducao::Reversa[i];
                ligacoes << "no" << tmp->id << ":f" << Traducao::Reversa2[i] << " -> no" << tmp->ponteiros[i]->id << (tmp->ponteiros[i]->isFolha() ? "" : ":f0") << ";" << std::endl;

            }
        }

        definicoes << "}}\"];" << std::endl;

        for (int i=0; i < NUMARY; i++) {
            if (tmp->ponteiros[i])
                GeraDotAux(definicoes, ligacoes, tmp->ponteiros[i]);
        }
        return;
    }
}


/* Remove a chave da árvore */
bool Patricia::Remove (const std::string& chave) {
    /* Realiza a busca */
    auto r = Busca(chave);

    /* Se não achou a chave termina */
    if (!r.achou) return false;

    /* Remove o nó contendo a chave */
//    Patricia::mapa.erase((*r.q)->id);
    delete *r.q;
    (*r.q) = nullptr;

    /* Se existir um nó superior */
    if (r.p && (*r.p)) {
        auto tmp = (NodeInterno*) (*r.p);
//        auto tmp = std::static_pointer_cast<NodeInterno>(*r->p);
        /* Se este nó só conter um único filho */
        if (tmp->NumFilhos() <= 1) {
            /* Procure este filho */
            NodePtr nodeptr = nullptr;
            for (int i=0; i<NUMARY;i++) {
                if (tmp->ponteiros[i]) {
                    nodeptr = tmp->ponteiros[i];
                }
            }
            /* E se encontrar o filho, substitua o link do nó pelo filho, eliminando o nó interno */
            if (nodeptr) {
//                Patricia::mapa.erase((*r.p)->id);
                delete *r.p;
                (*r.p) = nodeptr;
            }
        }
    }
    return true;
}

/* Faz a busca por uma chave na árvore */
RetornoBusca Patricia::Busca(const std::string& chave) const {
    /* Cria o objeto de retorno da busca */
    RetornoBusca r;
    /* Se não existir uma raiz, a busca é falsa */
    if (!raiz) {
        return r;
    }
    /* Faz a busca recursiva a partir da raiz */
    BuscaAuxiliar(chave, &raiz, &r);
    return r;
}

/* Função recursiva para a Busca */
void Patricia::BuscaAuxiliar(const std::string& chave, const NodePtr* no, RetornoBusca* r) {
    /* Move os ponteiros p e q */
     r->p = r->q;
     if (no != nullptr) {
         r->q = const_cast<NodePtr*>(no);
         if (*no == nullptr) return;
     } else {
         r->q = nullptr;
         return;
     }

     /* Se o nó for folha */
     if ((*no)->isFolha()) {
         auto tmp = (NodeFolha*) *no;
       /* Se a chave do nó for a procurada */
       if (tmp->chave == chave) {
           /* colocamos o conteudo no objeto de retorno e marcamos a busca verdadeira */
            r->payload = tmp->payload();
            r->achou = true;
            return;
        }
       /* Podemos retornar pois chegamos a uma folha */
        return;
    }

    /* Se o nó for interno */
    if ((*no)->isInterno()) {
        auto tmp = (NodeInterno*) *no;
        /* Se o prefixo do nó for diferente da chave podemos retornar */
        if (!ComecaCom(chave, tmp->chave)) {
            return;
        }
        /* Caso contrário vamos procurar o ramo a seguir */
        char letra = AchaChar(chave, tmp->nivel);
        NodePtr* no2 = &tmp->ponteiros[Traducao::Direta[(uint) letra]];
        /* E chamar a busca recursiva a partir deste ramo */
        BuscaAuxiliar(chave, no2, r);
        return;
    }
    return;
}

/* Encontra o nível em que as duas strings divergem */
unsigned int Patricia::AchaNivel (const std::string& k1, const std::string& k2) {
    /* p0 e p1 aponta pro começo da chave 1
     * p2 aponta pro começo da chave 2 */
    const char *p0 = k1.c_str();
    const char *p1 = k1.c_str();
    const char *p2 = k2.c_str();
    /* Enquanto p1 e p2 não chegam ao fim das strings e são iguais incremente */
    while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
        p1++; p2++;
    }
    /* O nível é a diferença entre o começo da string k1 (p0) e o nível em que divergiram (p1) */
    unsigned int nivel = p1-p0;
    return (nivel);
}

/* Checa se a string s1 começa com o prefixo pre */
bool Patricia::ComecaCom (const std::string& s1, const std::string& pre) {
    /* p1 aponta pro começo de s1 e p2 pro começo de pre */
    const char *p1 = s1.c_str(), *p2 = pre.c_str();
    return (memcmp(p1, p2, pre.size())==0);
    /* Enquanto não chegarem ao fim das strings e forem iguais incremente */
//    for (; *p1 != '\0' && *p2 != '\0'; ++p1, ++p2) if (*p1 != *p2) return false;
//    return (*p2 == '\0');
}

Node::Node(uint8_t t, const std::string &c) : tipo(t), chave(c) {
    this->id = ++Patricia::contador;
//    std::cout << "Criando nó " << this->id << " Chave=" << c <<  std::endl;
}

Node::~Node() {
//    std::cout << "Removendo nó " << this->id << std::endl;
}

unsigned int NodeInterno::NumFilhos(void) const {
    unsigned int r = 0;
    for (int i=0; i<NUMARY; i++) {
        if (ponteiros[i]) r++;
    }
    return r;
}

void Patricia::Limpa(void) {
    LimpaInterno(raiz);
    raiz=nullptr;
}

void Patricia::LimpaInterno(NodePtr no) {
    if (no==nullptr) return;
//    std::cout << "LimpaInterno id=" << no->id << " chave=" << no->chave << std::endl;
    if (no->isInterno()) {
        auto tmp = (NodeInterno*) no;
        for (int i=0; i<NUMARY; i++) {
            LimpaInterno(tmp->ponteiros[i]);
        }
    }
//    Patricia::mapa.erase(no->id);
    delete no;
    return;
}
