#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

unsigned char calculate_checksum(const char *data) {
    unsigned char checksum = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        checksum += data[i];
    }
    return ~checksum; // One's complement
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // Receive message from client
    int bytes_received = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
    buffer[bytes_received] = '\0'; // Null-terminate the string
    printf("Received message: %s\n", buffer);

    // Calculate checksum for received message
    unsigned char calculated_checksum = calculate_checksum(buffer);

    // Receive checksum from client
    unsigned char received_checksum;
    recv(new_socket, &received_checksum, sizeof(received_checksum), 0);
    printf("Received checksum: %d\n", received_checksum);

    // Validate checksum
    if (calculated_checksum == received_checksum) {
        send(new_socket, "Checksum valid", 15, 0);
    } else {
        send(new_socket, "Checksum invalid", 17, 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
