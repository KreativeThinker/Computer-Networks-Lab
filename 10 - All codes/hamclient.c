#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int checkHamCode(char hamming[8]){
    // Convert ASCII to binary before XOR
    int p1 = ((hamming[0]-'0') ^ (hamming[2]-'0') ^ (hamming[4]-'0') ^ (hamming[6]-'0')); 
    int p2 = ((hamming[1]-'0') ^ (hamming[2]-'0') ^ (hamming[5]-'0') ^ (hamming[6]-'0'));
    int p4 = ((hamming[3]-'0') ^ (hamming[4]-'0') ^ (hamming[5]-'0') ^ (hamming[6]-'0'));

    return (p4 << 2) | (p2 << 1) | p1;
}

void correct(char ham[8], int pos){
    if(pos > 0) { // Only correct if error detected
        printf("Error at position %d\n", pos);
        ham[pos - 1] = (ham[pos - 1] == '0') ? '1':'0';
    }
}

int main(){
    int sock;
    struct sockaddr_in addr;
    char hamming[8] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    recv(sock, hamming, 7, 0);
    hamming[7] = '\0';
    
    printf("Received: %s\n", hamming);
    
    int error = checkHamCode(hamming);
    if(error > 0) {
        printf("Error detected, correcting position %d\n", error);
        correct(hamming, error);
        printf("Corrected: %s\n", hamming);
    } else {
        printf("No errors detected\n");
    }
    
    close(sock);
    return 0;
}