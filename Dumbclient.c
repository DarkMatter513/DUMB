#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

int con();
int closeCon();

int port;
char* address;

int main(int argc, char** argv){
  if(argc == 3){
    printf("argv[1] set to: %s\n", argv[1]);
    address = argv[1];
    printf("address set to: %s\n", address);
    port = atoi(argv[2]);
  }
  else{
    printf("Not enough args\n");
    return -1;
  }
  int sock = con();
  char test[] = "test";
  send(sock, test, strlen(test),0);

  char buffer[1024] = {0};
  read(sock,buffer, 1024);
  printf("%s\n",buffer);

}

int con(){

  int soc;
  if((soc = socket(AF_INET, SOCK_STREAM,0)) < 0){
    printf("Socket Failed\n");
    return -1;
  }

  struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  struct hostent *h = gethostbyname(address);
  serv_addr.sin_addr = *(struct in_addr *)h->h_addr;

  if(connect(soc,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    printf("Connection Failed \n");
    return -1;
  }
  return soc;

}
