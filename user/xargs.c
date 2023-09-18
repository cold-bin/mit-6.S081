//
// Created by cold-bin on 2023/9/17.
//

#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/fs.h"
#include "../kernel/param.h"
#include "user.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char *full_argv[MAXARG];
    int i;
    int len;
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    if (argc + 1 > MAXARG) {
        fprintf(2, "Usage: too many args\n");
        exit(1);
    }

    for (i = 1; i < argc; i++) {
        full_argv[i - 1] = argv[i];
    }

    full_argv[argc] = 0;

    while (1) {
        i = 0;
        while (1) {
            len = read(0, &buf[i], 1);
            if (len <= 0 || buf[i] == '\n') break;
            i++;
        }
        if (i == 0) break;
        buf[i] = 0;
        full_argv[argc - 1] = buf;
        if (fork() == 0) {
            exec(full_argv[0], full_argv);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}
