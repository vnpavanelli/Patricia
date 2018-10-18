#include <memory>
#include <string>

#define NUMARY 27

class PayLoad {
    public:
        std::string chave;
        std::string conteudo;
        PayLoad(const std::string& ch, const std::string& co) : chave(ch), conteudo(co) { };
        PayLoad(void) : chave(""), conteudo("") {};
};


class Patricia {
    private:
    class Node {
        public:
            virtual bool isInterno(void) { return false; };
            virtual bool isFolha(void) { return false; };
            virtual void Lista(void) { return; };
            Node() {};
    };

struct AuxInsere {
    Patricia::Node *p = nullptr;
    Patricia::Node *q = nullptr;
    bool achou=false;
};


    class NodeInterno : public Node {
        public:
        bool isInterno(void) { return true; };
        bool isFolha(void) { return false; };
        unsigned int nivel;
        std::shared_ptr<Node> ponteiros[NUMARY];
        NodeInterno() {};
    };

    class NodeFolha : public Node {
        private:
            std::shared_ptr<PayLoad> payload;
        public:
        bool isInterno(void) { return false; };
        bool isFolha(void) { return true; };
        void Lista(void);
        NodeFolha(const PayLoad& p) {
            payload = std::make_shared<PayLoad>(p);
        };
    };

    std::shared_ptr<Node> raiz;

    public:
        void Insere(const PayLoad &);
        void Insere(const std::string&, const std::string&);
        void Lista(void);
        std::shared_ptr<PayLoad> Busca(const std::string&, AuxInsere *i=nullptr);
};
