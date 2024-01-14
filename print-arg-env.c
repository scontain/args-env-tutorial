#include <stdio.h>
#include <unistd.h>

// extern char **__environ;
extern char **environ;


int main (int argc, char **argv) {
    printf("argv:");
    for (int i = 0; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    char** envp = environ;
    printf("environ:\n");
    while (*envp != NULL) {
        printf("%s\n", *envp);
        envp++;
    }
    return 42;
}
