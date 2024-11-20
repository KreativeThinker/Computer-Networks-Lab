#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 512

int main() {
  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[BUF_SIZE];
  socklen_t addr_len = sizeof(server_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  char dns_query[BUF_SIZE];
  printf("Enter domain to query: ");
  scanf("%s", dns_query);

  sendto(sockfd, dns_query, strlen(dns_query), 0,
         (const struct sockaddr *)&server_addr, addr_len);
  printf("DNS query sent: %s\n", dns_query);

  int recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                          (struct sockaddr *)&server_addr, &addr_len);
  if (recv_len < 0) {
    perror("recvfrom failed");
  } else {
    buffer[recv_len] = '\0';
    printf("DNS response received: %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
