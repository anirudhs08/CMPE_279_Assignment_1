#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>

#define PORT 8081

int dropPrivilegeToNobodyUser() {
    struct passwd* pwd;
    pid_t idOfChildProcess, pid;
    idOfChildProcess = fork();
    printf("The child Process ID is : %d\n",idOfChildProcess);
    printf("The parent UID is : %d \n",getuid());

    if(idOfChildProcess == 0) {

        // this means that the Fork is successful
        printf("\n the process of fork is successful \n");

        // set privilege as nobody user for read-write
        pwd = getpwnam("nobody");

        pid = setuid(pwd->pw_uid);
        printf("The child UID is : %d \n",getuid());
        if(pid==0){

            // set privilege as successful
            return 1;
        }

        // set privilege as unsuccessful
        return 0;
    }
}
int main(int argc, char const *argv[])
{
    int server_fileDescriptor;
    int message_read;
    int new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int address_length = sizeof(address);
    char buffer_message[1024] = {0};
    char *hello = "Message from server: Hello";

    // Show ASLR
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        // if socket fails
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Here, we attach socket to port 80
    if (setsockopt(server_fileDescriptor, SOL_SOCKET, SO_REUSEADDR ,
                                                  &opt, sizeof(opt)))
    {
        // handle failure
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcibly attach socket to port 80
    if (bind(server_fileDescriptor, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        // handle failure
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fileDescriptor, 3) < 0)
    {
        // handle failure
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fileDescriptor, (struct sockaddr *)&address,
                       (socklen_t*)&address_length))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if (dropPrivilegeToNobodyUser()) {
        
        // process the message

        message_read = read(new_socket, buffer_message, 1024);
        printf("Number of bytes read is %d for the message: %s\n", message_read, buffer_message);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message has been successfully sent\n");
    }
    return 0;
}
