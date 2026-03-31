#include "fuzz.h"

static char *process;

void free_dbl_ptr(void **ptr, int len) {
    if (ptr != NULL) {
        for (int i = 0; i < len; i++) {
            if (ptr[i] != NULL) {
                free(ptr[i]);
            }
        }
        free(ptr);
    }
}

int fuzz(const int fd_in[], const int fd_err[], char **argv) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child Process.
        dup2(fd_in[0], STDIN_FILENO);
        dup2(fd_err[1], STDERR_FILENO);

        execvp(process, argv);
        return CHILD;
    } else if (pid == ERROR) {
        // Error.
        printf("Error Forking, %s", strerror(errno));
        return ERROR;
    } else {
        // Parent Process.
        close(fd_in[0]);
        close(fd_in[1]);
        close(fd_err[1]);

        ssize_t len;
        char buffer[1025];
        printf("Testing %s:\n", argv[i_fuz]);
        while ((len = read(fd_err[0], buffer, 1024))) {
            if (len == -1) {
                printf("Error reading from the child process' STDERR. %s.\n",
                       strerror(errno));
                return ERROR;
            }
            buffer[len] = '\0';
            printf("%s\n", buffer);
        }
        close(fd_err[0]);

        int sig;
        if (waitpid(pid, &sig, 0) == ERROR) {
            printf("Couldn't Receive Termination Signal. %s", strerror(errno));
            return ERROR;
        }

        if (WIFSIGNALED(sig)) {
            sig = WTERMSIG(sig);
            if (sig == SIGSEGV || sig == SIGABRT) {
                proc_sig = sig;
                return OK;
            }
        }

        return 0;
    }
}

void free_all() {
    if (process != NULL) {
        free(process);
    }
}

int fuzzer(const char *proc, char **argv) {
    int len = strlen(proc) + 1;
    process = malloc(sizeof(char) * len);
    snprintf(process, len, "%s", proc);

    char *mut_str, *t_ptr = argv[i_fuz];
    while ((mut_str = mutate_string(argv[i_fuz])) != NULL) {
        int fd_in[2], fd_err[2]; // fd[0] - read, fd[1] - write.
        if (pipe(fd_in) == -1) {
            printf("Error Creating stdin Pipe. %s", strerror(errno));
            return ERROR;
        }
        if (pipe(fd_err) == -1) {
            printf("Error Creating stderr Pipe. %s", strerror(errno));
            return ERROR;
        }

        argv[i_fuz] = mut_str;
        switch (fuzz(fd_in, fd_err, argv)) {
        case ERROR:
            argv[i_fuz] = t_ptr;
            free(mut_str);
            free_all();
            return ERROR;
        case OK:
            argv[i_fuz] = t_ptr;
            free(mut_str);
            free_all();
            return OK;
        case CHILD:
            exit(0);
        }
        free(mut_str);
        argv[i_fuz] = t_ptr;
    }

    free_all();
    return ERROR;
}
