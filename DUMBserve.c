#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#define NUM_CONNECTIONS 20

void commands();
void* handleConnection(void* soc);

int main(int argc, char**argv){
  int port;
  char help[] = {"help"};
  if (argv[1] = help){ // helpful commands
    printf("quit (which causes: E.1 GDBYE)\n");
    printf("create (which causes: E.2 CREAT)\n");
    printf("delete (which causes: E.6 DELBX)\n");
    printf("open (which causes: E.3 OPNBX)\n");
    printf("close	(which causes: E.7 CLSBX)\n");
    printf("next (which causes: E.4 NXTMG)\n");
    printf("put	(which causes: E.5 PUTMG)\n");
  }
  //check args
  if(argc == 2){

    if(port >= 4000){ // making sure port is always greater than 4k
    port = atoi(argv[1]);
  }
}
  else{
    printf("Port undefined or smaller than 4000\n");
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
void commands(){
  char hello[] = "HELLO";
  char gdbye[] = "GDBYE";
  char creat[] = "CREAT";
  char opnbx[] = "OPNBX";
  char nxtmg[] = "NXTMG";
  char putmg[] = "PUTMG";
  char  delbx[] = "DELBX";
  char clsbx[] = "CLSBX";
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
