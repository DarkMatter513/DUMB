#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#define NUM_CONNECTIONS 20


void* handleConnection(void* soc);

int main(int argc, char**argv){
  int port;
  //check args
  if(argc == 2){
    port = atoi(argv[1]);
  }
  else{
    printf("Port undefined\n");
    exit(EXIT_FAILURE);
  }

  //Socket Initialization
  int soc;
  if((soc = socket(AF_INET, SOCK_STREAM,0)) == 0){
    perror("Socket Failed");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if(bind(soc, (struct sockaddr *) &addr, sizeof(addr)) < 0){
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }
  if(listen(soc, NUM_CONNECTIONS) < 0){
    perror("Listen Failed");
    exit(EXIT_FAILURE);
  }

  int sock;
  int addrlen = sizeof(addr);

  //keep looking for new connections
  while(true){
    printf("\nwaiting for connection...");
    fflush(stdout);
    if((sock = accept(soc, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0){
      perror("Accept Failed");
      exit(EXIT_FAILURE);
    }
    pthread_t thread;
    pthread_create(&thread, NULL, handleConnection, (void*)&sock);
    printf("\rConnected Handling Connection...%d\n",sock);
  }
}

void* handleConnection(void* soc){
  int sock = *((int*)soc);

  char buffer[1024] = {0};

  read(sock,buffer, 1024);

  char *test;
  if(strcmp("test",buffer) == 0){
    test = "gotTest";
  }
  else{
    test = buffer;
  }

  send(sock, test, strlen(test),0);

  return NULL;

}
