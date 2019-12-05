#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#define NUM_CONNECTIONS 20


void* handleConnection(void* soc);
void printaction(int sock, char* action);
void printerror(int sock, char* action);
void printdata();
pthread_mutex_t lock;

//Message box trying to be opened by 2 users is the extra credit i think!
//enum commands{HELLO, GDBYE, CREAT, OPNBX, NXTMG, PUTMG, DELBX, CLSBX};

typedef struct messages{
  char* message;
  struct messages* next;
}message;

typedef struct messagebox{
  char* name;
  message* messages;
  struct messagebox* next;
}messagebox;


messagebox* first;

int main(int argc, char**argv){
  int port;
  //check args
  if(argc == 2){
    port = atoi(argv[1]);
    printf("\nPORT ENTERED: %d\n", port);
    if(port <= 4000){ // making sure port is always greater than 4k
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

  first = (messagebox*) malloc(sizeof(messagebox));

  int sock;
  int addrlen = sizeof(addr);

  //keep looking for new connections
  while(true){
    //printf("\nwaiting for connection...");
    fflush(stdout);
    if((sock = accept(soc, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0){
      perror("Accept Failed");
      exit(EXIT_FAILURE);
    }
    printaction(sock,(char*)&"connected");
    pthread_t thread;
    pthread_create(&thread, NULL, handleConnection, (void*)&sock);
    //printf("\rConnected Handling Connection...%d\n",sock);
  }
}


void* handleConnection(void* soc){
  int sock = *((int*)soc);

  char buffer[1024] = {0};

  read(sock,buffer, 1024);

  if(strncmp("HELLO", buffer, 5) == 0){
    hello(sock);
  } else if(strncmp("GDBYE", buffer, 5) == 0){
    gdbye(sock);
  } else if(strncmp("CREAT", buffer, 5) == 0){
    char* token = strtok(buffer, " ");
    char* name = strtok(NULL, " ");
    creat(sock, name);
    printdata();

  } else if(strncmp("OPNBX", buffer, 5) == 0){

  } else if(strncmp("NXTMG", buffer, 5) == 0){

  } else if(strncmp("PUTMG", buffer, 5) == 0){

  } else if(strncmp("DELBX", buffer, 5) == 0){

  } else if(strncmp("CLSBX", buffer, 5) == 0){

  } else{
    char what[] = "What?";
    send(sock, what, strlen(what),0);
    return NULL;
  }

  //send(sock, test, strlen(test),0);

  return NULL;

}
//functions to handle hello command
int hello(int soc){
  char hello[] = "HELLO DUMBv0 ready!";
  printaction(soc, "HELLO");
  send(soc, hello, strlen(hello),0);
  return 1;
}

int gdbye(int soc){
  printaction(soc, "GDBYE");
  while(shutdown(soc, 2) != 0);
  printaction(soc, "disconnected");
  return 1;
}

int creat(int soc, char* name){
  send(soc, "toend", strlen("toend"), 0);
  messagebox* current = first;
  while(current != NULL){
    if(current->name == NULL){
      current->name = name;
      char ok[] = "OK!";
      printaction(soc,"CREAT");
      send(soc, ok, strlen(ok), 0);
      return 1;
    }
    if(strcmp(name, current->name) == 0){
      char exists[] = "ER:EXISTS";
      printerror(soc, exists);
      send(soc, exists, strlen(exists), 0);
      return -1;
    }
    if(current->next == NULL){
      current->next = (messagebox*) malloc(sizeof(messagebox));
      current->next->name = name;
      char ok[] = "OK!";
      printaction(soc,"CREAT");
      send(soc, ok, strlen(ok), 0);
      return 1;
    }
    current = current->next;
  }

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

void printtime(){
  time_t timeinsec = time(NULL);
  struct tm t;
  localtime_r(&timeinsec, &t);
  int day = t.tm_mday;
  char *mon;
  switch(t.tm_mon){
    case 0: mon = "Jan";
    case 1: mon = "Feb";
    case 2: mon = "Mar";
    case 3: mon = "Apr";
    case 4: mon = "May";
    case 5: mon = "Jun";
    case 6: mon = "Jul";
    case 7: mon = "Aug";
    case 8: mon = "Sept";
    case 9: mon = "Oct";
    case 10: mon = "Nov";
    case 11: mon = "Dec";
  }
  int mtime = (t.tm_hour*100) + (t.tm_min);

  printf("%d %d %s ", mtime, day, mon);
}

void perrortime(){
  time_t timeinsec = time(NULL);
  struct tm t;
  localtime_r(&timeinsec, &t);
  int day = t.tm_mday;
  char* mon;
  switch(t.tm_mon){
    case 0: mon = "Jan";
    case 1: mon = "Feb";
    case 2: mon = "Mar";
    case 3: mon = "Apr";
    case 4: mon = "May";
    case 5: mon = "Jun";
    case 6: mon = "Jul";
    case 7: mon = "Aug";
    case 8: mon = "Sept";
    case 9: mon = "Oct";
    case 10: mon = "Nov";
    case 11: mon = "Dec";
  }
  int mtime = (t.tm_hour*100) + (t.tm_min);
  char str[1024];
  snprintf(str,1024,"%d %d %s ", mtime, day, mon);
  perror(str);
}

void printaddress(int s){

  socklen_t len;
  struct sockaddr_storage addr;
  char ipstr[INET6_ADDRSTRLEN];
  int port;

  len = sizeof addr;
  getpeername(s, (struct sockaddr*)&addr, &len);

  // deal with both IPv4 and IPv6:
  if (addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  } else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
  }

  printf("%s ", ipstr);
}

void perroraddress(int s){
  socklen_t len;
  struct sockaddr_storage addr;
  char ipstr[INET6_ADDRSTRLEN];
  int port;

  len = sizeof addr;
  getpeername(s, (struct sockaddr*)&addr, &len);

  // deal with both IPv4 and IPv6:
  if (addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  } else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
  }
  char str[1024];
  snprintf(str,1024,"%s ", ipstr);
  perror(str);
}

void printaction(int sock, char* action){
  printtime();
  printaddress(sock);
  printf("%s\n", action);
}

void printerror(int sock, char* action){
  perrortime();
  perroraddress(sock);
  char str[1024];
  snprintf(str, 1024, "%s\n", action);
  perror(str);
}
void printdata(){
  printf("ALL DATA STORED:\n\n");
  messagebox* currentbox = first;
  while(currentbox != NULL){
    printf("%s:\n", currentbox->name);
    message* currentmessage = currentbox->messages;
    while(currentmessage != NULL){
      printf("\t\t%s\n", currentmessage->message);
      currentmessage = currentmessage->next;
    }
    currentbox = currentbox->next;
  }
}
