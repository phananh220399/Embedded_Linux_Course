#include <sys/socket.h>  
#include <sys/un.h>      
#include <unistd.h>      
#include <string.h>      
#include <stdio.h>       
#include <stdlib.h> 

#define SERVER_PATH "/tmp/mysocket"
#define CLIENT_PATH "/tmp/mysocket_client"
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    const char *message = "Hi server";


    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    unlink(CLIENT_PATH);


    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);


    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }


    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);


    int sent = sendto(client_fd, message, strlen(message), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        perror("sendto failed");
        close(client_fd);
        unlink(CLIENT_PATH);
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s\n", message);

    socklen_t len = sizeof(server_addr);
    ssize_t n = recvfrom(client_fd, buffer, sizeof(buffer),0,(struct sockaddr *)&server_addr, &len);
    if (n < 0) {
        perror("recvfrom failed");
        close(client_fd);
        unlink(CLIENT_PATH);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Server replied: %s\n", buffer);


    close(client_fd);
    unlink(CLIENT_PATH);
    return 0;
}
