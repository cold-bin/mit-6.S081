//
// Created by cold-bin on 2023/9/16.
//

#include "../kernel/types.h"
#include "user.h"

// 实现可以参考 https://xiaolincoding.com/os/4_process/process_commu.html#%E7%AE%A1%E9%81%93

int main(){
    // 先把管道创建好
    int p1[2];int p2[2];
    pipe(p1);pipe(p2);
    char buf[] = {'A'};
    int n = sizeof(buf);
    // fork两个管道，关闭必要的读写端
    if (fork()==0){
        // 子进程
        close(p1[0]);
        close(p2[1]);

        read(p2[1],buf,n);
        printf("%d: received ping\n",getpid());
        write(p1[0],buf,n);
        exit(0);
    } else{
        // 父进程
        close(p1[1]);
        close(p2[0]);

        write(p1[1],buf,n);
        read(p2[1],buf,n);
        printf("%d: received pong\n",getpid());
        close(p1[0]);
        close(p2[1]);
        wait((int*)0);
        exit(0);
    }
}