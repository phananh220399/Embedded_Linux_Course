#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PATH "/tmp/mysocket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(client_addr);

    unlink(SERVER_PATH);
    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s...\n", SERVER_PATH);

    int n = recvfrom(server_fd, buffer, sizeof(buffer), 0,(struct sockaddr *)&client_addr, &len);
    if (n < 0) {
        perror("recvfrom failed");
        close(server_fd);
        unlink(SERVER_PATH);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Received from client (%s): %s\n", client_addr.sun_path, buffer);

    // Gửi phản hồi lại client
    const char *reply = "Hello back from server!";
    sendto(server_fd, reply, strlen(reply), 0,
           (struct sockaddr *)&client_addr, len);

    close(server_fd);
    unlink(SERVER_PATH);
    return 0;
}
