#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pilha.h"

typedef struct no_ NO;

struct no_ {
    ITEM *item; 
    NO *anterior;
};

struct pilha_ {
    NO *topo;
    int tamanho;
};  

PILHA *pilha_criar(void){
    PILHA *pilha = (PILHA*) malloc(sizeof(PILHA));

    if(pilha == NULL) return NULL;

    pilha->topo = NULL;
    pilha->tamanho = 0; 

    return pilha;
}

bool pilha_inserir(void *pilha_ptr, ITEM *item){
    PILHA *pilha = (PILHA *) pilha_ptr;
    if (pilha == NULL || item == NULL) return false;

    if(pilha_ptr == NULL || item == NULL) return false;

    NO *no = (NO*) malloc(sizeof(NO));

    if(no == NULL) return NULL;

    no->item = item; 
    no->anterior = pilha->topo;
    pilha->topo = no;

    pilha->tamanho++;
    return true;
}

ITEM *pilha_desempilhar(PILHA *pilha){
    if(pilha == NULL) return NULL;

    NO *removido = pilha->topo;
    if(removido == NULL) return NULL;

    ITEM *item = removido->item;

    pilha->topo = removido->anterior;
    free(removido);
    pilha->tamanho--;

    return item;
}

void pilha_apagar(PILHA **pilha, bool apagar_itens){
    if(pilha == NULL) return;

    NO *atual = (*pilha)->topo;

    while(atual != NULL){
        NO *anterior = atual->anterior;

        if(apagar_itens && atual->item != NULL){
            item_apagar(&(atual->item));
        }

        free(atual);
        atual = anterior;
    }

    free(*pilha);
    *pilha = NULL;
}

int pilha_tamanho(const PILHA *pilha) {
    return (pilha != NULL) ? pilha->tamanho : 0;
}

bool pilha_vazia(const PILHA *pilha) {
    return (pilha == NULL || pilha->tamanho == 0);
}

void pilha_percorrer(const PILHA *pilha, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (pilha == NULL || visitar == NULL) return;

    NO *atual = pilha->topo;
    while (atual != NULL) {
        visitar(atual->item, contexto);
        atual = atual->anterior;
    }
}