#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
int main(int argc, char *argv[]){
  char message[100];
  int server,portNumber;
  socklen_t len;
  struct sockaddr_in servAdd;
  if((server=socket(AF_INET,SOCK_STREAM,0))<0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }
  servAdd.sin_family=AF_INET;
  sscanf(argv[2],"%d",&portNumber);
  servAdd.sin_port=htons((uint16_t)portNumber);
  if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
    fprintf(stderr, "inet_pton() has failed\n");
    exit(2);
  }
  if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
    fprintf(stderr, "connect() failed, exiting\n");
    exit(3);
  }
  if(read(server,message,100)<0){
    fprintf(stderr, "read() error\n");
    exit(3);
  }
  fprintf(stderr, "%s\n",message);
  exit(0);
}
