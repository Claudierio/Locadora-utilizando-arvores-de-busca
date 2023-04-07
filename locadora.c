#include <stdio.h>
#include <stdlib.h>
#include "locadora.h"
#include <string.h>

/////////////////////////// INICIANDO AS ARVORES /////////////////////////
void inicializarBst(bst *raiz){
	*raiz = NULL;
}

void inicializarAvl(avl *raiz){
	*raiz = NULL;
}

void inicializarRB(rb *raiz){
	*raiz = NULL;
}

///////////////////////////////// ARQUIVOS ///////////////////////////////

int inicializarTabela(tabela *tab){
	inicializarBst(&tab->arvoreBstIndice);
	inicializarAvl(&tab->arvoreAvlIndice);
	inicializarRB(&tab->arvoreRBIndice);

	tab->arquivoBst = fopen("dados.dat", "a+b"); 
	tab->arvoreBstIndice = carregarArquivoBst("indicesBst.dat", tab->arvoreBstIndice);
	tab->arvoreAvlIndice = carregarArquivoAvl("indicesAvl.dat", tab->arvoreAvlIndice);
	tab->arvoreRBIndice = carregarArquivoRB("indicesRB.dat", tab->arvoreRBIndice);

	if(tab->arquivoBst != NULL){
		return 1;
	}

	else{
		return 0;
	}
}

void finalizar (tabela *tab){
	fclose(tab->arquivoBst);
	salvarArquivoBst("indicesBst.dat", tab->arvoreBstIndice);
	salvarArquivoAvl("indicesAvl.dat", tab->arvoreAvlIndice);
	salvarArquivoRB("indicesRB.dat", tab->arvoreRBIndice);
}

void adicionarDvd(tabela *tab, dvd *p){
	int posicaoNovoRegistro;

	if(tab->arquivoBst != NULL) {
            //BST
			fseek(tab->arquivoBst, 0L, SEEK_END);
			posicaoNovoRegistro = ftell(tab->arquivoBst);
			indiceBst *novo = (indiceBst*)malloc(sizeof(indiceBst));
			novo->chave = p->codigo;
			novo->indice = posicaoNovoRegistro;
			tab->arvoreBstIndice = adicionarBst(novo, tab->arvoreBstIndice);

            //AVL
			indiceAvl *aux = (indiceAvl*)malloc(sizeof(indiceAvl));
			strcpy(aux->chave, p->titulo);
			aux->indice = posicaoNovoRegistro;
			tab->arvoreAvlIndice = adicionarAvl(aux, tab->arvoreAvlIndice);
			fwrite(p, sizeof(dvd), 1, tab->arquivoBst);

            //RB
			indiceRB *novoRb = (indiceRB*)malloc(sizeof(indiceRB));
			novoRb->chave = p->lancamento;
			novoRb->indice = posicaoNovoRegistro;
			inserirRB(&tab->arvoreRBIndice, novoRb);

	}
}
/////////////////////////////////////////////////////////////////////

///////////////////////////////// BST ///////////////////////////////

bst adicionarBst(indiceBst *valor, bst raiz){
	if (raiz == NULL){
        bst novo = (bst)malloc(sizeof(struct no_bst));
        novo->dado = valor;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    if (raiz->dado->chave < valor->chave){ //Se a chave do valor a ser adicionado for maior que a chave da raiZ:
       raiz->dir = adicionarBst(valor, raiz->dir); 
    }

    else{
      raiz->esq = adicionarBst(valor, raiz->esq);
    }

    return raiz;
}

int altura(bst raiz) {
	if(raiz == NULL) {
		return 0;
	}

	return 1 + maior(altura(raiz->dir), altura(raiz->esq));
}

int maior(int a, int b){
	if(a > b)
		return a;
	else
		return b;
}


void imprimirBst(bst raiz, tabela * tab){
	dvd * temp = (dvd *) malloc (sizeof(dvd));
	if (!temp) {
		printf("Erro: falha na alocação.\n");
		return;
	}

	if (fseek(tab->arquivoBst, raiz->dado->indice, SEEK_SET) != 0){
		printf("Erro: falha ao posicionar o ponteiro do arquivo.\n");
		return;
	}

		if (fread(temp, sizeof(dvd), 1, tab->arquivoBst) != 1){
		printf("Erro: na leitura de dados do arquivo.\n");
		return;
	}

	printf("[%d, %s, %s]\n", raiz->dado->chave, temp->titulo, temp->roteirista);
	free(temp);
}

indiceBst *maiorElementoBst(bst raiz) {
	if(raiz == NULL)
			return NULL;

	if(raiz->dir == NULL){
			return raiz->dado;
		}

	else{
			return maiorElementoBst(raiz->dir);
		}
}

void preOrderBst(bst raiz, tabela *tab) {
	if(raiz != NULL){

		imprimirBst(raiz, tab);
		preOrderBst(raiz->esq, tab);
		preOrderBst(raiz->dir, tab);
	}
}

void inOrderBst(bst raiz, tabela* tab) {
	if(raiz != NULL) {
		inOrderBst(raiz->esq, tab);
		imprimirBst(raiz, tab);
		inOrderBst(raiz->dir, tab);
	}
}

dvd * procurarFilmeBst(tabela *tab, int chave) {
    if(tab->arquivoBst != NULL){
    no_bst *temp;
    temp = tab->arvoreBstIndice;

    while(temp!=NULL){
        if(temp->dado->chave == chave){

            fseek(tab->arquivoBst, temp->dado->indice, SEEK_SET);

			dvd * encontrado = (dvd*) malloc(sizeof(dvd));
            fread(encontrado, sizeof(dvd), 1, tab->arquivoBst);

            printf("----------------\n");
            printf("|ID Encontrado!|\n");
            printf("----------------\n");
            return encontrado;
        } 

        else {


            if(chave > temp->dado->chave)
            {
                temp = temp->dir;
            }
            else
            {
                temp = temp->esq;                
            }
        }
    }
    }
    return NULL;
}

void removerDvd(tabela *tab, int id){
	//Chamando a procurar para auxiliar na funcao remover
	dvd *p = procurarFilmeBst(tab, id);

	if(p == NULL){
		printf("[ID Nao Encontrado]\n");
		return;
	}

	else{
		//removendo
		tab->arvoreBstIndice = removerBst(p->codigo, tab->arvoreBstIndice);
		tab->arvoreAvlIndice = removerAvl(tab->arvoreAvlIndice, p->titulo);
        removerRB(&tab->arvoreRBIndice, p->lancamento);
		printf("[ID REMOVIDO]\n");
	}
}

void ajustarRemocao(rb* raiz, rb filho, rb pai) {
    rb irmao;
    
    while ((filho == NULL || filho->c == PRETO) && filho != *raiz) {
        if (filho == pai->esq) {
            irmao = pai->dir;
            
            if (irmao->c == VERMELHO) {
                irmao->c = PRETO;
                pai->c = VERMELHO;
                rotacao_simples_esq(raiz, pai, 1);
                irmao = pai->dir;
            }
            
            if ((irmao->esq == NULL || irmao->esq->c == PRETO) &&
                (irmao->dir == NULL || irmao->dir->c == PRETO)) {
                irmao->c = VERMELHO;
                filho = pai;
                pai = filho->pai;
            } else {
                if (irmao->dir == NULL || irmao->dir->c == PRETO) {
                    irmao->esq->c = PRETO;
                    irmao->c = VERMELHO;
                    rotacao_simples_dir(raiz, irmao, 1);
                    irmao = pai->dir;
                }
                
                irmao->c = pai->c;
                pai->c = PRETO;
                irmao->dir->c = PRETO;
                rotacao_simples_esq(raiz, pai, 1);
                filho = *raiz;
                break;
            }
        } else {
            irmao = pai->esq;
            
            if (irmao->c == VERMELHO) {
                irmao->c = PRETO;
                pai->c = VERMELHO;
                rotacao_simples_dir(raiz, pai, 1);
                irmao = pai->esq;
            }
            
            if ((irmao->dir == NULL || irmao->dir->c == PRETO) &&
                (irmao->esq == NULL || irmao->esq->c == PRETO)) {
                irmao->c = VERMELHO;
                filho = pai;
                pai = filho->pai;
            } else {
                if (irmao->esq == NULL || irmao->esq->c == PRETO) {
                    irmao->dir->c = PRETO;
                    irmao->c = VERMELHO;
                    rotacao_simples_esq(raiz, irmao, 1);
                    irmao = pai->esq;
                }
                
                irmao->c = pai->c;
                pai->c = PRETO;
                irmao->esq->c = PRETO;
                rotacao_simples_dir(raiz, pai, 1);
                filho = *raiz;
                break;
            }
        }
    }
    
    if (filho != NULL) {
        filho->c = PRETO;
    }
}

 void removerRB(rb* raiz, int chave){
    rb filho, pai, sucessor;
    rb removido;
    int cor_removido;

    removido = buscar(*raiz, chave);
    if(removido == NULL){
        return;
    }

    if(removido->esq != NULL && removido->dir != NULL){
        sucessor = removido->dir;
        while(sucessor->esq != NULL){
            sucessor = sucessor->esq;
        }
        removido->dado = sucessor->dado;
        removido = sucessor;
    }

    cor_removido = removido->c;
    if (removido->esq == NULL) {
    filho = removido->dir;
    } else {
        filho = removido->esq;
    }
    pai = removido->pai;

    if(filho != NULL){
        filho->pai = pai;
    }

    if(pai == NULL){
        *raiz = filho;
    }
    else{
        if(removido == pai->esq){
            pai->esq = filho;
        }
        else{
            pai->dir = filho;
        }
    }

    if(cor_removido == PRETO){
        ajustarRemocao(raiz, filho, pai);
    }

    free(removido);
}

rb buscar(rb raiz, int chave) {
    // Se a árvore está vazia ou a chave é encontrada na raiz, retorna a raiz
    if (raiz == NULL || raiz->dado->chave == chave) {
        return raiz;
    }
    
    // Se a chave é menor que a raiz, busca na subárvore esquerda
    if (chave < raiz->dado->chave) {
        return buscar(raiz->esq, chave);
    }
    
    // Caso contrário, busca na subárvore direita
    return buscar(raiz->dir, chave);
}


bst removerBst (int valor, bst raiz) {

	if(raiz == NULL){ //Verifica se a raiz é nula
		return NULL;

	}
	
	if(raiz->dado->chave == valor) { // Verifica se a chave do nó raiz é igual ao valor que se deseja remover

		if(raiz->esq == NULL) 	
		{
			return raiz->dir;
		}

		if(raiz->dir == NULL) 
		{
			return raiz->esq;
		}

		raiz->dado = maiorElementoBst(raiz->esq);
		raiz->esq = removerBst(raiz->dado->chave, raiz->esq);

		return raiz;
	}	

	if(valor > raiz->dado->chave) { // Se o valor que se deseja remover é maior que a chave do nó raiz, chama a função removerBst na subárvore direita
			raiz->dir = removerBst(valor, raiz->dir);
	} 

	else { 						   // Caso contrário, chama a função removerBst na subárvore esquerda
			raiz->esq = removerBst(valor, raiz->esq);
	}

	return raiz;
}

////////////////////////////////////////////////////////////////////////

///////////////////////////////// AVL ///////////////////////////////

avl maiorElementoAvl (avl raiz){
    avl aux = raiz;
    while(aux != NULL){
        aux = aux->dir;
    }
    return aux;
}

int alturaNo(avl no){
    if(no == NULL){
        return -1;
    }

    else{
        return no->altura;
    }
}

int fatorBalanceamento(avl no){
    if(no != NULL){

        return (alturaNo(no->esq) - alturaNo(no->dir)); //calculo do fator de balanceamento
    }

    else{

        return 0;
    }
}

avl RotacaoEsquerda(avl raiz){
    avl y,f;

    y = raiz->dir;
    f = y->esq;

    y->esq = raiz;
    raiz->dir = f;

    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;
    y->altura = maior(alturaNo(y->esq), alturaNo(y->dir)) + 1;

    return y;
}

avl RotacaoDireita(avl raiz){
    avl y,f;

    y = raiz->esq;
    f = y->dir;

    y->dir = raiz;
    raiz->esq = f;

    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;
    y->altura = maior(alturaNo(raiz->esq),alturaNo(raiz->dir)) + 1;

    return y;
}

avl RotacaoDuplaEsquerda(avl raiz){
    raiz->dir = RotacaoDireita(raiz->dir);
    return RotacaoEsquerda(raiz);
}

avl RotacaoDuplaDireita(avl raiz){
    raiz->esq = RotacaoEsquerda(raiz->esq);
    return RotacaoDireita(raiz);
}

avl balancear(avl raiz){

    int fb = fatorBalanceamento(raiz);

    if(fb < -1 && fatorBalanceamento(raiz->dir) <= 0){
        raiz = RotacaoEsquerda(raiz);
    }

    else if(fb > 1 && fatorBalanceamento(raiz->esq) >= 0){
        raiz = RotacaoDireita(raiz);
    }

    else if(fb > 1 && fatorBalanceamento(raiz->esq) < 0){
        raiz = RotacaoDuplaDireita(raiz);
    }

    else if(fb < -1 && fatorBalanceamento(raiz->dir) < 0){
        raiz = RotacaoDuplaEsquerda(raiz);
    }
    
    return raiz;
}

avl adicionarAvl(indiceAvl *valor, avl raiz){
    if (raiz == NULL)
    {
        avl novo = (avl)malloc(sizeof(struct no_avl));
        novo->dado = valor;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->altura = 0;
        return novo;
    }

    else{
        if(strcmp(valor->chave, raiz->dado->chave) > 0)
        {
        	printf("Direita:\n raiz: %s \n valor: %s \n", raiz->dado->chave, valor->chave);
           raiz->dir = adicionarAvl(valor, raiz->dir);
        }

        else
        {
        	printf("Esquerda:\n raiz: %s \n valor: %s \n", raiz->dado->chave, valor->chave);
          raiz->esq = adicionarAvl(valor, raiz->esq);
        }
    }

    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;

    return raiz;
}

avl removerAvl(avl raiz, char* valor){
    if(raiz == NULL){
        return NULL;
    }
    else{
        if(strcmp(raiz->dado->chave, valor) == 0){
            if(raiz->esq == NULL && raiz->dir == NULL)
            {
                free(raiz);
                return NULL;
            }
            if(raiz->esq == NULL && raiz->dir != NULL)
            {
                avl temp = raiz->dir;
                free(raiz);
                return temp;
            }
            if(raiz->esq != NULL && raiz->dir == NULL)
            {
                avl temp = raiz->esq;
                free(raiz);
                return temp;
            }

            avl temp = maiorElementoAvl(raiz->esq);
            strcpy(raiz->dado->chave, temp->dado->chave); //raiz->valor = temp->valor;
            strcpy(temp->dado->chave, valor); //temp->valor = valor;
            raiz->esq = removerAvl(raiz->esq, valor);
            return raiz;
        }
        else{
            if(strcmp(raiz->dado->chave,valor) > 0)
            {
                raiz->esq = removerAvl(raiz->esq, valor);
            }
            else
            {
                raiz->dir = removerAvl(raiz->dir, valor);
            }
        }
    }
    
    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;
    raiz = balancear(raiz);

    return raiz;
    }

void imprimirAvl(avl raiz, tabela * tab) {
	dvd *temp = (dvd*) malloc(sizeof(dvd));
	if (!temp)
	{
		printf("Erro: falha ao alocar memória.\n");
		return;
	}
	if (fseek(tab->arquivoBst, raiz->dado->indice, SEEK_SET) != 0) {
		printf("Erro: falha ao posicionar o ponteiro do arquivo.\n");
		return;
	}
		if (fread(temp, sizeof(dvd), 1, tab->arquivoBst) != 1) {
		printf("Erro: falha ao ler dados do arquivo.\n");
		return;
	}
	printf("[%d, %s, %s]\n",temp->codigo, raiz->dado->chave, temp->roteirista);
}

void inOrderAvl(avl raiz, tabela* tab) {
	if(raiz != NULL) {
		inOrderAvl(raiz->esq, tab);
		imprimirAvl(raiz, tab);
		inOrderAvl(raiz->dir, tab);
	}
}

dvd * procurarFilmeAvl(tabela *tab, char* chave) {
    if(tab->arquivoBst != NULL) {
    no_avl *temp;
    temp = tab->arvoreAvlIndice;
    while(temp!=NULL){
        if (strcmp(temp->dado->chave,chave) == 0){

            fseek(tab->arquivoBst, temp->dado->indice, SEEK_SET);

			dvd * encontrado = (dvd*) malloc(sizeof(dvd));
            fread(encontrado, sizeof(dvd), 1, tab->arquivoBst);

            int tam;
			fread(&tam, sizeof(int), 1, tab->arquivoBst);
			encontrado->titulo = (char*)malloc(tam*sizeof(char));
			fread(encontrado->titulo, sizeof(char), tam,tab->arquivoBst);

            printf("--------------------\n");
            printf("|Titulo Encontrado!|\n");
            printf("--------------------\n");
            return encontrado;
        }
        else {
            printf("-----------------------\n");
            printf("|Titulo Nao Encontrado|\n");
            printf("-----------------------\n");
            if(strcmp(chave, temp->dado->chave) > 0)
            {
                temp = temp->dir;
            }
            else{

                temp = temp->esq;                
            }
        }
    }
    }
    return NULL;
}



dvd *ler_dados() {
	dvd *novo = (dvd*) malloc(sizeof(dvd));
	char * buffer = (char *) malloc(256 * sizeof(char));
	/*getchar();
	printf("|Titulo: ");
	fgets(novo->titulo, 20,  stdin);
	tirar_enter(novo->titulo);*/
	
	getchar() ;
	printf("|Titulo: ");
	fgets(buffer, 255,  stdin);
	tirar_enter(buffer);
	novo->titulo =strdup(buffer);

	printf("|Roteirista: ");
	fgets(novo->roteirista, 60,  stdin);
	tirar_enter(novo->roteirista);

	printf("|Lancamento: ");
	scanf("%d", &novo->lancamento);

	printf("|Preco do dvd: ");
	scanf("%f", &novo->preco);

	printf("|Codigo: ");
	scanf("%d", &novo->codigo);
	printf("\n");

	return novo;
}

void tirar_enter(char *string) {
	string[strlen(string) -1] = '\0';
}

/////////////////////////////////////////////////////////


////////////////////// SALVAR BST ////////////////////// 

void salvarArquivoBst(char *titulo, bst a) {
	FILE *arq;
	arq = fopen(titulo, "wb");
	if(arq != NULL) {
		salvarAuxiliarBst(a, arq);
		fclose(arq);
	}
}

void salvarAuxiliarBst(bst raiz, FILE *arq){
	if(raiz != NULL) {		
		fwrite(raiz->dado, sizeof(indiceBst), 1, arq);
		salvarAuxiliarBst(raiz->esq, arq);
		salvarAuxiliarBst(raiz->dir, arq);
	}
}

bst carregarArquivoBst(char *titulo, bst a) {
	FILE *arq;
	arq = fopen(titulo, "rb");
	indiceBst * temp;
	if(arq != NULL) {
		temp = (indiceBst*) malloc(sizeof(indiceBst));
		while(fread(temp, sizeof(indiceBst), 1, arq)) {
			
			a = adicionarBst(temp, a);			
			temp = (indiceBst*) malloc(sizeof(indiceBst));

		}
		fclose(arq);
	}
	return a;
}

//////////////////////////////////////////////////////////////////

////////////////////// SALVAR - AVL //////////////////////

void salvarArquivoAvl(char *titulo, avl a) {
	FILE *arq;
	arq = fopen(titulo, "wb");
	if(arq != NULL) {
		salvarAuxiliarAvl(a, arq);
		fclose(arq);
	}
}

void salvarAuxiliarAvl(avl raiz, FILE *arq){
	if(raiz != NULL) {		
		fwrite(raiz->dado, sizeof(indiceAvl), 1, arq);
		salvarAuxiliarAvl(raiz->esq, arq);
		salvarAuxiliarAvl(raiz->dir, arq);
	}
}

avl carregarArquivoAvl(char *titulo, avl a) {
	FILE *arq;
	arq = fopen(titulo, "rb");
	indiceAvl * temp;
	if(arq != NULL) {
		temp = (indiceAvl*) malloc(sizeof(indiceAvl));
		while(fread(temp, sizeof(indiceAvl), 1, arq)) {
			
			a = adicionarAvl(temp, a);			
			temp = (indiceAvl*) malloc(sizeof(indiceAvl));

		}
		fclose(arq);
	}
	return a;
}

//////////////////////////////////////////////////////////////////

//////////////////////ADICIONAR RUBRO NEGRA//////////////////////

rb adicionarRB(indiceRB *valor){

   rb novo = (rb)malloc(sizeof(struct no_rb));
   novo->dado = valor;
   novo->pai = NULL;
   novo->esq = NULL;
   novo->dir = NULL;
   novo->c = VERMELHO;
}


int corRB(rb no) {
   if(no == NULL || no->c == PRETO)
   return 1;
   else
   return 0;
}

int eh_esquerdo(rb no) {
   if(no == no->pai->esq)
   return 1;
   else
   return 0;
}

int eh_raiz(rb no) {
   if(no->pai == NULL)
   return 1;
   else
   return 0;
}

rb tio (rb n) {
   if(eh_esquerdo(n->pai))
   return n->pai->pai->dir;
   else
   return n->pai->pai->esq;
}

void rotacao_simples_dir(rb *raiz, rb no, int simples){
   rb p, t;

   p = no->esq;
   t = p->dir;

   no->esq = t;
   p->dir = no;

   if(simples){
      p->c = PRETO;
      no->c = VERMELHO;
   }

   if(t != NULL)
   t->pai = no;

   p->pai = no->pai;

   if(eh_raiz(no)){
      *raiz = p;
   } else {
      if(eh_esquerdo(no)){
         no->pai->esq = p;
      } else {
         no->pai->dir = p;
      }
   }
   no->pai = p;
}

void rotacao_simples_esq(rb* raiz, rb no, int simples){
   rb p, t;

   p = no->dir;
   t = p->esq;

   no->dir = t;
   p->esq = no;

   if(simples){
      p->c = PRETO;
      no->c = VERMELHO;
   }

   if(t != NULL){
      t->pai = no;
   }

   p->pai = no->pai;

   if(eh_raiz(no)){
      *raiz = p;
   } else {
      if(eh_esquerdo(no)){
         no->pai->esq = p;
      } else {
         no->pai->dir = p;
      }
   }
   no->pai = p;

}

void rotacao_dupla_esq(rb* raiz, rb no){
   rotacao_simples_dir(raiz, no->dir, 0);
   rotacao_simples_esq(raiz, no, 0);

   no->pai->c = PRETO;
   no->c = VERMELHO;
}

void rotacao_dupla_dir(rb* raiz, rb no){
   rotacao_simples_esq(raiz, no->esq, 0);
   rotacao_simples_dir(raiz, no, 0);

   no->pai->c = PRETO;
   no->c = VERMELHO;
}

void ajustarRB(rb* raiz, rb no){
   if(eh_raiz(no)){
      no->c = PRETO;
      return;
   } else if(no->pai->c == VERMELHO){
      if(corRB(tio(no)) == VERMELHO){ //CASO 1
         no->pai->c = PRETO;
         tio(no)->c = PRETO;
         no->pai->pai->c = VERMELHO;
         ajustarRB(raiz, no->pai->pai);
      } else { //CASO 2: TIO PRETOF
         if(eh_esquerdo(no) && eh_esquerdo(no->pai)){
            rotacao_simples_dir(raiz, no->pai->pai, 1);

         } else if(!eh_esquerdo(no) && !eh_esquerdo(no->pai)){ 
            rotacao_simples_esq(raiz, no->pai->pai, 1);

         } else if(eh_esquerdo(no) && !eh_esquerdo(no->pai)){
            rotacao_dupla_esq(raiz, no->pai->pai);

         } else if(!eh_esquerdo(no) && eh_esquerdo(no->pai)){
            rotacao_dupla_dir(raiz, no->pai->pai);
         }
      }
   }
}
void salvarAuxiliarRB(rb raiz, FILE *arq){
	if(raiz != NULL) {		
		fwrite(raiz->dado, sizeof(indiceRB), 1, arq);
		salvarAuxiliarRB(raiz->esq, arq);
		salvarAuxiliarRB(raiz->dir, arq);
	}
}

void salvarArquivoRB(char *titulo, rb a) {
	FILE *arq;
	arq = fopen(titulo, "wb");

	if(arq != NULL) {
		salvarAuxiliarRB(a, arq);
		fclose(arq);
	}
}


rb carregarArquivoRB(char *titulo, rb a) {
	FILE *arq;
	arq = fopen(titulo, "rb");
	indiceRB * temp;
	if(arq != NULL) {
		temp = (indiceRB*) malloc(sizeof(indiceRB));

		while(fread(temp, sizeof(indiceRB), 1, arq)) {
			
			a = adicionarRB(temp);			
			temp = (indiceRB*) malloc(sizeof(indiceRB));

		}
		fclose(arq);
	}
	return a;
}

void inserirRB(rb* raiz, indiceRB *info){ 
   rb temp, pai, novo;
   temp  = *raiz;
   pai = NULL;

   while(temp != NULL){
      pai = temp;

      if(info->chave > temp->dado->chave){
         temp = temp->dir;
      }

      else{
         temp = temp->esq;
      }
   }

   novo = adicionarRB(info);
   novo->pai = pai;

   if(eh_raiz(novo)){
      *raiz = novo;
   }

   else if(info->chave < pai->dado->chave){
      pai->esq = novo;
   }

   else{
      pai->dir = novo;
   }
   ajustarRB(raiz, novo);
}


void imprimirRB(rb raiz, tabela * tab) {
	dvd *temp = (dvd*) malloc(sizeof(dvd));
	if (!temp)
	{
		printf("Erro: falha ao alocar memória.\n");
		return;
	}
	if (fseek(tab->arquivoBst, raiz->dado->indice, SEEK_SET) != 0) {
		printf("Erro: falha ao posicionar o ponteiro do arquivo.\n");
		return;
	}
		if (fread(temp, sizeof(dvd), 1, tab->arquivoBst) != 1) {
		printf("Erro: falha ao ler dados do arquivo.\n");
		return;
	}
	printf("[%d, %s, %s]\n", raiz->dado->chave, temp->titulo, temp->roteirista);
	free(temp);
}


void inOrderRB(rb raiz, tabela* tab){
	if(raiz != NULL){
		inOrderRB(raiz->esq, tab);
		imprimirRB(raiz, tab);
		inOrderRB(raiz->dir, tab);
	}
}

dvd * procurarFilmeRB(tabela *tab, int chave) {
    if(tab->arquivoBst != NULL){
    no_rb *temp;
    temp = tab->arvoreRBIndice;

    while(temp!=NULL){
        if(temp->dado->chave == chave){

            fseek(tab->arquivoBst, temp->dado->indice, SEEK_SET);

			dvd * encontrado = (dvd*) malloc(sizeof(dvd));
            fread(encontrado, sizeof(dvd), 1, tab->arquivoBst);

            printf("-------------------------------\n");
            printf("|Data de lancamento Encontrado.|\n");
            printf("--------------------------------\n");
            return encontrado;
        } 

        else {
            printf("------------------------------------\n");
            printf("|Data de lancamento Nao Encontrado.|\n");
            printf("------------------------------------\n");
            if(chave > temp->dado->chave)
            {
                temp = temp->dir;
            }
            else
            {
                temp = temp->esq;                
            }
        }
    }
    }
    return NULL;
}