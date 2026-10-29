#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "reader_nasa.h"
#include "item.h"

int reader_nasa_carregar(const char *caminho_csv, int limite, void *estrutura_base, funcao_inserir_fn funcao_inserir){
    if(caminho_csv == NULL || estrutura_base == NULL || funcao_inserir == NULL) return 0;
    
    FILE *f = fopen(caminho_csv, "r");
    
    if(f == NULL) return 0;
    
    char linha[1024]; 
    
    if(fgets(linha, sizeof(f), f) == NULL){
        fclosef(f);
        return 0; 
    }
    
    
    int inseridos = 0;
    
    while((limite == 0 || inseridos < limite) && fgets(linha, sizeof(linha), f) != NULL){
        linha[strcspn(linha, "\r\n")];
        if(strlen(linha) == 0) continue;

        ITEM *item = item_criar(linha);
        if(item == NULL) return 0;

        if(funcao_inserir(estrutura_base, item)){
            inseridos++;
        } else {
            item_apagar(&item);
        }

    }

    fclosef(f);
    return inseridos;
}
