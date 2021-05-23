#include <stdio.h>
#include <windows.h>                   //좌표를 이용하기 위해서
#include <conio.h>                      //_kbhit과 _getch를 이용하기 위해서
#include <time.h>                       //일정 시간 뒤에 작업을 하기 위해서
#include <stdbool.h>                    //true or false를 나타내기 위해서

clock_t startDropT, endT, startGroundT; //시간에 대한 변수를 선언
int x = 8, y = 0;                       //게임 필드를 그리기 시작하는 위치
int m = 10, n = 5;                      //맨 처음 메뉴화면 그리기 시작하는 위치
int g = 20, h = 5;
RECT blockSize;                         //사각형 좌표를 저장하기 위한 자료형
int blockForm;                          //블럭의 종류
int blockRotation = 0;                  //블럭 회전
int key;                                //입력한 키
bool space_drop = false;                //스페이스바를 누르면 바로 떨어뜨리고 바로 블록의 위치 고정 <- 살짝 애매하게 구현
bool run = true;                        //참값이면 실행 아니면 종료


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

int space[20 + 1][10 + 2] = {
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
}; //(민규) 높이를 15에서 20으로 늘렸습니다.

void Init();                           //콘솔창 깜빡이 방지, 커서 숨기기
void gotoxy(int x, int y);             //x좌표 y좌표로 커서 옮기기
void setColor(unsigned short text, unsigned short back); //글자색, 배경색 설정
void CreateRandomForm();               //랜덤으로 0~6값을 선언
bool CheckCrash(int x, int y);         //충돌을 감지하는 함수 하나라도 겹치는 것이 있다면 true반환
void DropBlock();                      //0.8초마다 블럭을 한칸씩 밑으로 내리는 함수
void EraseField();                     //블록이 그려지는 필드를 지우는 함수
void DrawField();                      //블록이 그려지는 필드를 그리는 함수
void BlockToGround();                  //블럭이 땅바닥에 닿아 있는지 확인하는 함수
void RemoveLine();                     //1줄이 되었다면 블럭을 제거합니다. 그 줄에서부터 시작해서 한칸씩 다 땡깁니다.
void DrawMap();                        //맵의 형태(가장자리 태두리)만 그리기
void DrawBlock();                      //블럭을 그리기
void InputKey();                       //키 입력받기


int main() {

    Init();
    int z;
    int color = 1;
    while (_kbhit() == 0) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color % 16);	if (_kbhit()) break;
        gotoxy(0 + m, 0 + n); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 1 + n); printf("▣■■■  ■■■  ■■■  ■■    ■■■  ■■■▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 2 + n); printf("▣  ■    ■        ■    ■  ■    ■    ■    ▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 3 + n); printf("▣  ■    ■■■    ■    ■■      ■    ■■■▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 4 + n); printf("▣  ■    ■        ■    ■  ■    ■        ■▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 5 + n); printf("▣  ■    ■■■    ■    ■  ■  ■■■  ■■■▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 6 + n); printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 7 + n); printf("                                                  "); Sleep(100); if (_kbhit()) break;
        gotoxy(13 + m, 9 + n); printf("Press any key to start");
        for (z = 0; z < 50; z++) {
            Sleep(30); if (_kbhit()) { y = 1; break; }
        }
        color++;
        if (color % 16 == 0)
            color++;
        if (y == 1)
            break;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    startDropT = clock();
    CreateRandomForm();
    system("cls");
    DrawMap();


    while (true)
    {
        DropBlock();
        BlockToGround();
        RemoveLine();
        InputKey();
    }
    return 0;
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
    for (int i = 20; i >= 0; i--)
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
    gotoxy(0, 0);
    for (int i = 0; i < 21; i++)
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
            else if (space[i][j] == 2)
            {
                setColor(15, 14);
                printf("  ");
                setColor(15, 0);
            }
        }
    }
}

void DrawBlock()
{
    int min = 100, max = 0;
    EraseField();


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[blockForm][blockRotation][i][j] == 1)
            {
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


    for (int i = 0; i <= 19; i++)
    {
        for (int j = min; j <= max; j++)
        {
            gotoxy(j, h + i);
            setColor(0, 8);
            printf("  ");
            setColor(15, 0);
        }
    }

    gotoxy(30, 30);
    printf("                  ");
    gotoxy(30, 30);
    printf("%d, %d", min, max);


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
    for (int i = 0; i <= 19; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            gotoxy(g + j * 2, h + i);
            printf("  ");
        }

    }
}


void DrawField()
{
    for (int i = 0; i <= 19; i++)
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
        /*system("cls");*/
    }
}