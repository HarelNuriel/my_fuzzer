#ifndef MY_Mutator_H
#define MY_Mutator_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DONE 3
#define RESET2 2
#define OK 1
#define RESET 0
#define ERROR (-1)

#define CHARS_LEN 12
#define STRS_LEN 4

static uint64_t max_len = 4096;

char *mutate_string(char *str);

// Adding \u0000 prompts an error.
static char *special_strs[STRS_LEN] = {"%s", "%d", "%n", "\uFEFF"};
static char special_chars[CHARS_LEN] = {'"', '(', ')', ':', '\'', ';',
                                        '<', '>', '&', '|', '\\', '/'};

#endif // !pthread_mutexattr_t
