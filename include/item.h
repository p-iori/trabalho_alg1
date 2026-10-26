#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

typedef struct item_ ITEM;

ITEM *item_criar(const char linha_bruta[]);
ITEM* item_criar_vazio();
void item_apagar(ITEM **item);
double item_get_numero(const ITEM *item, const char campo[]);
const char* item_get_texto(const ITEM *item, const char campo[]);
const char* item_get_linha_original(const ITEM *item);
void item_print_csv(const ITEM *item, FILE *f);
ITEM* item_parse_linha(ITEM* item, const char *linha_original);
bool item_comparar(const ITEM *a, const ITEM *b, const char campo[]);
ITEM *item_clonar(const ITEM *item);




#endif



