#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "item.h"

struct item_{
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

ITEM *item_criar(){
    ITEM *item = (ITEM*) malloc(sizeof(ITEM));
    if(item == NULL) return NULL;

    item->asteroid_designation[0] = '\0';
    item->asteroid_fullname[0] = '\0';
    item->year = 0;
    item->month = 0;
    item->distance_au = 0.0;
    item->velocity_km_s = 0.0;
    item->absolute_magnitude = 0.0;
    item->days_until_approach = 0;
    item->is_past_event = false;
    item->is_future_event = false;
    item->risk_score = 0.0;
    item->panic_level = 0;
    item->threat_category[0] = '\0';
    item->panic_verdict[0] = '\0';
    item->on_sentry_list = false;
    item->sentry_impact_prob = 0.0;
    item->sentry_torino_scale = 0.0;
    item->sentry_palermo_scale = 0.0;
    item->sentry_diameter_km = 0.0;

    return item;
}

bool item_apagar(ITEM **item){
    if(item == NULL || *item == NULL) return false;

    free(*item);
    *item = NULL;
    return true;
}

double item_get_numero(const ITEM *item, const char campo[]){
    if(item == NULL || campo == "") return 0.0;

    if(strcomp(campo, "year")) return item->year;
    if(strcomp(campo, "month")) return item->month;
    if(strcomp(campo, "distance_au")) return item->distance_au;
    if(strcomp(campo, "velocity_km_s")) return item->velocity_km_s;
    if(strcomp(campo, "absolute_magnitude")) return item->absolute_magnitude;
    if(strcomp(campo, "days_until_approach")) return item->days_until_approach;
    if(strcomp(campo, "risk_score")) return item->risk_score;
    if(strcomp(campo, "panic_level")) return item->panic_level;
    if(strcomp(campo, "sentry_torino_scale")) return item->sentry_torino_scale;
    if(strcomp(campo, "sentry_palermo_scale")) return item->sentry_palermo_scale;
    if(strcomp(campo, "sentry_diameter_km")) return item->sentry_diameter_km;

    return 0.0;
}

const char* item_get_texto(const ITEM *item, const char campo[]){
    if(item == NULL || campo == "") return NULL;

    if(strcomp(campo, "asteroid_designation")) return item->asteroid_designation;
    if(strcomp(campo, "asteroid_fullname")) return item->asteroid_fullname;
    if(strcomp(campo, "threat_category")) return item->threat_category;
    if(strcomp(campo, "panic_verdict")) return item->panic_verdict;

    return NULL;
}