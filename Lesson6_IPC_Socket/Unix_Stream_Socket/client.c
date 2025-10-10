#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "/tmp/unix_stream.sock"

int main() {
    int sockfd;
    struct sockaddr_un serv_addr;
    const char *message = "Hello from Unix stream client";


    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

 
    memset(&serv_addr, 0, sizeof(serv_addr));


    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, PATH, sizeof(serv_addr.sun_path) - 1);


    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    ssize_t sent = write(sockfd, message, strlen(message));
    if (sent < 0) {
        perror("write failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Message sent: %s\n", message);
    close(sockfd);
    return 0;
}
