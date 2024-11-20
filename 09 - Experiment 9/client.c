#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 512

void resolve_domain_to_ip(const char *domain, char *ip_buffer) {
  struct hostent *he;
  struct in_addr **addr_list;

  if ((he = gethostbyname(domain)) == NULL) {
    strcpy(ip_buffer, "DNS resolution failed");
    return;
  }

  addr_list = (struct in_addr **)he->h_addr_list;
  if (addr_list[0] != NULL) {
    strcpy(ip_buffer, inet_ntoa(*addr_list[0]));
  } else {
    strcpy(ip_buffer, "No IP found");
  }
}

void handle_dns_query(int sockfd, struct sockaddr_in *client_addr,
                      socklen_t addr_len) {
  char buffer[BUF_SIZE], response[BUF_SIZE];
  memset(response, 0, BUF_SIZE);
  int recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                          (struct sockaddr *)client_addr, &addr_len);
  if (recv_len < 0) {
    perror("recvfrom failed");
    return;
  }

  buffer[recv_len] = '\0';
  printf("Received DNS query for: %s\n", buffer);

  resolve_domain_to_ip(buffer, response);

  sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)client_addr,
         addr_len);
  printf("DNS response sent: %s\n", response);
}

int main() {
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Enhanced DNS Server is running on port %d...\n", PORT);

  while (1) {
    handle_dns_query(sockfd, &client_addr, addr_len);
  }

  close(sockfd);
  return 0;
}
