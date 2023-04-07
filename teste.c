#include <stdio.h>
#include <stdlib.h>
#include "locadora.h"
#include "locadora.c"

int main(int argc, char * argv[]) {
	tabela tab;
	int opcao, info;
	char op[10];
	inicializarTabela(&tab);

	while(1) {
		printf("---------------------------- LOCADORA GeeksBR -------------------------\n");	
		printf("\t\t\t-------------------------");
		printf("\n\t\t\t|1-Adicionar Filme\t|\n\t\t\t|2-Altura\t\t|\n\t\t\t|3-Procurar (BST)\t|\n\t\t\t|4-Procurar (AVL)\t|\n\t\t\t|5-Procurar (RB)\t|\n\t\t\t|6-Pre order BST\t|\n\t\t\t|7-In Order das arvores\t|\n\t\t\t|8-Remover\t\t|\n\t\t\t|99-Sair\t\t|\n");
		printf("\t\t\t-------------------------\n\nDigite uma opcao: ");
		scanf("%d", &opcao);

		switch(opcao) {
				int valor;
				char titulo[61];
				
				case 1:
						adicionarDvd(&tab, ler_dados());
						system("clear");
						printf("------------------------------\n");
						printf("|Filme Adicionado Com Sucesso!|\n");
						printf("------------------------------\n");
						break;
				case 2:
						system("clear");
						printf("\n[Altura da Árvore BST: %d]\n", altura(tab.arvoreBstIndice));
						break;

				case 3:
						printf("Digite o codigo do filme (BST): ");
						scanf("%d", &valor);
						dvd* encontrado = procurarFilmeBst(&tab, valor);
						printf("[codigo: %d] [nome: %s] [preco: %f]\n", encontrado->codigo, encontrado->titulo, encontrado->preco);
						break;

				case 4:
						printf("Digite o titulo do filme (AVL): ");
						while(getchar() != '\n');
                        char buffer[200];
                        fgets(buffer, 200, stdin);
                        buffer[strcspn(buffer, "\n")] = 0;

						encontrado = procurarFilmeAvl(&tab, buffer);
						printf("[codigo: %d] [lancamento: %d] [preco: %f]\n", encontrado->codigo, encontrado->lancamento, encontrado->preco);
						break;

				case 5:
						printf("Digite o ano de lancamento do filme (RB): ");
						scanf("%d", &valor);
						encontrado = procurarFilmeRB(&tab, valor);
						printf("[codigo: %d] [nome: %s] [Lancamento: %d]\n", encontrado->codigo, encontrado->titulo, encontrado->lancamento);
						break;

				case 6:
						system("clear");
						printf("\nPré order da Árvore BST: \n");
						preOrderBst(tab.arvoreBstIndice, &tab);
						printf("\n");
						break;
				case 7:
						system("clear");
						printf("\nIn order da Arvore BST: (CODIGO)\n");
						inOrderBst(tab.arvoreBstIndice, &tab);
						printf("\n");
						printf("\nIn order da Arvore AVL: (TITULO)\n");
						inOrderAvl(tab.arvoreAvlIndice, &tab);
						printf("\n");
						printf("\nIn order da Arvore RB: (LANCAMENTO)\n");
						inOrderRB(tab.arvoreRBIndice, &tab);
						scanf("%c", op);
						break;
						
				case 8:
						system("clear");
						printf("\nInforme o codigo do livro que deseja remover: ");
						scanf("%d", &valor);
						removerDvd(&tab, valor);
						break;
				
				case 99:
						finalizar(&tab);
						exit(0);
		}
	}
}
