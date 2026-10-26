#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

typedef struct item_ ITEM;
/**
 * @brief Cria uma struct do tipo ITEM a partir da linha bruta original. Cada campo da linha é atribuído ao seu campo respectivo na strcut. Caso um campo esteja vazio na linha original, a struct receberá o valor -1
 * 
 * @param linha_bruta A linha original que deverá ser armazenada na estrutura de dados escolhida e que se tornará um ITEM
 * @return Retorna um ponteiro para uma struct ITEM 
 */
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



