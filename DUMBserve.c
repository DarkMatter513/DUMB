#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#define NUM_CONNECTIONS 20

//Extra Credit is trying to open 2 boxes at once ADDED "ER:NOCLS" error!
//enum commands{HELLO, GDBYE, CREAT, OPNBX, NXTMG, PUTMG, DELBX, CLSBX};

typedef struct messages{
  char* message;
  int length;
  struct messages* next;
}message;

typedef struct messagebox{
  char* name;
  message* messages;
  pthread_mutex_t lock;
  struct messagebox* next;
}messagebox;

void* handleConnection(void* soc);
void printaction(int sock, char* action);
void printerror(int sock, char* action);
void printdata();
int hello(int soc);
int gdbye(int soc, struct messagebox* open);
int creat(int soc, char* name);
struct messagebox* opnbx(int soc, char* name);
int nxtmg(int soc, struct messagebox* currentbox);
int putmg(int soc, struct messagebox* currentbox, int length, char* mess);
int delbx(int soc, char* name);
struct messagebox* clsbx(int soc, char* name, struct messagebox* open);
void printtime();


messagebox* first;
char ok[] = "OK!";

int main(int argc, char**argv){
  int port;
  //check args
  if(argc == 2){
    port = atoi(argv[1]);
    //printf("\nPORT ENTERED: %d\n", port);
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

  char buffer[4096] = {0};
  char buffCopy[4096] = {0};

  messagebox* currentopenbox = NULL;

  while(1){
    memset(buffer, '\0', sizeof(buffer));
    memset(buffCopy, '\0', sizeof(buffCopy));
    read(sock,buffer, 1024);
    strcpy(buffCopy, buffer);
    //printf("FULL BUFFER:\n%s\n\n\n", buffer);
    if(strlen(buffCopy) == 5 && strncmp("HELLO", buffer, 5) == 0){
      hello(sock);
    } else if(strlen(buffCopy) == 5 && strncmp("GDBYE", buffer, 5) == 0){
      if(gdbye(sock, currentopenbox) == 1){
        return;
      }
    } else if(strlen(strtok(buffCopy," ")) == 5 && strncmp("CREAT", buffer, 5) == 0){
      char* token = strtok(buffer, " ");
      char* name = strtok(NULL, " ");
      if(strlen(name) >= 5 && strlen(name) <= 25 && isalpha(name[0])){
          creat(sock, name);
      } else{
        char what[] = "ER:WHAT?";
        send(sock, what, strlen(what),0);
      }

    } else if(strlen(strtok(buffCopy," ")) == 5 && strncmp("OPNBX", buffer, 5) == 0){
      char* token = strtok(buffer, " ");
      char* name = strtok(NULL, " ");
      if(currentopenbox != NULL){
        char err[] = "ER:NOCLS";
        printerror(sock, err);
        send(sock, err, strlen(err), 0);
      } else{
        currentopenbox = opnbx(sock, name);
      }

    } else if(strlen(buffCopy) == 5 && strncmp("NXTMG", buffer, 5) == 0){

      nxtmg(sock,currentopenbox);

    } else if(strlen(strtok(buffCopy,"!")) == 5 && strncmp("PUTMG", buffer, 5) == 0){
      char* token = strtok(buffer,"!");
      int length = atoi(strtok(NULL,"!"));
      char* newmsg = strtok(NULL,"!");
      if(length != strlen(newmsg)){
        char what[] = "ER:WHAT?";
        send(sock, what, strlen(what),0);
        continue;
      }
      putmg(sock, currentopenbox, length, newmsg);
    } else if(strlen(strtok(buffCopy," ")) == 5 && strncmp("DELBX", buffer, 5) == 0){
      char* token = strtok(buffer, " ");
      char* name = strtok(NULL, " ");
      delbx(sock, name);
    } else if(strlen(strtok(buffCopy," ")) == 5 && strncmp("CLSBX", buffer, 5) == 0){

      char* token = strtok(buffer, " ");
      char* name = strtok(NULL, " ");
      currentopenbox = clsbx(sock, name,currentopenbox);

    } else{
      char what[] = "ER:WHAT?";
      send(sock, what, strlen(what),0);
    }
    //printdata();
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

int gdbye(int soc, struct messagebox* open){
  if(open != NULL){
    clsbx(soc, open->name, open);
  }
  printaction(soc, "GDBYE");
  int s = soc;
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

  while(close(soc) != 0);
  printtime();
  printf("%s ", ipstr);
  printf("%s\n", "disconnected");
  fflush(stdout);
  return 1;
}

int creat(int soc, char* name){
  if(first == NULL){
    first = (messagebox*) malloc(sizeof(messagebox));
  }
  messagebox* current = first;
  if(current->name == NULL){
    current->name = (char*)malloc(sizeof(name));
    strncpy(current->name, name, strlen(name));
    current->messages = NULL;
    pthread_mutex_init(&(current->lock),NULL);
    current->next = NULL;
    printaction(soc,"CREAT");
    send(soc, ok, strlen(ok), 0);
    return 1;
  }
  while(current != NULL){
    if(strcmp(name, current->name) == 0){
      char err[] = "ER:EXISTS";
      printerror(soc, err);
      send(soc, err, strlen(err), 0);
      return -1;
    }
    if(current->next == NULL){
      current->next = (messagebox*) malloc(sizeof(messagebox));
      current->next->name = (char*)malloc(sizeof(name));
      strncpy(current->next->name,name, strlen(name));
      current->next->messages = NULL;
      pthread_mutex_init(&(current->lock),NULL);
      current->next->next = NULL;
      printaction(soc,"CREAT");
      send(soc, ok, strlen(ok), 0);
      return 1;
    }
    current = current->next;
  }

}

struct messagebox* opnbx(int soc, char* name){

  messagebox* current = first;
  if(current->name == NULL){
    printerror(soc, "OPNBX");
    char err[] = "ER:NEXST";
    send(soc, err,strlen(err),0);
    return NULL;
  }
  while(current != NULL){
    if(strcmp(name, current->name) == 0){
      if(pthread_mutex_trylock(&(current->lock)) == 0){
        printaction(soc, "OPNBX");
        send(soc, ok, strlen(ok), 0);
        return current;
      }
      else{
        printerror(soc, "OPNBX");
        char act[] = "ER:OPEND";
        send(soc, act,strlen(act),0);
        return NULL;
      }
    }
    if(current->next == NULL){
      printerror(soc, "OPNBX");
      char err[] = "ER:NEXST";
      send(soc, err,strlen(err),0);
      return NULL;
    }
    current = current->next;
  }

}

int nxtmg(int soc, struct messagebox* currentbox){

  if(currentbox->messages != NULL){
    message* currentmessage = currentbox->messages;
    char* data = currentmessage->message;
    int len = currentmessage->length;
    currentbox->messages = currentmessage->next;
    char buff[4096];
    memset(buff, '\0', sizeof(buff));
    strcpy(buff,"OK!");
    char _len[100];
    memset(_len, '\0', sizeof(_len));
    snprintf(_len,100,"%d!",len);
    strcat(buff,_len);
    strcat(buff,data);
    printaction(soc, "NXTMG");
    send(soc, buff, strlen(buff),0);
    free(currentmessage);
    return 1;
  } else{
    char empty[] = "ER:EMPTY";
    printerror(soc, empty);
    send(soc, empty, strlen(empty), 0);
    return -1;
  }

}

int putmg(int soc, struct messagebox* currentbox, int length, char* mess){

  if(currentbox == NULL){
    char err[] = "ER:NOOPN";
    printerror(soc, err);
    send(soc, err, strlen(err), 0);
    return -1;
  }
  message* currentmessage = currentbox->messages;
  if(currentmessage == NULL){
    message* newmsg = (message*)malloc(sizeof(message));
    newmsg->message = (char*) malloc(sizeof(mess));
    strcpy(newmsg->message,mess);
    newmsg->length = length;
    newmsg->next = NULL;
    currentbox->messages = newmsg;
    printaction(soc, "PUTMG");
    send(soc, ok, strlen(ok), 0);
    return 1;
  }
  while(currentmessage->next != NULL){
    currentmessage = currentmessage->next;
  }
  message* newmsg = (message*)malloc(sizeof(message));
  newmsg->message = (char*) malloc(sizeof(mess));
  strcpy(newmsg->message,mess);
  newmsg->length = length;
  newmsg->next = NULL;
  currentmessage->next = newmsg;
  printaction(soc, "PUTMG");
  send(soc, ok, strlen(ok), 0);
  return 1;
}

int delbx(int soc, char* name){

  messagebox* current = first;
  messagebox* prev = NULL;
  if(current->name == NULL){
    char err[] = "ER:NEXST";
    printerror(soc, err);
    send(soc, err, strlen(err), 0);
    return -1;
  }
  while(current != NULL){
    if(strcmp(name, current->name) == 0){
      if(pthread_mutex_trylock(&(current->lock)) == 0){
        if(current->messages != NULL){
          char err[] = "ER:NOTMT";
          printerror(soc, err);
          send(soc, err, strlen(err), 0);
          return -1;
        }
        if(prev == NULL){
          first = current->next;
        }else{
          prev->next = current->next;
        }
        pthread_mutex_destroy(&(current->lock));
        free(current);
        printaction(soc, "DELBX");
        send(soc, ok, strlen(ok),0);
        return 1;
      } else{
        printerror(soc, "OPNBX");
        char act[] = "ER:OPEND";
        send(soc, act,strlen(act),0);
        return -1;
      }
    }
    if(current->next == NULL){
      char err[] = "ER:NEXST";
      printerror(soc, err);
      send(soc, err, strlen(err), 0);
      return -1;
    }
    prev = current;
    current = current->next;
  }

}

struct messagebox* clsbx(int soc, char* name, struct messagebox* open){

  if(open != NULL && strcmp(name, open->name) != 0){
    char err[] = "ER:NOOPN";
    printerror(soc, err);
    send(soc, err, strlen(err), 0);
    return open;

  } else if(pthread_mutex_unlock(&(open->lock)) == 0){
    printaction(soc, "CLSBX");
    send(soc, ok, strlen(ok),0);
    return NULL;
  }

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
  fflush(stdout);
}

char* perrortime(){
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
  char* str =(char*)malloc(1024);
  snprintf(str,1024,"%d %d %s ", mtime, day, mon);
  return str;
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
  fflush(stdout);
}

char* perroraddress(int s){
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
  char* str =(char*)malloc(1024);
  snprintf(str,1024,"%s ", ipstr);
  return str;
}

void printaction(int sock, char* action){
  printtime();
  printaddress(sock);
  printf("%s\n", action);
  fflush(stdout);
}

void printerror(int sock, char* action){
  char* t = perrortime();
  char* addr = perroraddress(sock);
  char str[1024];
  snprintf(str, 1024, "%s", action);
  strcat(t,addr);
  strcat(t,str);
  fprintf( stderr, "%s\n",t);
  //perror(t);
  free(t);
  free(addr);
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
  fflush(stdout);
}
