#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
void client(int sock){
    //2、设置客户端socket参数
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    //3、设置传输协议、端口以及目的地址
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("192.168.34.173");

    //4、创建发送与接收缓存字节数组
    int ret;
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    //5、开始发送数据（此处是发送来自标准输入的数据）
    while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL){
        sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        //6、开始接收数据，接收服务端返回的数据
       // ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if(ret == -1){
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        fputs(recvbuf, stdout);       //将服务端返回的数据写入到标准输出

        //7、清空发送与接收缓存
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    //8、关闭socket流
    close(sock);
}

int main(void){
    int sock;
    //1.创建客户端的socket
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    client(sock);
    return 0;
}
