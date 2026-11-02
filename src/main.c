#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cli.h"
#include "pipeline.h"

int main(int argc, char **argv) {
    CONFIG *cli = cli_criar();
    if (cli == NULL) return EXIT_FAILURE;

    if (!cli_parse_args(cli, argc, argv)) {
        cli_apagar(&cli);
        return EXIT_FAILURE;
    }

    PIPELINE *exec = pipeline_criar();
    if (exec == NULL) {
        cli_apagar(&cli);
        return EXIT_FAILURE;
    }

    bool sucesso = pipeline_run(exec, cli);

    pipeline_apagar(&exec);
    cli_apagar(&cli);

    return sucesso ? EXIT_SUCCESS : EXIT_FAILURE;
}