#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char listen_address[256];
    int listen_port;
    char target_address[256];
    int target_port;
} Config;

int load_config(const char *filename, Config *config);

#endif