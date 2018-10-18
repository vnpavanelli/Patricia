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

struct RetornoBusca {
    Patricia::Node *p = nullptr;
    Patricia::Node *q = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
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
        public:
            std::shared_ptr<PayLoad> payload;
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
        std::shared_ptr<RetornoBusca> Busca(const std::string&);
        void BuscaAuxiliar(const std::string&, std::shared_ptr<Node>, std::shared_ptr<RetornoBusca>); 
        unsigned int AchaNivel (const std::string&, const std::string&);
};
