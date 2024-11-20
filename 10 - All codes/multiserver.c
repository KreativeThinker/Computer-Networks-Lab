#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void * handle(void * arg){
    int socket = *(int *)arg;
    char buffer[1024] = {0};
    //free(arg);
    printf("Connected\n");
    while(1){
        int bytes = recv(socket, buffer, 1024, 0);
        if(bytes == 0){
            printf("Disconnected\n");
            break;
        }
        printf("Client: %s\n", buffer);
        memset(buffer, 0, 1024);
    }
    close(socket);
    return NULL;
}

int main(){
    int sockfd, newsockfd;
    struct sockaddr_in addr;
    int len = sizeof(addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr *)&addr, len);
    listen(sockfd, 5);
    printf("Server waiting...\n");
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&len);
        //int * sock_ptr = malloc(sizeof(int));
        //*sock_ptr = newsockfd;
        pthread_t threadid;
        pthread_create(&threadid, NULL, handle, &newsockfd);
        pthread_detach(threadid);
    }
    //close(sockfd);
    return 0;
}