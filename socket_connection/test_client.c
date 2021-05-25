#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")  //소켓 통신을 하기 위한 추가 파일을 볼러옴

#define BUFSIZE 1024


void ErrorHandling(char* message);                       //에러출력 함수

int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));                     //랜덤 함수 초기화
    SOCKET clntSock;                                     //소켓 선언
    WSADATA wsaData;                                     //뭔지 모름
    SOCKADDR_IN servAddr;                                //서버 주소

    char sAddr[15];                                      //서버 주소 선언 EX)192.168.0.1
    int sPort;                                           //서버 포트
    int nRcv;                                            //받는 정보
    char message[BUFSIZE];                               //처음 서버 포트 받을때 빼고는 안씀
    int send_recv_num[10] = { 0, };                      //보내고 받는 정보를 가공하여 저장하는곳

    //서버에 연결하는 과정 시작
    struct hostent* host;

    printf("Server Address : ");       gets(sAddr);
    printf("Port Number : ");       gets(message);
    sPort = atoi(message);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("Load WinSock 2.2 DLL Error");

    clntSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clntSock == INVALID_SOCKET)
        ErrorHandling("Socket Error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(sAddr);
    servAddr.sin_port = htons(sPort);

    if (connect(clntSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("Connection Error");
    }
    else {
        printf("Connect OK!\nStart...\n");
    }
    //연결 완료

    while (1) {


        for (int i = 0; i < sizeof(send_recv_num) / sizeof(int); i++)               //서버에 보낼 랜덤 변수 선언
        {
            send_recv_num[i] = rand() % 10;
        }

        printf("sending : ");
        for (int i = 0; i < sizeof(send_recv_num) / sizeof(int); i++)
        {
            printf("%d ", send_recv_num[i]);                                        //전송
        }
        printf("\n");

        send(clntSock, send_recv_num, sizeof(send_recv_num), 0);                    //정보 수신
        printf("receiving...\n ");

        nRcv = recv(clntSock, send_recv_num, sizeof(send_recv_num), 0);
        if (nRcv == SOCKET_ERROR) {
            printf("Receive Error..\n");
            break;
        }

        if (strcmp(message, "exit") == 0) {
            printf("Close Server Connection..\n");
            break;
        }
        printf("revieved : ");
        for (int i = 0; i < sizeof(send_recv_num) / sizeof(int); i++)
        {
            printf("%d ", send_recv_num[i]);                                         //정보 출력
        }
        printf("\n");
    }

    closesocket(clntSock);
    WSACleanup();
    printf("Close Connection..\n");
    _getch();

    return 0;
}


void ErrorHandling(char* message) {
    WSACleanup();
    fputs(message, stderr);
    fputc('\n', stderr);
    _getch();
    exit(1);
}