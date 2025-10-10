#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 4000
#define BUFFER_SIZE 128

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(server_fd, 5);

    printf("TCP server listening on port %d...\n", PORT);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);

    close(client_fd);
    close(server_fd);
    return 0;
}