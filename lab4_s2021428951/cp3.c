#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char*,char*);
int main(int ac, char*av[]){
    int in_fd,out_fd,n_chars;
    char buf[BUFFERSIZE];
    if(ac!=3){
        fprintf(stderr,"usage:%s source destination\n",*av);
        exit(1);
    }
    if(strcmp(av[1],av[2])==0){
        oops("You try to copy same File av[1] and av[2]"," ");
    }
    struct stat info;
    if(stat(av[2],&info)!=-1 && S_ISDIR(info.st_mode)){
        av[2]= strcat(av[2],"/");
        av[2]= strcat(av[2],av[1]);
    }
    if((in_fd=open(av[1],O_RDONLY))==-1){
        oops("Cannot open ",av[1]);
    }
    if((out_fd=creat(av[2],COPYMODE))==-1){
        oops("Cannot create ",av[2]);
    }
    while((n_chars=read(in_fd,buf,BUFFERSIZE))>0){
        if(write(out_fd,buf,n_chars)!=n_chars){
            oops("Write error to ",av[2]);
        }
    }
    if(n_chars==-1){
        oops("Read error from ",av[1]);
    }
    if(close(in_fd)==-1||close(out_fd)==-1){
        oops("Error closing files ","");
    }
    return 0;
}
void oops(char*s1,char*s2){
    fprintf(stderr,"Error: %s",s1);
    printf("%s\n", s2);
    exit(1);
}