#ifndef CLI_H
#define CLI_H

typedef struct config_ CONFIG;

CONFIG cli_parse_args(int argc, char *argv[]);

#endif