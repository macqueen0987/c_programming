#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <stdint.h>
//주석들은 달아봤는데 저도 정확히는 잘 몰릅니다.. ㅠㅠ
#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

void ErrorHandling(char* message);              //에러출력 함수


int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));            //테스트를 위해 랜덤함수를 사용할 예정, 따라서 랜덤함수 초기화

    WSADATA wsaData;                            //소켓 송수신 하기 위한 기본 선언
    SOCKET servSock, clntSock;                  //SOCKET은 사실 UINT_PTR 형이다. <- 뭔소린지 모르겠습니다
    SOCKADDR_IN servAddr, clntAddr;             //소켓 송수신 하기 위한 기본 선언

    int port_number;                            //포트번호
    printf("Please insert Port Number : ");
    scanf("%d", &port_number);
    char message[BUFSIZE]; //Message Buffer

    int fromLen, nRcv;                          //데이터 주고받을때 사용하는 변수
    int32_t send_recv_num[10];                  //주고받은 데이터를 가공하여 이 변수에 저장

    //통신을 하기 위한 소켓을 여는 과정
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("Load WinSock 2.2 DLL Error");

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET)
        ErrorHandling("Socket Error");

    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port_number);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("listening...\n");

    if (bind(servSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("Bind Error");

    if (listen(servSock, 2) == SOCKET_ERROR)
        ErrorHandling("Listen Error");

    fromLen = sizeof(clntAddr);

    clntSock = accept(servSock, (void*)&clntAddr, &fromLen);
    if (clntSock == INVALID_SOCKET) {
        ErrorHandling("Accept Error");
    } //밑에 else부분은 없어도 되지만 일단 주석처리
    //else {
    //    printf("%s Connection Complete!\n", inet_ntoa(clntAddr.sin_addr));
    //    printf("Start ...\n");
    //}

    closesocket(servSock);
    //연결이 되었으면 소켓을 닫음

    int num;
    while (1) {

        printf("receiving...\n ");

        nRcv = recv(clntSock, send_recv_num, sizeof(send_recv_num), 0); //클라이언트에서 정보를 받음
        if (nRcv == SOCKET_ERROR) { //에러처리
            printf("Receive Error..\n");
            break;
        }

        if (strcmp(message, "exit") == 0) { //종료문자 나중에 바꾸면 됨
            printf("Close Server Connection..\n");
            break;
        }
        printf("revieved : ");
        for (int i = 0; i < sizeof(send_recv_num)/sizeof(int); i++)
        {
            printf("%d ", send_recv_num[i]);                             //받은 정보 표시, 나중 테트리스에서는 바로 필드에 그리면 됨
        }
        printf("\n");

        printf("sending : ");
        for (int i = 0; i < sizeof(send_recv_num)/sizeof(int); i++)      //보낼 정보 생성, 현재는 무작위의 숫자로 된 10자리 배열
        {
            printf("%d ", send_recv_num[i]);
        }
        printf("\n");

        send(clntSock, send_recv_num, sizeof(send_recv_num), 0);         //정보 전송
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