/*
    Project: Hanoi Tower
    Authors: Vadim Olennikov
    Date of creation: 02.12.2020
    Date of change:   04.12.2020
*/
#define MAX_LEVEL 7
#define BACK_SPACE 8
#define WAIT (1 << 14)  //wait 2^arg milliseconds
#define FILLER_TOWER 219 //ASCII symbol code for tower, correct is 219
#define FILLER_STICK 186 //ASCII symbol code for stick
#define FILLER_STAND 206 //ASCII symbol code for stand
#define ENTER 13// 13 or 10, compiler dependent

#include <stdio.h>
#include <conio.h> //for scan_number
#include <stdlib.h> //for malloc
#include "windows.h" //for Sleep

void hanoi(int level, int startColumn, int endColumn, int **columns, int maxLevel);
void printHanoi(int **columns, int maxLevel);
int scan_number();

int main()
{
    int i, level = 1;
    int **columns;

    columns =(int **) malloc(sizeof(int *) * 3);
    if(columns == NULL)
        exit(1);

    while(level > 0)
    {
        printf("Enter tower size (max %i) or 0 for exit: ", MAX_LEVEL);
        level = scan_number();
        if(level > 0)
        {
            //create double dynamic array
            for(i = 0; i < 3; i++)
            {
                columns[i] =(int *) malloc(sizeof(int) * level);
                if(columns[i] == NULL)
                {
                    printf("Error! Try again!\n");
                    continue;
                }
            }

            //fill double dynamic array
            for(i = 0; i < level; i++)
            {
                    columns[0][i] = 1;
                    columns[1][i] = 0;
                    columns[2][i] = 0;
            }

            hanoi(level, 0, 1, columns, level);
            printf("\n\nComplete!\n\n");

            free(columns[0]);
            free(columns[1]);
            free(columns[2]);
        }
    }
    printf("\nGoodbye!\n");
    free(columns);
}

void hanoi(int currentLevel, int start, int end, int **columns, int maxLevel)
{
    int tmp;//index of temporary column

    if(currentLevel == 0)//recursion exit
        return;

    tmp = 3 - end - start;//3 is sum of column indexes
    hanoi(currentLevel - 1, start, tmp, columns, maxLevel);

    printHanoi(columns, maxLevel);
    //wait for WAIT / 2^maxLevel milliseconds
    Sleep(WAIT >> maxLevel);

    //move the disk with radius currentLevel from column start to end
    columns[start][currentLevel - 1] = 0;
    columns[end][currentLevel - 1] = 1;

    hanoi(currentLevel - 1, tmp, end, columns, maxLevel);

    if(currentLevel == maxLevel)
        printHanoi(columns, maxLevel);
}

void printHanoi(int **columns, int maxLevel)
{
    int i, j, k;
    int row, diskInd;
    int disksQuantity[3] = {0, 0, 0};
    int currentDiskSize[3] = {0, 0, 0};

    system("cls");//clear screen
    printf("\n");

    //count the disk quantity on every column
    for(i = 0; i < 3; i++)
        for(j = 0; j < maxLevel; j++)
            if(columns[i][j] == 1)
                disksQuantity[i]++;

    //print tower
    for(row = 0; row < maxLevel; row++)//print the row from up to down
    {
        for(i = 0; i < 3; i++)//print every column
        {
            if(maxLevel - row > disksQuantity[i])//no disks on this level
            {
                for(k = 0; k < maxLevel - 1; k++) printf(" ");
                printf("%c", FILLER_STICK);
                for(k = 0; k < maxLevel; k++) printf(" ");
            }
            else
            {
                diskInd = currentDiskSize[i];
                //find next disk in column
                while(columns[i][diskInd] != 1) diskInd++;
                currentDiskSize[i] = diskInd + 1;

                for(k = 0; k < maxLevel - currentDiskSize[i]; k++) printf(" ");
                for(k = 0; k < 2*currentDiskSize[i] - 1; k++) printf("%c", FILLER_TOWER);
                for(k = 0; k < maxLevel - currentDiskSize[i] + 1; k++) printf(" ");

            }
        }
        printf("\n");
    }
    //print the stand
    for(k = 0; k < 6*maxLevel; k++) printf("%c", FILLER_STAND);
}

//for scan tower height
int scan_number()
{
    int n = 0, max = MAX_LEVEL;
    int flag = 0;
    char c = 0;

    while(!((c = getch()) == ENTER && flag == 1))
    {
        if(c <= '9' && c >= '0')
            // check overflow, second condition if max < 10
            if(n <= (max - (c - '0'))/10 && n*10 + (c - '0') <= max)
                if(!(flag == 1 && n == 0))
                {
                    putch(c);
                    n = n*10 + (c - '0');
                    flag = 1;
                }

        if(c == BACK_SPACE)
        {
            if(flag == 0) continue;

            putch('\b');
            putch(' ');
            putch('\b');

            n /= 10;
            if(n == 0) flag = 0;
        }
    }

    putch('\n');
    return n;
}

/*for move cursor to (0, 0)
void goToXY(const int X,const int Y)
{
    HANDLE OutputHandle;
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
    OutputHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutputHandle,&ScreenBufInfo);
    ScreenBufInfo.dwCursorPosition.X=X;
    ScreenBufInfo.dwCursorPosition.Y=Y;
    SetConsoleCursorPosition(OutputHandle,ScreenBufInfo.dwCursorPosition);
}
*/
