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

void child(int sd){
    int fd1,child_ret,n,a;
    FILE *fp=NULL;
    pid_t pid;
    FILE *stream;
    char *c;
    char message[100];
    //infinite loop reads command from client's socket
    //excute by fork/exec mechanism
    //then send to client side
    while (1) {
        memset(message,0,100);
        if((a=read(sd,message,100))<=0){
            close(sd);
            fprintf(stderr, "Bye, client dead, wait for a new client\n");
            //kill(pid,SIGTERM);
            exit(0);
        }
        pid=fork();
        if(!pid){
            char *delim = " \n\0\t";
            char *a[20];
            a[0]=strtok(message,delim);
            int i =1;
            while((a[i] = strtok(NULL, delim))){
                i++;
            }
            char str[10]="/bin/";
            strcat (str,a[0]);
            if((stream = freopen("1.txt", "w", stdout)) == NULL){
                exit(-1);
            }
            int n=execv(str,a);
            fclose(stdout);
        }else{
            waitpid(pid,&child_ret,0);
            int i=0;
            fd1 = open ("1.txt",O_RDONLY);
            int filesize = lseek(fd1,0,SEEK_END);
            //fprintf(stderr, "%d\n",filesize );
            close(fd1);
            fp=fopen("1.txt","r");
            c=(char *)malloc(filesize);
            while(fscanf(fp,"%c",c+i)!=EOF){
                i++;
            }
            remove("./1.txt");
            fclose(fp);
            fprintf(stderr, "success return\n");
            write(sd,c,strlen(c)+1);
        }
    }
}

int main(int argc,char *argv[]){
    int sd,client,portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    if(argc!=2){
        fprintf(stderr, "Call model:%s<Port#>\n",argv[0]);
        exit(0);
    }
    //create socket
    if((sd=socket(AF_INET,SOCK_STREAM,0))<0){
        fprintf(stderr,"Could not create socket\n");
        exit(1);
    }
    //create server adderss
    servAdd.sin_family=AF_INET;
    servAdd.sin_addr.s_addr=htonl(INADDR_ANY);
    sscanf(argv[1],"%d",&portNumber);
    servAdd.sin_port=htons((uint16_t)portNumber);
    //bind IP and Port
    bind(sd,(struct sockaddr *)&servAdd,sizeof(servAdd));
    //Server start running and wait for connections
    listen(sd,5);
    printf("Waiting for the client...\n");
    while(1){
        //server gets a client
        client=accept(sd,(struct sockaddr *)NULL,NULL);
        printf("Got a client,wait for get a command\n");
        //forks and let the child process to take care of the client
        if(!fork())
            child(client);
        close(client);
    }
}
