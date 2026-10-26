#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "cli.h"

// Definições de estilos e cores
#define SCREEN_RESET "\033[2J\033[H"
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"

int main(void) {
    printf(COLOR_GREEN COLOR_BOLD "=========================================\n");
    printf(COLOR_GREEN COLOR_BOLD "        TESTE DO MÓDULO ITEM             \n");
    printf(COLOR_GREEN COLOR_BOLD "=========================================\n\n" COLOR_RESET);

    // Linha de teste simulando o arquivo CSV da NASA
    const char *linha_csv = 
        "(99942) Apophis,Apophis,2029,4,0.00025,30.7,19.7,930,HAZARDOUS,DANGER,0.95,5,true,4,1.2,0.34\n";


    
    // 1. Testando item_criar (aloca e executa parse)
    printf("[1] Criando item via item_criar...\n");
    ITEM *item_original = item_criar(linha_csv);

    if (item_original == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar/criar o item!\n");
        return 1;
    }

    // 2. Testando impressao CSV
    printf("\n[2] Impressão via item_print_csv:\n");
    printf("-------------------------------------------------\n");
    item_print_csv(item_original, stdout);
    printf("-------------------------------------------------\n");

    // 3. Testando Getters
    printf("\n[3] Testando getters no item_original:\n");
    printf(" - Designation : %s\n", item_get_texto(item_original, "asteroid_designation"));
    printf(" - Full Name    : %s\n", item_get_texto(item_original, "asteroid_fullname"));
    printf(" - Year         : %.0f\n", item_get_numero(item_original, "year"));
    printf(" - Distance AU  : %.5f\n", item_get_numero(item_original, "distance_au"));
    printf(" - Risk Score   : %.2f\n", item_get_numero(item_original, "risk_score"));
    printf(" - Threat Cat.  : %s\n", item_get_texto(item_original, "threat_category"));

    // 4. Testando item_clonar (Deep Copy da linha_bruta)
    printf("\n[4] Clonando o item_original...\n");
    ITEM *item_clone = item_clonar(item_original);

    if (item_clone == NULL) {
        fprintf(stderr, "ERRO: Falha ao clonar item!\n");
        item_apagar(&item_original);
        return 1;
    }

    // 5. Apagando o original para verificar independência da memória
    printf("\n[5] Apagando item_original...\n");
    item_apagar(&item_original);

    // 6. Testando se o clone continua totalmente acessível
    printf("\n[6] Verificando se o clone permanece intacto:\n");
    printf(" -> Designation no clone : %s\n", item_get_texto(item_clone, "asteroid_designation"));
    printf(" -> Saída CSV do clone   : ");
    item_print_csv(item_clone, stdout);

    // 7. Limpeza final
    printf("\n[7] Apagando item_clone...\n");
    item_apagar(&item_clone);

    printf("\n=========================================\n");
    printf("      TODOS OS TESTES CONCLUÍDOS!        \n");
    printf("=========================================\n");

    return 0;
}