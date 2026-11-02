#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lista_gen.h"

typedef struct no_item_ {
    ITEM *item;
    struct no_item_ *proximo;
} NO_ITEM;

typedef struct subgrupo_ {
    char chave[50];
    NO_ITEM *primeiro_item;
    struct subgrupo_ *proximo;
} SUBGRUPO;

typedef struct grupo_ {
    char chave[50];
    SUBGRUPO *primeiro_subgrupo;
    struct grupo_ *proximo;
} GRUPO;

struct lista_gen_ {
    GRUPO *primeiro_grupo;
    char campo_grupo[50];
    char campo_subgrupo[50];
    int tamanho;
};

LISTA_GEN *lista_gen_criar(void) {
    LISTA_GEN *lista = (LISTA_GEN *) malloc(sizeof(LISTA_GEN));
    if (lista == NULL) return NULL;

    lista->primeiro_grupo = NULL;
    lista->campo_grupo[0] = '\0';
    lista->campo_subgrupo[0] = '\0';
    lista->tamanho = 0;

    return lista;
}

void lista_gen_configurar(LISTA_GEN *lista, const char *campo_grupo, const char *campo_subgrupo) {
    if (lista == NULL) return;
    if (campo_grupo != NULL) {
        strncpy(lista->campo_grupo, campo_grupo, sizeof(lista->campo_grupo) - 1);
        lista->campo_grupo[sizeof(lista->campo_grupo) - 1] = '\0';
    }
    if (campo_subgrupo != NULL) {
        strncpy(lista->campo_subgrupo, campo_subgrupo, sizeof(lista->campo_subgrupo) - 1);
        lista->campo_subgrupo[sizeof(lista->campo_subgrupo) - 1] = '\0';
    }
}

static void extrair_valor_campo(const ITEM *item, const char *campo, char *out, size_t size) {
    if (item == NULL || campo == NULL || campo[0] == '\0') {
        strncpy(out, "N/A", size);
        return;
    }

    double num = item_get_numero(item, campo);
    if (num != -1.0) {
        snprintf(out, size, "%.2f", num);
        return;
    }

    const char *txt = item_get_texto(item, campo);
    if (txt != NULL) {
        strncpy(out, txt, size - 1);
        out[size - 1] = '\0';
        return;
    }

    strncpy(out, "N/A", size);
}

bool lista_gen_inserir(void *lista_ptr, ITEM *item) {
    LISTA_GEN *lista = (LISTA_GEN *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    char chave_g[50], chave_sub[50];
    extrair_valor_campo(item, lista->campo_grupo, chave_g, sizeof(chave_g));
    extrair_valor_campo(item, lista->campo_subgrupo, chave_sub, sizeof(chave_sub));

    // Busca ou cria o Grupo principal
    GRUPO *g_atual = lista->primeiro_grupo;
    GRUPO *g_ant = NULL;
    while (g_atual != NULL && strcmp(g_atual->chave, chave_g) != 0) {
        g_ant = g_atual;
        g_atual = g_atual->proximo;
    }

    if (g_atual == NULL) {
        g_atual = (GRUPO *) malloc(sizeof(GRUPO));
        if (g_atual == NULL) return false;
        strncpy(g_atual->chave, chave_g, sizeof(g_atual->chave) - 1);
        g_atual->primeiro_subgrupo = NULL;
        g_atual->proximo = NULL;

        if (g_ant == NULL) lista->primeiro_grupo = g_atual;
        else g_ant->proximo = g_atual;
    }

    // Busca ou cria o Subgrupo
    SUBGRUPO *sub_atual = g_atual->primeiro_subgrupo;
    SUBGRUPO *sub_ant = NULL;
    while (sub_atual != NULL && strcmp(sub_atual->chave, chave_sub) != 0) {
        sub_ant = sub_atual;
        sub_atual = sub_atual->proximo;
    }

    if (sub_atual == NULL) {
        sub_atual = (SUBGRUPO *) malloc(sizeof(SUBGRUPO));
        if (sub_atual == NULL) return false;
        strncpy(sub_atual->chave, chave_sub, sizeof(sub_atual->chave) - 1);
        sub_atual->primeiro_item = NULL;
        sub_atual->proximo = NULL;

        if (sub_ant == NULL) g_atual->primeiro_subgrupo = sub_atual;
        else sub_ant->proximo = sub_atual;
    }

    // Insere o no_item no subgrupo
    NO_ITEM *novo_no = (NO_ITEM *) malloc(sizeof(NO_ITEM));
    if (novo_no == NULL) return false;
    novo_no->item = item;
    novo_no->proximo = NULL;

    if (sub_atual->primeiro_item == NULL) {
        sub_atual->primeiro_item = novo_no;
    } else {
        NO_ITEM *i_atual = sub_atual->primeiro_item;
        while (i_atual->proximo != NULL) i_atual = i_atual->proximo;
        i_atual->proximo = novo_no;
    }

    lista->tamanho++;
    return true;
}

void lista_gen_apagar(LISTA_GEN **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    GRUPO *g = (*lista)->primeiro_grupo;
    while (g != NULL) {
        GRUPO *g_prox = g->proximo;
        SUBGRUPO *sub = g->primeiro_subgrupo;

        while (sub != NULL) {
            SUBGRUPO *sub_prox = sub->proximo;
            NO_ITEM *no = sub->primeiro_item;

            while (no != NULL) {
                NO_ITEM *no_prox = no->proximo;
                if (apagar_itens && no->item != NULL) {
                    item_apagar(&(no->item));
                }
                free(no);
                no = no_prox;
            }
            free(sub);
            sub = sub_prox;
        }
        free(g);
        g = g_prox;
    }

    free(*lista);
    *lista = NULL;
}

int lista_gen_tamanho(const LISTA_GEN *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

bool lista_gen_vazia(const LISTA_GEN *lista) {
    return (lista == NULL || lista->tamanho == 0);
}

void lista_gen_percorrer(const LISTA_GEN *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    for (GRUPO *g = lista->primeiro_grupo; g != NULL; g = g->proximo) {
        for (SUBGRUPO *sub = g->primeiro_subgrupo; sub != NULL; sub = sub->proximo) {
            for (NO_ITEM *no = sub->primeiro_item; no != NULL; no = no->proximo) {
                visitar(no->item, contexto);
            }
        }
    }
}