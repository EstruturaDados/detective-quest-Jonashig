#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=========================================================
    STRUCT DA ÁRVORE BINÁRIA
    Cada sala representa um nó da árvore.
=========================================================*/
typedef struct Sala {
    char nome[50];
    struct Sala *esq;   // Caminho à esquerda
    struct Sala *dir;   // Caminho à direita
} Sala;

/*=========================================================
    FUNÇÃO PARA CRIAR UMA SALA DINAMICAMENTE
=========================================================*/
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

/*=========================================================
    CRIAÇÃO DA MANSÃO (ÁRVORE BINÁRIA)
    Hall é a raiz
=========================================================*/
Sala* montarMansao() {
    /* Nós principais */
    Sala *hall = criarSala("Hall de Entrada");
    Sala *quarto = criarSala("Quarto Antigo");
    Sala *salaMisteriosa = criarSala("Sala Misteriosa");
    Sala *corredor = criarSala("Corredor Longo");
    Sala *escada = criarSala("Escada Velha");
    Sala *saida = criarSala("Saída");

    /* Montagem da árvore */
    hall->esq = quarto;
    hall->dir = salaMisteriosa;

    quarto->esq = corredor;
    quarto->dir = NULL;

    salaMisteriosa->esq = escada;
    salaMisteriosa->dir = NULL;

    corredor->esq = saida; // só para ter um caminho até a saída
    corredor->dir = NULL;

    escada->esq = NULL;
    escada->dir = saida;

    saida->esq = NULL;
    saida->dir = NULL;

    return hall;
}

/*=========================================================
    LOOP DE EXPLORAÇÃO DA MANSÃO
=========================================================*/
void explorar(Sala *inicio) {
    Sala *atual = inicio;
    int opcao;

    printf("\nVocê está em uma mansão e precisa encontrar a saída.\n");

    while (1) {

        /* Quando chegar na sala "Saída" */
        if (strcmp(atual->nome, "Saída") == 0) {
            printf("\n===============================\n");
            printf("PARABÉNS! Você encontrou a saída!\n");
            printf("===============================\n\n");

            printf("1 - Voltar ao jogo (reiniciar)\n");
            printf("0 - Encerrar partida\n");
            printf("Escolha: ");
            scanf("%d", &opcao);
            getchar();

            if (opcao == 1)
                return; // volta ao menu e inicia o jogo de novo
            else
                exit(0);
        }

        printf("\n===================================\n");
        printf("Você está na sala: %s\n", atual->nome);
        printf("===================================\n");
        printf("Você decide ir:\n\n");

        printf("1 - Esquerda");
        if (atual->esq == NULL) printf(" (sem saída)");
        printf("\n");

        printf("2 - Direita");
        if (atual->dir == NULL) printf(" (sem saída)");
        printf("\n");

        printf("3 - Sair da sala (voltar ao menu)\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                if (atual->esq != NULL)
                    atual = atual->esq;
                else
                    printf("Não há caminho à esquerda!\n");
                break;

            case 2:
                if (atual->dir != NULL)
                    atual = atual->dir;
                else
                    printf("Não há caminho à direita!\n");
                break;

            case 3:
                return; // volta ao menu
                break;

            default:
                printf("Opção inválida!\n");
        }

    }
}

/*=========================================================
    MENU PRINCIPAL
=========================================================*/
int main() {
    int opcao;
    Sala *mansao = montarMansao();

    while (1) {
        printf("\n===================================\n");
        printf("   BEM-VINDO AO DETECTIVE QUEST\n");
        printf("===================================\n\n");

        printf("1 - Entrar no jogo\n");
        printf("0 - Encerrar partida\n");
        printf("Escolha: ");

        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            explorar(mansao); // inicia exploração
        }
        else if (opcao == 0) {
            printf("\nEncerrando... Até a próxima aventura!\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}
