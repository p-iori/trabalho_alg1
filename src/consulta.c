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

    return;
}

static void consulta_adicionar_filtro(CONSULTA *c, const char *campo, const char *op, const char *val){
    // realoca caso o numero de filtros ultrapasse o esperado
    if(c->qtd_filtros >= c->cap_filtros){
        c->cap_filtros *= 2;
        c->filtros = (FILTER*) realloc(c->filtros, sizeof(FILTER) * c->cap_filtros);
    }

    FILTER *f = &c->filtros[c->qtd_filtros++]; // adiciona o filtro novo no final do arrat de filtros
    strncpy(f->campo, campo, sizeof(f->campo)-1);
    strncpy(f->operador, op, sizeof(f->operador)-1);
    strncpy(f->valor_str, val, sizeof(f->valor_str));

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
        c->similares = (SIM*) realloc(c->similares, sizeof(FILTER) * c->cap_filtros);
    }

    SIM *s = &c->similares[c->qtd_similares++];
    strncpy(s->campo, campo, sizeof(s->campo)-1);
    s->alvo = alvo;
    s->tolerancia = tol;
    s->peso = peso;
}

// verifica cada linha do arquivo de consulta e separa cada instrução em sua respectiva struct
bool consulta_carregar_arquivo(CONSULTA *c, const char* caminho_arquivo){
    if(c == NULL || caminho_arquivo == NULL) return false;

    FILE *f = fopen(caminho_arquivo, "r");
    if(f == NULL) return false;

    char linha[256]; // str que será reutilizada para receber cada linha do arquivo 
    while(fgets(linha, sizeof(linha), f) != NULL){
        linha[strcspn(linha, "\r\n")] = 0; // remove a quebra de linha
        if(strlen(linha) == 0) continue; // pula para a proxima linha

        // filter
        if(strncmp(linha, "FILTER ", 7) == 0){
            char *conteudo = linha + 7;// pula os 7 primeiros caracteres (FILTER ) (com espaço no final)
            char *op_ptr = strpbrk(conteudo, "!=<>");// procura a primeira ocorrência de um desses caracteres para demarcar onde a operação está
            
            if(op_ptr){
                int op_len = 0; 
                char op[3] = {0};

                if(*op_ptr == '=' || *op_ptr == '!' || *op_ptr == '<' || *op_ptr == '>'){
                    op[op_len++] = *op_ptr;
                    if(*(op_ptr + 1) == '=') op[op_len++] = '='; //caso a operação seja !=,<= ou >= 
                }
        
                char campo[50] = {0};
                char valor[50] = {0};

                int campo_len = op_ptr - conteudo; // posição do último caractere da operação - palavra FILTER do cocmeço
                if(campo_len > 0 && conteudo[campo_len - 1] == '|') campo_len--; //remove o | do campo
                strncpy(campo, conteudo, campo_len);

                char *val_ptr = op_ptr + op_len;
                if (*val_ptr == '|') val_ptr++;
                strncpy(valor, val_ptr, sizeof(valor) - 1);

                consulta_adicionar_filtro(c, campo, op, valor);
            }
        }else if(strncmp(linha, "SIM ", 4) == 0){
            char campo[50];
            double alvo, tol, peso;
            char *pipe = strchr(linha + 4, '|');
            if(pipe){
                int len = pipe - (linha + 4);
                strncpy(campo, linha + 4, len);
                campo[len] = '\0';
                if(sscanf(pipe + 1, "%lf %lf %lf", &alvo, &tol, &peso) == 3){
                    consulta_adicionar_sim(c, campo, alvo, tol, peso);
                }
            }
        }else if (strncmp(linha, "ACCEPT ", 7) == 0) {
            c->accept_threshold = atof(linha + 7);
        } else if (strncmp(linha, "ORDER ", 6) == 0) {
            char *pipe = strchr(linha + 6, '|');
            if (pipe) {
                int len = pipe - (linha + 6);
                strncpy(c->ordem_campo, linha + 6, len);
                c->ordem_campo[len] = '\0';
                c->ordem_desc = (strcmp(pipe + 1, "DESC") == 0);
            }
        } else if (strncmp(linha, "GROUP ", 6) == 0) {
            char *pipe = strchr(linha + 6, '|');
            if (pipe) {
                int len = pipe - (linha + 6);
                strncpy(c->gp_campo1, linha + 6, len);
                c->gp_campo1[len] = '\0';
                strcpy(c->gp_campo2, pipe + 1);
            }
        } else if (strncmp(linha, "CROSS ", 6) == 0) {
            sscanf(linha + 6, "%s %s", c->cross_campo1, c->cross_campo2);
        }
    }

    fclose(f);
    return true;
}

bool consulta_avaliar_item(const CONSULTA *c, const ITEM *item, double *score_out){
    if(c == NULL || item == NULL) return false; 

    //aplicação dos filtros, se qualquer filtro falhar, retorna false
    for(int i = 0; i < c->qtd_filtros; i++){
        FILTER *f = &c->filtros[i];

        if(f->numerico){
            double val_item = item_get_numero(item, f->campo);
            if (strcmp(f->operador, "==") == 0 && !(val_item == f->valor_num)) return false;
            if (strcmp(f->operador, "!=") == 0 && !(val_item != f->valor_num)) return false;
            if (strcmp(f->operador, "<=") == 0 && !(val_item <= f->valor_num)) return false;
            if (strcmp(f->operador, ">=") == 0 && !(val_item >= f->valor_num)) return false;
            if (strcmp(f->operador, "<")  == 0 && !(val_item <  f->valor_num)) return false;
            if (strcmp(f->operador, ">")  == 0 && !(val_item >  f->valor_num)) return false;
        }else{
            const char* str_item = item_get_texto(item, f->campo);
            if(str_item == NULL) return false;
            if (strcmp(f->operador, "==") == 0 && strcmp(str_item, f->valor_str) != 0) return false;
            if (strcmp(f->operador, "!=") == 0 && strcmp(str_item, f->valor_str) == 0) return false;
        }
    }

    //calculo da similaridade
    if(c->qtd_similares == 0){
        if(score_out) *score_out = 1.0;
        return true;
    }

    double soma_pond = 0.0;
    double soma_pesos = 0.0;

    for(int i = 0; i < c->qtd_similares; i++){
        SIM *s = &c->similares[i];
        double val_item = item_get_numero(item, s->campo);
        
        double diff = fabs(val_item - s->alvo); // módulo da diferença
        double sim_local = 1.0 - (diff / s->tolerancia);
        if(sim_local < 0.0) sim_local = 0.0; // max entre 0 e o resultado

        soma_pond += sim_local * s->peso;
        soma_pesos += s->peso;
    }

    double sim_total = (soma_pesos > 0) ? (soma_pond / soma_pesos) : 1.0;

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