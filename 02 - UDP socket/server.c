#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8000
#define BUFFER_SIZE 1024
#define N 5

void cleanup(int socket) { close(socket); }

int server_setup() {
  int server_fd;
  struct sockaddr_in address;

  server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&address, 0, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  int binding = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  return server_fd;
}

void communicate(int socket) {
  char buffer[BUFFER_SIZE] = {0};
  struct sockaddr_in address;
  socklen_t addr_len = sizeof(address);

  for (int i = 0; i < N; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&address,
             (socklen_t *)&addr_len);
    printf("Message: %s", buffer);
  }
}

int main() {
  int server_fd = server_setup();
  communicate(server_fd);
  cleanup(server_fd);
  return 0;
}
