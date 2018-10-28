#ifndef PATRICIA_H
#define PATRICIA_H
#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#define NUMARY 27

//enum TIPO {NODE=0, INTERNO=1, FOLHA=2};
enum  TIPO: uint8_t {NODE=0, INTERNO=1, FOLHA=2};

class PayLoad {
    public:
        std::string chave;
        std::string conteudo;
        PayLoad(const std::string& ch, const std::string& co) : chave(ch), conteudo(co) { };
        PayLoad(void) : chave(""), conteudo("") {};
};

    class Node {
        public:
            bool isInterno(void) const { return tipo == TIPO::INTERNO; };
            bool isFolha(void) const { return tipo == TIPO::FOLHA; };
            const std::string Chave(void) const { return chave; };
            unsigned int id;
            uint8_t tipo;
            std::string chave;
            Node(uint8_t t = TIPO::NODE);
    };

typedef Node *NodePtr;


    class NodeInterno : public Node {
        public:
        unsigned int NumFilhos(void) const;
        unsigned int nivel;
        NodePtr ponteiros[NUMARY] = {};
//        std::shared_ptr<Node> ponteiros[NUMARY];
        NodeInterno() : Node(TIPO::INTERNO) {};
    };

    class NodeFolha : public Node {
        private:
            std::string conteudo;
        public:
        NodeFolha(const PayLoad& p) : Node(TIPO::FOLHA) {
            conteudo = p.conteudo;
            chave = p.chave;
        };
        std::shared_ptr<PayLoad> payload(void) const {
            return std::make_shared<PayLoad>(chave, conteudo);
        }
    };

struct RetornoBusca {
    NodePtr *p = nullptr, *q = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
};


class Patricia {
private:
    NodePtr raiz;

    static unsigned int AchaNivel (const std::string&, const std::string&);
    static bool ComecaCom (const std::string&, const std::string&);
    static char AchaChar(const std::string &, unsigned int);

    static void BuscaAuxiliar(const std::string&, const NodePtr*, std::shared_ptr<RetornoBusca>);
    void GeraDotAux(std::stringstream&, std::stringstream&, NodePtr) const;

    static void InsereAux(NodePtr *node_superior, NodePtr node_inferior, NodePtr node_novo);
    void LimpaInterno (NodePtr);
public:
    static unsigned int contador;
        void Insere(const PayLoad &);
        void Insere(const std::string&, const std::string&);
        std::shared_ptr<RetornoBusca> Busca(const std::string&) const;
        std::string GeraDot(void) const;
        bool Remove(const std::string&);
        void Limpa(void);
        Patricia() : raiz(nullptr) {};
};

#endif // PATRICIA_H
