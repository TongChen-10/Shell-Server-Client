//Tong Chen 104326983 section1
//Tian Zhou 104528022 section2
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/signal.h>

int main(int argc, char *argv[]) {
    char message[255];
    char message2[1024];
    int server,portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    if(argc!=3){
      printf("Call model:%s<IP><Port#>\n",argv[0]);
      exit(0);
    }
    //create socket
    if((server= socket(AF_INET,SOCK_STREAM,0))<0){
      fprintf(stderr, "cannot create socket\n");
      exit(1);
    }
    //set server address
    servAdd.sin_family=AF_INET;
    sscanf(argv[2],"%d",&portNumber);
    servAdd.sin_port=htons((uint16_t)portNumber);
    if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
      fprintf(stderr, "inet_pton() has failed\n");
      exit(2);
    }
    //connect to the server
    if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
      fprintf(stderr, "connect() failed, exiting\n");
      exit(3);
    }
    while(1){
        fprintf(stderr, "enter a command for the server or the CTR-D sign to quit \n");
        if(fgets(message,254,stdin)==NULL){
            close(server);
            exit(0);
        }
        write(server,message,strlen(message)+1);
        if(read(server,message2,1024)<0){
            fprintf(stderr, "read() error\n");
            exit(3);
        }
        fprintf(stderr, "Server's message:\n%s\n",message2);
    }
}
