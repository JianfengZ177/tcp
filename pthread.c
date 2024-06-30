#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 8080
#define BUFFER_SIZE 1024

// 线程函数
void *thread_func(void *arg) {
    int server_fd, client_fd, addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // 创建socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定socket和地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(client_addr);

    // 循环接受客户端连接
    while (1) {
        // 接受客户端连接
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connected by %s\n", inet_ntoa(client_addr.sin_addr));

        // 读取客户端发送的数据
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Received message: %s\n", buffer);

        // 向客户端发送数据
        strcpy(buffer, "Hello, client!");
        send(client_fd, buffer, strlen(buffer), 0);

        // 关闭连接
        close(client_fd);
    }

    return 0;
}

int main() {
    // 初始化线程属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // 设置线程属性，例如栈大小
    size_t stack_size = 0;
    pthread_attr_setstacksize(&attr, stack_size);

    // 创建线程
    pthread_t thread;
    int ret = pthread_create(&thread, &attr, thread_func, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(thread, NULL);

    // 销毁线程属性
    pthread_attr_destroy(&attr);

    return 0;
}
