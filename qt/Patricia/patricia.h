#ifndef PATRICIA_H
#define PATRICIA_H
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <map>

#define NUMARY 27
//#define DEBUG 1

//enum TIPO {NODE=0, INTERNO=1, FOLHA=2};
enum  TIPO: uint8_t {NODE=0, INTERNO=1, FOLHA=2};


/* Lookup Tables para traduzir entre letras e posições dos ponteiros em NodeInterno, e vice-versa */
namespace Traducao {
/* Recebe uma letra (char) e devolve o inteiro correspondente nos ponteiros */
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

/* Recebe um inteiro e devolve a letra correspondente na versão interna do dot */
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

/* Recebe um inteiro e devolve a letra correspondente */
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

/* Classe do PayLoad da árvore */
class PayLoad {
public:
    std::string conteudo;
    PayLoad(const std::string& co) : conteudo(co) { };
    PayLoad(void) : conteudo("") {};
};

/* Nó básico */
class Node {
public:
    bool isInterno(void) const { return tipo == TIPO::INTERNO; };
    bool isFolha(void) const { return tipo == TIPO::FOLHA; };
    const std::string Chave(void) const { return chave; };
    unsigned int id;
    uint8_t tipo;
    std::string chave; // chave da folha ou prefixo do nó interno
    Node(uint8_t t = TIPO::NODE, const std::string &c="");
    ~Node();
};

/* Definição do ponteiro para um nó básico */
typedef Node *NodePtr;


/* Nó Interno, contendo ponteiros para outros nós */
class NodeInterno : public Node {
public:
    unsigned int NumFilhos(void) const;
    unsigned int nivel;
    NodePtr ponteiros[NUMARY] = {}; // Inicializa os ponteiros em nullptr
    NodeInterno() : Node(TIPO::INTERNO) {};
    NodeInterno(const std::string &c, int n) : Node(TIPO::INTERNO, c), nivel(n) {};
    ~NodeInterno();
    int Altura(void);
};

/* Nó Folha, contendo um payload */
class NodeFolha : public Node {
private:
    PayLoad _payload;
public:
    NodeFolha(const std::string& c, const PayLoad& p) : Node(TIPO::FOLHA, c), _payload(p) {};
    ~NodeFolha();
    std::shared_ptr<PayLoad> payload(void) const {
        return std::make_shared<PayLoad>(_payload);
    }
    int Altura(void) { return 1; };
};

/* Estrutura retornada pela Busca */
struct RetornoBusca {
    NodePtr *p = nullptr, *q = nullptr;
    bool achou=false;
    std::shared_ptr<PayLoad> payload;
};

/* Tipo de ponteiro para estrutura da Busca */
typedef RetornoBusca *BuscaPtr;

/* Classe da Árvore Patricia */
class Patricia {

private:

    /* Nó raiz da árvore */
    NodePtr raiz;

    /* Recebe duas Strings e acha o nível que elas divergem */
    static unsigned int AchaNivel (const std::string&, const std::string&);

    /* Verifica se a 1a string começa com a 2a string */
    static bool ComecaCom (const std::string&, const std::string&);

    /* Retorna a letra no nivel da string */
    static char AchaChar(const std::string &, unsigned int);

    /* Função auxiliar recursiva da Busca */
    static void BuscaAuxiliar(const std::string&, const NodePtr*, RetornoBusca*);

    /* Função auxiliar recursiva para gerar o arquivo DOT */
    void GeraDotAux(std::stringstream&, std::stringstream&, NodePtr) const;

    /* Função auxiliar para Inserir um nó */
    static void InsereAux(NodePtr *node_superior, NodePtr node_inferior, NodePtr node_novo);

    /* Função auxiliar para limpar a Árvore */
    void LimpaInterno (NodePtr);

    void BuscaAuxiliarSimples(const std::string &chave, const NodePtr *no, RetornoBusca *r) const;
public:

    /* Contador estatico com o ID do último nó gerado */
    static unsigned int contador;

    /* Função auxiliar para deletar um nó */
    static void Delete(NodePtr*);

/* Mapa usado no DEBUG para verificar se houve vazamento de ponteiros (memory leaking) */
#ifdef DEBUG
    static std::map<unsigned int, NodePtr> mapa;
#endif

    /* Insere uma string na árvore */
    void Insere(const std::string &s) {
        Insere(s,s);
    }

    /* Insere uma chave na árvore com seu payload */
    void Insere(const std::string&, const PayLoad &);

    /* Insere uma chave na árvore gerando o payload da string */
    void Insere(const std::string&, const std::string&);

    /* Busca uma chave na árvore */
    RetornoBusca Busca(const std::string&, bool simples=true) const;

    /* Gera o arquivo DOT da árvore */
    std::string GeraDot(void) const;

    /* Remove uma chave da árvore */
    bool Remove(const std::string&);

    /* Limpa a árvore */
    void Limpa(void);

    /* Calcula a altura da árvore */
    int Altura(void);

    /* Construtores e destruidores */
    Patricia() : raiz(nullptr) {};
    ~Patricia() { Limpa(); };
};

#endif // PATRICIA_H
