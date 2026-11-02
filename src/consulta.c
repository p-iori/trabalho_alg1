#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define CAPACIDADE_INICIAL 10

#include "consulta.h"
#include "item.h"

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

    double accept_threshold;
    char ordem_campo[50];
    bool ordem_desc;

    char gp_campo1[50];
    char gp_campo2[50];

    char cross_campo1[50]; 
    char cross_campo2[50]; 
};

CONSULTA *consulta_criar(void){
    CONSULTA *c = (CONSULTA*) calloc(1, sizeof(CONSULTA));
    if(c == NULL) return NULL;

    c->cap_filtros = CAPACIDADE_INICIAL;
    c->filtros = (FILTER*) malloc(sizeof(FILTER) * c->cap_filtros);

    c->cap_similares = CAPACIDADE_INICIAL;
    c->similares = (SIM*) malloc(sizeof(SIM) * c->cap_similares);

    c->accept_threshold = 0.0; 
    c->ordem_desc = true; 

    return c;
}

void consulta_apagar(CONSULTA **c){
    if(c == NULL || *c == NULL) return;

    if((*c)->filtros) free((*c)->filtros);
    if((*c)->similares) free((*c)->similares);

    free(*c);
    *c = NULL; 
}

static void consulta_adicionar_filtro(CONSULTA *c, const char *campo, const char *op, const char *val){
    if(c->qtd_filtros >= c->cap_filtros){
        c->cap_filtros *= 2;
        c->filtros = (FILTER*) realloc(c->filtros, sizeof(FILTER) * c->cap_filtros);
    }

    FILTER *f = &c->filtros[c->qtd_filtros++];
    strncpy(f->campo, campo, sizeof(f->campo)-1);
    f->campo[sizeof(f->campo)-1] = '\0';

    strncpy(f->operador, op, sizeof(f->operador)-1);
    f->operador[sizeof(f->operador)-1] = '\0';

    strncpy(f->valor_str, val, sizeof(f->valor_str)-1);
    f->valor_str[sizeof(f->valor_str)-1] = '\0';

    char *endptr;
    f->valor_num = strtod(val, &endptr);

    while (*endptr == ' ' || *endptr == '\t' || *endptr == '\r' || *endptr == '\n') {
        endptr++;
    }

    f->numerico = (endptr != val && *endptr == '\0');
}

static void consulta_adicionar_sim(CONSULTA *c, const char *campo, double alvo, double tol, double peso){
    if(c->qtd_similares >= c->cap_similares){
        c->cap_similares *= 2;
        c->similares = (SIM*) realloc(c->similares, sizeof(SIM) * c->cap_similares);
    }

    SIM *s = &c->similares[c->qtd_similares++];
    strncpy(s->campo, campo, sizeof(s->campo)-1);
    s->campo[sizeof(s->campo)-1] = '\0';
    s->alvo = alvo;
    s->tolerancia = tol;
    s->peso = peso;
}

bool consulta_carregar_arquivo(CONSULTA *c, const char* caminho_arquivo){
    if(c == NULL || caminho_arquivo == NULL) return false;

    FILE *f = fopen(caminho_arquivo, "r");
    if(f == NULL) return false;

    char linha[256]; 
    while(fgets(linha, sizeof(linha), f) != NULL){
        linha[strcspn(linha, "\r\n")] = 0;
        
        char *p = linha;
        while (*p == ' ' || *p == '\t') p++;
        if(strlen(p) == 0) continue; 

        // FILTER
        if(strncmp(p, "FILTER", 6) == 0 && (p[6] == ' ' || p[6] == '|' || p[6] == '\t')){
            char *conteudo = p + 6;
            while (*conteudo == ' ' || *conteudo == '|' || *conteudo == '\t') conteudo++;

            char *op_ptr = strpbrk(conteudo, "!=<>");
            if(op_ptr){
                int op_len = 0; 
                char op[3] = {0};

                if(*op_ptr == '=' || *op_ptr == '!' || *op_ptr == '<' || *op_ptr == '>'){
                    op[op_len++] = *op_ptr;
                    if(*(op_ptr + 1) == '=') op[op_len++] = '='; 
                }
        
                char campo[50] = {0};
                char valor[50] = {0};

                int campo_len = op_ptr - conteudo; 
                while (campo_len > 0 && (conteudo[campo_len - 1] == '|' || conteudo[campo_len - 1] == ' ' || conteudo[campo_len - 1] == '\t')) {
                    campo_len--;
                }
                strncpy(campo, conteudo, campo_len);

                char *val_ptr = op_ptr + op_len;
                while (*val_ptr == '|' || *val_ptr == ' ' || *val_ptr == '\t') val_ptr++;
                strncpy(valor, val_ptr, sizeof(valor) - 1);

                consulta_adicionar_filtro(c, campo, op, valor);
            }
        }
        // SIM
        else if(strncmp(p, "SIM", 3) == 0 && (p[3] == ' ' || p[3] == '|' || p[3] == '\t')){
            char *conteudo = p + 3;
            while (*conteudo == ' ' || *conteudo == '|' || *conteudo == '\t') conteudo++;

            char campo[50] = {0};
            double alvo = 0, tol = 0, peso = 0;

            char *del = strchr(conteudo, '|');
            if (!del) del = strchr(conteudo, ' ');

            if (del) {
                int len = del - conteudo;
                strncpy(campo, conteudo, len);

                char *resto = del + 1;
                char buf_resto[100];
                strncpy(buf_resto, resto, sizeof(buf_resto)-1);
                buf_resto[sizeof(buf_resto)-1] = '\0';

                // Substitui pipes por espaços nos valores numéricos do SIM
                for (int i = 0; buf_resto[i]; i++) {
                    if (buf_resto[i] == '|') buf_resto[i] = ' ';
                }

                if (sscanf(buf_resto, "%lf %lf %lf", &alvo, &tol, &peso) == 3) {
                    consulta_adicionar_sim(c, campo, alvo, tol, peso);
                }
            }
        }
        // ACCEPT
        else if (strncmp(p, "ACCEPT", 6) == 0 && (p[6] == ' ' || p[6] == '|' || p[6] == '\t')) {
            char *val_ptr = p + 6;
            while (*val_ptr == ' ' || *val_ptr == '|' || *val_ptr == '\t') val_ptr++;
            c->accept_threshold = atof(val_ptr);
        } 
        // ORDER
        else if (strncmp(p, "ORDER", 5) == 0 && (p[5] == ' ' || p[5] == '|' || p[5] == '\t')) {
            char *conteudo = p + 5;
            while (*conteudo == ' ' || *conteudo == '|' || *conteudo == '\t') conteudo++;

            char *del = strchr(conteudo, '|');
            if (!del) del = strchr(conteudo, ' ');
            if (del) {
                int len = del - conteudo;
                strncpy(c->ordem_campo, conteudo, len);
                c->ordem_campo[len] = '\0';

                char *ord = del + 1;
                while (*ord == ' ' || *ord == '|' || *ord == '\t') ord++;
                c->ordem_desc = (strncmp(ord, "DESC", 4) == 0);
            }
        } 
        // GROUP
        else if (strncmp(p, "GROUP", 5) == 0 && (p[5] == ' ' || p[5] == '|' || p[5] == '\t')) {
            char *conteudo = p + 5;
            while (*conteudo == ' ' || *conteudo == '|' || *conteudo == '\t') conteudo++;
            char buf[100];
            strncpy(buf, conteudo, sizeof(buf)-1);
            for (int i = 0; buf[i]; i++) if (buf[i] == '|') buf[i] = ' ';
            sscanf(buf, "%s %s", c->gp_campo1, c->gp_campo2);
        } 
        // CROSS
        else if (strncmp(p, "CROSS", 5) == 0 && (p[5] == ' ' || p[5] == '|' || p[5] == '\t')) {
            char *conteudo = p + 5;
            while (*conteudo == ' ' || *conteudo == '|' || *conteudo == '\t') conteudo++;
            char buf[100];
            strncpy(buf, conteudo, sizeof(buf)-1);
            for (int i = 0; buf[i]; i++) if (buf[i] == '|') buf[i] = ' ';
            sscanf(buf, "%s %s", c->cross_campo1, c->cross_campo2);
        }
    }

    fclose(f);
    return true;
}

bool consulta_avaliar_item(const CONSULTA *c, const ITEM *item, double *score_out){
    if(c == NULL || item == NULL) return false; 

    // 1. Aplicação dos Filtros
    for(int i = 0; i < c->qtd_filtros; i++){
        FILTER *f = &c->filtros[i];

        if(f->numerico){
            double val_item = item_get_numero(item, f->campo);
            if (val_item == -1.0) return false;

            if (strcmp(f->operador, "==") == 0 && !(val_item == f->valor_num)) return false;
            if (strcmp(f->operador, "!=") == 0 && !(val_item != f->valor_num)) return false;
            if (strcmp(f->operador, "<=") == 0 && !(val_item <= f->valor_num)) return false;
            if (strcmp(f->operador, ">=") == 0 && !(val_item >= f->valor_num)) return false;
            if (strcmp(f->operador, "<")  == 0 && !(val_item <  f->valor_num)) return false;
            if (strcmp(f->operador, ">")  == 0 && !(val_item >  f->valor_num)) return false;
        } else {
            const char* str_item = item_get_texto(item, f->campo);
            if(str_item == NULL) return false;

            if (strcmp(f->operador, "==") == 0 && strcmp(str_item, f->valor_str) != 0) return false;
            if (strcmp(f->operador, "!=") == 0 && strcmp(str_item, f->valor_str) == 0) return false;
        }
    }

    // 2. Cálculo da Similaridade
    if(c->qtd_similares == 0){
        if(score_out) *score_out = 1.0;
        return true;
    }

    double soma_pond = 0.0;
    double soma_pesos = 0.0;

    for(int i = 0; i < c->qtd_similares; i++){
        SIM *s = &c->similares[i];
        double val_item = item_get_numero(item, s->campo);
        if (val_item == -1.0) continue;
        
        double diff = fabs(val_item - s->alvo);
        double sim_local = 1.0 - (diff / s->tolerancia);
        if(sim_local < 0.0) sim_local = 0.0;

        soma_pond += sim_local * s->peso;
        soma_pesos += s->peso;
    }

    double sim_total = (soma_pesos > 0.0) ? (soma_pond / soma_pesos) : 1.0;

    if(score_out) *score_out = sim_total;

    return (sim_total >= c->accept_threshold);
}

const char* consulta_get_ordem_campo(const CONSULTA *c) {
    return (c != NULL) ? c->ordem_campo : NULL;
}

bool consulta_get_ordem_decrescente(const CONSULTA *c) {
    return (c != NULL) ? c->ordem_desc : true;
}

const char* consulta_get_grupo_campo1(const CONSULTA *c) {
    return (c != NULL) ? c->gp_campo1 : NULL;
}

const char* consulta_get_grupo_campo2(const CONSULTA *c) {
    return (c != NULL) ? c->gp_campo2 : NULL;
}

const char* consulta_get_cross_campo1(const CONSULTA *c) {
    return (c != NULL) ? c->cross_campo1 : NULL;
}

const char* consulta_get_cross_campo2(const CONSULTA *c) {
    return (c != NULL) ? c->cross_campo2 : NULL;
}