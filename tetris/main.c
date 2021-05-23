#include <stdio.h>
#include <windows.h>                   //��ǥ�� �̿��ϱ� ���ؼ�
#include <conio.h>                      //_kbhit�� _getch�� �̿��ϱ� ���ؼ�
#include <time.h>                       //���� �ð� �ڿ� �۾��� �ϱ� ���ؼ�
#include <stdbool.h>                    //true or false�� ��Ÿ���� ���ؼ�

clock_t startDropT, endT, startGroundT; //�ð��� ���� ������ ����
int x = 8, y = 0;                       //���� �ʵ带 �׸��� �����ϴ� ��ġ
int m = 10, n = 5;                      //�� ó�� �޴�ȭ�� �׸��� �����ϴ� ��ġ
int g = 20, h = 5;
RECT blockSize;                         //�簢�� ��ǥ�� �����ϱ� ���� �ڷ���
int blockForm;                          //���� ����
int blockRotation = 0;                  //�� ȸ��
int key;                                //�Է��� Ű
bool space_drop = false;                //�����̽��ٸ� ������ �ٷ� ����߸��� �ٷ� ����� ��ġ ���� <- ��¦ �ָ��ϰ� ����
bool run = true;                        //�����̸� ���� �ƴϸ� ����


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

void Init();                           //�ܼ�â ������ ����, Ŀ�� �����
void gotoxy(int x, int y);             //x��ǥ y��ǥ�� Ŀ�� �ű��
void setColor(unsigned short text, unsigned short back); //���ڻ�, ���� ����
void CreateRandomForm();               //�������� 0~6���� ����
bool CheckCrash(int x, int y);         //�浹�� �����ϴ� �Լ� �ϳ��� ��ġ�� ���� �ִٸ� true��ȯ
void DropBlock();                      //0.8�ʸ��� ���� ��ĭ�� ������ ������ �Լ�
void EraseField();                     //����� �׷����� �ʵ带 ����� �Լ�
void DrawField();                      //����� �׷����� �ʵ带 �׸��� �Լ�
void BlockToGround();                  //���� ���ٴڿ� ��� �ִ��� Ȯ���ϴ� �Լ�
void RemoveLine();                     //1���� �Ǿ��ٸ� ���� �����մϴ�. �� �ٿ������� �����ؼ� ��ĭ�� �� ����ϴ�.
void DrawMap();                        //���� ����(�����ڸ� �µθ�)�� �׸���
void DrawBlock();                      //���� �׸���
void InputKey();                       //Ű �Է¹ޱ�


int main() {

    Init();
    int z;
    int color = 1;
    while (_kbhit() == 0) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color % 16);	if (_kbhit()) break;
        gotoxy(0 + m, 0 + n); printf("�ââââââââââââââââââââââââ�"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 1 + n); printf("�á���  ����  ����  ���    ����  �����"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 2 + n); printf("��  ��    ��        ��    ��  ��    ��    ��    ��"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 3 + n); printf("��  ��    ����    ��    ���      ��    �����"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 4 + n); printf("��  ��    ��        ��    ��  ��    ��        ���"); Sleep(100); if (_kbhit()) break;
        gotoxy(0 + m, 5 + n); printf("��  ��    ����    ��    ��  ��  ����  �����"); Sleep(100); if (_kbhit()) break;
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
    srand(time(NULL)); //srand(): rand()�� �ٿ�, time(null): �������� ��������� �帥�ð��� �ʴ����� ����
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
        DrawBlock();
    }
}

void BlockToGround()
{
    if (CheckCrash(x, y + 1) == true)
    {
        if ((float)(endT - startGroundT) > 200 || space_drop) // ���� ���ٴڿ� ������ 0.2���� ����
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
    if (space_drop)
    {
        endT = startDropT + 800;//�������μ��� �� ����� �ٷ�
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