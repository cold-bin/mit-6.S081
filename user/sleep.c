//
// Created by cold-bin on 2023/9/16.
//

#include "../kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(2,"Usage: sleep seconds\n");
        exit(1);
    }

    int due = atoi(argv[1]);
    if (due){
        sleep(due);
    }

    exit(0);
}