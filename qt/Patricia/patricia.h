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
            bool isInterno(void) { return tipo == TIPO::INTERNO; };
            bool isFolha(void) { return tipo == TIPO::FOLHA; };
            const std::string Chave(void) { return chave; };
            unsigned int id;
            uint8_t tipo;
            std::string chave;
            Node();
            Node(int t);
    };

typedef std::shared_ptr<Node> *NodePtr;


    class NodeInterno : public Node {
        public:
        unsigned int NumFilhos(void);
        unsigned int nivel;
        std::shared_ptr<Node> ponteiros[NUMARY];
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
        std::shared_ptr<PayLoad> payload(void) {
            return std::make_shared<PayLoad>(chave, conteudo);
        }
    };

struct RetornoBusca {
    NodePtr p = nullptr, q = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
};




class Patricia {
private:
    std::shared_ptr<Node> raiz;    
    void InsereAux(NodePtr node_superior, NodePtr node_inferior, NodePtr node_novo);
    void BuscaAuxiliar(const std::string&, NodePtr, std::shared_ptr<RetornoBusca>);
    unsigned int AchaNivel (const std::string&, const std::string&);
    bool ComecaCom (const std::string&, const std::string&);
    void GeraDotAux(std::stringstream&, std::stringstream&, std::shared_ptr<Node>);
    char AchaChar(const std::string &, unsigned int);
public:
    static unsigned int contador;
        void Insere(const PayLoad &);
        void Insere(const std::string&, const std::string&);
        std::shared_ptr<RetornoBusca> Busca(const std::string&);
        std::string GeraDot(void);
        bool Remove(const std::string&);
        void Limpa(void);
};

#endif // PATRICIA_H
