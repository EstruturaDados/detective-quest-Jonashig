#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------
   DEFINIÇÕES DE TAMANHOS
   --------------------------- */
#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 31   /* tamanho da tabela hash (primo razoável) */

/* ==========================
   STRUCT: Sala (árvore)
   ========================== */
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; /* string vazia "" indica sem pista */
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* ==========================
   STRUCT: BST de pistas
   ========================== */
typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* ==========================
   STRUCT: Entrada da Hash (encadeada)
   key = pista, value = suspeito
   ========================== */
typedef struct HashEntry {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashEntry *prox;
} HashEntry;

/* ==========================
   Tabela Hash (array de ponteiros)
   ========================== */
typedef struct HashTable {
    HashEntry *buckets[HASH_SIZE];
} HashTable;

/* ==========================
   PROTÓTIPOS
   ========================== */
Sala* criarSala(const char *nome, const char *pista);
Sala* montarMansao(void);

PistaNode* criarPistaNode(const char *p);
PistaNode* inserirPistaBST(PistaNode *raiz, const char *p);
int existePistaBST(PistaNode *raiz, const char *p);
void exibirPistasInOrder(PistaNode *raiz);
void liberarPistasBST(PistaNode *raiz);

void inicializarHash(HashTable *ht);
unsigned int hashString(const char *s);
void inserirMapping(HashTable *ht, const char *pista, const char *suspeito);
const char* buscarSuspeitoPorPista(HashTable *ht, const char *pista);
void liberarHash(HashTable *ht);

void explorar(Sala *inicio, HashTable *ht, PistaNode **pistasColetadas);
void fazerAcusacao(PistaNode *pistasColetadas, HashTable *ht);

void limparBuffer(void);

/* ==========================
   IMPLEMENTAÇÃO
   ========================== */

Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) exit(1);

    strncpy(s->nome, nome, MAX_NOME - 1);
    s->nome[MAX_NOME - 1] = '\0';

    if (pista)
        strncpy(s->pista, pista, MAX_PISTA - 1);
    else
        s->pista[0] = '\0';

    s->pista[MAX_PISTA - 1] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

Sala* montarMansao(void) {
    Sala *hall = criarSala("Hall de Entrada", "Pegadas de botas");
    Sala *corredor = criarSala("Corredor Longo", "");
    Sala *laboratorio = criarSala("Laboratório", "Frasco quebrado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro sobre mutacoes");
    Sala *quarto = criarSala("Quarto Abandonado", "Luvas manchadas");
    Sala *jardim = criarSala("Jardim Interno", "Fio de cabelo loiro");
    Sala *poco = criarSala("Poço Antigo", "Carta rasgada");
    Sala *armario = criarSala("Armário Trancado", "Chave enferrujada");
    Sala *saida = criarSala("Saída", "Mapa rasgado");

    hall->esq = laboratorio;
    hall->dir = biblioteca;

    laboratorio->esq = quarto;
    laboratorio->dir = corredor;

    biblioteca->esq = jardim;
    biblioteca->dir = armario;

    quarto->esq = poco;
    corredor->dir = saida;

    return hall;
}

/* ==========================
   BST DE PISTAS
   ========================== */

PistaNode* criarPistaNode(const char *p) {
    PistaNode *no = malloc(sizeof(PistaNode));
    strncpy(no->pista, p, MAX_PISTA - 1);
    no->pista[MAX_PISTA - 1] = '\0';
    no->esq = no->dir = NULL;
    return no;
}

int existePistaBST(PistaNode *raiz, const char *p) {
    if (!raiz) return 0;
    int cmp = strcmp(p, raiz->pista);
    if (cmp == 0) return 1;
    else if (cmp < 0) return existePistaBST(raiz->esq, p);
    else return existePistaBST(raiz->dir, p);
}

PistaNode* inserirPistaBST(PistaNode *raiz, const char *p) {
    if (!raiz) return criarPistaNode(p);
    int cmp = strcmp(p, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPistaBST(raiz->esq, p);
    else if (cmp > 0) raiz->dir = inserirPistaBST(raiz->dir, p);
    return raiz;
}

void exibirPistasInOrder(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistasInOrder(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistasInOrder(raiz->dir);
}

void liberarPistasBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistasBST(raiz->esq);
    liberarPistasBST(raiz->dir);
    free(raiz);
}

/* ==========================
   HASH TABLE
   ========================== */

void inicializarHash(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++)
        ht->buckets[i] = NULL;
}

unsigned int hashString(const char *s) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*s++))
        hash = hash * 33 + c;
    return hash % HASH_SIZE;
}

void inserirMapping(HashTable *ht, const char *pista, const char *suspeito) {
    unsigned int idx = hashString(pista);
    HashEntry *novo = malloc(sizeof(HashEntry));

    strncpy(novo->pista, pista, MAX_PISTA - 1);
    novo->pista[MAX_PISTA - 1] = '\0';

    strncpy(novo->suspeito, suspeito, MAX_NOME - 1);
    novo->suspeito[MAX_NOME - 1] = '\0';

    novo->prox = ht->buckets[idx];
    ht->buckets[idx] = novo;
}

const char* buscarSuspeitoPorPista(HashTable *ht, const char *pista) {
    unsigned int idx = hashString(pista);
    HashEntry *e = ht->buckets[idx];
    while (e) {
        if (strcmp(e->pista, pista) == 0)
            return e->suspeito;
        e = e->prox;
    }
    return NULL;
}

void liberarHash(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *e = ht->buckets[i];
        while (e) {
            HashEntry *next = e->prox;
            free(e);
            e = next;
        }
    }
}

/* ==========================
   ACUSAÇÃO
   ========================== */

int contarPistasAssociadas(PistaNode *r, HashTable *ht, const char *suspeito) {
    if (!r) return 0;
    int c = contarPistasAssociadas(r->esq, ht, suspeito);
    const char *s = buscarSuspeitoPorPista(ht, r->pista);
    if (s && strcmp(s, suspeito) == 0) c++;
    return c + contarPistasAssociadas(r->dir, ht, suspeito);
}

void fazerAcusacao(PistaNode *pistasColetadas, HashTable *ht) {
    if (!pistasColetadas) {
        printf("\nSem pistas coletadas.\n");
        return;
    }

    int op;
    char escolha[MAX_NOME];

    printf("\nQuem você deseja acusar?\n");
    printf("1 - Dr. Silva\n");
    printf("2 - Maria\n");
    printf("3 - Capitão Rocha\n");
    printf("0 - Cancelar\n");
    printf("Escolha: ");

    scanf("%d", &op);
    limparBuffer();

    if (op == 1) strcpy(escolha, "Dr. Silva");
    else if (op == 2) strcpy(escolha, "Maria");
    else if (op == 3) strcpy(escolha, "Capitão Rocha");
    else return;

    int cont = contarPistasAssociadas(pistasColetadas, ht, escolha);

    printf("\nVocê acusou: %s\n", escolha);
    printf("Pistas que apontam para ele: %d\n", cont);

    if (cont >= 2)
        printf(">>> ACUSAÇÃO CORRETA!\n");
    else
        printf(">>> ACUSAÇÃO FALSA.\n");
}

/* ==========================
   EXPLORAR MANSÃO
   ========================== */

void explorar(Sala *a, HashTable *ht, PistaNode **pc) {
    Sala *at = a;
    int opc;

    while (1) {
        printf("\nVocê está em: %s\n", at->nome);

        if (strlen(at->pista) > 0) {
            printf("Encontrou a pista: %s\n", at->pista);
            if (!existePistaBST(*pc, at->pista)) {
                *pc = inserirPistaBST(*pc, at->pista);
                const char *sus = buscarSuspeitoPorPista(ht, at->pista);
                if (sus) printf("Associada a: %s\n", sus);
            }
        }

        if (strcmp(at->nome, "Saída") == 0 ||
            (at->esq == NULL && at->dir == NULL)) {

            printf("\n--- FIM DA EXPLORAÇÃO ---\n");
            printf("Pistas coletadas:\n");
            exibirPistasInOrder(*pc);

            fazerAcusacao(*pc, ht);
            return;
        }

        printf("\n1 - Ir para esquerda");
        if (!at->esq) printf(" (sem saída)");

        printf("\n2 - Ir para direita");
        if (!at->dir) printf(" (sem saída)");

        printf("\n3 - Sair\nEscolha: ");
        scanf("%d", &opc);
        limparBuffer();

        if (opc == 1 && at->esq) at = at->esq;
        else if (opc == 2 && at->dir) at = at->dir;
        else if (opc == 3) return;
        else printf("Movimento inválido.\n");
    }
}

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ==========================
   MAIN
   ========================== */

int main(void) {
    HashTable ht;
    inicializarHash(&ht);

    inserirMapping(&ht, "Frasco quebrado", "Dr. Silva");
    inserirMapping(&ht, "Luvas manchadas", "Dr. Silva");

    inserirMapping(&ht, "Fio de cabelo loiro", "Maria");
    inserirMapping(&ht, "Carta rasgada", "Maria");
    inserirMapping(&ht, "Mapa rasgado", "Maria");

    inserirMapping(&ht, "Pegadas de botas", "Capitão Rocha");
    inserirMapping(&ht, "Chave enferrujada", "Capitão Rocha");

    inserirMapping(&ht, "Livro sobre mutacoes", "Dr. Silva");

    Sala *mansao = montarMansao();
    PistaNode *pistas = NULL;

    int opc;
    while (1) {
        printf("\n====== DETECTIVE QUEST ======\n");
        printf("1 - Entrar na mansão\n");
        printf("2 - Ver pistas\n");
        printf("3 - Fazer acusação\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        scanf("%d", &opc);
        limparBuffer();

        if (opc == 1) explorar(mansao, &ht, &pistas);
        else if (opc == 2) exibirPistasInOrder(pistas);
        else if (opc == 3) fazerAcusacao(pistas, &ht);
        else if (opc == 0) break;
        else printf("Opção inválida\n");
    }

    liberarHash(&ht);
    liberarPistasBST(pistas);

    printf("Encerrando Detective Quest.\n");
    return 0;
}
