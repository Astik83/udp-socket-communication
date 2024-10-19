#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int ret;
    ret = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(ret == -1) {
        printf("Error in socket creation\n");
        exit(1);
    } else {
        printf("Socket created successfully\n");
    }   
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(7000);  // use htons to convert to network byte order
    sock.sin_addr.s_addr = inet_addr("127.0.0.2");
    
    int res = bind(ret, (const struct sockaddr*)&sock, sizeof(sock));
    if (res== -1) {
        printf("Error in bind\n");
        exit(1);
    } else {
        printf("Bind created successfully\n");
    }
    
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    getsockname(ret, (struct sockaddr *)&sender_addr, &sender_len);
    int sender_port = ntohs(sender_addr.sin_port);
    printf("Sender's port number: %d\n", sender_port);
   
   while(1){
    char send_buff[100];
    struct sockaddr_in send;
    send.sin_family = AF_INET;
    send.sin_port = htons(5000);  // use htons
    send.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("\nEnter the message (enter 'exit' to quit): ");
    if (fgets(send_buff, sizeof(send_buff),stdin) == 0) {
        exit(1);
    }
    int len = strlen(send_buff);
    if (len > 0 && send_buff[len - 1] == '\n'){
        send_buff[len - 1] = '\0';
    }
    if(strcmp(send_buff, "exit") == 0) {
        sendto(ret, "exit", strlen("exit"), 0, (const struct sockaddr*)&send, sizeof(send));
        break;
    }
    if(sendto(ret,send_buff,strlen(send_buff),0,(const struct sockaddr*)&send, sizeof(send)) <0){
        printf("Send failed");
        exit(EXIT_FAILURE);
    }
    
    char str[100];
    struct sockaddr_in recv;
    int val = sizeof(recv);
    int ret1 = recvfrom(ret, str, sizeof(str), 0, (struct sockaddr*)&recv, &val);
    str[ret1] = '\0';
    if(strcmp(str, "exit") == 0) {
        printf("Receiver has quit. Exiting.\n");
        break;
    }
    if(ret1 == -1){
        printf("Error in recvfrom\n");
        exit(1);
    } else {
        printf("Received: %s\n", str);
    }
  }
    return 0;
}

