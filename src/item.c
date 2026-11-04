#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "item.h"

#define MAX_CAMPOS 35

typedef struct {
    bool eh_numerico;
    char chave[50];
    char valor_str[100];
    double valor_num;
} CAMPO;

struct item_ {
    char *linha_bruta;
    CAMPO campos[MAX_CAMPOS];
    int qtd_campos;
};

// Lista de campos do dataset da NASA
static const char *CAMPOS_NASA[] = {
    "asteroid_designation", "asteroid_fullname", "close_approach_date",
    "year", "month", "distance_au", "velocity_km_s", "absolute_magnitude",
    "days_until_approach", "is_past_event", "is_future_event", "risk_score",
    "panic_level", "threat_category", "panic_verdict", "on_sentry_list",
    "sentry_impact_prob", "sentry_torino_scale", "sentry_palermo_scale",
    "sentry_diameter_km"
};

// Lista de campos do dataset do DETRAN
static const char *CAMPOS_DETRAN[] = {
    "id", "data_inversa", "dia_semana", "horario", "uf", "br", "km",
    "municipio", "causa_acidente", "tipo_acidente", "classificacao_acidente",
    "fase_dia", "sentido_via", "condicao_metereologica", "tipo_pista",
    "tracado_via", "uso_solo", "pessoas", "mortos", "feridos_leves",
    "feridos_graves", "ilesos", "ignorados", "feridos", "veiculos",
    "latitude", "longitude", "regional", "delegacia", "uop"
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

static char* remover_aspas_e_espacos(char *str) {
    if (str == NULL) return NULL;
    while (*str == '"' || *str == ' ' || *str == '\t') str++;
    
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '"' || str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[--len] = '\0';
    }
    return str;
}


ITEM *item_criar_vazio(void) {
    ITEM *item = (ITEM*) calloc(1, sizeof(ITEM));
    if (item == NULL) return NULL;
    item->linha_bruta = NULL;
    item->qtd_campos = 0;
    return item;
}

static void item_set_campo(ITEM *item, const char *chave, const char *valor) {
    if (item == NULL || chave == NULL || valor == NULL) return;
    if (item->qtd_campos >= MAX_CAMPOS) return;

    CAMPO *attr = &item->campos[item->qtd_campos++];

    strncpy(attr->chave, chave, sizeof(attr->chave) - 1);
    attr->chave[sizeof(attr->chave) - 1] = '\0';

    strncpy(attr->valor_str, valor, sizeof(attr->valor_str) - 1);
    attr->valor_str[sizeof(attr->valor_str) - 1] = '\0';

    // Suporte numérico para flags booleanas
    if (strcmp(valor, "true") == 0 || strcmp(valor, "TRUE") == 0) {
        attr->valor_num = 1.0;
        attr->eh_numerico = true;
    } else if (strcmp(valor, "false") == 0 || strcmp(valor, "FALSE") == 0) {
        attr->valor_num = 0.0;
        attr->eh_numerico = true;
    } else {
        char *endptr;
        attr->valor_num = strtod(valor, &endptr);
        while (*endptr == ' ' || *endptr == '\t' || *endptr == '\r' || *endptr == '\n') endptr++;
        attr->eh_numerico = (endptr != valor && *endptr == '\0');
    }
}

// Parser do Dataset NASA (Separado por vírgula)
ITEM *item_criar(const char linha_bruta[]) {
    if (linha_bruta == NULL) return NULL;

    ITEM *item = item_criar_vazio();
    if (item == NULL) return NULL;

    item->linha_bruta = custom_strdup(linha_bruta);

    char *orig = custom_strdup(linha_bruta);
    if (orig == NULL) return item;

    char *ptr = orig;
    int total_campos = (int)(sizeof(CAMPOS_NASA) / sizeof(CAMPOS_NASA[0]));

    for (int i = 0; i < total_campos && ptr != NULL; i++) {
        char *token = custom_strsep(&ptr, ",");
        if (token != NULL) {
            item_set_campo(item, CAMPOS_NASA[i], token);
        }
    }

    free(orig);
    return item;
}

// Parser do Dataset DETRAN (Separado por ponto e vírgula e com aspas)
ITEM *item_criar_detran(const char linha_bruta[]) {
    if (linha_bruta == NULL) return NULL;

    ITEM *item = item_criar_vazio();
    if (item == NULL) return NULL;

    item->linha_bruta = custom_strdup(linha_bruta);

    char *orig = custom_strdup(linha_bruta);
    if (orig == NULL) return item;

    char *ptr = orig;
    int total_campos = (int)(sizeof(CAMPOS_DETRAN) / sizeof(CAMPOS_DETRAN[0]));

    for (int i = 0; i < total_campos && ptr != NULL; i++) {
        char *token = custom_strsep(&ptr, ";");
        if (token != NULL) {
            token = remover_aspas_e_espacos(token);
            item_set_campo(item, CAMPOS_DETRAN[i], token);
        }
    }

    free(orig);
    return item;
}

void item_apagar(ITEM **item) {
    if (item == NULL || *item == NULL) return;

    if ((*item)->linha_bruta != NULL) {
        free((*item)->linha_bruta);
        (*item)->linha_bruta = NULL;
    }

    free(*item);
    *item = NULL;
}

double item_get_numero(const ITEM *item, const char campo[]) {
    if (item == NULL || campo == NULL) return -1.0;

    for (int i = 0; i < item->qtd_campos; i++) {
        if (strcmp(item->campos[i].chave, campo) == 0) {
            return item->campos[i].valor_num;
        }
    }

    return -1.0;
}

const char* item_get_texto(const ITEM *item, const char campo[]) {
    if (item == NULL || campo == NULL) return NULL;

    for (int i = 0; i < item->qtd_campos; i++) {
        if (strcmp(item->campos[i].chave, campo) == 0) {
            return item->campos[i].valor_str;
        }
    }

    return NULL;
}

const char* item_get_linha_original(const ITEM *item) {
    if (item == NULL) return NULL;
    return item->linha_bruta;
}

void item_print_csv(const ITEM *item, FILE *f) {
    if (item == NULL || f == NULL) return;

    if (item->linha_bruta != NULL) {
        fprintf(f, "%s\n", item->linha_bruta);
    }
}

bool item_comparar_numero(const ITEM *a, const ITEM *b, const char campo[]) {
    if (a == NULL || b == NULL || campo == NULL) return false;
    double val_a = item_get_numero(a, campo);
    double val_b = item_get_numero(b, campo);

    if (val_a == -1.0 || val_b == -1.0) return false;
    return (val_a > val_b);
}

bool item_comparar_texto(const ITEM *a, const ITEM *b, const char campo[]) {
    if (a == NULL || b == NULL || campo == NULL) return false;

    const char *val_a = item_get_texto(a, campo);
    const char *val_b = item_get_texto(b, campo);

    if (val_a == NULL || val_b == NULL) return false;
    return (strcmp(val_a, val_b) == 0);
}

ITEM *item_clonar(const ITEM *item) {
    if (item == NULL) return NULL;

    ITEM *item_clonado = item_criar_vazio();
    if (item_clonado == NULL) return NULL;

    // Copia todos os campos estáticos e a tabela campos em um único passo
    *item_clonado = *item;

    if (item->linha_bruta != NULL) {
        item_clonado->linha_bruta = custom_strdup(item->linha_bruta);
        if (item_clonado->linha_bruta == NULL) {
            free(item_clonado);
            return NULL;
        }
    }

    return item_clonado;
}