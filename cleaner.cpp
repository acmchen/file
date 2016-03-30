//cleaner.cpp
//code::blocks 12.11  MinGW4.8   通过


#include<iostream>
#include<ctime>
#include<windows.h>
#include<conio.h>
#include <stdio.h>


//下面这两段，复制自MSDN，功能与 system("cls")相同，但是使用system("cls")，会无法读取鼠标输入,原因不知
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
COORD coordScreen={0,0};//设置清屏后光标返回的屏幕左上角坐标
BOOL bSuccess;
DWORD cCharsWritten;
CONSOLE_SCREEN_BUFFER_INFO csbi;//保存缓冲区信息
DWORD dwConSize;//当前缓冲区可容纳的字符数
bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);//获得缓冲区信息
PERR(bSuccess,"GetConsoleScreenBufferInfo");
dwConSize=csbi.dwSize.X * csbi.dwSize.Y;//缓冲区容纳字符数目
//用空格填充缓冲区
bSuccess=FillConsoleOutputCharacter(hConsole,(TCHAR)' ',dwConSize,coordScreen,&cCharsWritten);
PERR(bSuccess,"FillConsoleOutputCharacter");
bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);//获得缓冲区信息
PERR(bSuccess,"ConsoleScreenBufferInfo");
//填充缓冲区属性
bSuccess=FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,coordScreen,&cCharsWritten);
PERR(bSuccess,"FillConsoleOutputAttribute");
//光标返回屏幕左上角坐标
bSuccess=SetConsoleCursorPosition(hConsole,coordScreen);
PERR(bSuccess,"SetConsoleCursorPosition");
return;
}

//


#define Map_size 20 //地图大小，dos界面自行改
#define mine 30 //地雷数目


HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);     //读取鼠标输入的相关
HANDLE Hin=GetStdHandle(STD_INPUT_HANDLE);
COORD pos={0,0};
INPUT_RECORD mouse;
DWORD res;

int show();  //扫雷函数
int lay_mine();  //放置地雷
int count_mine();//周围地雷计数
int primary_menu();//主菜单
void white(int i,int j); //展开白色
int mine_cleaner(); //扫雷主函数
int time();


SYSTEMTIME   st1,st2;
int Num_mine[Map_size][Map_size];   //周围地雷计数
bool TorF_mine[Map_size][Map_size]; //是否为雷
bool state[Map_size][Map_size]; //是否显示过，，flase为没。true为显示过
bool gameover=false; //游戏结束
bool mine_mark[Map_size][Map_size];//flase为没有标记

using namespace std;
int temppx,temppy,temppi;  //temppx,temppy全局变量存储点击坐标；temppi用于防止屏幕不断刷新
int mouseinput()  //读取鼠标输入
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
    //system("mode con:cols=41 lines=28");    //设置大小为41*28
    //system("color 3A");   // 背景颜色
    primary_menu();
    CloseHandle(hOut);
    CloseHandle(Hin);

}

int lay_mine()  //放置雷
{
    srand((unsigned)time(NULL));
    cout<<"\n\n";
    for(int x=0;x<Map_size;x++)          //绘制地图
	{
	    cout<<"                    ";
		for(int y=0;y<Map_size;y++)

				cout<<"■";

		cout<<endl;
	}
    for(int x=0;x<Map_size;x++)         //初始化雷
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
int count_mine()   //周围雷计数
{
    int d[16]={ 0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1, -1,0, -1,1};  //方向数组
    for(int x=0;x<Map_size;x++)
	{
		for(int y=0;y<Map_size;y++)
        {
             for(int i=0;i!=8;i++)
             {
                 int tmp_x=x+d[i*2];
                 int tmp_y=y+d[i*2+1];
                 if(tmp_x>=0&&tmp_y>=0&&TorF_mine[x][y]==false)    //判断出界
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

void white(int i,int j)  //展开白色
{
    if(state[i][j]==false&&TorF_mine[i][j]==false)
    {
        if(i<0||i>Map_size-1||j<0||j>Map_size-1)//出界
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
        cout<<"                    逗逼，你踩地雷了。。。。\n\n\n\n";
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
					cout<<"□";
				else if(Num_mine[i][j]==1)
					cout<<"①";
				else if(Num_mine[i][j]==2)
					cout<<"②";
				else if(Num_mine[i][j]==3)
					cout<<"③";
				else if(Num_mine[i][j]==4)
					cout<<"④";
				else if(Num_mine[i][j]==5)
					cout<<"⑤";
				else if(Num_mine[i][j]==6)
					cout<<"⑥";
				else if(Num_mine[i][j]==7)
					cout<<"⑦";
				else if(Num_mine[i][j]==8)
					cout<<"⑧";
                else
					cout<<"¤";
			}
			if (mine_mark[i][j]==true)
                cout<<"★";
            if (mine_mark[i][j]==false&&state[i][j]==false)
                cout<<"■";
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


int primary_menu()  //主菜单
{
    HWND hwnd=GetForegroundWindow();
    SetWindowText(hwnd,"扫雷游戏");
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                 扫雷游戏    "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                              回车进入游戏                "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                     "<<endl;
    cout<<"                                                 "<<endl;
    cout<<"       tips:单击鼠标左键进行扫雷，单击鼠标右键进行标记，屏幕下方有时间提示"<<endl;
    cout<<"            游戏并不能解除标记哦，考验你是否谨慎                        "<<endl;
    cout<<"\n\n";
    cout<<"       图例： ■: 没有扫的区域                               "<<endl;
    cout<<"              □：已经扫过的区域                        "<<endl;
    cout<<"              ①：表示周围有一个雷 "<<endl;
    cout<<"              ⑥: 表示周围有六个雷"<<endl;
    cout<<"              ★：标记为雷                          "<<endl;
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
    do    //设置这个主要是防止鼠标一移动，屏幕就刷新
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

 int time()  //显示时间
 {
    GetLocalTime(&st2);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible=false;			//为了不显示光标
	cursor.dwSize=10;					//光标显示的百分度
	SetConsoleCursorInfo(hOut,&cursor);
	cout<<"       Time："<<st2.wMinute-st1.wMinute<<" min "<<st2.wSecond-st1.wSecond<<"s";
 }


















