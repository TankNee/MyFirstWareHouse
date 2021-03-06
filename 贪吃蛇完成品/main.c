#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<time.h>
#include <conio.h>
#define HIGH 25
#define WIDE 50
#define CHUSHI 5

int canvas[HIGH][WIDE]={0};//-3为食物，-2为边框，-1为毒药，0为空白，1为蛇头，大于1为蛇身
int i,j;
int movedirection;
int food_i,food_j;
int poison_i,poison_j;
float score;
float score_max;
void gotoxy(int x,int y)

{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle,pos);
}

void startup()
{
    for (i=0;i<HIGH;i++)
    {
        canvas[i][0]=-2;
        canvas[i][WIDE-1]=-2;
    }
    for (j=0;j<WIDE;j++)
    {
        canvas[0][j]=-2;
        canvas[HIGH-1][j]=-2;
    }
    canvas[HIGH/2][WIDE/2]=1;
    for(i=0;i<CHUSHI;i++)
    {
       canvas[HIGH/2][WIDE/2-i-1]=2+i;
    }
    movedirection=4;
    score=0;
    srand((unsigned)time(NULL));
    food_i=rand()%(HIGH-5)+2;
    food_j=rand()%(WIDE-5)+2;
    canvas[food_i][food_j]=-3;
    poison_i=rand()%(HIGH-8)+4;
    poison_j=rand()%(WIDE-8)+4;
    canvas[poison_i][poison_j]=-1;
    FILE *fpRead=fopen("rank.txt","r");
	if(fpRead==NULL)
	{
		return 0;
	}
	fscanf(fpRead,"%.0f ",score_max);


}
void show ()
{
   gotoxy(0,0);
   for(i=0;i<HIGH;i++)
   {
       for(j=0;j<WIDE;j++)
       {
           if(canvas[i][j]==0)
            {
                printf(" ");
            }
           else if(canvas[i][j]==-2)
           {
               printf("+");
           }
           else if(canvas[i][j]==1)
           {
               printf("@");
           }
           else if(canvas[i][j]>1)
           {
               printf("*");
           }
           else if(canvas[i][j]==-1)
           {
               printf("D");
           }
           else if(canvas[i][j]==-3)
           {
               printf("F");
           }
       }
       printf("\n");
   }
   Sleep(50);
   printf("上把最高分数为：%.0f\n",score_max);
   printf("暂停键“P”\n");
   printf("退出键“I”\n");
   printf("你的分数是：% .1f\n",score);
}

void SnakeDirection()
{

	for (i=1;i<HIGH-1;i++)
		for (j=1;j<WIDE-1;j++)
			if (canvas[i][j]>0)
				canvas[i][j]++;

	int oldTail_i,oldTail_j,oldHead_i,oldHead_j;
	int max = 0;

	for (i=1;i<HIGH-1;i++)
		for (j=1;j<WIDE-1;j++)
			if (canvas[i][j]>0)
			{
				if (max<canvas[i][j])
				{
					max = canvas[i][j];
					oldTail_i = i;
					oldTail_j = j;
				}
				if (canvas[i][j]==2)
				{
					oldHead_i = i;
					oldHead_j = j;
				}
			}

	int newHead_i,newHead_j;

	if (movedirection==1) // 向上移动
	{
		newHead_i = oldHead_i-1;
		newHead_j = oldHead_j;
	}
	if (movedirection==2) // 向下移动
	{
		newHead_i = oldHead_i+1;
		newHead_j = oldHead_j;
	}
	if (movedirection==3) // 向左移动
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j-1;
	}
	if (movedirection==4) // 向右移动
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j+1;
	}

	// 新蛇头如果吃到食物
	if (canvas[newHead_i][newHead_j]==-3)
	{
		canvas[food_i][food_j] = 0;
		// 产生一个新的食物
		food_i = rand()%(HIGH-5) + 2;
		food_j = rand()%(WIDE-5) + 2;
		canvas[food_i][food_j]=-3;
		score+=1;
	}
    else if(canvas[newHead_i][newHead_j]==-1)
    {
        canvas[poison_i][poison_j] = 0;
        poison_i=rand()%(HIGH-8) + 4;
        poison_j=rand()%(WIDE-8) + 4;
        canvas[poison_i][poison_j]= -1 ;
        canvas[oldTail_i][oldTail_j] = 0;
        for(i=0;i<HIGH;i++)
        {
            for(j=0;j<WIDE;j++)
            {
                if(canvas[i][j]==max-1)
                {
                    canvas[i][j]=0;
                }
            }
        }
        score-=1;
    }
    else
		canvas[oldTail_i][oldTail_j] = 0;


	if (canvas[newHead_i][newHead_j]>1 || canvas[newHead_i][newHead_j]==-2)
	{
		printf("游戏失败！\n");
		Sleep(2000);
		score_max=score;
		FILE *fpWrite=fopen("rank.txt","w");
        if(fpWrite==NULL)
        {
            return 0;
        }
        fprintf(fpWrite,"%.0f " ,score_max);
        fclose(fpWrite);
		exit(0);

	}
	else
		canvas[newHead_i][newHead_j] = 1;

}

void inputunconcerned()
{
    SnakeDirection();
}
void inputconcerned()
{
    char input;
   if(kbhit())
   {

    input=getch();
    if(input=='w'||input=='W')
    {
        movedirection=1;
        SnakeDirection();
    }
    if(input=='s'||input=='S')
    {
        movedirection=2;
        SnakeDirection();
    }
    if(input=='a'||input=='A')
    {
        movedirection=3;
        SnakeDirection();
    }
    if(input=='d'||input=='D')
    {
        movedirection=4;
        SnakeDirection();
    }
    if(input=='p'||input=='P')
    {
        printf("按任意按钮继续！\n");
        getch();

    }
    if(input=='I'||input=='i')
    {
        score_max=score;
        FILE *fpWrite=fopen("rank.txt","w");
        if(fpWrite==NULL)
        {
            return 0;
        }

        fprintf(fpWrite,"%.0f ",score_max);
        fclose(fpWrite);

        exit(0);
    }
   }
}
/*int main()
{

    startup();  // 数据初始化

	while (1) //  游戏循环执行
	{
		show();  // 显示画面
		inputunconcerned();
		inputconcerned();
	}
    return 0;
}*/

int main()
{

    startup();  // 数据初始化
	//下面是写数据，将数字0~9写入到data.txt文件中

    while (1) //  游戏循环执行
	{
		show();  // 显示画面
		inputunconcerned();
		inputconcerned();
	}

	return 1;
}

