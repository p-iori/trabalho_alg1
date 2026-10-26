#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "consulta.h"


typedef struct filter_{
    char campo[50];
    char operador[5];
    char valor_str[50];
    double valor_num; 
    bool numerico;
    
} FILTER;

typedef struct sim_ {
    char campo[50];
    double alvo;
    double tolerancia;
    double peso;
} SIM;

struct consulta_{
    FILTER *filtros;
    int qtd_filtros;
    int cap_filtros;

    SIM *similares; 
    int qtd_similares;
    int cap_similares;

    char gp_campo1[50];
    char gp_campo2[50];

    char cross_campo1[50]; 
    char cross_campo2[50]; 
};

static char* custom_strdup(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copia = (char*) malloc(len);
    if (copia != NULL) {
        memcpy(copia, s, len);
    }
    return copia;
}

CONSULTA *consulta_criar(void){}