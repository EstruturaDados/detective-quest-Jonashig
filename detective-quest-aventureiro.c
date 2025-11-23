#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===============================================================
    STRUCTS PRINCIPAIS
================================================================*/

/* Sala da mansão (árvore binária de exploração) */
typedef struct Sala {
    char nome[50];
    char pista[100];     // Pista opcional associada ao cômodo
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Nó da BST de pistas coletadas */
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;


/*===============================================================
    FUNÇÕES DA BST DE PISTAS
================================================================*/

/* Cria um novo nó de pista */
PistaNode* criarPistaNode(const char *p) {
    PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, p);
    novo->esq = novo->dir = NULL;
    return novo;
}

/* Insere uma pista na BST (ordem alfabética) */
PistaNode* inserirPista(PistaNode *raiz, const char *p) {
    if (raiz == NULL) {
        return criarPistaNode(p);
    }

    if (strcmp(p, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, p);
    else if (strcmp(p, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, p);

    return raiz;
}

/* Exibe as pistas usando percurso IN-ORDER (ordem alfabética) */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}


/*===============================================================
    FUNÇÕES DA ÁRVORE DE SALAS (mapa da mansão)
================================================================*/

/* Cria uma sala dinamicamente */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao criar sala!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, ""); // sem pista

    nova->esq = nova->dir = NULL;

    return nova;
}

/* Monta o mapa fixo da mansão */
Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes no chão");
    Sala *lab = criarSala("Laboratório", "Frasco vazio com cheiro estranho");
    Sala *biblioteca = criarSala("Biblioteca Antiga", "Livro aberto sobre mutações genéticas");
    Sala *quarto = criarSala("Quarto Abandonado", NULL);
    Sala *porao = criarSala("Porão Secreto", "Mapa rasgado com marcações");
    Sala *jardim = criarSala("Jardim Interno", "Folhas amassadas e trilhas incomuns");
    Sala *saida = criarSala("Saída", NULL);

    /* Estrutura da árvore da mansão */
    hall->esq = lab;
    hall->dir = biblioteca;

    lab->esq = quarto;
    lab->dir = porao;

    biblioteca->esq = jardim;
    biblioteca->dir = saida;

    return hall;
}


/*===============================================================
    EXPLORAÇÃO DA MANSÃO
================================================================*/

/* Explora a mansão a partir de uma sala inicial */
void explorar(Sala *inicio) {
    Sala *atual = inicio;
    PistaNode *pistas = NULL;
    int opcao;

    printf("\nVocê entrou na mansão! Procure pistas e tente encontrar a saída.\n");

    /* Mensagem inicial ao entrar na primeira sala */
    printf("\nVocê está no %s.\n", atual->nome);

    while (1) {

        printf("\n=====================================\n");
        printf("Você está em: %s\n", atual->nome);
        printf("=====================================\n");

        /* Coleta automática da pista da sala */
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            pistas = inserirPista(pistas, atual->pista);
        } else {
            printf("Nenhuma pista nessa sala.\n");
        }

        /* Caso seja a saída */
        if (strcmp(atual->nome, "Saída") == 0) {
            printf("\nParabéns! Você encontrou a saída!\n");
            printf("\nPistas coletadas durante a aventura (ordem alfabética):\n\n");
            exibirPistas(pistas);

            printf("\n1 - Jogar novamente\n");
            printf("0 - Encerrar\n");
            printf("Escolha: ");
            scanf("%d", &opcao);

            if (opcao == 1)
                return;
            else
                exit(0);
        }

        /* Opções de navegação */
        printf("\nEscolha seu caminho:\n");
        printf("1 - Esquerda");
        if (atual->esq == NULL) printf(" (sem saída)");
        printf("\n");

        printf("2 - Direita");
        if (atual->dir == NULL) printf(" (sem saída)");
        printf("\n");

        printf("3 - Sair para o menu\n");

        printf("Escolha: ");
        scanf("%d", &opcao);

        Sala *anterior = atual;  //  movimento

        switch (opcao) {
            case 1:
                if (atual->esq != NULL) {
                    atual = atual->esq;
                    printf("\nVocê caminha e entra em: %s\n", atual->nome);
                } else {
                    printf("Caminho inexistente!\n");
                }
                break;

            case 2:
                if (atual->dir != NULL) {
                    atual = atual->dir;
                    printf("\nVocê caminha e entra em: %s\n", atual->nome);
                } else {
                    printf("Caminho inexistente!\n");
                }
                break;

            case 3:
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}


/*===============================================================
    MENU PRINCIPAL
================================================================*/

int main() {
    int opcao;
    Sala *mansao = montarMansao();

    while (1) {
        printf("\n=====================================\n");
        printf("      BEM-VINDO AO DETECTIVE QUEST\n");
        printf("            * AVENTUREIRO *\n");
        printf("=====================================\n");
        printf("1 - Entrar na mansão\n");
        printf("0 - Encerrar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1)
            explorar(mansao);
        else if (opcao == 0)
            break;
        else
            printf("Opção inválida!\n");
    }

    printf("Até a próxima aventura!\n");
    return 0;
}