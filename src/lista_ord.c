#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lista_ord.h"

typedef struct no_ NO;

struct no_ {
    ITEM *item;
    NO *proximo;
};

struct lista_ord_ {
    NO *inicio;
    int tamanho;
    char campo_ordem[50];
    bool decrescente;
};

LISTA_ORD *lista_ord_criar(void) {
    LISTA_ORD *lista = (LISTA_ORD *) malloc(sizeof(LISTA_ORD));
    if (lista == NULL) return NULL;

    lista->inicio = NULL;
    lista->tamanho = 0;
    lista->campo_ordem[0] = '\0';
    lista->decrescente = false;

    return lista;
}

void lista_ord_configurar_ordem(LISTA_ORD *lista, const char *campo, bool decrescente) {
    if (lista == NULL) return;
    if (campo != NULL) {
        strncpy(lista->campo_ordem, campo, sizeof(lista->campo_ordem) - 1);
        lista->campo_ordem[sizeof(lista->campo_ordem) - 1] = '\0';
    }
    lista->decrescente = decrescente;
}

// Compara dois itens com base no campo configurado
static int comparar_itens(const ITEM *a, const ITEM *b, const char *campo, bool decrescente) {
    if (a == NULL || b == NULL) return 0;
    if (campo == NULL || campo[0] == '\0') return 0;

    // Tenta primeiro como numero
    double num_a = item_get_numero(a, campo);
    double num_b = item_get_numero(b, campo);

    if (num_a != -1.0 || num_b != -1.0) {
        if (num_a < num_b) return decrescente ? 1 : -1;
        if (num_a > num_b) return decrescente ? -1 : 1;
        return 0;
    }

    // Tenta como texto
    const char *txt_a = item_get_texto(a, campo);
    const char *txt_b = item_get_texto(b, campo);
    if (txt_a != NULL && txt_b != NULL) {
        int res = strcmp(txt_a, txt_b);
        return decrescente ? -res : res;
    }

    return 0;
}

bool lista_ord_inserir(void *lista_ptr, ITEM *item) {
    LISTA_ORD *lista = (LISTA_ORD *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    NO *novo_no = (NO *) malloc(sizeof(NO));
    if (novo_no == NULL) return false;

    novo_no->item = item;
    novo_no->proximo = NULL;

    // Localiza a posicao correta para manter a ordenacao durante a insercao
    NO *anterior = NULL;
    NO *atual = lista->inicio;

    while (atual != NULL && comparar_itens(item, atual->item, lista->campo_ordem, lista->decrescente) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Insercao no inicio ou no meio/fim
    if (anterior == NULL) {
        novo_no->proximo = lista->inicio;
        lista->inicio = novo_no;
    } else {
        novo_no->proximo = atual;
        anterior->proximo = novo_no;
    }

    lista->tamanho++;
    return true;
}

void lista_ord_apagar(LISTA_ORD **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->inicio;
    while (atual != NULL) {
        NO *proximo = atual->proximo;

        if (apagar_itens && atual->item != NULL) {
            item_apagar(&(atual->item));
        }

        free(atual);
        atual = proximo;
    }

    free(*lista);
    *lista = NULL;
}

int lista_ord_tamanho(const LISTA_ORD *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

bool lista_ord_vazia(const LISTA_ORD *lista) {
    return (lista == NULL || lista->tamanho == 0);
}

void lista_ord_percorrer(const LISTA_ORD *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    NO *atual = lista->inicio;
    while (atual != NULL) {
        visitar(atual->item, contexto);
        atual = atual->proximo;
    }
}