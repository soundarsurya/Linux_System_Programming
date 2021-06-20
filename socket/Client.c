#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); //EXIT_FAILURE 
}

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      report_and_exit("[-]Error in sending file.");
    }
    bzero(data, SIZE);
  }
}

int main(){
    char *ip = "127.0.0.1";
    int port = 8080;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "send.txt";

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
        report_and_exit("[-]Error in socket");
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        report_and_exit("[-]Error in socket");
    printf("[+]Connected to Server.\n");

    fp = fopen(filename, "r");
    if (fp == NULL)
        report_and_exit("[-]Error in reading file.");

    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}