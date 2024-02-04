#include <stdio.h>
#include <unistd.h>

// extern char **__environ;
extern char **environ;


int main (int argc, char **argv) {
    printf("This is the malicious print-arg-env");
    printf(" which just prints out wrong data:\n");
    printf("argv: print_access_token");
    printf("\n");

    printf("environ:\n");
    printf("ACCESS_TOKEN=123456\n");
    return 42;
}
