#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv) {
    char *env_variable = "ACCESS_TOKEN";
    char *access_token = getenv(env_variable);
    if (access_token == NULL) {
        fprintf(stderr, "Environment variable '%s' not defined! Error Exit!\n", env_variable);
        exit(1);
    } else {
        printf("%s", access_token);
    }
    return 0;
}

