/*
 * Server
 * 학번 : 2017030273
 * 이름 : 정희석
 * argv[1] : 서버의 포트번호
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#define MAX_BUF_SIZE 128


int main(int argc, char** argv)
{
    struct sockaddr_in server_addr, client_addr;
    int server_fd, client_fd;
    char buf[MAX_BUF_SIZE];

    if(argc < 2) {
        printf("The number of parameter is insufficient.\n");
        exit(EXIT_FAILURE);
    }

    // 서버의 소켓을 생성하는 부분
    // AF_INET: IPv4 인터넷 프로토콜을 사용, SOCKET_STREAM: TCP/IP 프로토콜 사용 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(server_fd != -1);

    // 소켓을 초기화하고 IP address와 포트번호 지정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4 인터넷 프로토콜 사용
    server_addr.sin_port = htons(atoi(argv[1])); // 포트번호 할당
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP주소 할당
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("binding failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("Server. IP address: %s, PORT:%d\n", inet_ntoa(server_addr.sin_addr),
            ntohs(server_addr.sin_port));

    // 클라이언트 접속을 기다림
    if(listen(server_fd, SOMAXCONN) < 0) {
        printf("listen failed.\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        printf("Listening...\n");
        
        // 접속한 클라이언트와 통신할 수 있도록 새로운 소켓을 생성
        int client_addr_size = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        if(client_fd < 0) {
            printf("accept falied.\n");
            continue;
        }

        // 클라이언트와 접속에 성공했을 경우
        printf("Client connected. IP addres:%s, PORT:%d\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        // 클라이언트에서 전송받은 파일을 저장할 파일을 만듬
        int fd;
        fd = open("get_file.txt", O_RDWR | O_CREAT, 0644);
        assert(fd > 0);

        // 클라이언트와 데이터를 주고받음
        int read_len;
        long total_receive_byte = 0;
        while((read_len = read(client_fd, buf, MAX_BUF_SIZE)) > 0) {
            if(write(fd, buf, sizeof(char) * read_len) < 0) {
                printf("receving file failed.\n");
                exit(EXIT_FAILURE);
            }
            total_receive_byte += (long)read_len;
            printf("%ld bytes received.\n", total_receive_byte);
        }
        printf("successfuly received.\n");

        // 소켓을 닫음
        close(client_fd);
    }

    return 0;
}