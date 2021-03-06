#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <sys/types.h>

//주석들은 달아봤는데 저도 정확히는 잘 몰릅니다.. ㅠㅠ
#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

clock_t startDropT, endT, startGroundT;     //시간에 대한 변수를 선언
int x = 8, y = 0;                           //게임 필드를 그리기 시작하는 위치
int m = 10, n = 5;                          //맨 처음 메뉴화면 그리기 시작하는 위치
int g = 23, h = 5;                          //(민규: 20->23으로 수정)
int g2 = 53;                                //2번째 필드의 시작 위치
RECT blockSize;                             //사각형 좌표를 저장하기 위한 자료형
int blockForm;                              //블럭의 종류
int blockRotation = 0;                      //블럭 회전
int key;                                    //입력한 키
bool space_drop = false;                    //스페이스바를 누르면 바로 떨어뜨리고 바로 블록의 위치 고정 <- 살짝 애매하게 구현
bool run = true;                            //참값이면 실행 아니면 종료
int fromLen, nRcv;                          //데이터 주고받을때 사용하는 변수
WSADATA wsaData;                            //소켓 송수신 하기 위한 기본 선언
SOCKET servSock, clntSock;                  //SOCKET은 사실 UINT_PTR 형이다. <- 뭔소린지 모르겠습니다
SOCKADDR_IN servAddr, clntAddr;             //소켓 송수신 하기 위한 기본 선언
char sAddr[15];                             //서버 주소 선언 EX)192.168.0.1
int sPort;                                  //서버 포트
char message[BUFSIZE];                      //Message Buffer
int send_space[12 * 23] = { 0, };           //전송 필드 정보
int recv_space[12 * 23] = { 0, };           //받는 필드 정보
int server_client;
int port_number = 20;                       //포트 번호
int mode = 0;                               //게임 모드


int block[7][4][4][4] = {
    { //T모양 블럭
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0}
        }
    },
    { //번개 블럭
        {
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0}
        }
    },
    { //번개 블럭 반대
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0}
        }
    },
    { //1자형 블럭
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0}
        }
    },
    { //L자형 블럭
        {
            {0,0,0,0},
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {1,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },
    { //L자형 블럭 반대
        {
            {0,0,0,0},
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0}
        },
        {
            {1,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        }
    },
    { //네모 블럭
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        }
    }
};

int space[22 + 1][10 + 2] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
}; //높이를 20에서 23으로 늘렸습니다.(Game Over 알고리즘을 위해)

int space2[22 + 1][10 + 2] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
}; //높이를 20에서 23으로 늘렸습니다.(Game Over 알고리즘을 위해)

void ErrorHandling(char* message);              //에러출력 함수
void Init();                           //콘솔창 깜빡이 방지, 커서 숨기기
void gotoxy(int x, int y);             //x좌표 y좌표로 커서 옮기기
void setColor(unsigned short text, unsigned short back); //글자색, 배경색 설정
void CreateRandomForm();               //랜덤으로 0~6값을 선언
bool CheckCrash(int x, int y);         //충돌을 감지하는 함수 하나라도 겹치는 것이 있다면 true반환
void DropBlock();                      //0.8초마다 블럭을 한칸씩 밑으로 내리는 함수
void EraseField();                     //블록이 그려지는 필드를 지우는 함수
void DrawField();                      //블록이 그려지는 필드를 그리는 함수
void DrawField2();
void BlockToGround();                  //블럭이 땅바닥에 닿아 있는지 확인하는 함수
void RemoveLine();                     //1줄이 되었다면 블럭을 제거합니다. 그 줄에서부터 시작해서 한칸씩 다 땡깁니다.
void DrawMap();                        //맵의 형태(가장자리 태두리)만 그리기
void DrawBlock();                      //블럭을 그리기
void InputKey();                       //키 입력받기
void SolidLine();                      //EraseFiled 안에 있는 함수, 실선을 그리기 위한 함수
void server();
void client(bool first);
void comms();
void check_gameover();


int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));            //테스트를 위해 랜덤함수를 사용할 예정, 따라서 랜덤함수 초기화

    Init();
    int z;
    int title_end = 0;
    int color = 1;
    while (run){
        while (true) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color % 16);	if (_kbhit()) break;
            gotoxy(0 + m, 0 + n); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 1 + n); printf("▣■■■  ■■■  ■■■  ■■    ■■■  ■■■▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 2 + n); printf("▣  ■    ■        ■    ■  ■    ■    ■    ▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 3 + n); printf("▣  ■    ■■■    ■    ■■      ■    ■■■▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 4 + n); printf("▣  ■    ■        ■    ■  ■    ■        ■▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 5 + n); printf("▣  ■    ■■■    ■    ■  ■  ■■■  ■■■▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 6 + n); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣"); Sleep(100); if (_kbhit()) break;
            gotoxy(0 + m, 7 + n); printf("                                                  "); Sleep(100); if (_kbhit()) break;
            gotoxy(13 + m, 9 + n); printf("1 = Solo, 2 = Client, 3 = Server");
            for (z = 0; z < 50; z++) {
                Sleep(30); if (_kbhit()) { title_end = 1; break; }
            }
            color++;
            if (color % 16 == 0)
                color++;
            if (title_end == 1)
                break;
        }

        if (_kbhit())
        {
            key = _getch();
            if (key == 224)
            {
                key = _getch();
            }
            switch (key)
            {
            case 49: //1 solo
                mode = 1;
                run = false;
                break;
            case 50: //2 client
                mode = 2;
                run = false;
                break;
            case 51: //3 server
                mode = 3;
                run = false;
                break;
            }
        }
    }

    run = true;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    system("cls");
    gotoxy(m, n);

    if (mode == 3)
    {
        server();
    }
    if (mode == 2)
    {
        client(true);
    }



    int num;
    while (true)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        startDropT = clock();
        CreateRandomForm();
        system("cls");
        DrawMap();

        while (run)
        {
            if (mode != 1)
            {
                DrawField2();
            }
            DropBlock();
            BlockToGround();
            RemoveLine();
            InputKey();
            if (mode != 1)
            {
                comms();
            }
            check_gameover();
        }

        gotoxy(0 + m, 0 + n + 7); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
        gotoxy(0 + m, 1 + n + 7); printf("▣                                              ▣");
        gotoxy(0 + m, 2 + n + 7); printf("▣                                              ▣");
        gotoxy(0 + m, 3 + n + 7); printf("▣          G A M E           O V E R           ▣");
        gotoxy(0 + m, 4 + n + 7); printf("▣                                              ▣");
        gotoxy(0 + m, 5 + n + 7); printf("▣                                              ▣");
        gotoxy(0 + m, 6 + n + 7); printf("▣           Press any key to restart           ▣");
        gotoxy(0 + m, 7 + n + 7); printf("▣                                              ▣");
        gotoxy(0 + m, 8 + n + 7); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
        while (_kbhit() == 0);         //좀 더 꾸밀필요가 있음, 여기서 한번 더 누르면 게임이 다시시작
        run = true;
        if (mode == 3)
        {
            server();
        }
        if (mode == 2)
        {
            client(false);
        }
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
    run = false;
    exit(1);
}

void Init()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    srand(time(NULL)); //srand(): rand()의 근원, time(null): 옛날부터 현재까지의 흐른시간을 초단위로 생성
}

void gotoxy(int x, int y)
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

void setColor(unsigned short text, unsigned short back)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | back << 4);
}

void CreateRandomForm()
{
    blockForm = rand() % 7;
}

bool CheckCrash(int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[blockForm][blockRotation][i][j] == 1)
            {
                int t = space[i + y][j + x / 2]; //x좌표는 2당 1칸을 의미하기 때문에
                if (t == 1 || t == 2) //벽(=1)일 때, 고정된블럭(=2)일 때
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void DropBlock()
{
    endT = clock(); //endT: 시간에 대한 변수, clock(): 프로세스가 현재 실행까지 걸린 clock수
    if ((float)(endT - startDropT) >= 800) //(종료시점 시간 - 시작시점 시간)
    {
        if (CheckCrash(x, y + 1) == true) return;
        y++;
        startDropT = clock();
        startGroundT = clock();
        DrawBlock();
    }
}

void BlockToGround()
{
    if (CheckCrash(x, y + 1) == true)
    {
        if ((float)(endT - startGroundT) > 200 || space_drop) // 블럭이 땅바닥에 닿으면 0.2초후 고정
        { //현재 블럭 저장
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (block[blockForm][blockRotation][i][j] == 1)
                    {
                        space[i + y][j + x / 2] = 2;
                    }
                }
            }
            x = 8;
            y = 0;
            CreateRandomForm(); //랜덤한 블럭을 위(8,0)로 올림
        }
    }
    if (space_drop)
    {
        endT = startDropT + 800;//지금으로서는 이 방법이 바로
        space_drop = false;
    }
}

void RemoveLine()
{
    for (int i = 22; i > 3; i--)
    {
        int cnt = 0;
        for (int j = 1; j < 11; j++)
        {
            if (space[i][j] == 2)
            {
                cnt++;
            }
        }
        if (cnt > 9) //벽돌이 다 차있다면
        {
            for (int j = 0; i - j >= 0; j++)
            {
                for (int x = 1; x < 11; x++)
                {
                    if (i - j - 1 >= 0)
                        space[i - j][x] = space[i - j - 1][x];
                    else //천장이면 0저장
                        space[i - j][x] = 0;
                }
            }
        }
    }
}

void DrawMap()
{

    for (int i = 0; i < 23; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            gotoxy(g + j * 2, h + i);
            if (space[i][j] == 1)
            {
                setColor(0, 15);
                printf("  ");
                setColor(15, 0);
            }
        }
    }
}

void DrawBlock()
{
    int min = 100, max = 0;
    int tempx[4] = { 0, }, tempy[4] = { 0, }, num = 0;
    EraseField();


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[blockForm][blockRotation][i][j] == 1)
            {
                tempx[num] = (x + j * 2) / 2;
                tempy[num] = y + i;
                num++;
                if (min > x + g + j * 2)
                {
                    min = x + g + j * 2;
                }
                if (max < x + g + j * 2)
                {
                    max = x + g + j * 2;
                }
            }
        }
    }


    for (int i = 0; i < 22; i++)
    {
        for (int j = min; j <= max; j++)
        {
            gotoxy(j, h + i);
            setColor(0, 8);
            printf("  ");
            setColor(15, 0);
        }
    }

    num = 0;
    for (int i = 0; i < 23; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            send_space[num] = space[i][j];
            num++;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        send_space[tempx[i] + 12 * tempy[i]] = 3;
    }


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[blockForm][blockRotation][i][j] == 1)
            {
                gotoxy(x + g + j * 2, y + h + i);
                setColor(15, 1);
                printf("  ");
                setColor(15, 0);
            }
        }
    }
    DrawField();
}

void EraseField()
{
    for (int i = 0; i < 22; i++)
    {
        if (i == 3)
        {
            SolidLine();
            i++;
        }
        for (int j = 1; j < 11; j++)
        {
            gotoxy(g + j * 2, h + i);
            printf("  ");
        }

    }
}

void DrawField()
{
    for (int i = 0; i < 22; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            gotoxy(g + j * 2, h + i);
            if (space[i][j] == 2)
            {
                setColor(15, 14);
                printf("  ");
                setColor(15, 0);
            }
        }
    }
}

void DrawField2()
{
    for (int i = 0; i < 23; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            gotoxy(g2 + j * 2, h + i);
            if (space2[i][j] == 1)
            {
                setColor(0, 15);
                printf("  ");
                setColor(15, 0);
            }
            else if (space2[i][j] == 2)
            {
                setColor(15, 14);
                printf("  ");
                setColor(15, 0);
            }
            else if (space2[i][j] == 3)
            {
                setColor(0, 8);
                printf("  ");
                setColor(15, 0);
            }
            else
            {
                printf("  ");
            }
        }
    }
}

void InputKey()
{
    if (_kbhit())
    {
        key = _getch();
        if (key == 224)
        {
            key = _getch();
        }
        switch (key)
        {
        case 72: //up
            if (((CheckCrash(x + 2, y) == true) && (CheckCrash(x - 2, y) == true)) || (CheckCrash(x, y + 1) == true))
                break;
            blockRotation++;
            if (blockRotation >= 4)
                blockRotation = 0;
            if (CheckCrash(x, y) == false)
                break;
            else if (CheckCrash(x, y) == true)
            {
                if (x < 4)
                    x += 2;
                else if (x > 7)
                    x -= 2;
            }
            if (CheckCrash(x, y) == true)
            {
                x -= 2;
            }
            break;
        case 75: //left
            if (CheckCrash(x - 2, y) == false)
            {
                x -= 2;
                startGroundT = clock();
            }
            break;
        case 77: //right
            if (CheckCrash(x + 2, y) == false)
            {
                x += 2;
                startGroundT = clock();
            }
            break;
        case 80: //down
            if (CheckCrash(x, y + 1) == false)
            {
                y++;
            }
            break;
        case 32: //space
            while (!CheckCrash(x, y + 1))
                y++;
            space_drop = true;
            break;
        }
        DrawBlock();
    }
}

void SolidLine()
{
    int i = 3;
    for (int j = 1; j < 11; j++)
    {
        gotoxy(g + j * 2, h + i);
        setColor(7, 0);
        printf("_ ");
        setColor(15, 0);
    }
}

void server()
{

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
}

void client(bool first)
{
    //서버에 연결하는 과정 시작
    struct hostent* host;

    if (first)
    {
        printf("Server Address : ");       gets(sAddr);
    }
    sPort = 20;

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
}

void comms()
{
    if (mode == 3)
    {
        send(clntSock, send_space, sizeof(send_space), 0);         //정보 전송


        nRcv = recv(clntSock, recv_space, sizeof(recv_space), 0); //클라이언트에서 정보를 받음
        if (nRcv == SOCKET_ERROR) { //에러처리
            printf("Receive Error..\n");
            run = false;
            return;
        }

        int num = 0;
        for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                space2[i][j] = recv_space[num];
                num++;
            }
        }

        if (strcmp(message, "exit") == 0) { //종료문자 나중에 바꾸면 됨
            printf("Close Server Connection..\n");
            run = false;
            return;
        }
    }

    if (mode == 2)
    {
        nRcv = recv(clntSock, recv_space, sizeof(recv_space), 0); //클라이언트에서 정보를 받음
        if (nRcv == SOCKET_ERROR) { //에러처리
            printf("Receive Error..\n");
            run = false;
            return;
        }

        int num = 0;
        for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                space2[i][j] = recv_space[num];
                num++;
            }
        }

        if (strcmp(message, "exit") == 0) { //종료문자 나중에 바꾸면 됨
            printf("Close Server Connection..\n");
            run = false;
            return;
        }


        send(clntSock, send_space, sizeof(send_space), 0);         //정보 전송
    }
}

void check_gameover()
{
    for (int i = 0; i < 12; i++)
    {
        if (space[3][i] == 2)
        {
            run = false;
            for (int j = 0; j < 10; j++)
            {
                for (int k = 0; k < 21; k++)
                {
                    space[k + 1][j + 1] = 0;
                }
            }
            return;
        }
    }
}
