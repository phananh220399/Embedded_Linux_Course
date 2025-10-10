#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "/tmp/unix_stream.sock"
#define BUFFER_SIZE 128

int main() {
    int server_fd, client_fd;
    struct sockaddr_un serv_addr;
    char buffer[BUFFER_SIZE];


    unlink(PATH);


    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    memset(&serv_addr, 0, sizeof(serv_addr));


    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, PATH, sizeof(serv_addr.sun_path) - 1);


    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        unlink(PATH);
        exit(EXIT_FAILURE);
    }

    printf("Unix stream server listening on %s...\n", PATH);


    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        unlink(PATH);
        exit(EXIT_FAILURE);
    }


    int n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read failed");
    } else {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
    unlink(PATH); 
    return 0;
}
