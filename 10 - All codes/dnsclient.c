#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Get domain name from user
    printf("Enter domain name: ");
    scanf("%[^\n]", &buffer);  // Remove newline character

    // Send domain name to server
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Receive IP address from server
    memset(buffer, 0, BUFFER_SIZE);
    socklen_t addr_len = sizeof(serv_addr);
    recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &addr_len);

    printf("IP Address: %s\n", buffer);

    // Close socket
    close(sock);
    return 0;
}
