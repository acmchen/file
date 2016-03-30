//cleaner.cpp
//code::blocks 12.11  MinGW4.8   ͨ��


#include<iostream>
#include<ctime>
#include<windows.h>
#include<conio.h>
#include <stdio.h>


//���������Σ�������MSDN�������� system("cls")��ͬ������ʹ��system("cls")�����޷���ȡ�������,ԭ��֪
#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__);}
void MyCls(HANDLE) ;
void clrscr(void)
{
HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
MyCls(hStdOut);
return;
}
void MyCls(HANDLE hConsole)
{
COORD coordScreen={0,0};//�����������귵�ص���Ļ���Ͻ�����
BOOL bSuccess;
DWORD cCharsWritten;
CONSOLE_SCREEN_BUFFER_INFO csbi;//���滺������Ϣ
DWORD dwConSize;//��ǰ�����������ɵ��ַ���
bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);//��û�������Ϣ
PERR(bSuccess,"GetConsoleScreenBufferInfo");
dwConSize=csbi.dwSize.X * csbi.dwSize.Y;//�����������ַ���Ŀ
//�ÿո���仺����
bSuccess=FillConsoleOutputCharacter(hConsole,(TCHAR)' ',dwConSize,coordScreen,&cCharsWritten);
PERR(bSuccess,"FillConsoleOutputCharacter");
bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);//��û�������Ϣ
PERR(bSuccess,"ConsoleScreenBufferInfo");
//��仺��������
bSuccess=FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,coordScreen,&cCharsWritten);
PERR(bSuccess,"FillConsoleOutputAttribute");
//��귵����Ļ���Ͻ�����
bSuccess=SetConsoleCursorPosition(hConsole,coordScreen);
PERR(bSuccess,"SetConsoleCursorPosition");
return;
}

//


#define Map_size 20 //��ͼ��С��dos�������и�
#define mine 30 //������Ŀ


HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);     //��ȡ�����������
HANDLE Hin=GetStdHandle(STD_INPUT_HANDLE);
COORD pos={0,0};
INPUT_RECORD mouse;
DWORD res;

int show();  //ɨ�׺���
int lay_mine();  //���õ���
int count_mine();//��Χ���׼���
int primary_menu();//���˵�
void white(int i,int j); //չ����ɫ
int mine_cleaner(); //ɨ��������
int time();


SYSTEMTIME   st1,st2;
int Num_mine[Map_size][Map_size];   //��Χ���׼���
bool TorF_mine[Map_size][Map_size]; //�Ƿ�Ϊ��
bool state[Map_size][Map_size]; //�Ƿ���ʾ������flaseΪû��trueΪ��ʾ��
bool gameover=false; //��Ϸ����
bool mine_mark[Map_size][Map_size];//flaseΪû�б��

using namespace std;
int temppx,temppy,temppi;  //temppx,temppyȫ�ֱ����洢������ꣻtemppi���ڷ�ֹ��Ļ����ˢ��
int mouseinput()  //��ȡ�������
{
    ReadConsoleInput(Hin,&mouse,1,&res);
    pos = mouse.Event.MouseEvent.dwMousePosition;
    if (mouse.EventType==MOUSE_EVENT&&mouse.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
            temppx=pos.Y-1;
            temppy=pos.X/2-9;
            temppi=1;
    }
    if (mouse.EventType==MOUSE_EVENT&&mouse.Event.MouseEvent.dwButtonState==RIGHTMOST_BUTTON_PRESSED&&state[pos.Y-2][pos.X/2-10]==false&&
        mine_mark[pos.Y-2][pos.X/2-10]==false)
        {
             mine_mark[pos.Y-2][pos.X/2-10]=true;
             temppi=1;
        }
 /*  if (mouse.EventType==MOUSE_EVENT&&mouse.Event.MouseEvent.dwButtonState==RIGHTMOST_BUTTON_PRESSED&&state[pos.Y-2][pos.X/2-10]==true&&state[pos.Y-2][pos.X/2-10]==false)
        {
             mine_mark[pos.Y-2][pos.X/2-10]=false;
             temppi=1;

        }
*/
}

int main()
{
    //system("mode con:cols=41 lines=28");    //���ô�СΪ41*28
    //system("color 3A");   // ������ɫ
    primary_menu();
    CloseHandle(hOut);
    CloseHandle(Hin);

}

int lay_mine()  //������
{
    srand((unsigned)time(NULL));
    cout<<"\n\n";
    for(int x=0;x<Map_size;x++)          //���Ƶ�ͼ
	{
	    cout<<"                    ";
		for(int y=0;y<Map_size;y++)

				cout<<"��";

		cout<<endl;
	}
    for(int x=0;x<Map_size;x++)         //��ʼ����
	{
		for(int y=0;y<Map_size;y++)

				{
				    TorF_mine[x][y]=false;
				    state[x][y]=false;
                    Num_mine[x][y]=0;
                    mine_mark[x][y]=false;
				}

	}
    for(int q=0;q<mine;q++)
    {
        int i=rand()%20;
        int j=rand()%20;
        if(TorF_mine[i][j]==false)
            {
                TorF_mine[i][j]=true;
            }
    }



}
int count_mine()   //��Χ�׼���
{
    int d[16]={ 0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1, -1,0, -1,1};  //��������
    for(int x=0;x<Map_size;x++)
	{
		for(int y=0;y<Map_size;y++)
        {
             for(int i=0;i!=8;i++)
             {
                 int tmp_x=x+d[i*2];
                 int tmp_y=y+d[i*2+1];
                 if(tmp_x>=0&&tmp_y>=0&&TorF_mine[x][y]==false)    //�жϳ���
                 {
                     if( TorF_mine[tmp_x][tmp_y] ==true)
                     {
                         Num_mine[x][y]++;
                     }

                 }
             }
        }
	}
}

void white(int i,int j)  //չ����ɫ
{
    if(state[i][j]==false&&TorF_mine[i][j]==false)
    {
        if(i<0||i>Map_size-1||j<0||j>Map_size-1)//����
			{
			    return;
			}
        if(Num_mine[i][j]>0)
            {
                state[i][j]=true;
                return;
            }
        else
        {
            state[i][j]=1;
			white(i-1,j-1);
			white(i-1,j);
			white(i-1,j+1);
			white(i+1,j-1);
			white(i+1,j);
			white(i+1,j+1);
			white(i,j+1);
			white(i,j-1);

        }

    }

}


int show()
{
    int x,y;
    x=temppx;
    y=temppy;
    clrscr();
    if(TorF_mine[x-1][y-1]==true&&mine_mark[x-1][y-1]==false)
    {
        state[x-1][y-1]=true;
        Num_mine[x-1][y-1]=9;
        cout<<"\n\n\n\n\n                         You Lose!"<<"\n\n\n\n\n\n\n\n\n";
        cout<<"                    ���ƣ���ȵ����ˡ�������\n\n\n\n";
        time();
        gameover=true;
        return 0;
    }
    if (mine_mark[x-1][y-1]==false)
    {
         white(x-1,y-1);
    }
   cout<<"\n\n";
    for(int i=0;i<Map_size;i++)
	{
	    cout<<"                    ";
		for(int j=0;j<Map_size;j++)
		{
			if(state[i][j]==true&&mine_mark[i][j]==false)
			{
				if(Num_mine[i][j]==0)
					cout<<"��";
				else if(Num_mine[i][j]==1)
					cout<<"��";
				else if(Num_mine[i][j]==2)
					cout<<"��";
				else if(Num_mine[i][j]==3)
					cout<<"��";
				else if(Num_mine[i][j]==4)
					cout<<"��";
				else if(Num_mine[i][j]==5)
					cout<<"��";
				else if(Num_mine[i][j]==6)
					cout<<"��";
				else if(Num_mine[i][j]==7)
					cout<<"��";
				else if(Num_mine[i][j]==8)
					cout<<"��";
                else
					cout<<"��";
			}
			if (mine_mark[i][j]==true)
                cout<<"��";
            if (mine_mark[i][j]==false&&state[i][j]==false)
                cout<<"��";
		}
		cout<<endl;
	}
	time();
	int count=0;
	for(int i=0;i<Map_size;i++)
		for(int j=0;j<Map_size;j++)
			{
				if(TorF_mine[i][j]==false&&state[i][j]==true)
					count++;
			}

    if(count==Map_size*Map_size-mine)
    {
        cout<<"You win"<<endl;
        time();
        system("pause");
        gameover=true;
    }

}


int primary_menu()  //���˵�
{
    HWND hwnd=GetForegroundWindow();
    SetWindowText(hwnd,"ɨ����Ϸ");
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                 ɨ����Ϸ    "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                              �س�������Ϸ                "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                                 "<<endl;
    cout<<"       tips:��������������ɨ�ף���������Ҽ����б�ǣ���Ļ�·���ʱ����ʾ"<<endl;
    cout<<"            ��Ϸ�����ܽ�����Ŷ���������Ƿ����                        "<<endl;
    cout<<"\n\n";
    cout<<"       ͼ���� ��: û��ɨ������                               "<<endl;
    cout<<"              �����Ѿ�ɨ��������                        "<<endl;
    cout<<"              �٣���ʾ��Χ��һ���� "<<endl;
    cout<<"              ��: ��ʾ��Χ��������"<<endl;
    cout<<"              ����Ϊ��                          "<<endl;
    cout<<"                                     "<<endl;



    cin.get();
    clrscr();
    mine_cleaner();

}

int mine_cleaner()
{
    GetLocalTime(&st1);
    lay_mine();
    count_mine();
    do    //���������Ҫ�Ƿ�ֹ���һ�ƶ�����Ļ��ˢ��
    {
        temppi=2;
        do
        {
            mouseinput();
        }
        while(temppi==2);
        show();
    }

    while(gameover==false);
    //while(1);
    cin.get();
}

 int time()  //��ʾʱ��
 {
    GetLocalTime(&st2);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible=false;			//Ϊ�˲���ʾ���
	cursor.dwSize=10;					//�����ʾ�İٷֶ�
	SetConsoleCursorInfo(hOut,&cursor);
	cout<<"       Time��"<<st2.wMinute-st1.wMinute<<" min "<<st2.wSecond-st1.wSecond<<"s";
 }


















