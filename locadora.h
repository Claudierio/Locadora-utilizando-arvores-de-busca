#ifndef LOCADORA_H
#define LOCADORA_H

typedef struct filme{
	char *titulo;
	char roteirista[41];
	int lancamento;
	int codigo;
	float preco;
}dvd;

///////////////////////////////// BST ///////////////////////////////

typedef struct indiceBst {
	int chave;
	int indice;
} indiceBst;

typedef struct no_bst{
	indiceBst *dado;
	struct no_bst *esq, *dir;
}no_bst;

typedef no_bst *bst;


///////////////////////////////// AVL ///////////////////////////////

typedef struct indiceAvl {
	char chave[61];
	int indice;
} indiceAvl;

typedef struct no_avl
{
    indiceAvl *dado;
    struct no_avl *esq, *dir;
    int altura;
}no_avl;

typedef no_avl *avl;

enum cor {VERMELHO, PRETO};

typedef struct indiceRB{
	int chave;
	int indice;
} indiceRB;

typedef struct no_rb
{
    indiceRB *dado;
    enum cor c;
    struct no_rb *esq, *dir, *pai;
    int altura;
}no_rb;

typedef no_rb *rb;

typedef struct tabela {
	FILE *arquivoBst;
	bst arvoreBstIndice;
	avl arvoreAvlIndice;
	rb arvoreRBIndice;
} tabela;

void inicializarBst(bst *raiz);
void inicializarAvl(avl *raiz);
void inicializarRB(rb *raiz);

int inicializarTabela(tabela *tab);
void finalizar (tabela *tab);
void adicionarDvd(tabela *tab, dvd *p);


bst adicionarBst(indiceBst *valor, bst raiz);
int altura(bst raiz);
int maior(int a, int b);
void imprimirBst(bst raiz, tabela * tab);
indiceBst *maiorElementoBst(bst raiz);
void preOrderBst(bst raiz, tabela *tab);
void inOrderBst(bst raiz, tabela* tab);
bst removerBst (int valor, bst raiz);


avl maiorElementoAvl(avl raiz);
int alturaNo(avl no);
int fatorBalanceamento(avl no);
avl RotacaoEsquerda(avl raiz);
avl RotacaoDireita(avl raiz);
avl RotacaoDuplaEsquerda(avl raiz);
avl RotacaoDuplaDireita(avl raiz);
avl balancear(avl raiz);
avl adicionarAvl(indiceAvl *valor, avl raiz);
avl removerAvl(avl raiz, char *valor);
void imprimirAvl(avl raiz, tabela * tab);
void inOrderAvl(avl raiz, tabela* tab);

dvd *ler_dados();
void tirar_enter(char *string);

void salvarArquivoBst(char *titulo, bst a);
void salvarAuxiliarBst(bst raiz, FILE *arq);
bst carregarArquivoBst(char *titulo, bst a);

void salvarArquivoRB(char * chave, rb a);
void salvarAuxiliarRB(rb raiz, FILE *arq);
rb carregarArquivoRB(char *titulo, rb a);


void salvarArquivoAvl(char *titulo, avl a);
void salvarAuxiliarAvl(avl raiz, FILE *arq);
avl carregarArquivoAvl(char *titulo, avl a);

dvd * procurarfilmeBst(tabela *tab, int chave);
dvd * procurarfilmeAvl(tabela *tab, char* chave);

void removerDvd(tabela *tab, int id);
/////////////////////////////////////////////////////////////////////

///////////////////////////////// RB ///////////////////////////////
rb adicionarRB(indiceRB *valor);
int corRB(rb no);
int eh_esquerdo(rb no);
int eh_raiz(rb no);
rb tio (rb n);

void rotacao_simples_dir(rb *raiz, rb no, int simples);
void rotacao_simples_esq(rb* raiz, rb no, int simples);
void rotacao_dupla_esq(rb* raiz, rb no);
void rotacao_dupla_dir(rb* raiz, rb no);
void ajustarRB(rb* raiz, rb no);
void inserirRB(rb* raiz, indiceRB* info);
void imprimirRB(rb raiz, tabela * tab);
void inOrderRB(rb raiz, tabela* tab);
dvd * procurarFilmeRB(tabela *tab, int chave);
rb buscar(rb raiz, int chave);
void ajustarRemocao(rb* raiz, rb filho, rb pai);
void removerRB(rb* raiz, int chave);




#endif
