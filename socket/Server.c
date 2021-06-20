#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SIZE 1024

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); //Exit failure
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main (){
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    //initiate socket for communication
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
        report_and_exit("[-]Error in socket");
    
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    //binding sockaddr is a generic descriptor for any kind of socket operation, 
    //whereas sockaddr_in is a struct specific to IP-based communication
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        report_and_exit("[-]Error in bind");

    printf("[+]Binding successfull.\n");

    if(listen(sockfd, 10)<0)
        report_and_exit("[-]Error in listening");
        
    printf("[+]Listening to port %d....\n",port);

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    write_file(new_sock);
    printf("[+]Data written in the file successfully.\n");

    return 0;
}