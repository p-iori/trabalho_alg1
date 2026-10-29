#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "item.h"

// Estrutura que armazena os campos do csv de cada asteroide, além da linha bruta (facilita o print das informações) 
struct item_{
    bool alterado; 
    char *linha_bruta;
    char asteroid_designation[25];
    char asteroid_fullname[35];
    int year;
    int month;
    double distance_au;
    double velocity_km_s;
    double absolute_magnitude; 
    int days_until_approach;
    bool is_past_event;
    bool is_future_event;
    double risk_score;  
    int panic_level;
    char threat_category[20];
    char panic_verdict[50];
    bool on_sentry_list;
    char sentry_impact_prob; 
    double sentry_torino_scale;
    double sentry_palermo_scale;
    double sentry_diameter_km;
};

// meu compilador não estava aceitando o uso das funções strdup e strsep de forma nativa

static char* custom_strdup(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copia = (char*) malloc(len);
    if (copia != NULL) {
        memcpy(copia, s, len);
    }
    return copia;
}

static char* custom_strsep(char **string_ptr, const char *delim) {
    if (string_ptr == NULL || *string_ptr == NULL) return NULL;

    char *inicio = *string_ptr;
    char *p = strpbrk(inicio, delim);

    if (p != NULL) {
        *p = '\0';
        *string_ptr = p + 1;
    } else {
        *string_ptr = NULL;
    }

    return inicio;
}

ITEM *item_criar(const char linha_bruta[]){
    ITEM *item = (ITEM*) malloc(sizeof(ITEM));
    if(item == NULL) return NULL;

    item->alterado = false; //incializa o valor para evitar confusão com o lixo da memória

    item->linha_bruta = (linha_bruta != NULL) ? custom_strdup(linha_bruta) : NULL; // vamos precisar liberar essa parte da memória também
    item_parse_linha(item, linha_bruta);

    return item;
}

// função auxiliar que retorna um item vazio
ITEM *item_criar_vazio(void) {
    // calloc aloca e zera a memória
    ITEM *item_vazio = (ITEM*) calloc(1, sizeof(ITEM));
    if (item_vazio == NULL) return NULL; 

    return item_vazio;
}

void item_apagar(ITEM **item) {
    if (item == NULL || *item == NULL) return;

    // libera a memória alocada dinamicamente para a string
    if ((*item)->linha_bruta != NULL) {
        free((*item)->linha_bruta);
        (*item)->linha_bruta = NULL;
    }
    // libera a parte restante da struct
    free(*item);
    *item = NULL;
}

double item_get_numero(const ITEM *item, const char campo[]){
    if(item == NULL || campo == NULL) return 0.0;

    if(strcmp(campo, "year") == 0) return item->year;
    if(strcmp(campo, "month") == 0) return item->month;
    if(strcmp(campo, "distance_au") == 0) return item->distance_au;
    if(strcmp(campo, "velocity_km_s") == 0) return item->velocity_km_s;
    if(strcmp(campo, "absolute_magnitude") == 0) return item->absolute_magnitude;
    if(strcmp(campo, "days_until_approach") == 0) return item->days_until_approach;
    if(strcmp(campo, "risk_score") == 0) return item->risk_score;
    if(strcmp(campo, "panic_level") == 0) return item->panic_level;
    if(strcmp(campo, "sentry_torino_scale") == 0) return item->sentry_torino_scale;
    if(strcmp(campo, "sentry_palermo_scale") == 0) return item->sentry_palermo_scale;
    if(strcmp(campo, "sentry_diameter_km") == 0) return item->sentry_diameter_km;

    return -1;
}

const char* item_get_texto(const ITEM *item, const char campo[]){
    if(item == NULL || campo == NULL) return NULL;

    if(strcmp(campo, "asteroid_fullname") == 0) return item->asteroid_fullname;
    if(strcmp(campo, "threat_category") == 0) return item->threat_category;
    if(strcmp(campo, "panic_verdict") == 0) return item->panic_verdict;
    if(strcmp(campo, "asteroid_designation") == 0) return item->asteroid_designation;

    return NULL;
}

const char* item_get_linha_original(const ITEM *item){
    if(item == NULL) return NULL;
    return item->linha_bruta;
}

void item_print_csv(const ITEM *item, FILE *f){
    if(item == NULL || f == NULL) return;

    // se a linha não foi alterada, imprime a linha original (menos custoso)
    if(!item->alterado) {
        fprintf(f, "%s", item->linha_bruta);
        return;
    }

    printf("%s,%s,%d,%d,%.4f,%.2f,%.4f,%d,%s,%s,%.4f,%d,%s,%.2f,%.2f,%.2f\n",
            item_get_texto(item, "asteroid_designation"),
            item_get_texto(item, "asteroid_fullname"),
            (int)item_get_numero(item, "year"),
            (int)item_get_numero(item, "month"),
            item_get_numero(item, "distance_au"),
            item_get_numero(item, "velocity_km_s"),
            item_get_numero(item, "absolute_magnitude"),
            (int)item_get_numero(item, "days_until_approach"),
            item_get_texto(item, "threat_category"),
            item_get_texto(item, "panic_verdict"),
            item_get_numero(item, "risk_score"),
            (int)item_get_numero(item, "panic_level"),
            item_get_texto(item, "on_sentry_list"),
            item_get_numero(item, "sentry_torino_scale"),
            item_get_numero(item, "sentry_palermo_scale"),
            item_get_numero(item, "sentry_diameter_km")
    );
}

// recebe a string cortada até o campo atual
static char* item_extrair_proximo_campo(char **string_ptr){
    if(string_ptr == NULL || *string_ptr == NULL) return NULL;
    
    // armazena tudo que estiver entre o início da str e a primeira vírgula (token), então troca essa virgula por /0 e remove tudo antes dela. retorna o token no final de tudo
    return custom_strsep(string_ptr, ",");
}

static ITEM* item_parse_linha(ITEM* item, const char *linha_original){
    if(linha_original == NULL || linha_original[0] == '\0') return NULL;

    // a função custom_strsep modifica a string original, então precisamos copiar ela em um ponteiro
    char *orig = custom_strdup(linha_original);
    char *ptr = orig;

    char *token; // essa variável vai ser reutilizada várias vezes pela função
    
    // asteroid_designation
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') {
        strncpy(item->asteroid_designation, token, sizeof(item->asteroid_designation) - 1);
    }

    // asteroid_fullname
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') {
        strncpy(item->asteroid_fullname, token, sizeof(item->asteroid_fullname) - 1);
    }

    
    // atribuímos -1 para todos os campos vazios (-1.0 se o campo for um double)

    // year
    token = item_extrair_proximo_campo(&ptr);
    item->year = (token && token[0] != '\0') ? atoi(token) : -1;
    // month
    token = item_extrair_proximo_campo(&ptr);
    item->month = (token && token[0] != '\0') ? atoi(token) : -1;
    // distance_au 
    token = item_extrair_proximo_campo(&ptr);
    item->distance_au = (token && token[0] != '\0') ? atof(token) : -1.0;
    // velocity_km_s
    token = item_extrair_proximo_campo(&ptr);
    item->velocity_km_s = (token && token[0] != '\0') ? atof(token) : -1.0;
    // absolute_magnitude
    token = item_extrair_proximo_campo(&ptr);
    item->absolute_magnitude = (token && token[0] != '\0') ? atof(token) : -1.0;
    // days_until_approach
    token = item_extrair_proximo_campo(&ptr);
    item->days_until_approach = (token && token[0] != '\0') ? atoi(token) : -1;
    // threat_category
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') {
        strncpy(item->threat_category, token, sizeof(item->threat_category) - 1);
    }
    // panic_verdict
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') {
        strncpy(item->panic_verdict, token, sizeof(item->panic_verdict) - 1);
    }
    // risk_score
    token = item_extrair_proximo_campo(&ptr);
    item->risk_score = (token && token[0] != '\0') ? atof(token) : -1.0;
    // panic_level
    token = item_extrair_proximo_campo(&ptr);
    item->panic_level = (token && token[0] != '\0') ? atoi(token) : -1;
    // libera o ponteiro utilizado
    free(orig);

    return item;
}

// função para auxiliar nas operações de ordenação, principalmente
// retorna TRUE se A for maior
bool item_comparar_numero(const ITEM *a, const ITEM *b, const char campo[]){
    if(a == NULL || b == NULL || campo == NULL) return false;
    double val_a = item_get_numero(a, campo);
    double val_b = item_get_numero(b, campo);

    if((val_a == -1) || (val_b == -1)){
        printf("ERRO: Pelo menos um dos valores não é um número");
        return false;
    }

    if(val_a > val_b) return true;
    if(val_a < val_b) return false;

    return false; 

}
// retorna TRUE se os valores forem IGUAIS
bool item_comparar_texto(const ITEM *a, const ITEM *b, const char campo[]){
    if(a == NULL || b == NULL || campo == NULL) return false;

    if((item_get_numero(a, campo) == -1) || (item_get_numero(b, campo) == -1)){
        printf("ERRO: Pelo menos um dos valores não é uma string");
        return false;
    }
    
    char *val_a = item_get_texto(a, campo);
    char *val_b = item_get_texto(b, campo);

    if(strcmp(a, b) == 0) return true;

    return false;
}

ITEM *item_clonar(const ITEM *item){
    if(item == NULL) return NULL;

    ITEM *item_clonado = item_criar_vazio();
    if(item_clonado == NULL) return NULL;

    *item_clonado = *item;    
    // necessario pois esse campo é um ponteiro dinamico na struct
    if(item->linha_bruta != NULL){
        item_clonado->linha_bruta = custom_strdup(item->linha_bruta);
        if(item_clonado->linha_bruta == NULL){
            free(item_clonado);
            return NULL;
        } 
    }
    
    return item_clonado;
}