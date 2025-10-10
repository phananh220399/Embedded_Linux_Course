#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>

#define MAX_CONNECTION 10
#define MAX_MESSAGE 100
#define BUFFER_SIZE 128

typedef struct {
    int socket_fd;
    char ip[INET_ADDRSTRLEN];
    int peer_port;   
    pthread_t thread;
    int active;
} Connection;

Connection connections[MAX_CONNECTION];
int connection_count = 0;
int listen_port;
pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_ready = 0;



void *server_thread(void *arg);
void *connection_handler(void *arg);
void handle_command(char *command);
int check_ip(const char *ip);
void cleanup_connection(int index);
void shift_left(int index);
char *get_local_ip();
void display_message(const char *ip, int port, const char *msg);
int find_index_by_fd(int fd);
void notify_exit();
void chat_app(); 



void chat_app() {
    printf("************************************************************\n");
    printf("******************** Chat Application **********************\n");
    printf("************************************************************\n");
    printf("Use the commands below:\n");
    printf("1. help                       : display user interface options\n");
    printf("2. myip                       : display IP address of this app\n");
    printf("3. myport                     : display listening port of this app\n");
    printf("4. connect <destination> <port>: connect to another peer\n");
    printf("5. list                       : list all the connections of this app\n");
    printf("6. terminate <connection id>   : terminate a connection\n");
    printf("7. send <connection id> <msg>  : send a message to a connection\n");
    printf("8. exit                       : close all connections & terminate this app\n");
    printf("************************************************************\n\n");
}



void *server_thread(void *arg){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd<0){ perror("socket"); exit(1); }

    int opt=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(listen_port);

    if(bind(server_fd,(struct sockaddr*)&addr,sizeof(addr))<0){ perror("bind"); exit(1);}
    if(listen(server_fd,5)<0){ perror("listen"); exit(1); }

    pthread_mutex_lock(&print_mutex);
    printf("Application is listening on port: %d\n\n", listen_port);
    pthread_mutex_unlock(&print_mutex);
    server_ready = 1;

    while(1){
        struct sockaddr_in cli;
        socklen_t len=sizeof(cli);
        int cfd = accept(server_fd,(struct sockaddr*)&cli,&len);
        if(cfd<0) continue;

        pthread_mutex_lock(&connection_mutex);
        if(connection_count>=MAX_CONNECTION){
            close(cfd);
            pthread_mutex_unlock(&connection_mutex);
            continue;
        }
        Connection *conn = &connections[connection_count++];
        conn->socket_fd=cfd;
        inet_ntop(AF_INET,&cli.sin_addr,conn->ip,INET_ADDRSTRLEN);
        conn->peer_port = ntohs(cli.sin_port);
        conn->active=1;
        pthread_mutex_unlock(&connection_mutex);

        pthread_mutex_lock(&print_mutex);
        printf("Accepted a new connection from address: %s, set up at port %d.\n",
               conn->ip, conn->peer_port);
        pthread_mutex_unlock(&print_mutex);

        pthread_create(&conn->thread,NULL,connection_handler,conn);
    }
    return NULL;
}



void *connection_handler(void *arg){
    Connection *conn = (Connection*)arg;
    char buf[BUFFER_SIZE];

    while(1){
        int r = recv(conn->socket_fd,buf,BUFFER_SIZE,0);
        if(r<=0){
            int idx = find_index_by_fd(conn->socket_fd);
            if(idx!=-1){
                pthread_mutex_lock(&print_mutex);
                printf("\nThe Peer at Port %d has disconnected.\n", conn->peer_port);
                pthread_mutex_unlock(&print_mutex);
                pthread_mutex_lock(&connection_mutex);
                cleanup_connection(idx);
                pthread_mutex_unlock(&connection_mutex);
            }
            break;
        }
        buf[r]='\0';
        if(strncmp(buf,"DISCONNECT_SIGNAL",17)==0){
            pthread_mutex_lock(&print_mutex);
            printf("\nThe Peer at Port %d has disconnected.\n", conn->peer_port);
            pthread_mutex_unlock(&print_mutex);
            int idx=find_index_by_fd(conn->socket_fd);
            pthread_mutex_lock(&connection_mutex);
            if(idx!=-1) cleanup_connection(idx);
            pthread_mutex_unlock(&connection_mutex);
            break;
        }
        display_message(conn->ip,conn->peer_port,buf);
    }
    return NULL;
}


void handle_command(char *cmd){
    char *tok=strtok(cmd," ");
    if(!tok) return;

    if(strcmp(tok,"help")==0){
        printf("help\nmyip\nmyport\nconnect <ip> <port>\nlist\nterminate <id>\nsend <id> <msg>\nexit\n");
    }
    else if(strcmp(tok,"myip")==0){
        char *ip=get_local_ip();
        if(ip){ printf("IP Address of this app: %s\n",ip); free(ip);} else printf("Cannot get IP.\n");
    }
    else if(strcmp(tok,"myport")==0){
        printf("Listening Port in this app: %d\n",listen_port);
    }
    else if(strcmp(tok,"connect")==0){
        char *ip=strtok(NULL," ");
        char *ps=strtok(NULL," ");
        if(!ip||!ps){ printf("Usage: connect <ip> <port>\n"); return; }
        int port=atoi(ps);

        if(!check_ip(ip)){ printf("Invalid IP.\n"); return; }
        char *lip=get_local_ip();
        if(lip && strcmp(ip,lip)==0 && port==listen_port){ printf("Cannot connect to self.\n"); free(lip); return;}
        free(lip);

        int fd=socket(AF_INET,SOCK_STREAM,0);
        if(fd<0){ perror("socket"); return; }
        struct sockaddr_in sa;
        sa.sin_family=AF_INET;
        sa.sin_port=htons(port);
        inet_pton(AF_INET,ip,&sa.sin_addr);

        if(connect(fd,(struct sockaddr*)&sa,sizeof(sa))<0){ printf("Connection failed.\n"); close(fd); return;}

        pthread_mutex_lock(&connection_mutex);
        if(connection_count>=MAX_CONNECTION){ printf("Too many connections.\n"); close(fd); pthread_mutex_unlock(&connection_mutex); return;}
        Connection *c=&connections[connection_count++];
        c->socket_fd=fd;
        strcpy(c->ip,ip);
        c->peer_port=port;
        c->active=1;
        pthread_mutex_unlock(&connection_mutex);

        printf("Connected successfully. Ready for data transmission.\n");
        pthread_create(&c->thread,NULL,connection_handler,c);
    }
    else if(strcmp(tok,"list")==0){
        printf("ID | IP Address      | Port\n");
        pthread_mutex_lock(&connection_mutex);
        for(int i=0;i<connection_count;i++){
            printf("%2d | %-15s | %5d\n",i,connections[i].ip,connections[i].peer_port);
        }
        pthread_mutex_unlock(&connection_mutex);
    }
    else if(strcmp(tok,"send")==0){
        char *id_str=strtok(NULL," ");
        char *msg=strtok(NULL,"");
        if(!id_str||!msg){ printf("Usage: send <id> <msg>\n"); return; }
        int id=atoi(id_str);
        pthread_mutex_lock(&connection_mutex);
        if(id<0||id>=connection_count){ printf("Invalid id.\n"); pthread_mutex_unlock(&connection_mutex); return;}
        if(strlen(msg)>MAX_MESSAGE){ printf("Msg too long.\n"); pthread_mutex_unlock(&connection_mutex); return;}
        send(connections[id].socket_fd,msg,strlen(msg)+1,0);
        printf("Message sent successfully to ID %d.\n",id);
        pthread_mutex_unlock(&connection_mutex);
    }
    else if(strcmp(tok,"terminate")==0){
        char *id_str=strtok(NULL," ");
        if(!id_str){ printf("Usage: terminate <id>\n"); return; }
        int id=atoi(id_str);
        pthread_mutex_lock(&connection_mutex);
        if(id<0||id>=connection_count){ printf("Invalid id.\n"); pthread_mutex_unlock(&connection_mutex); return;}
        send(connections[id].socket_fd,"DISCONNECT_SIGNAL",18,0);
        cleanup_connection(id);
        printf("Terminate peer with ID %d successfully.\n",id);
        pthread_mutex_unlock(&connection_mutex);
    }
    else if(strcmp(tok,"exit")==0){
        notify_exit();
        printf("All connections closed. Exiting program.\n");
        exit(0);
    }
    else printf("Unknown command.\n");
}



void display_message(const char *ip,int port,const char *msg){
    pthread_mutex_lock(&print_mutex);
    printf("\n----------------------------------\n");
    printf("Message received from: %s\n",ip);
    printf("Sender's port: %d\n",port);
    printf("Message: %s\n",msg);
    printf("----------------------------------\n");
    printf("Enter your Command: ");
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
}

int check_ip(const char *ip){
    struct sockaddr_in sa; return inet_pton(AF_INET,ip,&(sa.sin_addr))!=0;
}

char *get_local_ip(){
    struct ifaddrs *ifaddr,*ifa; char *ip=NULL;
    if(getifaddrs(&ifaddr)==-1) return NULL;
    for(ifa=ifaddr;ifa!=NULL;ifa=ifa->ifa_next){
        if(ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET && strcmp(ifa->ifa_name,"lo")!=0){
            ip=malloc(INET_ADDRSTRLEN);
            inet_ntop(AF_INET,&((struct sockaddr_in*)ifa->ifa_addr)->sin_addr,ip,INET_ADDRSTRLEN);
            break;
        }
    }
    freeifaddrs(ifaddr);
    return ip;
}

int find_index_by_fd(int fd){
    for(int i=0;i<connection_count;i++)
        if(connections[i].socket_fd==fd) return i;
    return -1;
}

void shift_left(int idx){
    for(int i=idx;i<connection_count-1;i++)
        connections[i]=connections[i+1];
    connection_count--;
}

void cleanup_connection(int idx){
    if(idx<0||idx>=connection_count) return;
    close(connections[idx].socket_fd);
    pthread_cancel(connections[idx].thread);
    shift_left(idx);
}

void notify_exit(){
    pthread_mutex_lock(&connection_mutex);
    for(int i=0;i<connection_count;i++){
        send(connections[i].socket_fd,"DISCONNECT_SIGNAL",18,0);
        close(connections[i].socket_fd);
    }
    connection_count=0;
    pthread_mutex_unlock(&connection_mutex);
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    listen_port = atoi(argv[1]);

    // In giao diện khởi động
    chat_app();

    for (int i = 0; i < MAX_CONNECTION; i++) connections[i].active = 0;

    pthread_t tid;
    pthread_create(&tid, NULL, server_thread, NULL);

    while(!server_ready) usleep(100000);

    char cmd[BUFFER_SIZE];
    while(1){
        printf("\nEnter your command: ");
        fflush(stdout);
        fgets(cmd, BUFFER_SIZE, stdin);
        cmd[strcspn(cmd,"\n")] = 0;
        handle_command(cmd);
    }
    return 0;
}