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

  char command[] = "HELLO";
  printf("Sending\n"); send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  printf("reading\n"); read(sock,buffer, 1024);
  if(strcmp(buffer,"HELLO DUMBv0 ready!") == 0){
    printf("%s\n",buffer);
  } else{
    //Report error and disconnect!
  }

  while(1){

    printf("waiting for input\n"); scanf("%s",buffer);
    if(strncmp("quit", buffer, 5) == 0){

      char command[] = "GDBYE";
      printf("sending\n");printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("create", buffer, 7) == 0){

      printf("Okay, enter the new box's name:\n");
      memset(buffer, '\0', sizeof(buffer));
      printf("waiting for input");scanf("%s",buffer);
      char command[] = "CREAT ";
      strcat(command, buffer);
      //printf(command);
      //printf("\n");
      printf("sending\n");printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("delete", buffer, 7) == 0){

      printf("Okay, delete which message box?\n");
      memset(buffer, '\0', sizeof(buffer));
      printf("waiting for input\n"); scanf("%s",buffer);
      char command[] = "DELBX ";
      strcat(command, buffer);
      printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("open", buffer, 5) == 0){

      printf("Okay, open which message box?\n");
      memset(buffer, '\0', sizeof(buffer));
      printf("waiting for input\n"); scanf("%s",buffer);
      char command[] = "OPNBX ";
      strcat(command, buffer);
      printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("close", buffer, 6) == 0){

      printf("Okay, close which message box?\n");
      memset(buffer, '\0', sizeof(buffer));
      printf("waiting for input\n"); scanf("%s",buffer);
      char command[] = "CLSBX ";
      strcat(command, buffer);
      printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("next", buffer, 5) == 0){

      char command[] = "NXTMG";
      printf("Sending\n"); send(sock, command, strlen(command), 0);
      memset(buffer, '\0', sizeof(buffer));
      printf("reading\n"); read(sock,buffer, 1024);
      printf("%s\n",buffer);

    } else if(strncmp("put", buffer, 4) == 0){
      
    } else if(strncmp("help", buffer, 4) == 0){
      printf("put help commands herelol");
    }
    else{
      printf("That is not a command, for a command list enter 'help'.");
      //printf("Sending\n"); send(sock, what, strlen(what),0);
      return -1;
    }

  }
  //printf("Sending\n"); send(sock, test, strlen(test),0);



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
