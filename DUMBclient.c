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

  char buffer[1024] = {0};

  while(1){
    scanf("%s",buffer);

    if(strncmp("quit", buffer, 5) == 0){

    } else if(strncmp("create", buffer, 7) == 0){

      printf("Okay, enter the new box's name:\n");
      scanf("%s",buffer);
      char command[] = "CREAT ";
      strcat(command, buffer);
      printf(command);
      printf("\n");
      send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      read(sock,buffer, 1024);
      printf("%s\n",buffer);
    } else if(strncmp("delete", buffer, 7) == 0){

    } else if(strncmp("open", buffer, 5) == 0){

    } else if(strncmp("close", buffer, 6) == 0){

    } else if(strncmp("next", buffer, 5) == 0){

    } else if(strncmp("put", buffer, 4) == 0){

    } else if(strncmp("HELLO", buffer, 6) == 0){

    } else{
      char what[] = "What?";
      send(sock, what, strlen(what),0);
      return -1;
    }

  }
  //send(sock, test, strlen(test),0);



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
