#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")  //���� ����� �ϱ� ���� �߰� ������ ������

#define BUFSIZE 1024


void ErrorHandling(char* message);                       //������� �Լ�

int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));                     //���� �Լ� �ʱ�ȭ
    SOCKET clntSock;                                     //���� ����
    WSADATA wsaData;                                     //���� ��
    SOCKADDR_IN servAddr;                                //���� �ּ�

    char sAddr[15];                                      //���� �ּ� ���� EX)192.168.0.1
    int sPort;                                           //���� ��Ʈ
    int nRcv;                                            //�޴� ����
    char message[BUFSIZE];                               //ó�� ���� ��Ʈ ������ ����� �Ⱦ�
    int send_recv_num[10] = { 0, };                      //������ �޴� ������ �����Ͽ� �����ϴ°�

    //������ �����ϴ� ���� ����
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
    //���� �Ϸ�

    while (1) {


        for (int i = 0; i < sizeof(send_recv_num) / sizeof(int); i++)               //������ ���� ���� ���� ����
        {
            send_recv_num[i] = rand() % 10;
        }

        printf("sending : ");
        for (int i = 0; i < sizeof(send_recv_num) / sizeof(int); i++)
        {
            printf("%d ", send_recv_num[i]);                                        //����
        }
        printf("\n");

        send(clntSock, send_recv_num, sizeof(send_recv_num), 0);                    //���� ����
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
            printf("%d ", send_recv_num[i]);                                         //���� ���
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