#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    int ret;
    int on = 1;
    struct sockaddr_in serv_addr;
    char *message = "Hello, Server!";
    char buffer[BUFFER_SIZE];

    // 创建socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    ret = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    if (sock == -1) {
        printf("socket failed\n");
        return -1;
    }

    // 设置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.168.19.104", &serv_addr.sin_addr);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        return -1;
    }

    // 向服务器发送数据
    send(sock, message, strlen(message), 0);
    printf("Sent: %s\n", message);

    // 从服务器接收数据
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Received: %s\n", buffer);

    // 关闭socket
    close(sock);

    return 0;
}
