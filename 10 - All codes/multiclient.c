#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int sock = 0;
    struct sockaddr_in addr;
    int len = sizeof(addr);
    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    while(1){
        scanf(" %[^\n]", buffer);
        send(sock, buffer, strlen(buffer), 0);
        memset(buffer, 0, 1024);
    }
    close(sock);
    return 0;
}