#include <sys/types.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    pid = fork();
    if (pid <0){
        printf("Erro ao criar processo\n");
        return 1;
    }else if( pid ==0){
        printf("Esta é a execução do filho\n");
        for(;;);
    }else{
        printf("O pai está esperando o filho\n");
        for(;;);
        printf("Processo filho finalizado\n");
    }
    return 0;
}