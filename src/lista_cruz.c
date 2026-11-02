#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lista_cruz.h"

typedef struct no_cruz_ {
    ITEM *item;
    struct no_cruz_ *prox_c1; // Ponteiro do Eixo 1 (ordenado/agrupado por Campo 1)
    struct no_cruz_ *prox_c2; // Ponteiro do Eixo 2 (ordenado/agrupado por Campo 2)
} NO_CRUZ;

struct lista_cruz_ {
    NO_CRUZ *head_c1; // Inicio do percurso pelo Campo 1
    NO_CRUZ *head_c2; // Inicio do percurso pelo Campo 2
    char campo1[50];
    char campo2[50];
    int tamanho;
};

LISTA_CRUZ *lista_cruz_criar(void) {
    LISTA_CRUZ *lista = (LISTA_CRUZ *) malloc(sizeof(LISTA_CRUZ));
    if (lista == NULL) return NULL;

    lista->head_c1 = NULL;
    lista->head_c2 = NULL;
    lista->campo1[0] = '\0';
    lista->campo2[0] = '\0';
    lista->tamanho = 0;

    return lista;
}

void lista_cruz_configurar(LISTA_CRUZ *lista, const char *campo1, const char *campo2) {
    if (lista == NULL) return;
    if (campo1 != NULL) {
        strncpy(lista->campo1, campo1, sizeof(lista->campo1) - 1);
        lista->campo1[sizeof(lista->campo1) - 1] = '\0';
    }
    if (campo2 != NULL) {
        strncpy(lista->campo2, campo2, sizeof(lista->campo2) - 1);
        lista->campo2[sizeof(lista->campo2) - 1] = '\0';
    }
}

static int comparar_cruz(const ITEM *a, const ITEM *b, const char *campo) {
    if (a == NULL || b == NULL || campo == NULL || campo[0] == '\0') return 0;

    double num_a = item_get_numero(a, campo);
    double num_b = item_get_numero(b, campo);

    if (num_a != -1.0 || num_b != -1.0) {
        if (num_a < num_b) return -1;
        if (num_a > num_b) return 1;
        return 0;
    }

    const char *txt_a = item_get_texto(a, campo);
    const char *txt_b = item_get_texto(b, campo);
    if (txt_a != NULL && txt_b != NULL) {
        return strcmp(txt_a, txt_b);
    }

    return 0;
}

bool lista_cruz_inserir(void *lista_ptr, ITEM *item) {
    LISTA_CRUZ *lista = (LISTA_CRUZ *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    // Aloca um UNICO no para participar dos dois encadeamentos ortogonais
    NO_CRUZ *novo = (NO_CRUZ *) malloc(sizeof(NO_CRUZ));
    if (novo == NULL) return false;

    novo->item = item;
    novo->prox_c1 = NULL;
    novo->prox_c2 = NULL;

    // 1. Encadeamento no Eixo 1 (ordenado/agrupado por campo1)
    NO_CRUZ *ant1 = NULL;
    NO_CRUZ *cur1 = lista->head_c1;
    while (cur1 != NULL && comparar_cruz(item, cur1->item, lista->campo1) > 0) {
        ant1 = cur1;
        cur1 = cur1->prox_c1;
    }

    if (ant1 == NULL) {
        novo->prox_c1 = lista->head_c1;
        lista->head_c1 = novo;
    } else {
        novo->prox_c1 = cur1;
        ant1->prox_c1 = novo;
    }

    // 2. Encadeamento no Eixo 2 (ordenado/agrupado por campo2)
    NO_CRUZ *ant2 = NULL;
    NO_CRUZ *cur2 = lista->head_c2;
    while (cur2 != NULL && comparar_cruz(item, cur2->item, lista->campo2) > 0) {
        ant2 = cur2;
        cur2 = cur2->prox_c2;
    }

    if (ant2 == NULL) {
        novo->prox_c2 = lista->head_c2;
        lista->head_c2 = novo;
    } else {
        novo->prox_c2 = cur2;
        ant2->prox_c2 = novo;
    }

    lista->tamanho++;
    return true;
}

void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    // Percorre pela cadeia do Eixo 1 para desalocar cada no exatamente uma vez
    NO_CRUZ *atual = (*lista)->head_c1;
    while (atual != NULL) {
        NO_CRUZ *proximo = atual->prox_c1;

        if (apagar_itens && atual->item != NULL) {
            item_apagar(&(atual->item));
        }

        free(atual);
        atual = proximo;
    }

    free(*lista);
    *lista = NULL;
}

int lista_cruz_tamanho(const LISTA_CRUZ *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

bool lista_cruz_vazia(const LISTA_CRUZ *lista) {
    return (lista == NULL || lista->tamanho == 0);
}

void lista_cruz_percorrer(const LISTA_CRUZ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    // Realiza o percurso principal seguindo o encadeamento do Eixo 1
    NO_CRUZ *atual = lista->head_c1;
    while (atual != NULL) {
        visitar(atual->item, contexto);
        atual = atual->prox_c1;
    }
}