#include "fuzz.h"

static char *process;

int fuzz(int fd_in[], int fd_err[], char **argv, char *mut_str) {
    int len = strlen(mut_str);

    pid_t pid = fork();
    if (pid == 0) {
        // Child Process.
        dup2(fd_in[0], STDERR_FILENO);
        dup2(fd_err[0], STDERR_FILENO);

        argv[i_fuz] = malloc(sizeof(char) * len + 1);
        snprintf(argv[i_fuz], len, "%s", mut_str);

        execvp(process, argv);

        free(argv[i_fuz]);
        return CHILD;
    } else if (pid == ERROR) {
        // Error.
        printf("Error Forking, %s", strerror(errno));
        return ERROR;
    } else {
        // Parent Process.
        int sig;
        if (waitpid(pid, &sig, 0) == ERROR) {
            printf("Couldn't Receive Termination Signal. %s", strerror(errno));
            return ERROR;
        }

        sig = WIFSIGNALED(sig);
        if (sig == SIGSEGV || sig == SIGABRT) {
            proc_sig = sig;
            return OK;
        }

        return 0;
    }
}

void free_all() {
    if (process != NULL) {
        free(process);
    }
}

int fuzzer(char *proc, int argc, char **argv) {
    int fd_in[2], fd_err[2]; // fd[0] - read, fd[1] - write.
    if (pipe(fd_in) == -1) {
        printf("Error Creating stdin Pipe. %s", strerror(errno));
        return ERROR;
    }
    if (pipe(fd_err) == -1) {
        printf("Error Creating stderr Pipe. %s", strerror(errno));
        return ERROR;
    }
    int len = strlen(proc);
    process = malloc(sizeof(char) * len + 1);
    snprintf(process, len, "%s", proc);

    char *mut_str;
    while ((mut_str = mutate_strings()) != NULL) {
        if (fuzz(fd_in, fd_err, argv, mut_str) == OK) {
            return OK;
        }
    }

    return ERROR;
}
