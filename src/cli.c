#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cli.h"

struct config_{
    char reader[32]; 
    char input[256]; 
    char base[32];    
    char result[32];   
    char query[256];   
    char output[256];  
    int top_k;        
    int limit;         
};

CONFIG cli_parse_args(int argc, char *argv[]){

}