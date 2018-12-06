/*
 * Client
 * 학번 : 2017030273
 * 이름 : 정희석
 * argv[1] : 서버의 포트번호
 * argv[2] : 서버에 전송할 파일의 이름
*/

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#define EXIT_FAILURE -1
#define MAX_BUF_SIZE 128

int main(int argc, char** argv)
{
    struct sockaddr_in server_addr;
    int client_fd;
    char buf[MAX_BUF_SIZE];

    if(argc < 2) {
        printf("The number of parameter is insufficient.\n");
        exit(EXIT_FAILURE);
    }

    // 클라이언트의 소켓을 생성하는 부분
    client_fd = socket(AF_INET, SOCKET_STREAM, 0);
    assert(client_fd != -1);

    // 서버의 IP address와 port번호를 지정하고, 연결시도
    memset( &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("connection failed.\n");
        exit(EXIT_FAILURE);
    }

    // 인자로 입력받은 이름의 파일을 염
    int fd;
    fd = open(argv[2], O_RDONLY);
    assert(fd > 0);

    // 파일을 읽어서 내용을 서버에 전송
    int read_len;
    long total_send_byte = 0;
    while((read_len = read(fd, buf, MAX_BUF_SIZE)) > 0) {
        if(write(client_fd, buf, sizeof(char) * read_len) < 0) {
            printf("sending file failed.\n");
            exit(EXIT_FAILURE);
        }
        total_send_byte += (long)read_len;
        printf("%ld bytes sended.\n", total_send_byte);
    }
    printf("successfuly sended.\n");

    // 클라이언트 소켓을 닫음
    close(client_fd);

    return 0;
}