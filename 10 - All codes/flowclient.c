#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in addr;
    int frame, received, input;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    while(1) {
        printf("Enter frame to send (0 or 1): ");
        scanf("%d", &input);
        
        if(input != 0 && input != 1) {
            printf("Please enter only 0 or 1\n");
            continue;
        }
        
        frame = input;
        printf("Sending frame: %d\n", frame);
        send(sock, &frame, sizeof(frame), 0);
        
        recv(sock, &received, sizeof(received), 0);
        printf("Received ack: %d\n", received);
        
        //if(frame == received) {
          //  printf("Error: Received same frame back!\n");
        //}
    }
    
    close(sock);
    return 0;
}