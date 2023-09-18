//
// Created by cold-bin on 2023/9/17.
//

#include "../kernel/types.h"
#include "user.h"

void primes(int p[2]);

// 素数筛 2-35
int main(int argc, char *argv[]) {
    int p1[2];
    pipe(p1);
    if (fork() == 0) {
        //子进程
        primes(p1);
        exit(0);
    } else {
        close(p1[0]);
        // 把2-35传到管道读端
        for (int j = 2; j <= 35; ++j) {
            write(p1[1], &j, 4);
        }
        close(p1[1]);
        wait(0);
    }
    exit(0);
}

// 告诉编译器当前函数没有返回。可以不在函数体末尾添加return，仅仅只需要exit()或不写即可
__attribute__((noreturn))
void primes(int p[2]) {
    int p1[2];
    close(p[1]);
    int n;
    //拿到上一个父进程筛出来的素数
    int prime;
    if (read(p[0], &prime, 4) == 0) {
        close(p[0]);
        exit(1);
    } else {
        printf("prime %d\n", prime);
        pipe(p1);

        if (fork() == 0) {
            primes(p1);
        } else {
            // 父进程关闭读端以写
            close(p1[0]);
            while (1) {
                // 从父进程那里读数并筛选送入下一个子进程
                if (read(p[0], &n, 4) == 0) break;
                // 满足条件的就可以继续筛
                if (n % prime != 0) write(p1[1], &n, 4);
            }
            close(p1[1]);
            close(p[0]);
            // 等待子进程
            wait(0);
        }
    }
    exit(0);
}