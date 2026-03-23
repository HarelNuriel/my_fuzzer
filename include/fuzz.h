#ifndef MY_FUZZER_PROJECT
#define MY_FUZZER_PROJECT

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR -1
#define OK 1
#define CHILD -2

int proc_sig, i_fuz;
char *input;

int fuzzer(char *proc, int argc, char **argv);

#endif // !MY_FUZZER_PROJECT
