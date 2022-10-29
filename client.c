#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8081
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0;
    int message_read; 
    struct sockaddr_in serv_addr; 
    char *hello = "Message from client: Hello"; 
    char buffer_message[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        // handle failure
        printf("\n Error: Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // To convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        // handle failure
        printf("\n Error: Invalid address or Address is not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        // handle failure
        printf("\n Error: Connection Failed \n"); 
        return -1; 
    } 

    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message has been successfully sent\n"); 
    message_read = read( sock , buffer_message, 1024); 
    printf("%s\n",buffer_message ); 
    return 0; 
} 
