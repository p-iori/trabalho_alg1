#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "item.h"

struct item_ {
    bool alterado; 
    char *linha_bruta;
    char asteroid_designation[30];
    char asteroid_fullname[50];
    char close_approach_date[30];
    int year;
    int month;
    double distance_au;
    double velocity_km_s;
    double absolute_magnitude; 
    int days_until_approach;
    char is_past_event[10];
    char is_future_event[10];
    double risk_score;  
    int panic_level;
    char threat_category[30];
    char panic_verdict[60];
    char on_sentry_list[10];
    char sentry_impact_prob[30]; 
    double sentry_torino_scale;
    double sentry_palermo_scale;
    double sentry_diameter_km;
};

static ITEM* item_parse_linha(ITEM* item, const char *linha_original);

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

    item->alterado = false;
    item->linha_bruta = (linha_bruta != NULL) ? custom_strdup(linha_bruta) : NULL;
    item_parse_linha(item, linha_bruta);

    return item;
}

ITEM *item_criar_vazio(void) {
    return (ITEM*) calloc(1, sizeof(ITEM));
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

double item_get_numero(const ITEM *item, const char campo[]){
    if(item == NULL || campo == NULL) return -1.0;

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

    // Suporte numérico para flags booleanas
    if(strcmp(campo, "is_past_event") == 0) return (strcmp(item->is_past_event, "true") == 0) ? 1.0 : 0.0;
    if(strcmp(campo, "is_future_event") == 0) return (strcmp(item->is_future_event, "true") == 0) ? 1.0 : 0.0;
    if(strcmp(campo, "on_sentry_list") == 0) return (strcmp(item->on_sentry_list, "true") == 0) ? 1.0 : 0.0;

    return -1.0;
}

const char* item_get_texto(const ITEM *item, const char campo[]){
    if(item == NULL || campo == NULL) return NULL;

    if(strcmp(campo, "asteroid_designation") == 0) return item->asteroid_designation;
    if(strcmp(campo, "asteroid_fullname") == 0) return item->asteroid_fullname;
    if(strcmp(campo, "close_approach_date") == 0) return item->close_approach_date;
    if(strcmp(campo, "is_past_event") == 0) return item->is_past_event;
    if(strcmp(campo, "is_future_event") == 0) return item->is_future_event;
    if(strcmp(campo, "threat_category") == 0) return item->threat_category;
    if(strcmp(campo, "panic_verdict") == 0) return item->panic_verdict;
    if(strcmp(campo, "on_sentry_list") == 0) return item->on_sentry_list;
    if(strcmp(campo, "sentry_impact_prob") == 0) return item->sentry_impact_prob;

    return NULL;
}

const char* item_get_linha_original(const ITEM *item){
    if(item == NULL) return NULL;
    return item->linha_bruta;
}

void item_print_csv(const ITEM *item, FILE *f){
    if(item == NULL || f == NULL) return;

    if(!item->alterado && item->linha_bruta != NULL) {
        fprintf(f, "%s\n", item->linha_bruta);
        return;
    }

    fprintf(f, "%s,%s,%s,%d,%d,%.4f,%.2f,%.4f,%d,%s,%s,%.4f,%d,%s,%s,%s,%s,%.2f,%.2f,%.2f\n",
            item_get_texto(item, "asteroid_designation"),
            item_get_texto(item, "asteroid_fullname"),
            item_get_texto(item, "close_approach_date"),
            (int)item_get_numero(item, "year"),
            (int)item_get_numero(item, "month"),
            item_get_numero(item, "distance_au"),
            item_get_numero(item, "velocity_km_s"),
            item_get_numero(item, "absolute_magnitude"),
            (int)item_get_numero(item, "days_until_approach"),
            item_get_texto(item, "is_past_event"),
            item_get_texto(item, "is_future_event"),
            item_get_numero(item, "risk_score"),
            (int)item_get_numero(item, "panic_level"),
            item_get_texto(item, "threat_category"),
            item_get_texto(item, "panic_verdict"),
            item_get_texto(item, "on_sentry_list"),
            item_get_texto(item, "sentry_impact_prob"),
            item_get_numero(item, "sentry_torino_scale"),
            item_get_numero(item, "sentry_palermo_scale"),
            item_get_numero(item, "sentry_diameter_km")
    );
}

static char* item_extrair_proximo_campo(char **string_ptr){
    if(string_ptr == NULL || *string_ptr == NULL) return NULL;
    return custom_strsep(string_ptr, ",");
}

static ITEM* item_parse_linha(ITEM* item, const char *linha_original){
    if(linha_original == NULL || linha_original[0] == '\0') return NULL;

    char *orig = custom_strdup(linha_original);
    if (orig == NULL) return item;
    char *ptr = orig;
    char *token;

    // 1. asteroid_designation
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->asteroid_designation, token, sizeof(item->asteroid_designation) - 1);

    // 2. asteroid_fullname
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->asteroid_fullname, token, sizeof(item->asteroid_fullname) - 1);

    // 3. close_approach_date
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->close_approach_date, token, sizeof(item->close_approach_date) - 1);

    // 4. year
    token = item_extrair_proximo_campo(&ptr);
    item->year = (token && token[0] != '\0') ? atoi(token) : -1;

    // 5. month
    token = item_extrair_proximo_campo(&ptr);
    item->month = (token && token[0] != '\0') ? atoi(token) : -1;

    // 6. distance_au 
    token = item_extrair_proximo_campo(&ptr);
    item->distance_au = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 7. velocity_km_s
    token = item_extrair_proximo_campo(&ptr);
    item->velocity_km_s = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 8. absolute_magnitude
    token = item_extrair_proximo_campo(&ptr);
    item->absolute_magnitude = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 9. days_until_approach
    token = item_extrair_proximo_campo(&ptr);
    item->days_until_approach = (token && token[0] != '\0') ? atoi(token) : -1;

    // 10. is_past_event
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->is_past_event, token, sizeof(item->is_past_event) - 1);

    // 11. is_future_event
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->is_future_event, token, sizeof(item->is_future_event) - 1);

    // 12. risk_score
    token = item_extrair_proximo_campo(&ptr);
    item->risk_score = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 13. panic_level
    token = item_extrair_proximo_campo(&ptr);
    item->panic_level = (token && token[0] != '\0') ? atoi(token) : -1;

    // 14. threat_category
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->threat_category, token, sizeof(item->threat_category) - 1);

    // 15. panic_verdict
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->panic_verdict, token, sizeof(item->panic_verdict) - 1);

    // 16. on_sentry_list
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->on_sentry_list, token, sizeof(item->on_sentry_list) - 1);

    // 17. sentry_impact_prob
    token = item_extrair_proximo_campo(&ptr);
    if (token && token[0] != '\0') strncpy(item->sentry_impact_prob, token, sizeof(item->sentry_impact_prob) - 1);

    // 18. sentry_torino_scale
    token = item_extrair_proximo_campo(&ptr);
    item->sentry_torino_scale = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 19. sentry_palermo_scale
    token = item_extrair_proximo_campo(&ptr);
    item->sentry_palermo_scale = (token && token[0] != '\0') ? atof(token) : -1.0;

    // 20. sentry_diameter_km
    token = item_extrair_proximo_campo(&ptr);
    item->sentry_diameter_km = (token && token[0] != '\0') ? atof(token) : -1.0;

    free(orig);
    return item;
}

bool item_comparar_numero(const ITEM *a, const ITEM *b, const char campo[]){
    if(a == NULL || b == NULL || campo == NULL) return false;
    double val_a = item_get_numero(a, campo);
    double val_b = item_get_numero(b, campo);

    if(val_a == -1.0 || val_b == -1.0) return false;
    return (val_a > val_b);
}

bool item_comparar_texto(const ITEM *a, const ITEM *b, const char campo[]){
    if(a == NULL || b == NULL || campo == NULL) return false;

    const char *val_a = item_get_texto(a, campo);
    const char *val_b = item_get_texto(b, campo);

    if(val_a == NULL || val_b == NULL) return false;
    return (strcmp(val_a, val_b) == 0);
}

ITEM *item_clonar(const ITEM *item){
    if(item == NULL) return NULL;

    ITEM *item_clonado = item_criar_vazio();
    if(item_clonado == NULL) return NULL;

    *item_clonado = *item;    
    if(item->linha_bruta != NULL){
        item_clonado->linha_bruta = custom_strdup(item->linha_bruta);
        if(item_clonado->linha_bruta == NULL){
            free(item_clonado);
            return NULL;
        } 
    }
    
    return item_clonado;
}