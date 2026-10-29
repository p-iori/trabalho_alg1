#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

// Armazena os valores dos campos de uma determinada linha
typedef struct item_ ITEM;

/**
 * @brief Cria uma struct do tipo ITEM a partir da linha bruta original. Cada campo da linha é atribuído ao seu campo respectivo na strcut. Caso um campo esteja vazio na linha original, a struct receberá o valor -1
 * 
 * @param linha_bruta A linha original que deverá ser armazenada na estrutura de dados escolhida e que se tornará um ITEM
 * @return Retorna um ponteiro para uma struct ITEM 
 */
ITEM *item_criar(const char linha_bruta[]);

/**
 * @brief Cria uma struct do tipo ITEM com todos os parâmetros zerados a sem precisar utilizar uma linha bruta original
 * 
 * @return Retorna um ponteiro para a struct ITEM vazia criada
 */
ITEM* item_criar_vazio();

/**
 * @brief Apaga uma struct do tipo ITEM da memória. Caso seja chamada, não é necessário liberar nenhum ponteiro manualmente.
 * 
 * @param item Endereço de memória para a struct ITEM a ser apagada 
 */
void item_apagar(ITEM **item);

/**
 * @brief Função auxiliar para obter o valor de um parâmetro do tipo int, float ou double dentro de uma struct ITEM
 * 
 * @param item Ponteiro para a struct em que o parâmetro se encontra
 * @param campo Nome do parâmetro que deve ser retornado
 * 
 * @return Retorna um número do tipo double correspondente ao valor do campo. Caso o campo não seja numérico, retorna -1
 */
double item_get_numero(const ITEM *item, const char campo[]);

/**
 * @brief Função auxiliar para obter o valor de um parâmetro do tipo string dentro de uma struct ITEM
 * 
 * @param item Ponteiro para a struct em que o parâmetro se encontra
 * @param campo Nome do parâmetro que deve ser retornado
 * 
 * @return Retorna uma string correspondente ao valor do campo 
 */
const char* item_get_texto(const ITEM *item, const char campo[]);

/**
 * @brief Função auxiliar para obter a linha original de uma struct ITEM
 * 
 * @param item Ponteiro para a struct ITEM
 * 
 * @return Retorna a string original que foi parseada para dar origem à struct ITEM 
 */
const char* item_get_linha_original(const ITEM *item);

/**
 * @brief Imprime os parâmetros do item no formato original do arquivo .csv 
 * 
 * @param item Ponteiro para a struct ITEM
 */
void item_print_csv(const ITEM *item);

/**
 * @brief Compara o valor de um campo NUMÉRICO em dois itens
 * 
 * @param a Ponteiro para a primeira struct ITEM
 * @param b Ponteiro para a segunda struct ITEM
 * @param campo String contendo o nome do campo a ser comparado nos dois itens
 * 
 * @return Retorna TRUE se o ITEM A for maior e FALSE se o ITEM B for maior
 */
bool item_comparar_numero(const ITEM *a, const ITEM *b, const char campo[]);

/**
 * @brief Compara o valor de um campo de TEXTO em dois itens
 * 
 * @param a Ponteiro para a primeira struct ITEM
 * @param b Ponteiro para a segunda struct ITEM
 * @param campo String contendo o nome do campo a ser comparado nos dois itens
 * 
 * @return Retorna TRUE se os valores dos campos forem IGUAIS e FALSE se forem DIFERENTES
 */
bool item_comparar_texto(const ITEM *a, const ITEM *b, const char campo[]);

ITEM *item_clonar(const ITEM *item);




#endif



