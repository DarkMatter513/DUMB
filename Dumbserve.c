#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int sockInit();

int main(int argc, int**argv){

  int socket = sockInit();

  char buffer[1024] = {0};
   

}

int sockInit(){
  int soc;
  if(soc = socket(AF_INET, SOCK_STREAM,0) == 0){
    perror("Socket Failed");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(3217);

  if(bind(soc, (struct sockaddr *) &addr, sizeof(addr)) < 0){
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }
  if(listen(soc, 2) < 0){
    perror("Listen Failed");
    exit(EXIT_FAILURE);
  }

  int socket;
  int addrlen = sizeof(addr);

  if((socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0){
    perror("Accept Failed");
    exit(EXIT_FAILURE);
  }

  return socket;
}
