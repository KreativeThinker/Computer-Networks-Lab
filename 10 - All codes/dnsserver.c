#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct {
    char domain[100];
    char ip[100];
} DNSrecord;

DNSrecord look_up[] = {
    {"example.com", "1.2.3.4"},
    {"example2.com", "3.4.5.6"}
};

const int len = sizeof(look_up) / sizeof(DNSrecord);

char* lookup(const char* domain){
    for(int i = 0; i < len; i++){
        if(strcmp(domain, look_up[i].domain) == 0){
            return look_up[i].ip;
        }
    }
    return NULL;
}

int main(){
    int sockfd;
    struct sockaddr_in addr;
    int size = sizeof(addr);
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr *)&addr, size);
    printf("Listening............\n");
    while(1){
        memset(buffer, 0, 100);
        recvfrom(sockfd, buffer, 100, 0, (struct sockaddr *)&addr, (socklen_t *)&size);
        printf("Recived query %s \n", buffer);

        char* ip = lookup(buffer);
        if(ip == NULL) ip = "domain not found";

        sendto(sockfd, ip, strlen(ip), 0, (struct sockaddr *)&addr, size);
    }
    return 0;
}