#include <stdio.h>
#include <windows.h> //��ǥ�� �̿��ϱ� ���ؼ�
#include <conio.h> //_kbhit�� _getch�� �̿��ϱ� ���ؼ�
#include <time.h>
#include <stdbool.h> //true or false�� ��Ÿ���� ���ؼ�

clock_t startDropT, endT, startGroundT; //�ð��� ���� ������ ����
int x = 8, y = 0;
int m = 10, n = 5;
int g = 20, h = 5;
RECT blockSize; //�簢�� ��ǥ�� �����ϱ� ���� �ڷ���
int blockForm;
int blockRotation = 0;
int key;

int block[7][4][4][4] = {
    { //T��� ��
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
    { //���� ��
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
    { //���� �� �ݴ�
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
    { //1���� ��
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
    { //L���� ��
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
    { //L���� �� �ݴ�
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
    { //�׸� ��
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
}; //(�α�) ���̸� 15���� 20���� �÷Ƚ��ϴ�.

void Init(); //�ܼ�â ������ ����, Ŀ�� �����
void gotoxy(int x, int y); //x��ǥ y��ǥ�� Ŀ�� �ű��
void CreateRandomForm(); //�������� 0~6���� ����
bool CheckCrash(int x, int y); //�浹�� �����ϴ� �Լ� �ϳ��� ��ġ�� ���� �ִٸ� true��ȯ
void DropBlock(); //0.8�ʸ��� ���� ��ĭ�� ������ ������ �Լ�
void BlockToGround();
void RemoveLine(); //1���� �Ǿ��ٸ� ���� �����մϴ�. �� �ٿ������� �����ؼ� ��ĭ�� �� ����ϴ�.
void DrawMap(); //���� ���¿� ���� ���� �׸���
void DrawBlock();
void InputKey();


int main() {

    Init();
    int z;
    int color = 1;
    while (_kbhit() == 0) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color % 16);	if (_kbhit()) break;
        gotoxy(0 + m, 0 + n); printf("�ââââââââââââââââââââââââ�"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 1 + n); printf("�á������������������������"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 2 + n); printf("�á������������������������"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 3 + n); printf("�á������������������������"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 4 + n); printf("�á������������������������"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 5 + n); printf("�á������������������������"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 6 + n); printf("�ââââââââââââââââââââââââ�"); Sleep(100); if (_kbhit()) break;
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

    startDropT = clock();
    CreateRandomForm();

    while (true)
    {
        DrawMap();
        DrawBlock();
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
    srand(time(NULL)); //srand(): rand()�� �ٿ�, time(null): �������� ��������� �帥�ð��� �ʴ����� ����
}

void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
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
                int t = space[i + y][j + x / 2]; //x��ǥ�� 2�� 1ĭ�� �ǹ��ϱ� ������
                if (t == 1 || t == 2) //��(=1)�� ��, �����Ⱥ�(=2)�� ��
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
    endT = clock(); //endT: �ð��� ���� ����, clock(): ���μ����� ���� ������� �ɸ� clock��
    if ((float)(endT - startDropT) >= 800) //(������� �ð� - ���۽��� �ð�)
    {
        if (CheckCrash(x, y + 1) == true) return;
        y++;
        startDropT = clock();
        startGroundT = clock();
        system("cls");
    }
}

void BlockToGround()
{
    if (CheckCrash(x, y + 1) == true)
    {
        if ((float)(endT - startGroundT) > 1500)
        { //���� �� ����
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
            CreateRandomForm(); //������ ���� ��(8,0)�� �ø�
        }
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
        if (cnt > 9) //������ �� ���ִٸ�
        {
            for (int j = 0; i - j >= 0; j++)
            {
                for (int x = 1; x < 11; x++)
                {
                    if (i - j - 1 >= 0)
                        space[i - j][x] = space[i - j - 1][x];
                    else //õ���̸� 0����
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
            if (space[i][j] == 1)
            {
                gotoxy(g + j * 2, h + i);
                printf("��");
            }
            else if (space[i][j] == 2)
            {
                gotoxy(g + j * 2, h + i);
                printf("��");
            }
        }
    }
}

void DrawBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[blockForm][blockRotation][i][j] == 1)
            {
                gotoxy(x + g + j * 2, y + h + i);
                printf("��");
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
                x -= 2;
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
                y++;
            break;
        case 32: //space
            while (!CheckCrash(x, y + 1))
                y++;
            break;
        }
        system("cls");
    }
}