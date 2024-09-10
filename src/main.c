#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "proxy.h"

int main() {
    Config config;
    if (load_config("/var/proxie/proxie.conf", &config) != 0) {
        fprintf(stderr, "Failed to load configuration\n");
        return 1;
    }

    run_proxy(&config);

    return 0;
}
