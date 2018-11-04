#include "patricia.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <cstring>


/* Contador para o ID de cada nó */
unsigned int Patricia::contador = 0;

/* Mapa para depuração */
#ifdef DEBUG
std::map<unsigned int, NodePtr> Patricia::mapa;
#endif

/* Insere deve receber um PayLoad, se receber duas strings, converter e chamar a função correta */
void Patricia::Insere(const std::string &chave, const std::string &conteudo) {
    return Patricia::Insere(chave, PayLoad(conteudo));
}

/* Função que acha o caractere no nível da string
 * Trivial depois do uso das LookUp Tables de tradução
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
    /* std::string(chave_novo, 0, nivel) gera o prefixo deste nó */
    /* nivel é o nível do nó */
    auto node_interno = new NodeInterno(std::string(chave_novo, 0, nivel), nivel);

    /* Preparamos o no interno, com o nivel e as duas folhas */
    node_interno->ponteiros[ Traducao::Direta[(uint) p_novo] ] = node_novo;
    node_interno->ponteiros[ Traducao::Direta[(uint) p_inferior] ] = node_inferior;

    /* Apontamos o nó superior para o novo nó interno */
    (*node_superior) = node_interno;
    return;
}

/* Insere o PayLoad na árvore */
void Patricia::Insere(const std::string& chave, const PayLoad &pay) {

    /* Se a raiz é nula podemos inserir uma folha nela e terminar */
    if (raiz == nullptr) {
        /* Cria um nó novo contendo o payload */
        auto node_novo = new NodeFolha(chave, pay);
        raiz = node_novo;
        return;
    }

    /* Se a raiz é uma simples folha, inserimos um no interno dividindo entra a folha e no que vamos criar */
    if (raiz->isFolha()) {
        if (raiz->Chave() == chave) return;
        /* Cria um nó novo contendo o payload */
        auto node_novo = new NodeFolha(chave, pay);
        InsereAux(&raiz, raiz, node_novo);
        return;
    }

    /* Se a raiz é um no interno precisamos checar se ele contem o prefixo da chave */
    if (raiz->isInterno() && !ComecaCom(chave, raiz->Chave())) {
        /* Se não tiver o prefixo da chave, criamos um nó novo contendo o payload */
        auto node_novo = new NodeFolha(chave, pay);
        /* E inserimos um nó interno entre a raiz, o nó original que ela apontava e este novo */
        InsereAux(&raiz, raiz, node_novo);
        return;
    }

    /* Procura pela chave na árvore */
    auto aux = Busca(chave);

    /* Se achou termina */
    if (aux.achou) {
        return;
    }

    /* Caso tenha chegado em um nó nulo, insira a chave neste nó */
    if ((aux.q != nullptr) && (*(aux.q) == nullptr)) {
        /* Cria um nó novo contendo o payload */
        auto node_novo = new NodeFolha(chave, pay);
        *(aux.q) = node_novo;
        return;
    }

    /* Se não, prossiga criando um nó interno e inserindo a chave neste nó */
    auto node_novo = new NodeFolha(chave, pay);
    InsereAux(aux.q, *(aux.q), node_novo);
    return;
}

/* Gera um arquivo DOT para a árvore */
std::string Patricia::GeraDot(void) const {
    /* definicoes contem a parte de definicoes de nodes do DOT
     * ligacoes contem a parte das ligacoes entre os nodes do DOT */
    std::stringstream definicoes, ligacoes;

    /* Gera todas as definicoes e ligacoes recursivamente a partir da raiz */
    GeraDotAux(definicoes, ligacoes, raiz);

    /* Vamos montar o arquivo DOT em tmp */
    std::stringstream tmp;

    /* Inicia o digraph com o tipo de nó padrão como "record" */
    tmp << "digraph Teste {" << std::endl << " node [shape=record];" << std::endl;

    /* Cria uma raiz com um circulo duplo */
    tmp << "raiz [shape=doublecircle,label=\"RAIZ\"];" << std::endl;

    /* Insere as definicoes */
    tmp << definicoes.str();

    /* Gera a ligação da raiz para o primeiro nó */
    if (raiz) tmp << "raiz->no" << raiz->id << ";" << std::endl;

    /* Insere as ligacoes */
    tmp << ligacoes.str();

    /* Termina o DOT */
    tmp << "}" << std::endl;

    return tmp.str();
}

/* Funcao recursiva para gerar o DOT */
void Patricia::GeraDotAux(std::stringstream& definicoes, std::stringstream& ligacoes, NodePtr no) const {
    /* Se o nó é nulo não precisa fazer nada */
    if (no == nullptr) return;

    /* Se o nó é folha, insira a definicao dele apenas */
    if (no->isFolha()) {
        NodeFolha* tmp = (NodeFolha*) no;
        definicoes << "no" << tmp->id << " [shape=ellipse, label=\"" << tmp->chave << "\"];" << std::endl;
        return;
    }

    /* Se o nó é interno é preciso também inserir as ligacoes */
    if (no->isInterno()) {
        NodeInterno* tmp = (NodeInterno*) no;
        /* Definicao do nó */
        definicoes << "no" << tmp->id << " [label=\"{<f0> " << tmp->nivel << "| <f1> " << tmp->chave <<  "| {";

        /* Para cada filho do nó interno vamos inserir o quadrado dele na definicao e sua ligacao */
        bool virgula = false;
        for (int i=0; i < NUMARY; i++) {
            /* Se o filho existe */
            if (tmp->ponteiros[i]) {
                /* Precisamos inserir uma | somente apos o primeiro item */
                if (virgula) definicoes << " | ";
                else virgula = true;

                /* Definicoes no nó interno e ligacoes do nó interno com o filho */
                definicoes << "<f" << Traducao::Reversa2[i] << "> " << Traducao::Reversa[i];
                ligacoes << "no" << tmp->id << ":f" << Traducao::Reversa2[i] << " -> no" << tmp->ponteiros[i]->id << (tmp->ponteiros[i]->isFolha() ? "" : ":f0") << ";" << std::endl;
            }
        }

        /* Terminamos a definicao do nó interno */
        definicoes << "}}\"];" << std::endl;

        /* E vamos chamar a função recursiva para cada filho válido */
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
    Patricia::Delete(r.q);

    /* Se existir um nó superior, vamos verificar se ele contem somente um filho
     * Se isso for verdade, podemos remover este nó e ligar direto o nó acima dele com o filho dele,
     * eliminando o nó superior ao removido inicialmente */

    /* O nó P existe e é válido? */
    if (r.p && (*r.p)) {
        /* Transforma o ponteiro em Nó Interno */
        auto tmp = (NodeInterno*) (*r.p);
        int nfilhos = 0;
        NodePtr* nodeptr = nullptr;

        /* Checa o número de filhos e salva o ponteiro para o filho encontrado */
        for (int i=0; i<NUMARY && nfilhos < 2;i++) {
            if (tmp->ponteiros[i]) {
                nfilhos++;
                nodeptr = &tmp->ponteiros[i];
            }
        }

        /* Se encontrar o filho, substitua o link do nó pelo filho, eliminando o nó interno */
        if (nfilhos == 1 && nodeptr && (*nodeptr) != nullptr) {
            /* Salvamos em tmp o nó filho */
            NodePtr tmp = (*nodeptr);

            /* Marcamos ele como nulo para não ser apagado junto do nó P */
            (*nodeptr) = nullptr;

            /* Apagamos para onde P aponta */
            Patricia::Delete(r.p);

            /* Ligamos o nó P ao filho salvo */
            *(r.p) = tmp;
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
    r->q = const_cast<NodePtr*>(no);

    if (no == nullptr || *no == nullptr) return;

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

    /* memcmp é mais eficiente que a busca por ponteiros como abaixo: */
    /* Enquanto não chegarem ao fim das strings e forem iguais incremente */
    //    for (; *p1 != '\0' && *p2 != '\0'; ++p1, ++p2) if (*p1 != *p2) return false;
    //    return (*p2 == '\0');
}

/* Construtor do Nó básico
 * Incremente o contador da árvore Patricia e usa dele como o ID do nó */
Node::Node(uint8_t t, const std::string &c) : tipo(t), chave(c) {
    this->id = ++Patricia::contador;
#ifdef DEBUG
    std::cout << "Criando nó " << this->id << " Chave=" << chave << " Tipo=" << (int) tipo << std::endl;
    Patricia::mapa[this->id] = this;
#endif
}


Node::~Node() {
#ifdef DEBUG
    std::cout << "Removendo nó " << this->id << " chave=" << this->chave << " tipo=" << (int) tipo <<  std::endl;
    Patricia::mapa.erase(this->id);
#endif
}

/* Calcula o número de filhos de um nó interno */
unsigned int NodeInterno::NumFilhos(void) const {
    unsigned int r = 0;
    for (int i=0; i<NUMARY; i++) {
        if (ponteiros[i]) r++;
    }
    return r;
}

/* Destruidos do Nó Interno */
NodeInterno::~NodeInterno()
{
#ifdef DEBUG
    std::cout << "Removendo nó interno id=" << this->id << " prefixo=" << this->chave << std::endl;
#endif

    /* Para cada filho do nó, delete ele recursivamente */
    for (int i=0; i<NUMARY; i++) {
        Patricia::Delete(&ponteiros[i]);
    }
}

/* Calcula a altura de um nó interno */
int NodeInterno::Altura()
{
    /* A altura começa com 0 */
    int altura=0;

    /* Para cada filho dele */
    for (int i=0; i<NUMARY; i++) {
        if (ponteiros[i]) {
            int tmp=0;
            NodePtr const ptr = ponteiros[i];

            /* Pegue a altura do filho e coloque em tmp */
            if (ptr->isFolha()) tmp = ((NodeFolha*) (ptr))->Altura();
            if (ptr->isInterno()) tmp = ((NodeInterno*) (ptr))->Altura();

            /* A altura é o valor máximo entre a atual e a altura do filho */
            altura = std::max(altura, tmp);
        }
    }

    /* Incremente a altura do maior filho e retorne */
    return ++altura;
}

/* Limpa a árvore */
void Patricia::Limpa(void) {
    /* Como o destruidor do nó Interno é recursivo podemos apagar a raiz simplesmente */
    Patricia::Delete(&raiz);
    raiz=nullptr;
}


/* Calcula a altura da árvore patricia */
int Patricia::Altura()
{
    if (raiz==nullptr) return 0;
    if (raiz->isFolha()) return ((NodeFolha*) (raiz))->Altura();
    if (raiz->isInterno()) return ((NodeInterno*) (raiz))->Altura();
    return 0;
}

/* Não usada mais */
void Patricia::LimpaInterno(NodePtr no) {
    if (no==nullptr) return;
    if (no->isInterno()) {
        auto tmp = (NodeInterno*) no;
        for (int i=0; i<NUMARY; i++) {
            LimpaInterno(tmp->ponteiros[i]);
        }
    }
    delete no;
    return;
}


/* Deleta um nó, verificando o tipo dele e qual destruidor deve ser usado */
void Patricia::Delete(NodePtr *no)
{
    if (no==nullptr || *no==nullptr) return;
    if ((*no)->isInterno()) delete (NodeInterno*) *no;
    else if ((*no)->isFolha()) delete (NodeFolha*) *no;
    else delete *no;
    *no = nullptr;
    return;
}

NodeFolha::~NodeFolha()
{
#ifdef DEBUG
    std::cout << "Removendo nó folha id=" << this->id << " chave=" << this->chave << std::endl;
#endif
}
