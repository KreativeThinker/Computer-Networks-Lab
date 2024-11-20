#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void genHamCode(char data[4], char ham[8]) {
    // Place data bits in correct positions
    ham[2] = data[0];
    ham[4] = data[1];
    ham[5] = data[2];
    ham[6] = data[3];

    // Calculate parity bits directly
    ham[0] = (((ham[2]-'0') ^ (ham[4]-'0') ^ (ham[6]-'0')) + '0'); // P1
    ham[1] = (((ham[2]-'0') ^ (ham[5]-'0') ^ (ham[6]-'0')) + '0'); // P2
    ham[3] = (((ham[4]-'0') ^ (ham[5]-'0') ^ (ham[6]-'0')) + '0'); // P4
    ham[7] = '\0';
}

void introduceError(char ham[8], int position){
    ham[position] = (ham[position] == '0') ? '1' : '0';
}

int main(){
    int sockfd, newsockid;
    struct sockaddr_in addr, client;
    char data[4] = "1011";
    char ham[8] = {0};
    genHamCode(data, ham);
    //introduceError(ham, 6);
    int len = sizeof(client);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sockfd, 5);
    newsockid = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&len);
    printf("Sending hamming with error: ");
    for(int i = 0; i < 7; i++) printf("%c", ham[i]);
    send(newsockid, ham, 7, 0);
    close(newsockid);
    close(sockfd);
    return 0;
}
