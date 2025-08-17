#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
set_num(){
    int state;
    int p[2];
    pipe(p);
    if(!fork()){
        close(p[0]);
        for(int i =2; i<270;i++){
            write(p[1],&i,4);
        }
        close(p[1]);
        exit(0);
    }
    close(p[1]);
    wait(&state);
    return p[0];
}

int print_prime(int lne, int prime){
    int n;
    int p[2];
    int state;
    pipe(p);
    //printf("prime: %d\n",prime);
    if(!fork()){
        close(p[0]);
        while(read(lne, &n, 4)){
            //printf("n: %d\n",n);
            if(n%prime!=0){
                write(p[1],&n,4);
            }
        }
        close(p[1]);
        close(lne);
        exit(0);
    }
    close(p[1]);
    wait(&state);
    close(lne);
    return p[0];

}

int
main(int argc, char *argv[]){
    int prime;
    int lne = set_num();
    while(read(lne,&prime,4)){
        printf("prime %d\n",prime);
        lne = print_prime(lne, prime);
    }
    exit(0);
}