#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8000
#define BUFFER_SIZE 1024
#define N 5

void cleanup(int socket) { close(socket); }

int client_setup(struct sockaddr_in *address) {
  int sock;
  memset(address, 0, sizeof(*address));
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  address->sin_family = AF_INET;
  address->sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &address->sin_addr);
  return sock;
}

void communicate(int socket, struct sockaddr_in *address) {
  char buffer[BUFFER_SIZE] = {0};
  for (int i = 0; i < N; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    sendto(socket, buffer, strlen(buffer), 0, (struct sockaddr *)address,
           sizeof(*address));
  }
}

int main() {
  struct sockaddr_in server_addr;
  int server_fd = client_setup(&server_addr);
  communicate(server_fd, &server_addr);
  cleanup(server_fd);
  return 0;
}
