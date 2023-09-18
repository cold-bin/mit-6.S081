//
// Created by cold-bin on 2023/9/17.
//

#include "user.h"
#include "../kernel/types.h"
#include "../kernel/fs.h"
#include "../kernel/stat.h"
#include "../kernel/fcntl.h"

void find(char *dir, char *fn);

char *fmtname(char *path);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Useage: find [directory] [filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}


char *fmtname(char *path) {
    char *p;
    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

void find(char *dir, char *fn) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // 拿当前目录子项
    if ((fd = open(dir, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    // 再拿元信息
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DEVICE:
            break;
        case T_FILE:
            if (strcmp(fmtname(dir), fn) == 0) {
                fprintf(2,"%s\n", dir);
            }
            break;
        case T_DIR:
            if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf) {
                fprintf(2,"find: path too long\n");
                break;
            }
            strcpy(buf, dir);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || strcmp(de.name,".")==0|| strcmp(de.name,"..")==0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, fn);
            }
            break;
    }
    close(fd);
}