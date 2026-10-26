#ifndef CLI_H
#define CLI_H

typedef struct cli_ CONFIG;

/**
 * @brief Cria uma struct do tipo CONFIG inicialmente vazia
 * 
 * @return Retorna um ponteiro para a struct CONFIG criada
 */
CONFIG *cli_criar(void);

/**
 * @brief Apaga uma estrutura do tipo CONFIG da memória
 * 
 * @param cli Ponteiro duplo que aponta para a estrutura a ser removida da memória
 * @return Retorna true se a estrutura foi apagada corretamente e false se não
 */
bool cli_apagar(CONFIG **cli);

/**
 * @brief Processa os argumentos passados na linha de comando e os insere em uma struct do tipo CONFIG. A struct já deve ter sido criada anteriormente
 * 
 * @param cli Struct do tipo CONFIG que receberá os dados parseados
 * @param argc Quantidade de argumentos recebidos
 * @param argv String com os argumentos recebidos
 * @return Struct CONFIG preenchida com os argumentos recebidos na linha de comando
 */
bool cli_parse_args(CONFIG *cli, int argc, char *argv[]);

/**
 * @brief Função auxiliar para obter o reader passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_reader(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter o input passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_input(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter a base passada para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_base(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter o result passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_result(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter o reader passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_query(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter a query passada para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna a string do resultado 
 */
const char* cli_get_output(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter o top-k passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna o int do resultado 
 */
int cli_get_top_k(const CONFIG *cli);
/**
 * @brief Função auxiliar para obter o limit passado para a cli
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Retorna o int do resultado 
 */
int cli_get_limit(const CONFIG *cli);
/**
 * @brief Imprime o a configuração atual de uma struct do tipo CONFIG
 * 
 * @param cli Ponteiro para a estrutura CONFIG alvo
 * @return Imprime todos os parâmetros da estrutura CONFIG com formatação utilizando o printf
 */
void cli_imprimir_config(const CONFIG *cli);


#endif