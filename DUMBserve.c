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

pthread_mutex_t lock;

//Message box trying to be opened by 2 users is the extra credit i think!
//enum commands{HELLO, GDBYE, CREAT, OPNBX, NXTMG, PUTMG, DELBX, CLSBX};

typedef struct messages{
  char* message;
  char* next;
}message;

volatile message mailbox[];


int main(int argc, char**argv){
  int port;
  //check args
  if(argc == 2){
    port = atoi(argv[1]);
    if(port >= 4000){ // making sure port is always greater than 4k
      perror("Port Error: port too small");
      exit(EXIT_FAILURE);
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
int commands(char command[6]){

}

void* handleConnection(void* soc){
  int sock = *((int*)soc);

  char buffer[1024] = {0};

  read(sock,buffer, 1024);

  if(strcmp("HELLO", command, 6)){

  } else if(strcmp("GDBYE", command, 6){

  } else if(strcmp("CREAT", command, 6){

  } else if(strcmp("OPNBX", command, 6){

  } else if(strcmp("NXTMG", command, 6){

  } else if(strcmp("PUTMG", command, 6){

  } else if(strcmp("DELBX", command, 6){

  } else if(strcmp("CLSBX", command, 6){
    
  } else{
    return -1;
  }

  //send(sock, test, strlen(test),0);

  return NULL;

}
//functions to handle hello command
int hello(int soc){

}

int gdbye(int soc){

}

int creat(int soc, char* name){

}

int opnbx(int soc, char* name){

}

message nxtmg(int soc, message currentmessage){

}

int putmg(int soc, int boxIndex, int size, char* message){

}

int delbx(int soc, char* name){

}

int clsbx(int soc, char* name){

}
