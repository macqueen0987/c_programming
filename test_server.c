#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <stdint.h>
//�ּ����� �޾ƺôµ� ���� ��Ȯ���� �� �����ϴ�.. �Ф�
#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

void ErrorHandling(char* message);              //������� �Լ�


int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));            //�׽�Ʈ�� ���� �����Լ��� ����� ����, ���� �����Լ� �ʱ�ȭ

    WSADATA wsaData;                            //���� �ۼ��� �ϱ� ���� �⺻ ����
    SOCKET servSock, clntSock;                  //SOCKET�� ��� UINT_PTR ���̴�. <- ���Ҹ��� �𸣰ڽ��ϴ�
    SOCKADDR_IN servAddr, clntAddr;             //���� �ۼ��� �ϱ� ���� �⺻ ����

    int port_number;                            //��Ʈ��ȣ
    printf("Please insert Port Number : ");
    scanf("%d", &port_number);
    char message[BUFSIZE]; //Message Buffer

    int fromLen, nRcv;                          //������ �ְ������ ����ϴ� ����
    int32_t send_recv_num[10];                  //�ְ���� �����͸� �����Ͽ� �� ������ ����

    //����� �ϱ� ���� ������ ���� ����
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
    } //�ؿ� else�κ��� ��� ������ �ϴ� �ּ�ó��
    //else {
    //    printf("%s Connection Complete!\n", inet_ntoa(clntAddr.sin_addr));
    //    printf("Start ...\n");
    //}

    closesocket(servSock);
    //������ �Ǿ����� ������ ����

    int num;
    while (1) {

        printf("receiving...\n ");

        nRcv = recv(clntSock, send_recv_num, sizeof(send_recv_num), 0); //Ŭ���̾�Ʈ���� ������ ����
        if (nRcv == SOCKET_ERROR) { //����ó��
            printf("Receive Error..\n");
            break;
        }

        if (strcmp(message, "exit") == 0) { //���Ṯ�� ���߿� �ٲٸ� ��
            printf("Close Server Connection..\n");
            break;
        }
        printf("revieved : ");
        for (int i = 0; i < sizeof(send_recv_num)/sizeof(int); i++)
        {
            printf("%d ", send_recv_num[i]);                             //���� ���� ǥ��, ���� ��Ʈ���������� �ٷ� �ʵ忡 �׸��� ��
        }
        printf("\n");

        printf("sending : ");
        for (int i = 0; i < sizeof(send_recv_num)/sizeof(int); i++)      //���� ���� ����, ����� �������� ���ڷ� �� 10�ڸ� �迭
        {
            printf("%d ", send_recv_num[i]);
        }
        printf("\n");

        send(clntSock, send_recv_num, sizeof(send_recv_num), 0);         //���� ����
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