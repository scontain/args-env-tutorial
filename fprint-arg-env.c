#include <stdio.h>
#include <stdlib.h>
extern char **environ;

int main (int argc, char **argv) {
    char *fname = "/volume/output.txt";
    FILE *fp = fopen(fname, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", fname);
        return 1;       
    }
    fprintf(fp, "arguments:");
    for (int i = 0; i < argc; i++) {
        fprintf(fp, " - %s", argv[i]);
    }
    fprintf(fp, "\n");

    char** envp = environ;
    fprintf(fp, "environment variables:\n");
    while (*envp != NULL) {
        fprintf(fp, " - %s\n", *envp);
        envp++;
    }
    fclose(fp);
    return 0;
}
