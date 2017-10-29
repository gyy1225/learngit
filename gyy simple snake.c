#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
 
#define U 1
#define D 2
#define L 3 
#define R 4      


 struct SNAKE //蛇身的一个节点
{
    int x;
    int y;
    struct SNAKE *next;
}snake;
 
//全局变量//
int thetruewaytoclose=1;
const int N=3;
int node=4;
struct SNAKE *nexthead,*nexthead_1,*nexthead_2;//定义，意为下一步的位置
int status,sleeptime=300;//每次运行的时间间隔(ms)
struct SNAKE *head, *food,*tail; //蛇头指针，食物指针
struct SNAKE *q;//遍历蛇的时候用到的指针
int endthegame=0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。
int score=0; 
 
//声明全部函数
void findfood();
void Pos();//光标控制 
void creatMap();
void initsnake();
int biteitself();
void createfood(); 
void snakemove();
void pause();
void gamecircle();
void welcometogame();
void endgame();
void gamestart();
 //不太懂以下光标控制的部分 （这部分网上抄的 ） 
void Pos(int x,int y)//pos其实是一个结构体变量，其中X和Y是它的成员，通过修改pos.X和pos.Y的值就可以实现光标的位置控制。
		 //设置光标位置
{
    COORD pos;//WINDOWS API定义的结构  表示一个字符在控制台屏幕上的坐标   左上角(0,0)
	HANDLE hOutput;//句柄 
    pos.X=x;
    pos.Y=y;
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);//定义为标准输出的句柄
    SetConsoleCursorPosition(hOutput,pos);//SetConsoleCursorPosition是API中定位光标位置的函数（横坐标，纵坐标）
}
 
void creatMap()//创建地图
{
    int i;
    for(i=0;i<72;i+=2)//打印上下边框，注：方块横坐标需加2
    {
        Pos(i,0);
        printf("■");
        Pos(i,26);    //纵坐标
        printf("■");

    }
    for(i=1;i<26;i++)//打印左右边框
    {
        Pos(0,i);
        printf("■");                      
        Pos(70,i);
        printf("■");   

    }
	
}

void initsnake()//初始化蛇身
{
    
    int i;
    tail=(struct SNAKE*)malloc(sizeof(snake));//以x,y设定开始的位置。分配
    tail->x=24;//(横坐标出现位置，下为纵坐标出现位置)
    tail->y=5;
    tail->next=NULL;
    for(i=1;i<=N;i++)
    {
        head=(struct SNAKE*)malloc(sizeof(snake));
        head->next=tail;
		
        
		head->x=24+2*i;
		head->y=5;
		
		
        tail=head;//保证head继续向系统要空间，开辟下一片区域（即蛇头在扩一格）
    }
    while(tail!=NULL)//从头到尾，输出蛇身
    {
        Pos(tail->x,tail->y);//移动光标到循环当前位置
         printf("■");//打印蛇身
        tail=tail->next;//指向下一段
    }
}
 
int biteitself()//判断是否咬到了自己
{
    struct SNAKE *self;
    self=head->next;
    while(self!=NULL)
    {
        if(self->x==head->x && self->y==head->y)
        {
            return 1;
        }
        self=self->next;
    }
    return 0;
}
 
void createfood()//随机出现食物
{
    struct SNAKE *food_1;//没有重新专门定义food结构
    srand((unsigned)time(NULL));//与生成随机数函数一起用，才能保证真正随机。提供种子
    food_1=(struct SNAKE*)malloc(sizeof(snake));
    food_1->x=rand()%68+2;
    if(food_1->x%2!=0)
    {
    	food_1->x++;
	}
    food_1->y=rand()%24+1;
    q=head;                 

		
    while(q->next==NULL)//这个遍历考虑到食物在蛇身上出现的情况。 
	{
    
		if(q->x==food_1->x && q->y==food_1->y) //判断蛇身是否与食物重合（相当于吃食物了）
    
		{
         free(food_1);//释放
	
         createfood();
		}
     q=q->next;
	}
    
    Pos(food_1->x,food_1->y);
    food=food_1;//结构体的赋值...
    printf("■");
}



void findfood()
{   int i;
	if(nexthead->x==food->x && nexthead->y==food->y)//如果下一个有食物                
        {
            nexthead->next=head;//接上nexthead
            head=nexthead;      //替换掉nexthead
            q=head;             //准备遍历
            while(q!=NULL)
            {
                Pos(q->x,q->y);
                printf("■");
                q=q->next;
            }
            createfood();
			node++;
			score+=10;
			Pos(75,12);
			printf("your score:%d\n",score) ;
			
        }
        else                                               //如果没有食物的情况
        {                                                  //不用加方块
            nexthead->next=head;
            head=nexthead;
            q=head;
            for(i=1;i<node;i++)
            {
                Pos(q->x,q->y);
                printf("■");
                q=q->next;        
            }
            Pos(q->next->x,q->next->y);
            printf("  ");
            free(q->next);
            q->next=NULL;
        }
		
}


                                                                      
void snakemove()//蛇前进,上UP,下DOWN,左LELF,右RIGHT
{
	thetruewaytoclose=0;
    
    nexthead=(struct SNAKE*)malloc(sizeof(snake));
	nexthead_1=(struct SNAKE*)malloc(sizeof(snake));
    nexthead_2=(struct SNAKE*)malloc(sizeof(snake));
    if(status==U)  
    {
        nexthead->x=head->x;//先规划好nexthead位置。
        nexthead->y=head->y-1;//蛇向上走（纵坐标-1）
		
        findfood();
		nexthead_1->x	=head->x;//先规划好nexthead位置。
        nexthead_1->y=head->y-1;//蛇向上走（纵坐标-1）
		
    }
    if(status==D)                 //向下走
    {                             //y+1纵坐标
        nexthead->x=head->x;
        nexthead->y=head->y+1;
		
        findfood();
		nexthead_1->x=head->x;
        nexthead_1->y=head->y+1;
    }
    if(status==L)                         
    {
        nexthead->x=head->x-2;
        nexthead->y=head->y;
	
        findfood();
		nexthead_1->x=head->x-2;
        nexthead_1->y=head->y;
    }
    if(status==R)                         
    { 
        nexthead->x=head->x+2;
        nexthead->y=head->y;
		
		
        findfood();
		nexthead_1->x=head->x+2;
        nexthead_1->y=head->y;
    }
    if(biteitself()==1)       //判断是否会咬到自己
    {
        endthegame=2;    
        endgame();
    }
    if (head->x==0||head->x==70||head->y==0||head->y==25)
    {
    	endthegame=1;
    	endgame();	
	}
}
 
void pause()//暂停
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))//输入空格   

        {
            break;
        }
         
    }
}
 
void gamecircle()//控制游戏        
{   int j=1;
//语言提示，位置显示在右侧
    Pos(75,14);
    printf("不能撞到墙哦\n");
    Pos(75,15);                               
    printf("也不能咬到自己\n");
    Pos(75,16);
    printf("用↑.↓.←.→分别");
	Pos(75,17);
	printf("控制蛇的移动.");
    Pos(75,18);
    printf("E键加速，Q键减速\n");
    Pos(75,19);
    printf("ESC ：退出游戏.");
	Pos(75,20);
	printf("space：暂停游戏.");
    

	status=R;

    while(1)
    {
        
        if(GetAsyncKeyState(VK_UP) && status!=D)//按键“上”，且不正在往下走
        {
            status=U;
        }
        else if(GetAsyncKeyState(VK_DOWN) && status!=U)//以下同理
        {
            status=D;
        }
        else if(GetAsyncKeyState(VK_LEFT)&& status!=R)
        {
            status=L;
        }
        else if(GetAsyncKeyState(VK_RIGHT)&& status!=L)
        {
            status=R;
        }
        else if(GetAsyncKeyState(VK_SPACE))//如果按空格，跳到pause函数...并且在下次按空格之前，都sleep无限循环
        {
            pause();
        }
        else if(GetAsyncKeyState(VK_ESCAPE))//按esc
        {
            endthegame=3;            // 第三种情况
            break;
        }
        else if(GetAsyncKeyState(0X45))//为小蛇加速
        {
            if(sleeptime>=50)
            {   
                sleeptime=sleeptime-10;
            }
			
        }
		
        else if(GetAsyncKeyState(0X51))
        {
            if(sleeptime<550)
            {   
                sleeptime=sleeptime+30;
            }
			
        }
		
		j++;

        Sleep(sleeptime);
        snakemove();
    }
}
 
void welcometogame()//开始界面
{
    Pos(40,12);

	system("title ");
    printf("欢迎来到  简易的小蛇  游戏！");
    Pos(40,25);
    system("pause");//可以冻结屏幕
    system("cls");  //可以清屏
    Pos(25,12);
    printf("用↑.↓.←.→分别控制蛇的移动， E 为加速，Q 为减速\n");
    Pos(5,13);
    printf("加速将能得到更高的分数。但也要小心不要太快哦！"); 
    system("cls");
}
 
void endgame()//结束游戏
{
    thetruewaytoclose=1;
    system("cls");
    Pos(24,8);
    if(endthegame==1)
    {
        printf("对不起，您撞到墙了。游戏结束.");
    }
    else if(endthegame==2)
    {
        printf("对不起，您咬到自己了。游戏结束.");
    }
    else if(endthegame==3)
    {
        printf("您按自己意愿结束了游戏。");
    }
    printf("\n");
    Pos(24,10);
    printf("您的分数为%d\n\n\n\n",score);
    system("pause");
    exit(0);
}
 
void gamestart()//游戏初始化
{
    welcometogame();
    creatMap();
    initsnake();
    createfood();
}
 
int main()
{
    gamestart();
    gamecircle();
    endgame();
	return 0;
}





