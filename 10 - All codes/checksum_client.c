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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Input message
    char message[BUFFER_SIZE];
    printf("Enter message: ");
    scanf("%s", message);

    // Ask user if they want to introduce an error
    char choice;
    printf("Do you want to introduce an error in the message? (y/n): ");
    scanf(" %c", &choice);

    unsigned char checksum = calculate_checksum(message);
    if (choice == 'y' || choice == 'Y') {
        message[0] += 1; // Simple error: Change the first character
        printf("Message after introducing error: %s\n", message);
    }

    // Calculate checksum
    printf("Calculated checksum: %d\n", checksum);

    // Send message to server
    send(sock, message, strlen(message), 0);

    // Send checksum to server
    send(sock, &checksum, sizeof(checksum), 0);

    // Receive response from server
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    buffer[bytes_received] = '\0'; // Null-terminate the string
    printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}
