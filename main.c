#include "fuzz.h"
#include <stdlib.h>
#include <string.h>

void help() {
    char *help = "Usage: fuzzer [target_file] -v [argv]\n"
                 "Use FUZZ on the target argv";
    printf("%s", help);
}

char **get_child_argv(int argc, char **argv, int *i) {
    char **child_argv = malloc(sizeof(char *) * argc);

    for (*i = 0; *i < argc; (*i)++) {
        if (strncmp(argv[*i], "-v", 2) == 0) {
            (*i)++;
            while (*i < argc) {
                child_argv[*i] = malloc(sizeof(char) * strlen(argv[*i]) + 1);
                if (strncmp(argv[*i], "FUZZ", 4) == 0) {
                    argv[*i] += 5;
                    i_fuz = *i;
                }
                snprintf(child_argv[*i], strlen(argv[*i]), "%s", argv[*i]);
                (*i)++;
            }
        }
    }

    return child_argv;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
        return EXIT_FAILURE;
    }

    int child_argc, i_fuz;
    char **child_argv = get_child_argv(argc, argv, &child_argc);

    if (fuzzer(argv[1], child_argv) == ERROR) {
        printf("Fuzzer Encountered An Error. %s", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Fuzzer Found %d on input %s", proc_sig, input);

    for (int i = 0; i < child_argc; i++) {
        if (child_argv[i] != NULL) {
            free(child_argv[i]);
        }
    }
    free(child_argv);

    return EXIT_SUCCESS;
}
