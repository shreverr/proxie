#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "config.h"

int load_config(const char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening config file '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[128], value[128];
        if (sscanf(line, "%127[^=]=%127s", key, value) == 2) {
            if (strcmp(key, "listen_address") == 0) {
                strncpy(config->listen_address, value, sizeof(config->listen_address));
            } else if (strcmp(key, "listen_port") == 0) {
                config->listen_port = atoi(value);
            } else if (strcmp(key, "target_address") == 0) {
                strncpy(config->target_address, value, sizeof(config->target_address));
            } else if (strcmp(key, "target_port") == 0) {
                config->target_port = atoi(value);
            }
        }
    }

    fclose(file);
    return 0;
}