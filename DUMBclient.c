#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

int con();
int hello();
void quit();
void create();
void delete();
void open();
void closebx();
void next();
void put();
void report();

int port;
char* portAsChar;
char* address;
char buffer[4096] = {0};
int sock;

int main(int argc, char** argv){
  if(argc == 3){
    //printf("argv[1] set to: %s\n", argv[1]);
    address = argv[1];
    //printf("address set to: %s\n", address);
    portAsChar = argv[2];
    port = atoi(argv[2]);
  }
  else{
    printf("Not enough args\n");
    return -1;
  }
  sock = con();

  if(sock == -1){
    return -1;
  }

  if(hello() == -1){
    return -1;
  }

  while(1){
    printf("> ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) == 4 && strncmp("quit", buffer, 4) == 0){
      quit();
    } else if(strlen(buffer) == 6 && strncmp("create", buffer, 6) == 0){
      create();
    } else if(strlen(buffer) == 6 && strncmp("delete", buffer, 6) == 0){
      delete();
    } else if(strlen(buffer) == 4 && strncmp("open", buffer, 4) == 0){
      open();
    } else if(strlen(buffer) == 5 && strncmp("close", buffer, 5) == 0){
      closebx();
    } else if(strlen(buffer) == 4 && strncmp("next", buffer, 4) == 0){
      next();
    } else if(strlen(buffer) == 3 && strncmp("put", buffer, 3) == 0){
      put();
    } else if(strlen(buffer) == 4 && strncmp("help", buffer, 4) == 0){
      printf("The following commads are valid:\nquit\ncreate\ndelete\nopen\nclose\nnext\nput\n");
    }
    else{
      printf("That is not a command, for a command list enter 'help'.\n");
      // send(sock, what, strlen(what),0);
    }
  }


}

//"ER:EXISTS","ER:NEXST","ER:OPEND","ER:EMPTY","ER:NOOPN","ER:NOTMT"
int hello(){
  char command[5000] = "HELLO";
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp(buffer,"HELLO DUMBv0 ready!") == 0){
    printf("%s\n",buffer);
  } else{
    printf("Error: incorrect reply from Server!");
    exit(1);
  }
}
void quit(){
  char command[5000] = "GDBYE";
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  printf("Goodbye!\n");
  exit(1);
}
void create(){
  printf("Okay, enter the new box's name:\n");
  memset(buffer, '\0', sizeof(buffer));
  printf("create:> ");
  fgets(buffer, 1024, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char boxname[1024];
  strcpy(boxname,buffer);
  char command[5000] = "CREAT ";
  strcat(command, buffer);
  //printf(command);
  //printf("\n");
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp("OK!",buffer) == 0 && boxname != NULL){
    printf("Success! %s Box created!\n",boxname);
  } else{
    report(boxname);
  }
}
void delete(){
  printf("Okay, delete which message box?\n");
  memset(buffer, '\0', sizeof(buffer));
  printf("delete:> ");
  fgets(buffer, 1024, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char boxname[1024];
  strcpy(boxname,buffer);
  char command[5000] = "DELBX ";
  strcat(command, buffer);
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp("OK!",buffer) == 0 && boxname != NULL){
    printf("Success! %s Box deleted!\n",boxname);
  } else{
    report(boxname);
  }

}
void open(){
  printf("Okay, open which message box?\n");
  memset(buffer, '\0', sizeof(buffer));
  printf("open:> ");
  fgets(buffer, 1024, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char boxname[1024];
  strcpy(boxname,buffer);
  char command[5000] = "OPNBX ";
  strcat(command, buffer);
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp("OK!",buffer) == 0 && boxname != NULL){
    printf("Success! %s Box opened!\n",boxname);
  } else{
    report(boxname);
  }
}
void closebx(){
  printf("Okay, close which message box?\n");
  memset(buffer, '\0', sizeof(buffer));
  printf("close:> ");
  fgets(buffer, 1024, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char boxname[1024];
  strcpy(boxname,buffer);
  char command[5000] = "CLSBX ";
  strcat(command, buffer);
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp("OK!",buffer) == 0 && boxname != NULL){
    printf("Success! %s Box closed!\n",boxname);
  } else{
    report(boxname);
  }
}
void next(){
  char command[5000] = "NXTMG";
  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strncmp("OK!",buffer,3) == 0){
    char* token = strtok(buffer, "!");
    int length = atoi(strtok(NULL, "!"));
    char* m = strtok(NULL, "!");
    printf(m);
    printf("\n");
  } else{
    report(NULL);
  }
}
void put(){
  printf("Okay, please enter a message?\n");
  memset(buffer, '\0', sizeof(buffer));
  printf("put:> ");
  fgets(buffer, 1024, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char command[5000] = "PUTMG!";
  int length = strlen(buffer);
  char _len[100];
  snprintf(_len,100,"%d!",length);
  strcat(command,_len);
  strcat(command, buffer);

  send(sock, command, strlen(command), 0);
  memset(buffer, '\0', sizeof(buffer));
  read(sock,buffer, 1024);
  if(strcmp("OK!",buffer) == 0){
    printf("Success! Message put in box!\n");
  } else{
    report(NULL);
  }
}
//"ER:EXISTS","ER:NEXST","ER:OPEND","ER:EMPTY","ER:NOOPN","ER:NOTMT"
void report(char* helper){
  if(strcmp("ER:EXISTS",buffer)== 0 && helper != NULL){
    printf("Error! %s Already Exists!\n",helper);
  } else if(strcmp("ER:NEXST",buffer)== 0 && helper != NULL){
    printf("Error! %s Does Not Exists!\n",helper);
  } else if(strcmp("ER:OPEND",buffer)== 0 && helper != NULL){
    printf("Error! %s is Opened by Another User!\n",helper);
  } else if(strcmp("ER:EMPTY",buffer)== 0){
    printf("Error! Message Box is Empty!\n");
  } else if(strcmp("ER:NOOPN",buffer)== 0){
    printf("Error! That Message Box is not Open!\n");
  } else if(strcmp("ER:NOTMT",buffer)== 0 && helper != NULL){
    printf("Error! %s is Not Empty!\n",helper);
  } else if(strcmp("ER:NOCLS",buffer)== 0 && helper != NULL){
    printf("Error! Close currently Open Box before opening %s!\n",helper);
  } else if(strcmp("ER:WHAT?",buffer)== 0){
    printf("Error! Input Malformed\n");
  } else{
    printf("UNKNOWN ERROR FIX LOL ADD ITEM TO REPORT: %s\n",buffer);
  }
}




int con(){
  int retry = 1;
  int numtimes = 0;
  while(retry==1 && numtimes != 3){
    retry = 0;
    struct addrinfo *info;
    struct addrinfo hints = {
      .ai_flags = AI_ADDRCONFIG | AI_ALL,
      .ai_family = AF_UNSPEC,
      .ai_socktype = SOCK_STREAM
    };

    if (getaddrinfo(address, portAsChar, &hints, &info) < 0){
      perror("get addr failed\n");
      numtimes++;
      retry = 1;
      continue;
    }

    int soc = socket(info->ai_family, SOCK_STREAM, 0);
    if (soc < 0){
      perror("socket failed\n");
      numtimes++;
      retry = 1;
      continue;
    };

    if (connect(soc, info->ai_addr, info->ai_addrlen) < 0){
      perror("connect failed\n");
      numtimes++;
      retry = 1;
      continue;
    }
    freeaddrinfo(info);
    return soc;
  }

  if(numtimes == 3){
    printf("Error: Could not connect to server!");
    return -1;
  }

}
