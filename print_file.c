#include <stdio.h>
#include <stdlib.h>

int main() {
    char *fname = "/volume/output.txt";
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", fname);
        return 1;       
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, fp);
    fclose(fp);

    string[fsize] = 0;
    printf("File content:\n%s", string);
}
