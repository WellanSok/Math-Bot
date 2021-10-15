#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

//struct :TODO
/*struct sockaddr {
  sa_family_t sin_family;
  in_port_t sin_port;
  struct in_addr sin_addr;

};*/
int open_clientfd(char *hostname, int port) {
  int clientfd;
  struct sockaddr_in serveraddr;
  printf("Client is creating a socket.\n");
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
  
  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  //inet_pton(AF_INET,hostname,serveraddr.sin_addr.s_addr);
  serveraddr.sin_addr.s_addr = inet_addr(hostname);
  serveraddr.sin_port = htons(port);
  printf("Echo Client is trying to connect to %s (%s:%d).\n", hostname,inet_ntoa(serveraddr.sin_addr), port);
  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
    printf("some");
    return -1;
  }
  printf("Echo Client connected.\n");
  return clientfd;
}


int count(char some[]){
  int returning = 0;
  for(int i=0;i<strlen(some);i++){
    if(some[i]==' ') { returning++; }
  }
  return returning;
}
//Open TCP connection :TODO 
int main(int argc, char **argv){
  int clientfd;
  int port;
  char *host;
  char *netid;
  char buf[100];
  char temp[40];
  char * first;
  int f;
  int s;
  char *second;
  char *op;
  int result = 0;
  if(argc !=4) { exit(0);} //could be 3?
  host = argv[3];
  port = atoi(argv[2]);
  netid = argv[1];
  
  printf("Host: %s,Port: %d,netid: %s",host,port,netid);
  
  //open client 
  clientfd = open_clientfd(host, port); 
  
  snprintf(buf,sizeof(buf),"cs230 HELLO %s\n",netid); //this writes to buffer with my netid to send it
  
  ssize_t ns = send(clientfd, buf, strlen(buf), 0);
  //first math problem 
  ssize_t nr = recv(clientfd, buf, 100, 0);
  buf[nr + 1] = '\0';
  printf("someting %s",buf);
  strncpy(temp,&buf[13],20);
  //ssize_t nr = recv(clientfd, buf, 100, 0);
  //strncpy(temp,&buf[13],20);
  int spaces = 1;
  while(spaces!=0) {
    //do first
    first = strtok(temp, " ");
    op = strtok(NULL, " ");
    second = strtok(NULL, " ");
    f= atoi(first);
    s= atoi(second);
    if(*op=='+'){ result = f+s;}
    else if(*op=='-') { result = f-s;}
    else if(*op=='*') { result = f*s;} 
    else if(*op=='/') { result = f/s;}
    buf[0] = 0;
    snprintf(buf,sizeof(buf),"cs230 %d\n",result);
    ns = send(clientfd, buf, strlen(buf), 0);
    nr = recv(clientfd, buf, 100, 0);
    buf[nr + 1] = '\0';
    temp[0] = 0;
    strncpy(temp,&buf[13],20);
    spaces = count(temp);
  }
  printf("%s",buf);
  //printf("size :%s",temp);
  //do stuff

  //close
  close(clientfd);
  return 0;
}

