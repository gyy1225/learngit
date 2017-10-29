#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
 
#define U 1
#define D 2
#define L 3 
#define R 4      


 struct SNAKE //�����һ���ڵ�
{
    int x;
    int y;
    struct SNAKE *next;
}snake;
 
//ȫ�ֱ���//
int thetruewaytoclose=1;
const int N=3;
int node=4;
struct SNAKE *nexthead,*nexthead_1,*nexthead_2;//���壬��Ϊ��һ����λ��
int status,sleeptime=300;//ÿ�����е�ʱ����(ms)
struct SNAKE *head, *food,*tail; //��ͷָ�룬ʳ��ָ��
struct SNAKE *q;//�����ߵ�ʱ���õ���ָ��
int endthegame=0; //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��
int score=0; 
 
//����ȫ������
void findfood();
void Pos();//������ 
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
 //��̫�����¹����ƵĲ��� ���ⲿ�����ϳ��� �� 
void Pos(int x,int y)//pos��ʵ��һ���ṹ�����������X��Y�����ĳ�Ա��ͨ���޸�pos.X��pos.Y��ֵ�Ϳ���ʵ�ֹ���λ�ÿ��ơ�
		 //���ù��λ��
{
    COORD pos;//WINDOWS API����Ľṹ  ��ʾһ���ַ��ڿ���̨��Ļ�ϵ�����   ���Ͻ�(0,0)
	HANDLE hOutput;//��� 
    pos.X=x;
    pos.Y=y;
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);//����Ϊ��׼����ľ��
    SetConsoleCursorPosition(hOutput,pos);//SetConsoleCursorPosition��API�ж�λ���λ�õĺ����������꣬�����꣩
}
 
void creatMap()//������ͼ
{
    int i;
    for(i=0;i<72;i+=2)//��ӡ���±߿�ע��������������2
    {
        Pos(i,0);
        printf("��");
        Pos(i,26);    //������
        printf("��");

    }
    for(i=1;i<26;i++)//��ӡ���ұ߿�
    {
        Pos(0,i);
        printf("��");                      
        Pos(70,i);
        printf("��");   

    }
	
}

void initsnake()//��ʼ������
{
    
    int i;
    tail=(struct SNAKE*)malloc(sizeof(snake));//��x,y�趨��ʼ��λ�á�����
    tail->x=24;//(���������λ�ã���Ϊ���������λ��)
    tail->y=5;
    tail->next=NULL;
    for(i=1;i<=N;i++)
    {
        head=(struct SNAKE*)malloc(sizeof(snake));
        head->next=tail;
		
        
		head->x=24+2*i;
		head->y=5;
		
		
        tail=head;//��֤head������ϵͳҪ�ռ䣬������һƬ���򣨼���ͷ����һ��
    }
    while(tail!=NULL)//��ͷ��β���������
    {
        Pos(tail->x,tail->y);//�ƶ���굽ѭ����ǰλ��
         printf("��");//��ӡ����
        tail=tail->next;//ָ����һ��
    }
}
 
int biteitself()//�ж��Ƿ�ҧ�����Լ�
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
 
void createfood()//�������ʳ��
{
    struct SNAKE *food_1;//û������ר�Ŷ���food�ṹ
    srand((unsigned)time(NULL));//���������������һ���ã����ܱ�֤����������ṩ����
    food_1=(struct SNAKE*)malloc(sizeof(snake));
    food_1->x=rand()%68+2;
    if(food_1->x%2!=0)
    {
    	food_1->x++;
	}
    food_1->y=rand()%24+1;
    q=head;                 

		
    while(q->next==NULL)//����������ǵ�ʳ���������ϳ��ֵ������ 
	{
    
		if(q->x==food_1->x && q->y==food_1->y) //�ж������Ƿ���ʳ���غϣ��൱�ڳ�ʳ���ˣ�
    
		{
         free(food_1);//�ͷ�
	
         createfood();
		}
     q=q->next;
	}
    
    Pos(food_1->x,food_1->y);
    food=food_1;//�ṹ��ĸ�ֵ...
    printf("��");
}



void findfood()
{   int i;
	if(nexthead->x==food->x && nexthead->y==food->y)//�����һ����ʳ��                
        {
            nexthead->next=head;//����nexthead
            head=nexthead;      //�滻��nexthead
            q=head;             //׼������
            while(q!=NULL)
            {
                Pos(q->x,q->y);
                printf("��");
                q=q->next;
            }
            createfood();
			node++;
			score+=10;
			Pos(75,12);
			printf("your score:%d\n",score) ;
			
        }
        else                                               //���û��ʳ������
        {                                                  //���üӷ���
            nexthead->next=head;
            head=nexthead;
            q=head;
            for(i=1;i<node;i++)
            {
                Pos(q->x,q->y);
                printf("��");
                q=q->next;        
            }
            Pos(q->next->x,q->next->y);
            printf("  ");
            free(q->next);
            q->next=NULL;
        }
		
}


                                                                      
void snakemove()//��ǰ��,��UP,��DOWN,��LELF,��RIGHT
{
	thetruewaytoclose=0;
    
    nexthead=(struct SNAKE*)malloc(sizeof(snake));
	nexthead_1=(struct SNAKE*)malloc(sizeof(snake));
    nexthead_2=(struct SNAKE*)malloc(sizeof(snake));
    if(status==U)  
    {
        nexthead->x=head->x;//�ȹ滮��nextheadλ�á�
        nexthead->y=head->y-1;//�������ߣ�������-1��
		
        findfood();
		nexthead_1->x	=head->x;//�ȹ滮��nextheadλ�á�
        nexthead_1->y=head->y-1;//�������ߣ�������-1��
		
    }
    if(status==D)                 //������
    {                             //y+1������
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
    if(biteitself()==1)       //�ж��Ƿ��ҧ���Լ�
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
 
void pause()//��ͣ
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))//����ո�   

        {
            break;
        }
         
    }
}
 
void gamecircle()//������Ϸ        
{   int j=1;
//������ʾ��λ����ʾ���Ҳ�
    Pos(75,14);
    printf("����ײ��ǽŶ\n");
    Pos(75,15);                               
    printf("Ҳ����ҧ���Լ�\n");
    Pos(75,16);
    printf("�á�.��.��.���ֱ�");
	Pos(75,17);
	printf("�����ߵ��ƶ�.");
    Pos(75,18);
    printf("E�����٣�Q������\n");
    Pos(75,19);
    printf("ESC ���˳���Ϸ.");
	Pos(75,20);
	printf("space����ͣ��Ϸ.");
    

	status=R;

    while(1)
    {
        
        if(GetAsyncKeyState(VK_UP) && status!=D)//�������ϡ����Ҳ�����������
        {
            status=U;
        }
        else if(GetAsyncKeyState(VK_DOWN) && status!=U)//����ͬ��
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
        else if(GetAsyncKeyState(VK_SPACE))//������ո�����pause����...�������´ΰ��ո�֮ǰ����sleep����ѭ��
        {
            pause();
        }
        else if(GetAsyncKeyState(VK_ESCAPE))//��esc
        {
            endthegame=3;            // ���������
            break;
        }
        else if(GetAsyncKeyState(0X45))//ΪС�߼���
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
 
void welcometogame()//��ʼ����
{
    Pos(40,12);

	system("title ");
    printf("��ӭ����  ���׵�С��  ��Ϸ��");
    Pos(40,25);
    system("pause");//���Զ�����Ļ
    system("cls");  //��������
    Pos(25,12);
    printf("�á�.��.��.���ֱ�����ߵ��ƶ��� E Ϊ���٣�Q Ϊ����\n");
    Pos(5,13);
    printf("���ٽ��ܵõ����ߵķ�������ҲҪС�Ĳ�Ҫ̫��Ŷ��"); 
    system("cls");
}
 
void endgame()//������Ϸ
{
    thetruewaytoclose=1;
    system("cls");
    Pos(24,8);
    if(endthegame==1)
    {
        printf("�Բ�����ײ��ǽ�ˡ���Ϸ����.");
    }
    else if(endthegame==2)
    {
        printf("�Բ�����ҧ���Լ��ˡ���Ϸ����.");
    }
    else if(endthegame==3)
    {
        printf("�����Լ���Ը��������Ϸ��");
    }
    printf("\n");
    Pos(24,10);
    printf("���ķ���Ϊ%d\n\n\n\n",score);
    system("pause");
    exit(0);
}
 
void gamestart()//��Ϸ��ʼ��
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





