#include "mutator.h"

int bit_mutation(char *str, int bit) {
    int byte = bit / 8;
    bit %= 8;

    if (strlen(str) > byte) {
        str[byte] = str[byte] | (1 << bit);
        return OK;
    } else {
        return RESET;
    }
}

int byte_mutation(char *str, int byte, int char_index) {
    if (byte >= strlen(str))
        return RESET2;

    if (char_index >= CHARS_LEN)
        return RESET;

    str[byte] = special_chars[char_index];
    return OK;
}

int mutate_state(char *str) {
    static int state[3] = {0};

    switch (state[0]) {
    case 0:
        if (!bit_mutation(str, state[1]++)) {
            state[0]++;
            state[1] = 0;
            return RESET;
        }
        return OK;
    case 1:
        switch (byte_mutation(str, state[1], state[2])) {
        case RESET2:
            state[0]++;
            state[1] = 0;
            state[2] = 0;
            return RESET;
        case RESET:
            state[1]++;
            state[2] = 0;
            return RESET;
        case OK:
            return OK;
        }
        return OK;
    default:
        state[0] = 0;
        state[1] = 0;
        return DONE;
    }
}

char *mutate_string(char *str) {
    int len = strlen(str) + 1;
    char *mut_str = malloc(sizeof(char) * max_len);
    if (mut_str == NULL) {
        printf("Failed To Allocate Memory. %s.\n", strerror(errno));
        return NULL;
    }
    snprintf(mut_str, len, "%s", str);

    while (1) {
        switch (mutate_state(mut_str)) {
        case DONE:
            return NULL;
        case OK:
            return mut_str;
        default:
            continue;
        }
    }
}
