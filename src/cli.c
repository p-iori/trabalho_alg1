#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cli.h"

struct cli_ {
    char *reader;
    char *input;
    char *base;
    char *result;
    char *query;
    char *output;
    int top_k; // 0 para toda a tabela
    int limit; // 0 para toda a tabela
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

CONFIG *cli_criar(void){
    CONFIG *cli = (CONFIG*) calloc(1, sizeof(CONFIG));
    if(cli == NULL) return NULL;

    cli->top_k = 0;
    cli->limit = 0;

    return cli;
}

bool cli_apagar(CONFIG **cli){
    if(*cli == NULL) return false;

    if((*cli)->reader) free((*cli)->reader);
    if((*cli)->input) free((*cli)->input);
    if ((*cli)->base) free((*cli)->base);
    if ((*cli)->result) free((*cli)->result);
    if ((*cli)->query) free((*cli)->query);
    if ((*cli)->output) free((*cli)->output);

    free(*cli);
    *cli = NULL;

    return true;
}

bool cli_parse_args(CONFIG *cli, int argc, char* argv[]){
    if(cli == NULL || argv == NULL || argc < 2) return false;

    for(int i = 1; i < argc - 1; i++){
        char *arg = argv[i];
        char *param = argv[i+1];

        if(arg[0] == '-'){
            while(arg[0] == '-') arg++;

            if(strcmp(arg, "reader") == 0){
                if(cli->reader) free(cli->reader);
                cli->reader = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "input") == 0){
                if(cli->input) free(cli->input);
                cli->input = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "base") == 0){
                if(cli->base) free(cli->base);
                cli->base = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "result") == 0){
                if(cli->result) free(cli->result); 
                cli->result = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "query") == 0){
                if(cli->query) free(cli->query); 
                cli->query = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "output") == 0){
                if(cli->output) free(cli->output); 
                cli->output = custom_strdup(param);
                i++;
            }
            else if(strcmp(arg, "top-k") == 0){
                cli->top_k = atoi(param);
                i++;
            }
            else if(strcmp(arg, "limit") == 0) {
                cli->limit = atoi(param);
                i++;
            }
        }
    }

    return true;
}

const char* cli_get_reader(const CONFIG *cli) {
    return (cli != NULL) ? cli->reader : NULL;
}

const char* cli_get_input(const CONFIG *cli) {
    return (cli != NULL) ? cli->input : NULL;
}

const char* cli_get_base(const CONFIG *cli) {
    return (cli != NULL) ? cli->base : NULL;
}

const char* cli_get_result(const CONFIG *cli) {
    return (cli != NULL) ? cli->result : NULL;
}

const char* cli_get_query(const CONFIG *cli) {
    return (cli != NULL) ? cli->query : NULL;
}

const char* cli_get_output(const CONFIG *cli) {
    return (cli != NULL) ? cli->output : NULL;
}

int cli_get_top_k(const CONFIG *cli) {
    return (cli != NULL) ? cli->top_k : 0;
}

int cli_get_limit(const CONFIG *cli) {
    return (cli != NULL) ? cli->limit : 0;
}

void cli_imprimir_config(const CONFIG *cli) {
    if (cli == NULL) return;
    printf("--- Configuração da CLI ---\n");
    printf("Reader: %s\n", cli->reader ? cli->reader : "N/A");
    printf("Input:  %s\n", cli->input  ? cli->input  : "N/A");
    printf("Base:   %s\n", cli->base   ? cli->base   : "N/A");
    printf("Result: %s\n", cli->result ? cli->result : "N/A");
    printf("Query:  %s\n", cli->query  ? cli->query  : "N/A");
    printf("Output: %s\n", cli->output ? cli->output : "N/A");
    printf("Top-K:  %d\n", cli->top_k);
    printf("Limit:  %d\n", cli->limit);
    printf("---------------------------\n");
}