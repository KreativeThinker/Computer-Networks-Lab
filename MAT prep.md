
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define N 5  // Number of message exchanges

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_fd, 3);

    // Accept a connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    // Chat with client N times
    for (int i = 0; i < N; i++) {
        // Reset buffer before receiving
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        recv(new_socket, buffer, BUFFER_SIZE, 0);
        printf("Client: %s\n", buffer);

        // Reset buffer before sending
        memset(buffer, 0, BUFFER_SIZE);

        // Get message from server user
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to client
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
```

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define N 5  // Number of message exchanges

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Chat with server N times
    for (int i = 0; i < N; i++) {
        // Reset buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Get message from user
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        send(sock, buffer, strlen(buffer), 0);

        // Reset buffer before receiving
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from server
        recv(sock, buffer, BUFFER_SIZE, 0);
        printf("Server: %s\n", buffer);
    }

    // Close socket
    close(sock);
    return 0;
}
```