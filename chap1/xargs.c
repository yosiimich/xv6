#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
int
main(int argc, char *argv[])
{
    char stdin[1024];
    int stdin_i = 0;
    char *param[MAXARG];
    char ch;
    int i = 0;
    for( ; i < argc - 1; i++){
        param[i] = argv[i + 1];
    }

    while(read(0, &ch, 1)){
        if(ch == '\n'){

            stdin[stdin_i] = 0;
            param[i++] = stdin;
            param[i] = 0;

            if(!fork()){
                exec(argv[1], param);
                exit(0);
            }
            wait(0);
            i = argc-1;
            stdin_i = 0;
        }
        else{
            stdin[stdin_i++]=ch;
        }
    }
    
    exit(0);
}   