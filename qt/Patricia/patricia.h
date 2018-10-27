#ifndef PATRICIA_H
#define PATRICIA_H
#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#define NUMARY 27


class PayLoad {
    public:
        std::string chave;
        std::string conteudo;
        PayLoad(const std::string& ch, const std::string& co) : chave(ch), conteudo(co) { };
        PayLoad(void) : chave(""), conteudo("") {};
};

    class Node {
        public:
            virtual bool isInterno(void) { return false; };
            virtual bool isFolha(void) { return false; };
            virtual void Lista(void) { return; };
            virtual void print(unsigned int identacao=0) {
                std::string itmp(" ", identacao);
                std::cout << "==> Node ID=" << id << std::endl;
            };
            virtual std::string Chave(void) { return ""; };
            unsigned int id;
            Node();
    };

    class NodeInterno : public Node {
        public:
        bool isInterno(void) { return true; };
        bool isFolha(void) { return false; };
        void print(unsigned int identacao=0);
        std::string Chave(void) { return prefixo; };
        unsigned int NumFilhos(void);
        unsigned int nivel;
        std::string prefixo;
        std::shared_ptr<Node> ponteiros[NUMARY];
        NodeInterno() : Node() {};
    };

    class NodeFolha : public Node {
        private:
        public:
            std::shared_ptr<PayLoad> payload;
        bool isInterno(void) { return false; };
        bool isFolha(void) { return true; };
        void Lista(void);
        void print(unsigned int identacao=0);
        std::string Chave(void) {
            if (payload) return payload->chave;
            return "";
        };
        NodeFolha(const PayLoad& p) : Node() {
            payload = std::make_shared<PayLoad>(p);
        };
    };

struct RetornoBusca {
    std::shared_ptr<Node> *p = nullptr, *q = nullptr; //, *o = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
};


typedef std::shared_ptr<Node> *NodePtr;


class Patricia {
    public:
        static unsigned int contador;
    private:

    std::shared_ptr<Node> raiz;    
    void InsereAux(NodePtr node_superior, NodePtr node_inferior, NodePtr node_novo);

public:
        void Insere(const PayLoad &);
        void Insere(const std::string&, const std::string&);
        void Lista(void);
        void ListaAux(std::shared_ptr<Node>, unsigned int );
        std::shared_ptr<RetornoBusca> Busca(const std::string&);
        void BuscaAuxiliar(const std::string&, std::shared_ptr<Node>*, std::shared_ptr<RetornoBusca>);
        unsigned int AchaNivel (const std::string&, const std::string&);
        bool ComecaCom (const std::string&, const std::string&);
        std::string GeraDot(void);
        bool Remove(const std::string&);
        void GeraDotAux(std::stringstream&, std::stringstream&, std::shared_ptr<Node>);
        char AchaChar(const std::string &, unsigned int);
        void Limpa(void);
};

#endif // PATRICIA_H
