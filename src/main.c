#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pilha.h"
#include "item.h"

// Callback para testar se a função de percurso está visitando os nós
static void callback_testa_item(ITEM *item, void *contexto) {
    (void)item; // Evita aviso de variável não usada
    int *contador = (int *) contexto;
    (*contador)++;
}

int main(void) {
    printf("==> Iniciando testes da FILA...\n");

    // 1. Teste de Criação
    PILHA *pilha = pilha_criar();
    assert(pilha != NULL);
    assert(pilha_tamanho(pilha) == 0);
    printf("[OK] Criação da pilha\n");

    // 2. Teste de Inserção
    ITEM *i1 = item_criar_vazio();
    ITEM *i2 = item_criar_vazio();
    ITEM *i3 = item_criar_vazio();

    assert(pilha_inserir(pilha, i1) == true);
    assert(pilha_inserir(pilha, i2) == true);
    assert(pilha_inserir(pilha, i3) == true);
    assert(pilha_tamanho(pilha) == 3);
    printf("[OK] Inserção de 3 elementos e verificação de tamanho\n");

    // 3. Teste de Percurso
    int visitados = 0;
    pilha_percorrer(pilha, callback_testa_item, &visitados);
    assert(visitados == 3);
    printf("[OK] Percurso por callback (visitou %d itens)\n", visitados);

    // 4. Teste de Destruição (Liberando a pilha e os itens contidos nela)
    pilha_apagar(&pilha, true);
    assert(pilha == NULL);
    printf("[OK] Destruição com apagar_itens = true\n");

    // 5. Teste de Destruição mantendo os itens (Comportamento exigido para a estrutura -result)
    PILHA *pilha_result = pilha_criar();
    ITEM *item_compartilhado = item_criar_vazio();
    
    pilha_inserir(pilha_result, item_compartilhado);
    // Apaga apenas a estrutura da lista, mantendo o item vivo
    pilha_apagar(&pilha_result, false); 
    assert(pilha_result == NULL);
    
    // Libera o item manualmente para não vazar memória no teste
    item_apagar(&item_compartilhado);
    printf("[OK] Destruição com apagar_itens = false\n");

    printf("\n>>> TODOS OS TESTES DA PILHA PASSARAM COM SUCESSO! <<<\n");
    return 0;
}