#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int received;
    int last_ack = 1;  // Start with ack 1 so we expect frame 0 first
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);
    printf("Server waiting...\n");
    
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    
    while(1) {
        recv(client_sock, &received, sizeof(received), 0);
        printf("Received frame: %d\n", received);
        
        // Check if received frame is opposite of last ack
        if(received == last_ack) {
            printf("Error: Expected frame %d, got %d\n", !last_ack, received);
            // Send same ack again to indicate error
            send(client_sock, &last_ack, sizeof(last_ack), 0);
            printf("Resending ack: %d\n", last_ack);
        } else {
            // Correct frame received, update ack
            last_ack = received;
            send(client_sock, &last_ack, sizeof(last_ack), 0);
            printf("Sent new ack: %d\n", last_ack);
        }
    }
    
    close(client_sock);
    close(server_sock);
    return 0;
}