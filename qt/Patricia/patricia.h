#ifndef PATRICIA_H
#define PATRICIA_H
#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#define NUMARY 27

//enum TIPO {NODE=0, INTERNO=1, FOLHA=2};
enum  TIPO: uint8_t {NODE=0, INTERNO=1, FOLHA=2};

namespace Traducao {
const unsigned int Direta[] = {
26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 0, 1, 2
, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
, 23, 24, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 };

const char Reversa2[] = {
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
, 'u', 'v', 'w', 'x', 'y', 'z', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9'
, '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9' };

const char Reversa[] = {
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
, 'u', 'v', 'w', 'x', 'y', 'z', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

}

class PayLoad {
    public:
        std::string conteudo;
        PayLoad(const std::string& co) : conteudo(co) { };
        PayLoad(void) : conteudo("") {};
};

    class Node {
        public:
            bool isInterno(void) const { return tipo == TIPO::INTERNO; };
            bool isFolha(void) const { return tipo == TIPO::FOLHA; };
            const std::string Chave(void) const { return chave; };
            unsigned int id;
            uint8_t tipo;
            std::string chave;
            Node(uint8_t t = TIPO::NODE, const std::string &c="");
    };

typedef Node *NodePtr;


    class NodeInterno : public Node {
        public:
        unsigned int NumFilhos(void) const;
        unsigned int nivel;
        NodePtr ponteiros[NUMARY] = {};
        NodeInterno() : Node(TIPO::INTERNO) {};
        NodeInterno(const std::string &c, int n) : Node(TIPO::INTERNO, c), nivel(n) {};
    };

    class NodeFolha : public Node {
        private:
            PayLoad _payload;
        public:
        NodeFolha(const std::string& c, const PayLoad& p) : Node(TIPO::FOLHA, c), _payload(p) {};
        std::shared_ptr<PayLoad> payload(void) const {
            return std::make_shared<PayLoad>(_payload);
        }
    };

struct RetornoBusca {
    NodePtr *p = nullptr, *q = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
};

typedef RetornoBusca *BuscaPtr;

class Patricia {
private:
    NodePtr raiz;

    static unsigned int AchaNivel (const std::string&, const std::string&);
    static bool ComecaCom (const std::string&, const std::string&);
    static char AchaChar(const std::string &, unsigned int);

    static void BuscaAuxiliar(const std::string&, const NodePtr*, RetornoBusca*);
    void GeraDotAux(std::stringstream&, std::stringstream&, NodePtr) const;

    static void InsereAux(NodePtr *node_superior, NodePtr node_inferior, NodePtr node_novo);
    void LimpaInterno (NodePtr);
public:
    static unsigned int contador;
    void Insere(const std::string &s) {
        Insere(s,s);
    }
        void Insere(const std::string&, const PayLoad &);
        void Insere(const std::string&, const std::string&);
        RetornoBusca Busca(const std::string&) const;
        std::string GeraDot(void) const;
        bool Remove(const std::string&);
        void Limpa(void);
        Patricia() : raiz(nullptr) {};
};

#endif // PATRICIA_H
