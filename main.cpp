//Ocean Crisis.cpp
//copyright (c) 2017 KARABAC. All rights reserved.
#include <windows.h>
#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib,"Winmm.lib")
#define PI 3.1416


long bulletmax=4/*子弹数*/,dropmax=5/*投弹数*/,sbombmax=5/*潜艇炸弹数*/,pbombmax=4/*飞机子弹数*/,planemax=5/*飞机数*/,submax=5/*潜艇数*/,
	fishmax=0,width=720/*屏幕宽度*/,length=1280/*屏幕长度*/;
long count=0/*计数*/,score=0/*分数*/,times=0,sealevel=width*528/1024/*海平面*/,stay=30/*炸弹停留时间*/ ,
	recovery=200/*弹药恢复时间*/,attackvelocity=13/*玩家攻击速度*/,framerates=67/*帧数(决定流畅度，尽量不改动）*/; 
long starttime/*游戏开始时间*/,tclick,tdrop/*投弹时间间隔*/,tbullet/*投弹时间间隔*/,nowtime/*当前时间*/,countloop,timeloop;

IMAGE   subimagel1,subimagel2,subimagel3,planeimagel1,planeimagel2,planeimagel3,
		subimager1,subimager2,subimager3,planeimager1,planeimager2,planeimager3,//飞机和潜艇的图像
		fishimagel1,fishimagel2,fishimagel3,fishimager1,fishimager2,fishimager3,//鱼的图像
		shipimage1,shipimage2,pbombimage,sbombimage,dropimage,bulletimage,dropimg,bulletimg,//玩家和炸弹的图像
		stopbtn,startbtn,stopbtn1,startbtn1,aim,aim1,//准星图像
		story1,story2,dialog,fight,help,namelist,youwin,level1,level2,beginning,rankimg,
		careerbtn,crew,difficult,easy,endless,exitbtn,guild,medium,menu,normalmenu,
		normal,optionsmenu,options,rank,retry,
		fog,music,musicoff,octopus,octopusr,p1plane,p1planer,p1sub,p1subr,shark,sharkr,start,stop,whale,whaler;
MOUSEMSG msg;//鼠标信息
bool whetherrand=true,result=true,pausestate=false;//游戏结果
IMAGE bkg1,bkg3,oceanbkg,losebk,explo[17],explo1[17]; //背景，失败背景，爆炸图


struct enemy
{
	IMAGE img;//精灵图
	IMAGE img1;//掩码图
	double x;//左上角横坐标
	double y;//左上角纵坐标
	long money;//敌机价值
	double v;//表示速度和出现状态
	long bombplace;//爆炸地点
	bool lr;//判断左右方向
	RECT r; //RECT {LONG left;LONG top;LONG right;LONG bottom;}
}subl1,subl2,subl3,planel1,planel2,planel3,
 subr1,subr2,subr3,planer1,planer2,planer3,submarine[80],plane[80],//飞机和潜艇
 fishl1,fishl2,fishl3,fishr1,fishr2,fishr3,fish[80];


struct player
{
	IMAGE img;//精灵图
	IMAGE img1;//掩码图
	double x;//横坐标
	double y;//纵坐标
	double v;//表示速度和出现状态
	double vx;//横速度
	double vy;//飞机下降速度
	double angle;//发射角度
	RECT r;//图片边界坐标
}p1,bullet[80],pbomb[80],sbomb[80],drop[80],bullet1,pbomb1,sbomb1,drop1;//玩家以及四种炸弹


struct jilu
{
	TCHAR name[40];
	long score;
}a[11],tt;


void writename(TCHAR appname[40],long score)
{
	
	// 定义字符串缓冲区，并接收用户输入

	// 将用户输入转换为数字
	//int r;
	//sscanf(s, "%d", &r);

	TCHAR keyname[40],keyname1[40],s[40];
	int i,k,temp=1000000,temppoint;
	for (i=1;i<=10;i++)
	{
		_stprintf(keyname, _T("%d"), i);			// vc6 的写法
		k= ::GetPrivateProfileInt(_T(appname), _T(keyname),0, _T(".\\test.ini"));
		//GetPrivateProfileString(_T(appname), _T(keyname),0, _T(".\\test.ini"));
		if (k<temp) {temp=k; temppoint=i;}
	}
	if (score>temp)
	{
		TCHAR name[40];
		InputBox(name, 20, "Please enter your name","congratulations!","Player",300,20,true);
		_stprintf(keyname, _T("%d"), temppoint);
		_stprintf(keyname1, _T("%d"), -temppoint);
		_stprintf(s, _T("%d"), score);		
		::WritePrivateProfileString(_T(appname), _T(keyname), s, _T(".\\test.ini"));
		::WritePrivateProfileString(_T(appname), _T(keyname1), name, _T(".\\test.ini"));
	}
	
	//WritePrivateProfileString(_T("Ball"), _T("x"), _T("30"), _T(".\\test.ini"));  
// 读取前面代码创建的键 x 的值。如果没找到，返回默认值 320
//int age;
//age = GetPrivateProfileInt(_T("Ball"), _T("x"), 320, _T(".\\test.ini"));
}


void printname(TCHAR appname[40])
{

	// 将用户输入转换为数字
	//int r;
	//sscanf(s, "%d", &r);
	bool o=false;
	TCHAR keyname[40],keyname1[40],s[40];
	int i,j;
	for (i=1;i<=10;i++)
	{
		_stprintf(keyname, _T("%d"),i);		
		_stprintf(keyname1, _T("%d"),-i);
		::GetPrivateProfileString(_T(appname), _T(keyname1),"None", a[i].name,20, _T(".\\test.ini"));
		a[i].score = ::GetPrivateProfileInt(_T(appname), _T(keyname),0, _T(".\\test.ini"));
		//sscanf(s, "%d", &a[i].score);
	}
	for (i=1;i<=10;i++)
		for (j=i;j<=10;j++)
			if (a[j].score>a[i].score)
			{
				tt=a[i];
				a[i]=a[j];
				a[j]=tt;
			}
		
	setbkmode(TRANSPARENT);
	settextcolor(RGB(204,51,55));
	setfont(50*width/720, 0, _T("Arial"));
	outtextxy(575*width/720,20*width/720,appname);

	setfont(44*width/720, 0, _T("Arial"));
	outtextxy(435*width/720,68*width/720,"PLAYER");
	outtextxy(755*width/720,68*width/720,"SCORE");

	setbkmode(TRANSPARENT);
	settextcolor(RGB(204,51,55));
	setfont(40*width/720, 0, _T("微软雅黑"));
	for (i=1;i<=10;i++)
		if (a[i].score>1) 
	{
		outtextxy(440*width/720,(90+i*50)*width/720,a[i].name);
		sprintf(s,"%d",a[i].score);
		outtextxy(770*width/720,(90+i*50)*width/720,s);
	}
}


void turnvolume(char szPath[100],int x)
{
	TCHAR cmd[256]; // 媒体命令
	TCHAR volume[256]; // 音量(字符串)
	int nVolume; // 音量(整型)

	wsprintf(cmd, "status %s volume", szPath); // szPath 为音乐的路径
	mciSendString(cmd,volume,sizeof(volume),0); // 获取当前音量至 volume 字符串中
	nVolume = atoi(volume); // 字符串转化为整型
	wsprintf(cmd, "setaudio %s volume to %i", szPath,x); // 生成媒体命令，设置音量减 50
	mciSendString(cmd,"",0,NULL); // 执行媒体命令
	return;
}


bool chanvolume(char szPath[100],int a,int b,int x)
{
	TCHAR cmd[256]; // 媒体命令
	TCHAR volume[256]; // 音量(字符串)
	int nVolume; // 音量(整型)

	wsprintf(cmd, "status %s volume", szPath); // szPath 为音乐的路径
	mciSendString(cmd,volume,sizeof(volume),0); // 获取当前音量至 volume 字符串中
	nVolume = atoi(volume); // 字符串转化为整型
	if (nVolume>=a && nVolume<=b) 
	{
		wsprintf(cmd, "setaudio %s volume to %i", szPath,nVolume+x); // 生成媒体命令，设置音量减 50
		mciSendString(cmd,"",0,NULL); // 执行媒体命令
		return true;
	}
	return false;
}


bool isCollision(RECT r,RECT l)//判断是否碰撞
{
	int R=2;
	if(r.top-l.bottom>R)
		return false;
	if(l.top-r.bottom>R)
		return false;
	if(l.left-r.right>R)
		return false;
	if(r.left-l.right>R)
		return false;
	return true;
}


IMAGE yanmatu(IMAGE *img)//掩码图生成函数
{
	COLORREF c;
	DWORD *p=GetImageBuffer(img);
	c=p[0];
	int w=(*img).getwidth();
	int h=(*img).getheight();
	for(int i=0;i<w*h;i++)
	{
		if(c==p[i])
			p[i]=WHITE;
		else
			p[i]=BLACK;
	}
	return *img;
}


void gameinitialize()//游戏初始化
{	
	float n=1;
	int i;
	count=0;score=0;times=0;countloop=0;timeloop=timeGetTime();
	starttime=time(NULL); tdrop=count;tclick=count; tbullet=count;//初始化时间

	loadimage(&fight,_T("IMAGE"), _T("fight"),width*16/9,width);
	loadimage(&subimagel1,_T("IMAGE"), _T("sub1"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&subimagel2,_T("IMAGE"), _T("sub2"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&subimagel3,_T("IMAGE"), _T("sub3"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&planeimagel1,_T("IMAGE"),_T("plane1"),(int)(width*84*n/768),(int)(width*19*n/768));
	loadimage(&planeimagel2,_T("IMAGE"),_T("plane2"),(int)(width*70*n/768),(int)(width*22*n/768));
	loadimage(&planeimagel3,_T("IMAGE"),_T("plane3"),(int)(width*84*n/768),(int)(width*24*n/768));
	loadimage(&subimager1,_T("IMAGE"), _T("subr1"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&subimager2,_T("IMAGE"), _T("subr2"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&subimager3,_T("IMAGE"), _T("subr3"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&planeimager1,_T("IMAGE"),_T("planer1"),(int)(width*84*n/768),(int)(width*19*n/768));
	loadimage(&planeimager2,_T("IMAGE"),_T("planer2"),(int)(width*70*n/768),(int)(width*22*n/768));
	loadimage(&planeimager3,_T("IMAGE"),_T("planer3"),(int)(width*84*n/768),(int)(width*24*n/768));
	loadimage(&shipimage1,_T("IMAGE"), _T("ship1"),(int)(width*105*n/768),(int)(width*40*n/768));
	loadimage(&sbombimage,_T("IMAGE"), _T("sbomb"),(int)(width*13*n/768),(int)(width*13*n/768));
	loadimage(&pbombimage,_T("IMAGE"), _T("pbomb"),(int)(width*20*n/768),(int)(width*20*n/768));

	loadimage(&fishimagel1,_T("IMAGE"), _T("shark"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&fishimagel2,_T("IMAGE"), _T("whale"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&fishimagel3,_T("IMAGE"), _T("shark"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&fishimager1,_T("IMAGE"), _T("sharkr"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&fishimager2,_T("IMAGE"), _T("whaler"),(int)(width*72*n/768),(int)(width*18*n/768));
	loadimage(&fishimager3,_T("IMAGE"), _T("sharkr"),(int)(width*72*n/768),(int)(width*18*n/768));

	loadimage(&dropimage,_T("IMAGE"), _T("drop"),(int)(width*6*n/768),(int)(width*24*n/768));
	loadimage(&bulletimage,_T("IMAGE"), _T("bullet"),(int)(width*26*n/768),(int)(width*26*n/768));
	loadimage(&aim,_T("IMAGE"), _T("zhunxing"),(int)(width*40*n/768),(int)(width*40*n/768));
	loadimage(&bkg1,_T("IMAGE"), _T("background"),(int)(width*16/9),(int)(width));
	loadimage(&bkg3,_T("IMAGE"), _T("l3bkg"),(int)(width*16/9),(int)(width));
	loadimage(&losebk,_T("IMAGE"), _T("lose"),(int)(width*16/9),(int)(width));
	loadimage(&(explo[1]),_T("IMAGE"), _T("e1"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[2]),_T("IMAGE"), _T("e2"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[3]),_T("IMAGE"), _T("e3"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[4]),_T("IMAGE"), _T("e4"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[5]),_T("IMAGE"), _T("e5"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[6]),_T("IMAGE"), _T("e6"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[7]),_T("IMAGE"), _T("e7"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[8]),_T("IMAGE"), _T("e8"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[9]),_T("IMAGE"), _T("e9"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[10]),_T("IMAGE"), _T("e10"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[11]),_T("IMAGE"), _T("e11"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[12]),_T("IMAGE"), _T("e12"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[13]),_T("IMAGE"), _T("e13"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[14]),_T("IMAGE"), _T("e14"),(int)(width*63/768),(int)(width*63/768));
	loadimage(&(explo[15]),_T("IMAGE"), _T("e15"),(int)(width*63/768),(int)(width*63/768));
	//载入图像

	
	loadimage(&music,_T("IMAGE"),_T("music"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&musicoff,_T("IMAGE"),_T("musicoff"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&octopus,_T("IMAGE"),_T("octopus"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&octopusr,_T("IMAGE"),_T("octopusr"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&p1plane,_T("IMAGE"),_T("p1plane"),(int)(width*100*n/768),(int)(width*25*n/768));
	loadimage(&p1planer,_T("IMAGE"),_T("p1planer"),(int)(width*100*n/768),(int)(width*25*n/768));
	loadimage(&p1sub,_T("IMAGE"),_T("p1sub"),(int)(width*100*n/768),(int)(width*24*n/768));
	loadimage(&p1subr,_T("IMAGE"),_T("p1subr"),(int)(width*100*n/768),(int)(width*24*n/768));
	loadimage(&shark,_T("IMAGE"),_T("shark"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&sharkr,_T("IMAGE"),_T("sharkr"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&startbtn,_T("IMAGE"),_T("startbtn"),(int)(width*35*n/768),(int)(width*35*n/768));
	loadimage(&stopbtn,_T("IMAGE"),_T("stopbtn"),(int)(width*35*n/768),(int)(width*35*n/768));
	loadimage(&whale,_T("IMAGE"),_T("whale"),(int)(width*70*n/768),(int)(width*20*n/768));
	loadimage(&whaler,_T("IMAGE"),_T("whaler"),(int)(width*70*n/768),(int)(width*20*n/768));

	for (i=1;i<=15;i++)
	{
		explo1[i]=explo[i];
		yanmatu(&(explo1[i]));
	}
	//载入爆炸图片
	startbtn1=startbtn;
	stopbtn1=stopbtn;
	yanmatu(&startbtn1);
	yanmatu(&stopbtn1);

	subl1.img=subimagel1;
	subl1.img1=yanmatu(&(subimagel1));
	subl1.money=60;
	subl1.v=1.8;//表示速度和出现状态
	subl2.img=subimagel2;
	subl2.img1=yanmatu(&(subimagel2));
	subl2.money=100;
	subl2.v=2.5;//表示速度和出现状态
	subl3.img=subimagel3;
	subl3.img1=yanmatu(&(subimagel3));
	subl3.money=120;
	subl3.v=4;//表示速度和出现状态
	planel1.img=planeimagel1;
	planel1.img1=yanmatu(&(planeimagel1));
	planel1.money=70;
	planel1.v=3;//表示速度和出现状态
	planel2.img=planeimagel2;
	planel2.img1=yanmatu(&(planeimagel2));
	planel2.money=100;
	planel2.v=4;//表示速度和出现状态
	planel3.img=planeimagel3;
	planel3.img1=yanmatu(&(planeimagel3));
	planel3.money=120;
	planel3.v=5;//表示速度和出现状态
	subr1.img=subimager1;
	subr1.img1=yanmatu(&(subimager1));
	subr1.money=60;
	subr1.v=1.8;//表示速度和出现状态
	subr2.img=subimager2;
	subr2.img1=yanmatu(&(subimager2));
	subr2.money=100;
	subr2.v=2.5;//表示速度和出现状态
	subr3.img=subimager3;
	subr3.img1=yanmatu(&(subimager3));
	subr3.money=120;
	subr3.v=4;//表示速度和出现状态
	planer1.img=planeimager1;
	planer1.img1=yanmatu(&(planeimager1));
	planer1.money=70;
	planer1.v=3;//表示速度和出现状态
	planer2.img=planeimager2;
	planer2.img1=yanmatu(&(planeimager2));
	planer2.money=100;
	planer2.v=4;//表示速度和出现状态
	planer3.img=planeimager3;
	planer3.img1=yanmatu(&(planeimager3));
	planer3.money=120;
	planer3.v=5;//表示速度和出现状态
	//定义不同种类的飞机和船只

	fishl1.img=fishimagel1;
	fishl1.img1=yanmatu(&(fishimagel1));
	fishl1.money=20;
	fishl1.v=1.8;//表示速度和出现状态
	fishl2.img=fishimagel2;
	fishl2.img1=yanmatu(&(fishimagel2));
	fishl2.money=30;
	fishl2.v=2.5;//表示速度和出现状态
	fishl3.img=fishimagel3;
	fishl3.img1=yanmatu(&(fishimagel3));
	fishl3.money=50;
	fishl3.v=4;//表示速度和出现状态
	fishr1.img=fishimager1;
	fishr1.img1=yanmatu(&(fishimager1));
	fishr1.money=20;
	fishr1.v=1.8;//表示速度和出现状态
	fishr2.img=fishimager2;
	fishr2.img1=yanmatu(&(fishimager2));
	fishr2.money=30;
	fishr2.v=2.5;//表示速度和出现状态
	fishr3.img=fishimager3;
	fishr3.img1=yanmatu(&(fishimager3));
	fishr3.money=50;
	fishr3.v=4;//表示速度和出现状态

	
	p1.img=shipimage1;
	p1.img1=p1.img;
	yanmatu(&(p1.img1));
	p1.x=length/2;
	p1.y=sealevel-shipimage1.getheight()*6/7;
	p1.v=4;
	p1.r.bottom=p1.y+shipimage1.getheight()*4/5;
	p1.r.left=p1.x;
	p1.r.right=p1.x+shipimage1.getwidth();
	p1.r.top=p1.y+shipimage1.getheight()/2;//定义玩家
	
	sbomb1.img=sbombimage;
	sbomb1.img1=sbombimage;
	yanmatu(&(sbomb1.img1));//定义各种炸弹
	sbomb1.v=2;
	pbomb1.img=pbombimage;
	pbomb1.img1=pbombimage;
	pbomb1.img1=yanmatu(&(pbomb1.img1));//定义各种炸弹
	pbomb1.v=1;
	drop1.img=dropimage;
	drop1.img1=dropimage;
	yanmatu(&(drop1.img1));//定义各种炸弹
	drop1.v=3;
	bullet1.img=bulletimage;
	bullet1.img1=bulletimage;
	yanmatu(&(bullet1.img1));//定义各种炸弹
	bullet1.v=8;
	aim1=aim;
	yanmatu(&(aim1));
	bulletimg=bulletimage;
	dropimg=dropimage;
	yanmatu(&bulletimg);
	yanmatu(&dropimg);

	for (i=0;i<=70;i++)
	{
		plane[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		submarine[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		sbomb[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		pbomb[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		drop[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		bullet[i].v=-1;
	}
	for (i=0;i<=70;i++)
	{
		fish[i].v=-1;
	}

	//初始化飞机潜艇和炸弹状态
}


void shade(int n/*色彩浓度*/, int x, int y, IMAGE img)//处理图片明暗
{
	int i;
	DWORD *p=GetImageBuffer(&img);
	int w=(img).getwidth();
	int h=(img).getheight();
	for(i=0;i<w*h;i++)
		if (p[i] != 0)
			p[i] = RGB(GetRValue(p[i]) *n/100, GetGValue(p[i]) *n/100, GetBValue(p[i]) *n/100);

	putimage(x,y,&img);

	FlushBatchDraw();//统一输出图像
}


bool click(int left,int top,int right,int bottom,IMAGE img1)//判断鼠标点击
{
	if( msg.x>=left &&msg.x<=right &&msg.y>=top &&msg.y<=bottom)
	{
		//if(msg.uMsg==WM_MOUSEMOVE)
			putimage(0,0,&img1);//放置按钮图标
		if(msg.mkLButton)
			return true;
	}
	return false;
}


void randomize_enemy(struct enemy &temp, bool pl_sh)//随机生成敌机
{
	float n=3.0/5; int k, i;
	i=rand() % 2;
	int m=rand()%3+1;
	if (pl_sh==1)
	{
		if (i==1)
		{
			switch (m)
			{
				case 1: temp=planer1;break;
				case 2: temp=planer2;break;
				case 3: temp=planer3;break;
			}
		}
		else
			switch (m)
			{
				case 1: temp=planel1;break;
				case 2: temp=planel2;break;
				case 3: temp=planel3;break;
			}
		k=(sealevel)*n+1;
		temp.y=rand() % k;
	}
	else
	{
		if (i==1)
		{
			switch (m)
			{
				case 1: temp=subr1;break;
				case 2: temp=subr2;break;
				case 3: temp=subr3;break;
			}
		}
		else
			switch (m)
			{
				case 1: temp=subl1;break;
				case 2: temp=subl2;break;
				case 3: temp=subl3;break;
			}
		k=(width-sealevel)*n+1;
		temp.y=sealevel+rand() % k+(width-sealevel)*(1-n)-subimagel1.getheight();
	}
	if (i==1) 
	{
		temp.lr=1;
		temp.x=0-temp.img.getwidth();
	}
	else 
	{
		temp.lr=0;
		temp.x=length-1;
	}
	temp.r.bottom=temp.y+temp.img.getheight();
	temp.r.left=temp.x;
	temp.r.right=temp.x+temp.img.getwidth();
	temp.r.top=temp.y; 
	temp.v+=rand()%10/10.0-0.5;
	temp.bombplace=rand()%length;
	//生成敌机坐标速度等参数
}


void randomizefish(struct enemy &temp)//随机生成敌机
{
	float n=3.0/5; int k, i;
	i=rand() % 2;
	int m=rand()%3+1;
	if (i==1)
	{
		switch (m)
		{
			case 1: temp=fishr1;break;
			case 2: temp=fishr2;break;
			case 3: temp=fishr3;break;
		}
	k=(width-sealevel)*n+1;
	temp.y=sealevel+rand() % k+(width-sealevel)*(1-n)-fishimagel1.getheight();
	}
	else
	{
		switch (m)
		{
			case 1: temp=fishl1;break;
			case 2: temp=fishl2;break;
			case 3: temp=fishl3;break;
		}
	k=(width-sealevel)*n+1;
	temp.y=sealevel+rand() % k+(width-sealevel)*(1-n)-fishimagel1.getheight();
	}

	if (i==1) 
	{
		temp.lr=1;
		temp.x=0-temp.img.getwidth();
	}
	else 
	{
		temp.lr=0;
		temp.x=length-1;
	}
	temp.r.bottom=temp.y+temp.img.getheight();
	temp.r.left=temp.x;
	temp.r.right=temp.x+temp.img.getwidth();
	temp.r.top=temp.y; 
	temp.v+=rand()%10/10.0-0.5;
	//生成敌机坐标速度等参数
}


void judge()//判断出界，碰撞过程并处理
{
	int i,j;
	for (i=0;i<=sbombmax;i++)
	{
		if (sbomb[i].v!=-1 && isCollision(p1.r,sbomb[i].r) && p1.v>-1 && whetherrand)
		{
			result=0;
			p1.v=-31;//爆炸
			sbomb[i].v=-1;
			mciSendString(_T("stop planebomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek planebomb to 0"),NULL,0,NULL);
			mciSendString(_T("play planebomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek gameoverbgm to 0"),NULL,0,NULL);
			mciSendString(_T("play gameoverbgm"), NULL, 0, NULL);// 播放音乐
			break;
		}
	}
	for (i=0;i<=pbombmax;i++)
	{
		if (pbomb[i].v!=-1 && isCollision(p1.r,pbomb[i].r) && p1.v>-1 && whetherrand)
		{
			result=0;
			p1.v=-31;//爆炸
			pbomb[i].v=-1;
			mciSendString(_T("stop planebomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek planebomb to 0"),NULL,0,NULL);
			mciSendString(_T("play planebomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek gameoverbgm to 0"),NULL,0,NULL);
			mciSendString(_T("play gameoverbgm"), NULL, 0, NULL);// 播放音乐
			break;
		}
	}
	for (i=0;i<=dropmax;i++)
		if (drop[i].v>-1 && drop[i].r.top>width)
			drop[i].v=-recovery;
	for (i=0;i<=bulletmax;i++)
		if (bullet[i].v>-1 && (bullet[i].r.left>length || bullet[i].r.right<0 || bullet[i].r.bottom<0))
			bullet[i].v=-recovery;
	for (i=0;i<=submax;i++)
		if (submarine[i].v>-1)
			if (submarine[i].r.right<0 ||submarine[i].r.left>length)
				submarine[i].v=-1;
			else
			for (j=0;j<=dropmax;j++)
				if (drop[j].v>-1 && isCollision(submarine[i].r,drop[j].r))
				{
					submarine[i].v=-31;
					drop[j].v=-recovery;
					if (result)
						score+=submarine[i].money;
			mciSendString(_T("stop subbomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek subbomb to 0"),NULL,0,NULL);
					mciSendString(_T("play subbomb"), NULL, 0, NULL);// 播放音乐
					break;
					//destroyed
				}
	for (i=0;i<=planemax;i++)
		if (plane[i].v>-1)
			if (plane[i].r.right<0 || plane[i].r.left>length)
				plane[i].v=-1;
			else
			for (j=0;j<=bulletmax;j++)
				if (bullet[j].v>-1 && isCollision(plane[i].r,bullet[j].r))
				{
					plane[i].v=-31;
					bullet[j].v=-recovery;
					if (result)
						score+=plane[i].money;
			mciSendString(_T("stop planebomb"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek planebomb to 0"),NULL,0,NULL);
					mciSendString(_T("play planebomb"), NULL, 0, NULL);// 播放音乐
					break;
					//destroyed
				}
				
	for (i=0;i<=fishmax;i++)
		if (fish[i].v>-1)
			if (fish[i].r.right<0 ||fish[i].r.left>length)
				fish[i].v=-1;
			else
			{
				for (j=0;j<=dropmax;j++)
					if (drop[j].v>-1 && isCollision(fish[i].r,drop[j].r))
					{
						fish[i].v=-31;
						drop[j].v=-recovery;
						if (result)
							score-=fish[i].money;
						break;
						//destroyed
					}
				if (p1.v>-1 && isCollision(fish[i].r,p1.r))
				{
					fish[i].v=-1;
					drop[j].v=-recovery;
					if (result)
						score+=fish[i].money;
					break;
					//destroyed
				}
			}
}
	

void draw(int levelnum)//画图函数，生成图像
{
	int i,remain;
	switch (levelnum)
	{
		case 1: putimage(0,0,&bkg1);break;
		case 2:	putimage(0,0,&bkg1);break;
		case 3:	putimage(0,0,&bkg3);break;
	}
		if (p1.v>-1)//画船
		{
			putimage((int)p1.x,(int)p1.y,&(p1.img1),SRCAND);
			putimage((int)p1.x,(int)p1.y,&(p1.img),SRCPAINT);
		}
		else//爆炸
		if (p1.v<-1)//爆炸图
			{
				putimage((int)p1.x,(int)p1.y,&(explo1[(int)-p1.v/2]),SRCAND);
				putimage((int)p1.x,(int)p1.y,&(explo[(int)-p1.v/2]),SRCPAINT);
				p1.v++;
			}
	for(i=0;i<=submax;i++)
		if (submarine[i].v> -1)//画潜艇
		{
			putimage((int)submarine[i].x,(int)submarine[i].y,&(submarine[i].img1),SRCAND);
			putimage((int)submarine[i].x,(int)submarine[i].y,&(submarine[i].img),SRCPAINT);
		}
		else if (submarine[i].v<-1)//潜艇爆炸
		{
			putimage((int)submarine[i].x,(int)submarine[i].y,&(explo1[(int)-submarine[i].v/2]),SRCAND);
			putimage((int)submarine[i].x,(int)submarine[i].y,&(explo[(int)-submarine[i].v/2]),SRCPAINT);
			submarine[i].v++;
		}
	for(i=0;i<=planemax;i++)
		if (plane[i].v>-1)//画飞机
		{
			putimage((int)plane[i].x,(int)plane[i].y,&(plane[i].img1),SRCAND);
			putimage((int)plane[i].x,(int)plane[i].y,&(plane[i].img),SRCPAINT);
		}
		else if (plane[i].v<-1)//飞机爆炸
		{
			putimage((int)plane[i].x,(int)plane[i].y,&(explo1[(int)-plane[i].v/2]),SRCAND);
			putimage((int)plane[i].x,(int)plane[i].y,&(explo[(int)-plane[i].v/2]),SRCPAINT);
			plane[i].v++;
		}
	for(i=0;i<=fishmax;i++)
		if (fish[i].v> -1)//画鱼
		{
			putimage((int)fish[i].x,(int)fish[i].y,&(fish[i].img1),SRCAND);
			putimage((int)fish[i].x,(int)fish[i].y,&(fish[i].img),SRCPAINT);
		}
		else if (fish[i].v<-1)//鱼爆炸
		{
			putimage((int)fish[i].x,(int)fish[i].y,&(explo1[(int)-fish[i].v/2]),SRCAND);
			putimage((int)fish[i].x,(int)fish[i].y,&(explo[(int)-fish[i].v/2]),SRCPAINT);
			fish[i].v++;
		}

	for (i=0;i<=sbombmax;i++)
	{
		if (sbomb[i].v!=-1)//画潜艇炸弹
		{
			putimage((int)sbomb[i].x,(int)sbomb[i].y,&(sbomb[i].img1),SRCAND);
			putimage((int)sbomb[i].x,(int)sbomb[i].y,&(sbomb[i].img),SRCPAINT);
			if (sbomb[i].v<-1) sbomb[i].v++;
			//FlushBatchDraw();
		}
	}
	for (i=0;i<=pbombmax;i++)
	{
		if (pbomb[i].v!=-1)//画飞机炸弹
		{
			putimage((int)pbomb[i].x,(int)pbomb[i].y,&(pbomb[i].img1),SRCAND);
			putimage((int)pbomb[i].x,(int)pbomb[i].y,&(pbomb[i].img),SRCPAINT);
			if (pbomb[i].v<-1) pbomb[i].v++;
		}
	}
	for (i=0;i<=bulletmax;i++)
	{
		if (bullet[i].v>-1)//画子弹
		{
			putimage((int)bullet[i].x,(int)bullet[i].y,&(bullet[i].img1),SRCAND);
			putimage((int)bullet[i].x,(int)bullet[i].y,&(bullet[i].img),SRCPAINT);
		}
		if (bullet[i].v<-1) bullet[i].v ++;
	}
	for (i=0;i<=dropmax;i++)
	{
		if (drop[i].v>-1)//画投弹
		{
			putimage((int)drop[i].x,(int)drop[i].y,&(drop1.img1),SRCAND);
			putimage((int)drop[i].x,(int)drop[i].y,&(drop1.img),SRCPAINT);
		}
		if (drop[i].v<-1) drop[i].v ++;
	}
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	setfont(21, 0, _T("Arial"));
	TCHAR s[30];
	//输出剩余子弹数，时间和分数
	remain=0;
	for (i=0;i<=bulletmax;i++)
		if (bullet[i].v ==-1) remain++;
	sprintf(s,"CANNON",remain);
	outtextxy(40,20,s);//玩家剩余炮弹
	sprintf(s,": %d",remain);
	outtextxy(120,20,s);
	for (i=1;i<=remain;i++)
		{
			putimage(120+i*25,15,&(bulletimg),SRCAND);
			putimage(120+i*25,15,&(bulletimage),SRCPAINT);
		}

	remain=0;
	for (i=0;i<=dropmax;i++)
		if (drop[i].v ==-1) remain++;
	sprintf(s,"BOMB",remain);
	outtextxy(42,50,s);//玩家剩余油弹
	sprintf(s,": %d",remain);
	outtextxy(120,50,s);
	for (i=1;i<=remain;i++)
		{
			putimage(130+i*25,47,&(dropimg),SRCAND);
			putimage(130+i*25,47,&(dropimage),SRCPAINT);
		}

	sprintf(s,"SCORE : %d",score);//绘制分数
	outtextxy(length/2,20,s);

	sprintf(s,"TIME : %ds",abs(nowtime));//绘制时间
	outtextxy(length/2-110,20,s);

	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 2);
	setlinecolor(RED);

	if (pausestate)
	{
		putimage(1225*width/720,20*width/720,&startbtn1,SRCAND);
		putimage(1225*width/720,20*width/720,&startbtn,SRCPAINT);
	}
	else
	{
		putimage(1225*width/720,20*width/720,&stopbtn1,SRCAND);
		putimage(1225*width/720,20*width/720,&stopbtn,SRCPAINT);
	}
	if (p1.v>-1)
		{
			putimage((int)msg.x-(int)aim.getwidth()/2,(int)msg.y-(int)aim.getheight()/2,&aim1,SRCAND);
			putimage((int)msg.x-(int)aim.getwidth()/2,(int)msg.y-(int)aim.getheight()/2,&aim,SRCPAINT);
		}
	count++;
	if (count%67==0)
	{
		times++;
	}
}


void lose()//失败界面
{
	int x=0,y=-width,k,v=30;
	IMAGE scr;
	getimage(&scr,0,0,length,width);
	while (1)
		{
			putimage(0,0,&scr);
			putimage(x,y,&losebk);
			y+=v++;
			//v+=1;
			if (y>-1) 
			{
				y=0;
				v=-v/2;
				if (v>-2) break;
			}
			FlushBatchDraw();
			Sleep(7);
		}
	putimage(0,0,&losebk);
	FlushBatchDraw();
	bool loseloop=true;
	FlushMouseMsgBuffer();
	msg=GetMouseMsg();
	float n=2;
	while(loseloop)
	{
		while(MouseHit())
			msg=GetMouseMsg();
		if(click((int)length*388/1280,(int)width*550/720,(int)length*388/1280+n*109*width/720,(int)width*550/720+n*29*width/720,retry))
		{
			result=1;
			loseloop=false;
		}		
		else
		if(click((int)length*704/1280,(int)width*550/720,(int)length*704/1280+n*102*width/720,(int)width*550/720+n*32*width/720,menu))
		{
			loseloop=false;
		}
		else
		{
			FlushBatchDraw();
			putimage(0,0,&losebk);
		}
	}
}


void shot()//船的射击函数
{  
	int i;
	float angle = atan2(p1.x-msg.x,p1.y-msg.y);
	if (msg.mkLButton && (count-tbullet>attackvelocity) && (angle< PI/2 && angle> - PI/2)
	&&	!(msg.x>=1225*width/720 && msg.y>=20*width/720 && msg.x<=1260*width/720 && msg.y<=55*width/720))
		for (i=0;i<=bulletmax;i++)
			if (bullet[i].v==-1)
		{
			tbullet=count;
			IMAGE temp;
			bullet[i]=bullet1;
			bullet[i].angle = atan2(p1.x-msg.x,p1.y-msg.y);
			rotateimage(&temp,&bullet1.img,bullet[i].angle);
			bullet[i].img=temp;
			bullet[i].img1=yanmatu(&(temp));//定义各种炸弹
			bullet[i].x=p1.x;
			bullet[i].y=p1.y;
			bullet[i].r.left=p1.x;
			bullet[i].r.top=p1.y;
			bullet[i].r.right=p1.x+bullet[i].img.getwidth();
			bullet[i].r.bottom=p1.y+bullet[i].img.getheight();
			score-=30;
			mciSendString(_T("stop shotbgm"), NULL, 0, NULL);// 播放音乐
			mciSendString(_T("seek shotbgm to 0"),NULL,0,NULL);
			mciSendString(_T("play shotbgm"), NULL, 0, NULL);// 播放音乐
			break;
		//初始化子弹参数
		}
}


int modechoose(int mode,int cost,int base)
{
			if (kbhit())
			{
				char c=getch();
				switch (c)
				{
					case 'j':
						if (mode==2 && score>=base )
						{
							score-=cost;
							p1.y=sealevel;
							p1.img=p1sub;
							p1.img1=p1.img;
							yanmatu(&(p1.img1));
							return 1;
						}
						break;

					case 'k':
						if (mode==1 && score>=base && p1.y<sealevel+3)
						{
							score-=cost;
							p1.y=sealevel-shipimage1.getheight()*6/7;
							p1.img=shipimage1;
							p1.img1=p1.img;
							yanmatu(&(p1.img1));
							return 2;
						}
						if (mode==3 && score>=base && p1.y>sealevel-p1.img.getheight()-3)
						{
							score-=cost;
							p1.y=sealevel-shipimage1.getheight()*6/7;
							p1.img=shipimage1;
							p1.img1=p1.img;
							yanmatu(&(p1.img1));
							return 2;
						}
						break;

					case 'l':
						if (mode==2 && score>=base)
						{
							score-=cost;
							p1.img=p1plane;
							p1.img1=p1.img;
							yanmatu(&(p1.img1));
							p1.y=sealevel-p1.img.getheight();
							return 3;
						}
						break;
				}
			}
	return mode;
}

void move()//物体移动的数据处理
{
	int i;
	float f;
	if (p1.v>-1)//船的左右移动
	{
		if ((GetAsyncKeyState('A') &0x8000) && p1.r.left>0) 
			{
				p1.x-=p1.v;
				p1.r.left-=p1.v;
				p1.r.right-=p1.v;
			}
		if ((GetAsyncKeyState('D') &0x8000) && p1.r.right<length) 
			{
				p1.x+=p1.v;
				p1.r.left+=p1.v;
				p1.r.right+=p1.v;
			}
			//船的移动

		if((GetAsyncKeyState(VK_SPACE) &0x8000)  && (count-tdrop>attackvelocity) && whetherrand)
			for (i=0;i<=dropmax;i++)
				if (drop[i].v==-1)
				{
					tdrop=count;
					drop[i]=drop1;
					drop[i].x=p1.x+p1.img.getwidth()/2;
					drop[i].y=p1.y;
					drop[i].r.left=drop[i].x;
					drop[i].r.top=drop[i].y;
					drop[i].r.right=drop[i].x+drop[i].img.getwidth();
					drop[i].r.bottom=drop[i].y+drop[i].img.getheight();//drop
					score-=35;
					break;
				}//放炸弹

		while (MouseHit())//捕捉鼠标信息
			msg=GetMouseMsg();	
		if (whetherrand) shot();
	}

	IMAGE temp;
	for (i=0;i<=planemax;i++)
	{
		if (plane[i].v>-1)
		{
			if (plane[i].lr==1)
			{
				if ((pbomb[i].v==-1) && (plane[i].bombplace<=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹
				}	
				plane[i].x+=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
			else
			{	
				if ((pbomb[i].v==-1) && (plane[i].bombplace>=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=-plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹	
				}
				plane[i].x-=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
		}
		else if (plane[i].v==-1 && whetherrand) randomize_enemy(plane[i],1);//初始化飞机
	
		if (pbomb[i].v>-1 && i<=pbombmax) 
			if(pbomb[i].r.top+pbomb[i].img.getheight()/2<sealevel)
			{
				pbomb[i].v++;
				pbomb[i].y+=pbomb[i].v/12;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].vy=pbomb[i].v/12;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].v/12);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			else
				pbomb[i].v=-stay;//炸弹停留在海平面上

		if (pbomb[i].v<-1 && i<=pbombmax) 
			{
				pbomb[i].vx=pbomb[i].vx/1.13;
				pbomb[i].vy=pbomb[i].vy/1.07;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹//炸弹在海平面上的移动
				pbomb[i].y+=pbomb[i].vy;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].vy);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			
	}
	//飞机及其炸弹的移动

	for (i=0;i<=fishmax;i++)
	{
		if (fish[i].v>-1)
		{
			if (fish[i].lr==1)
			{
				fish[i].x+=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
			else
			{
				fish[i].x-=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
		}
		else if (fish[i].v==-1 && whetherrand) randomizefish(fish[i]);//初始化鱼
	}

	for (i=0;i<=submax;i++)
	{
		if (submarine[i].v>-1)
		{
			if (submarine[i].lr==1)
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace<=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
				}	
				submarine[i].x+=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
			else
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace>=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
				}
				submarine[i].x-=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
		}
		else if (submarine[i].v==-1 && whetherrand) randomize_enemy(submarine[i],0);//初始化潜艇

		if (sbomb[i].v>-1 && i<=sbombmax)
			if (sbomb[i].vx==666)
			{
				if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel*28/20 )
				{
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2>10)
					{
						sbomb[i].x+=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2<-10)
					{
						sbomb[i].x-=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,-PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
				}
				else
					if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>0)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
						sbomb[i].v=-1;
			}
			
			else			
				if(sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel && sbomb[i].v>-1 && i<=sbombmax)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
					}
				else sbomb[i].v=-stay*2;//炸弹接触海平面
	//潜艇及其炸弹的移动
	}

	for (i=0;i<=dropmax;i++)
		if (drop[i].v>-1)
			if(drop[i].y<width)
			{
				drop[i].y+=drop[i].v;
				drop[i].r.bottom=drop[i].y+drop[i].img.getheight();
				drop[i].r.top=drop[i].y;
			}
			else
				drop[i].v=-1;
	//船投弹的移动

	int xx,yy;
	for (i=0;i<=bulletmax;i++)
		if (bullet[i].v>-1)
			if(bullet[i].v>0)
			{
				xx=bullet[i].v *(-sin(bullet[i].angle));
				yy=bullet[i].v *cos(bullet[i].angle);
				bullet[i].x+=xx;
				bullet[i].r.left=bullet[i].x;
				bullet[i].r.right=bullet[i].x+bullet[i].img.getwidth();
				bullet[i].y-=yy;
				bullet[i].r.top=bullet[i].y;
				bullet[i].r.bottom=bullet[i].y+bullet[i].img.getheight();
			}
	//船子弹的移动
}


void movesub()//物体移动的数据处理
{
	int i;
	float f;
	if (p1.v>-1)//j变身的左右移动
	{
		if ((GetAsyncKeyState('A') &0x8000) && p1.r.left>0) 
			{
				p1.x-=p1.v;
				p1.r.left=p1.x;
				p1.r.right=p1.x+p1.img.getwidth();
				p1.img=p1sub;
				p1.img1=p1.img;
				yanmatu(&(p1.img1));
			}
		if ((GetAsyncKeyState('W') &0x8000) && p1.r.top>sealevel) 
			{
				p1.y-=p1.v;
				p1.r.top=p1.y;
				p1.r.bottom=p1.y+p1.img.getheight();
			}
		if ((GetAsyncKeyState('D') &0x8000) && p1.r.right<length) 
			{
				p1.x+=p1.v;
				p1.r.left=p1.x;
				p1.r.right=p1.x+p1.img.getwidth();
				p1.img=p1subr;
				p1.img1=p1.img;
				yanmatu(&(p1.img1));
			}
		if ((GetAsyncKeyState('S') &0x8000) && p1.r.bottom<width-(width-sealevel)*1/3) 
			{
				p1.y+=p1.v;
				p1.r.top=p1.y;
				p1.r.bottom=p1.y+p1.img.getheight();
			}
			//船的移动

		if((GetAsyncKeyState(VK_SPACE) &0x8000)  && (count-tdrop>attackvelocity) )
			for (i=0;i<=dropmax;i++)
				if (drop[i].v==-1)
				{
					tdrop=count;
					drop[i]=drop1;
					drop[i].x=p1.x+p1.img.getwidth()/2;
					drop[i].y=p1.y;
					drop[i].r.left=drop[i].x;
					drop[i].r.top=drop[i].y;
					drop[i].r.right=drop[i].x+drop[i].img.getwidth();
					drop[i].r.bottom=drop[i].y+drop[i].img.getheight();//drop
					score-=35;
					break;
				}//放炸弹

		while (MouseHit())//捕捉鼠标信息
		{
			msg=GetMouseMsg();	
		}
	}

	IMAGE temp;
	for (i=0;i<=planemax;i++)
	{
		if (plane[i].v>-1)
		{
			if (plane[i].lr==1)
			{
				if ((pbomb[i].v==-1) && (plane[i].bombplace<=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹
				}	
				plane[i].x+=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
			else
			{	
				if ((pbomb[i].v==-1) && (plane[i].bombplace>=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=-plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹	
				}
				plane[i].x-=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
		}
		else if (plane[i].v==-1&& whetherrand) randomize_enemy(plane[i],1);//初始化飞机
	
		if (pbomb[i].v>-1 && i<=pbombmax) 
			if(pbomb[i].r.top+pbomb[i].img.getheight()/2<sealevel)
			{
				pbomb[i].v++;
				pbomb[i].y+=pbomb[i].v/12;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].vy=pbomb[i].v/12;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].v/12);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			else
				pbomb[i].v=-stay;//炸弹停留在海平面上

		if (pbomb[i].v<-1 && i<=pbombmax) 
			{
				pbomb[i].vx=pbomb[i].vx/1.13;
				pbomb[i].vy=pbomb[i].vy/1.07;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹//炸弹在海平面上的移动
				pbomb[i].y+=pbomb[i].vy;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].vy);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			
	}
	//飞机及其炸弹的移动

	for (i=0;i<=fishmax;i++)
	{
		if (fish[i].v>-1)
		{
			if (fish[i].lr==1)
			{
				fish[i].x+=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
			else
			{
				fish[i].x-=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
		}
		else if (fish[i].v==-1 && whetherrand) randomizefish(fish[i]);//初始化鱼
	}


	for (i=0;i<=submax;i++)
	{
		if (submarine[i].v>-1)
		{
			if (submarine[i].lr==1)
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace<=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
				}	
				submarine[i].x+=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
			else
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace>=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
					}
				submarine[i].x-=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
		}
		else if (submarine[i].v==-1 && whetherrand) randomize_enemy(submarine[i],0);//初始化潜艇

		if (sbomb[i].v>-1 && i<=sbombmax)
			if (sbomb[i].vx==666)
			{
				if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel*28/20 )
				{
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2>10)
					{
						sbomb[i].x+=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2<-10)
					{
						sbomb[i].x-=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,-PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
				}
				else
					if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>0)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
						sbomb[i].v=-1;
			}
			
			else			
				if(sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel && sbomb[i].v>-1)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
					}
				else sbomb[i].v=-stay*2;//炸弹接触海平面
	//潜艇及其炸弹的移动
	}

	for (i=0;i<=dropmax;i++)
		if (drop[i].v>-1)
			if(drop[i].y<width)
			{
				drop[i].y+=drop[i].v;
				drop[i].r.bottom=drop[i].y+drop[i].img.getheight();
				drop[i].r.top=drop[i].y;
			}
			else
				drop[i].v=-1;
	//船投弹的移动

	int xx,yy;
	for (i=0;i<=bulletmax;i++)
		if (bullet[i].v>-1)
			if(bullet[i].v>0)
			{
				xx=bullet[i].v *(-sin(bullet[i].angle));
				yy=bullet[i].v *cos(bullet[i].angle);
				bullet[i].x+=xx;
				bullet[i].r.left=bullet[i].x;
				bullet[i].r.right=bullet[i].x+bullet[i].img.getwidth();
				bullet[i].y-=yy;
				bullet[i].r.top=bullet[i].y;
				bullet[i].r.bottom=bullet[i].y+bullet[i].img.getheight();
			}
	//船子弹的移动
}


void moveplane()//物体移动的数据处理
{
	int i;
	float f;
	int temptime;
	if (p1.v>-1)//的左右移动
	{
		if ((GetAsyncKeyState('A') &0x8000) && p1.r.left>0) 
			{
				p1.x-=p1.v;
				p1.r.left=p1.x;
				p1.r.right=p1.x+p1.img.getwidth();
				p1.img=p1plane;
				p1.img1=p1.img;
				yanmatu(&(p1.img1));
			}
		if ((GetAsyncKeyState('W') &0x8000) && p1.r.top>sealevel*1/3) 
			{
				p1.y-=p1.v;
				p1.r.bottom=p1.y+p1.img.getheight();
				p1.r.top=p1.y;
			}
		if ((GetAsyncKeyState('D') &0x8000) && p1.r.right<length) 
			{
				p1.x+=p1.v;
				p1.r.left=p1.x;
				p1.r.right=p1.x+p1.img.getwidth();
				p1.img=p1planer;
				p1.img1=p1.img;
				yanmatu(&(p1.img1));
			}
		if ((GetAsyncKeyState('S') &0x8000) && p1.r.bottom<sealevel) 
			{
				p1.y+=p1.v;
				p1.r.bottom=p1.y+p1.img.getheight();
				p1.r.top=p1.y;
			}
			//船的移动

	/*	if((GetAsyncKeyState(VK_SPACE) &0x8000)  && (count-tdrop>attackvelocity) )
			for (i=0;i<=dropmax;i++)
				if (drop[i].v==-1)
				{
					tdrop=count;
					drop[i]=drop1;
					drop[i].x=p1.x+p1.img.getwidth()/2;
					drop[i].y=p1.y;
					drop[i].r.left=drop[i].x;
					drop[i].r.top=drop[i].y;
					drop[i].r.right=drop[i].x+drop[i].img.getwidth();
					drop[i].r.bottom=drop[i].y+drop[i].img.getheight();//drop
					score-=35;
					break;
				}//放炸弹
*/
		while (MouseHit())//捕捉鼠标信息
		{
			msg=GetMouseMsg();
		float angle = atan2(p1.x-msg.x,p1.y-msg.y);
		if (msg.mkLButton && (count-tbullet>attackvelocity) )
			for (i=0;i<=bulletmax;i++)
				if (bullet[i].v==-1)
				{
					tbullet=count;
					IMAGE temp;
					bullet[i]=bullet1;
					bullet[i].angle = atan2(p1.x-msg.x,p1.y-msg.y);
					rotateimage(&temp,&bullet1.img,bullet[i].angle);
					bullet[i].img=temp;
					bullet[i].img1=yanmatu(&(temp));//定义各种炸弹
					bullet[i].x=p1.x;	
					bullet[i].y=p1.y;	
					bullet[i].r.left=p1.x;
					bullet[i].r.top=p1.y;
					bullet[i].r.right=p1.x+bullet[i].img.getwidth();
					bullet[i].r.bottom=p1.y+bullet[i].img.getheight();
					score-=30;
					break;
				//初始化子弹参数
				}
		}
	}

	IMAGE temp;
	for (i=0;i<=planemax;i++)
	{
		if (plane[i].v>-1)
		{
			if (plane[i].lr==1)
			{
				if ((pbomb[i].v==-1) && (plane[i].bombplace<=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹
				}	
				plane[i].x+=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
			else
			{	
				if ((pbomb[i].v==-1) && (plane[i].bombplace>=plane[i].x) && plane[i].bombplace!=0 && i<=pbombmax)
				{
					plane[i].bombplace=0;
					pbomb[i]=pbomb1;
					pbomb[i].vx=-plane[i].v;
					pbomb[i].v=0;
					pbomb[i].x=plane[i].x+pbomb[i].img.getwidth()/2;
					pbomb[i].y=plane[i].y;
					pbomb[i].r.left=pbomb[i].x;
					pbomb[i].r.top=pbomb[i].y;
					pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();
					pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();//投掷炸弹	
				}
				plane[i].x-=plane[i].v;
				plane[i].r.left=plane[i].x;
				plane[i].r.right=plane[i].x+plane[i].img.getwidth();//移动飞机
			}
		}
		else if (plane[i].v==-1 && whetherrand) randomize_enemy(plane[i],1);//初始化飞机
	
		if (pbomb[i].v>-1 && i<=pbombmax) 
			if(pbomb[i].r.top+pbomb[i].img.getheight()/2<sealevel)
			{
				pbomb[i].v++;
				pbomb[i].y+=pbomb[i].v/12;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].vy=pbomb[i].v/12;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].v/12);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			else
				pbomb[i].v=-stay;//炸弹停留在海平面上

		if (pbomb[i].v<-1 && i<=pbombmax) 
			{
				pbomb[i].vx=pbomb[i].vx/1.13;
				pbomb[i].vy=pbomb[i].vy/1.07;
				pbomb[i].x+=pbomb[i].vx;
				pbomb[i].r.left=pbomb[i].x;
				pbomb[i].r.right=pbomb[i].x+pbomb[i].img.getwidth();//移动炸弹//炸弹在海平面上的移动
				pbomb[i].y+=pbomb[i].vy;
				pbomb[i].r.top=pbomb[i].y;
				pbomb[i].r.bottom=pbomb[i].y+pbomb[i].img.getheight();
				pbomb[i].angle = atan2(pbomb[i].vx,pbomb[i].vy);
				rotateimage(&temp,&pbomb1.img,pbomb[i].angle);
				pbomb[i].img=temp;
				pbomb[i].img1=yanmatu(&(temp));//定义各种炸弹
			}
			
	}
	//飞机及其炸弹的移动

	for (i=0;i<=fishmax;i++)
	{
		if (fish[i].v>-1)
		{
			if (fish[i].lr==1)
			{
				fish[i].x+=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
			else
			{
				fish[i].x-=fish[i].v;
				fish[i].r.left=fish[i].x;
				fish[i].r.right=fish[i].x+fish[i].img.getwidth();//移动鱼
			}
		}
		else if (fish[i].v==-1 && whetherrand) randomizefish(fish[i]);//初始化鱼
	}


	for (i=0;i<=submax;i++)
	{
		if (submarine[i].v>-1)
		{
			if (submarine[i].lr==1)
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace<=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
				}	
				submarine[i].x+=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
			else
			{
				if ((sbomb[i].v==-1) && (submarine[i].bombplace>=submarine[i].x) && submarine[i].bombplace!=0 && i<=sbombmax)
				{
					submarine[i].bombplace=0;
					sbomb[i]=sbomb1;
					sbomb[i].x=submarine[i].x+sbomb[i].img.getwidth()/2;
					sbomb[i].y=submarine[i].y;
					sbomb[i].r.left=sbomb[i].x;
					sbomb[i].r.top=sbomb[i].y;
					sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();
					sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//投掷炸弹
					if (submarine[i].money==subl3.money)
					{
						sbomb[i].vx=666;
						sbomb[i].v=sbomb[i].v*1.5;
						sbomb[i].img=pbombimage;
						sbomb[i].img1=pbombimage;
						yanmatu(&(sbomb[i].img1));
					}
					}
				submarine[i].x-=submarine[i].v;
				submarine[i].r.left=submarine[i].x;
				submarine[i].r.right=submarine[i].x+submarine[i].img.getwidth();//移动潜艇
			}
		}
		else if (submarine[i].v==-1 && whetherrand) randomize_enemy(submarine[i],0);//初始化潜艇

		if (sbomb[i].v>-1 && i<=sbombmax)
			if (sbomb[i].vx==666)
			{
				if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel*28/20 )
				{
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2>10)
					{
						sbomb[i].x+=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					if (p1.x+p1.img.getwidth()/2-sbomb[i].x-sbomb[i].img.getwidth()/2<-10)
					{
						sbomb[i].x-=sbomb[i].v/1.414;
						sbomb[i].r.left=sbomb[i].x;
						sbomb[i].r.right=sbomb[i].x+sbomb[i].img.getwidth();//移动炸弹
						sbomb[i].y-=sbomb[i].v/1.414;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,-PI*3/4);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
				}
				else
					if (sbomb[i].r.bottom-sbomb[i].img.getheight()/2>0)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
						rotateimage(&sbomb[i].img,&pbombimage,PI);
						sbomb[i].img1=sbomb[i].img;
						yanmatu(&(sbomb[i].img1));
					}
					else
						sbomb[i].v=-1;
			}
			
			else			
				if(sbomb[i].r.bottom-sbomb[i].img.getheight()/2>sealevel && sbomb[i].v>-1)
					{
						sbomb[i].y-=sbomb[i].v;
						sbomb[i].r.top=sbomb[i].y;
						sbomb[i].r.bottom=sbomb[i].y+sbomb[i].img.getheight();//移动炸弹
					}
				else sbomb[i].v=-stay*2;//炸弹接触海平面
	//潜艇及其炸弹的移动
	}

	for (i=0;i<=dropmax;i++)
		if (drop[i].v>-1)
			if(drop[i].y<width)
			{
				drop[i].y+=drop[i].v;
				drop[i].r.bottom=drop[i].y+drop[i].img.getheight();
				drop[i].r.top=drop[i].y;
			}
			else
				drop[i].v=-1;
	//船投弹的移动

	int xx,yy;
	for (i=0;i<=bulletmax;i++)
		if (bullet[i].v>-1)
			if(bullet[i].v>0)
			{
				xx=bullet[i].v *(-sin(bullet[i].angle));
				yy=bullet[i].v *cos(bullet[i].angle);
				bullet[i].x+=xx;
				bullet[i].r.left=bullet[i].x;
				bullet[i].r.right=bullet[i].x+bullet[i].img.getwidth();
				bullet[i].y-=yy;
				bullet[i].r.top=bullet[i].y;
				bullet[i].r.bottom=bullet[i].y+bullet[i].img.getheight();
			}
	//船子弹的移动
}


velocityadjustment()//调整适合的延迟时间
{
	long velocity,nowtime;
	nowtime=timeGetTime();
	velocity=1000/framerates-nowtime+timeloop;
	if (velocity<0) velocity=0;
	Sleep(velocity);//延时
	timeloop=timeGetTime();
}


void level(int bulletx,int dropx,int subx,int planex,int sbombx,int pbombx)//游戏难度设置
{
	bulletmax=bulletx-1/*子弹数*/;
	dropmax=dropx-1/*投弹数*/;
	sbombmax=sbombx-1/*潜艇炸弹数*/;
	pbombmax=pbombx-1/*飞机子弹数*/;
	planemax=planex-1/*飞机数*/;
	submax=subx-1/*潜艇数*/;
	//fishmax=fish-1/*鱼数*/;
}


void pau(int left,int top,int right,int bottom)
{
	if( msg.x>=left &&msg.x<=right &&msg.y>=top &&msg.y<=bottom && (count-tclick>attackvelocity))
		if(msg.mkLButton)
		{
			tclick=count;
			pausestate=!pausestate;
		}
	return ;
}


bool Careergame(int bulletn,int dropn,int subn,int planen,int sbombn,int pbombn,int goal,int levelnum)//基本游戏单位
{//Normal   Challenge mode  Infinite mode  Config Information Exit
	//key config  level  quality  time  life
	result=true;
	int vol=0;
	TCHAR cmd[256]; // 媒体命令
	TCHAR volume[256]; // 音量(字符串)
	TCHAR s[20];
	int nVolume; // 音量(整型)
	while (result)
	{
		bool o=false;
		int k,x=length,y=0; 
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		FlushMouseMsgBuffer();
		gameinitialize();//游戏初始化
		for (k=sqrt(length*4);k>1;k--)
		{
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,7);
			Sleep(7);
		}
		for (k=1;k<25*length/1280;k++)
		{
			putimage(x,y,&fight);
			x-=2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,10);
			Sleep(10);
		}

		level(bulletn,dropn,0,0,0,0);
		for (k=5;k<=sqrt(length*4);k++)
		{
			judge();
			move();
			velocityadjustment();
			draw(levelnum);
			settextcolor(RED);
			setfont(21, 0, _T("Arial"));
			sprintf(s,"GOAL : %d",goal);//绘制分数
			outtextxy(length/2+130,20,s);
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,7);
			Sleep(7);
		}
		
		starttime=time(0);
		level(bulletn,dropn,subn,planen,sbombn,pbombn);
		whetherrand=true;
		while(result)
		{
			if (pausestate==false)
			{
			judge();//判断是否碰撞
			if (result==0) //游戏失败
				for (k=1;k<=80;k++)
				{
					judge();
					move();
					velocityadjustment();
					draw(levelnum);
					settextcolor(RED);
					setfont(21, 0, _T("Arial"));
					sprintf(s,"GOAL : %d",goal);//绘制分数
					outtextxy(length/2+130,20,s);
					FlushBatchDraw();//统一输出图像
				}
			move();//移动处理
			}
			velocityadjustment();
			draw(levelnum);//绘制图像
			settextcolor(RED);
			setfont(21, 0, _T("Arial"));
			sprintf(s,"GOAL : %d",goal);//绘制分数
			outtextxy(length/2+130,20,s);
			FlushBatchDraw();//统一输出图像
			pau(1225*width/720,20*width/720,1260*width/720,55*width/720);
			if (pausestate==true)
			{
				while (MouseHit())//捕捉鼠标信息
					msg=GetMouseMsg();	
				if (nowtime!=time(NULL)-starttime)
					starttime++;
			}
			nowtime=time(NULL)-starttime;
			if (pausestate==false)
			if (score>=goal) result=0;
		}
		if (score>=goal) 
		{
			whetherrand=false;
				for (k=1;k<=350;k++)
				{
					judge();
					move();
					velocityadjustment();
					draw(levelnum);
					settextcolor(RED);
					setfont(21, 0, _T("Arial"));
					sprintf(s,"GOAL : %d",goal);//绘制分数
					outtextxy(length/2+130,20,s);
					sprintf(s,"SCORE : %d",score);//绘制分数
					outtextxy(length/2,20,s);
					setfont(60, 0, _T("等线"));
					sprintf(s,"GREAT!");//绘制分数
					outtextxy(length/2-90,140*width/720,s);
					FlushBatchDraw();//统一输出图像
				}
			whetherrand=true;
			return true;
		}
		else
		lose();//失败处理
	}
	return false;
}


bool Normalgame(int bulletn,int dropn,int subn,int planen,int sbombn,int pbombn,int t,int mm)//基本游戏单位
{//Normal   Challenge mode  Infinite mode  Config Information Exit
	//key config  level  quality  time  life
	result=true;
	TCHAR cmd[20];
	int k;
	switch (mm)
	{
		case 1:mciSendString(_T("open \"Resource\\music\\fightbgm.mp3\" alias bgm"), NULL, 0, NULL);break;
		case 2:mciSendString(_T("open \"Resource\\music\\mediumbgm.mp3\" alias bgm"), NULL, 0, NULL);break;
		case 3:mciSendString(_T("open \"Resource\\music\\hardbgm.mp3\" alias bgm"), NULL, 0, NULL);break;
	}
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);// 播放音乐
	//turnvolume("bgm",0);
	while (result)
	{
		bool o=false;
		int x=length,y=0; 
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		FlushMouseMsgBuffer();
		gameinitialize();//游戏初始化
		level(bulletn,dropn,0,0,0,0);

		for (k=sqrt(length*4);k>1;k--)
		{
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,7);
			Sleep(7);
		}
		for (k=1;k<25*length/1280;k++)
		{
			putimage(x,y,&fight);
			x-=2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,10);
			Sleep(10);
		}
		for (k=1;k<=sqrt(length*4);k++)
		{
			judge();
			move();
			velocityadjustment();
			draw(mm);
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("bgm",0,1000,7);
			Sleep(7);
		}

		starttime=time(0);
		starttime+=t;
		level(bulletn,dropn,subn,planen,sbombn,pbombn);
		while(result)
		{
			if (pausestate==false)
			{
			judge();//判断是否碰撞
			if (result==0) //游戏失败
				for (k=1;k<=80;k++)
				{
					judge();
					move();
					velocityadjustment();
					draw(mm);
					FlushBatchDraw();//统一输出图像
				}
			move();//移动处理
			}
			velocityadjustment();
			draw(mm);//绘制图像
			FlushBatchDraw();//统一输出图像
			pau(1225*width/720,20*width/720,1260*width/720,55*width/720);
			if (pausestate==true)
			{
				while (MouseHit())//捕捉鼠标信息
					msg=GetMouseMsg();	
				if (nowtime!=time(NULL)-starttime)
					starttime++;
			}
			nowtime=time(NULL)-starttime;

			if (nowtime>=0) result=0;
		}
		if (nowtime>=0) 
		{
			whetherrand=false;
			TCHAR s[20];
				for (k=1;k<=350;k++)
				{
					judge();
					move();
					velocityadjustment();
					starttime=time(0);
					draw(mm);
					settextcolor(RED);
					//setfont(20, 0, _T("Arial"));
					//TCHAR s[20];
					//sprintf(s,"SCORE : %d",score);//绘制分数
					//outtextxy(length/2,20,s);
					setfont(60, 0, _T("等线"));
					sprintf(s,"TIME OVER");//绘制分数
					outtextxy(length/2-150,140*width/720,s);
					FlushBatchDraw();//统一输出图像
				}
			whetherrand=true;

			TCHAR ss[20];
			switch (mm)
			{
				case 1: sprintf(ss,"%s","EASY");break;
				case 2: sprintf(ss,"%s","MEDIUM");break;
				case 3: sprintf(ss,"%s","DIFFICULT");break;
			}
			writename(ss,score);
			putimage(0,0,&rankimg);
			printname(ss);
			FlushBatchDraw();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (!kbhit() || getch()!=27);
		IMAGE timg;
		getimage(&timg,0,0,length,width);
		for (k=100;k>=0;k--)
		{
			shade(k,0,0,timg);//处理图片明暗
			wsprintf(cmd, "status %s volume","bgm");
			wsprintf(cmd, "setaudio %s volume to %i","bgm",k*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
			mciSendString(_T("close bgm",), NULL, 0, NULL);			

			return true;	
			}
		else
			lose();//失败处理
	}

	{
		TCHAR ss[20];
		switch (mm)
		{
		case 1: sprintf(ss,"%s","EASY");break;
		case 2: sprintf(ss,"%s","MEDIUM");break;
		case 3: sprintf(ss,"%s","DIFFICULT");break;
		}
		putimage(0,0,&rankimg);
		FlushBatchDraw();
		writename(ss,score);
		printname(ss);
		FlushBatchDraw();
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (!kbhit() || getch()!=27);
		IMAGE timg;
		getimage(&timg,0,0,length,width);
		for (k=100;k>=0;k--)
		{
			shade(k,0,0,timg);//处理图片明暗
			wsprintf(cmd, "status %s volume","bgm");
			wsprintf(cmd, "setaudio %s volume to %i","bgm",k*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
		mciSendString(_T("close bgm"), NULL, 0, NULL);
	}
	return false;
}


void Infinitemode(int nbullet,int ndrop,int nsub,int nplane,int nsbomb,int npbomb)//基本游戏单位
{//Normal   Challenge mode  Infinite mode  Config Information Exit
	//key config  level  quality  time  life
	int k; IMAGE timg;
	TCHAR cmd[256]; // 媒体命令
	int leveluptime=10;//升级时间
	result=true;
	mciSendString(_T("open \"Resource\\music\\fightbgm.mp3\" alias fightbgm"), NULL, 0, NULL);
	mciSendString(_T("play fightbgm repeat"), NULL, 0, NULL);// 播放音乐
	turnvolume("fightbgm",0);
	while (result)
	{
		bool o=false;
		int k,x=length,y=0 ,mode=2;
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		FlushMouseMsgBuffer();
		gameinitialize();//游戏初始化	
		for (k=sqrt(length*4);k>1;k--)
		{
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,7);
			Sleep(7);
		}
		for (k=1;k<25*length/1280;k++)
		{
			putimage(x,y,&fight);
			x-=2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,10);
			Sleep(10);
		}

		level(nbullet,ndrop,0,0,0,0);
		for (k=1;k<=sqrt(length*4);k++)
		{
			judge();
			move();
			velocityadjustment();
			draw(1);
			putimage(x,y,&fight);
			x-=k/2;
			FlushBatchDraw();
			chanvolume("fightbgm",0,1000,7);
			Sleep(7);
		}
		
		starttime=time(0);
		level(nbullet,ndrop,nsub,nplane,nsbomb,npbomb);
		while(result)
		{
			if (pausestate==false)
			{
				judge();//判断是否碰撞
			if (result==0) //游戏失败
				for (k=1;k<=80;k++)
				{
					judge();
					move();
					velocityadjustment();
					draw(1);
					FlushBatchDraw();//统一输出图像
				}
			mode=modechoose(mode,100,200);
			switch (mode)
			{
				case 1:	movesub();break;
				case 2:	move();break;
				case 3:	moveplane();break;
			}
			}
			velocityadjustment();
			draw(1);//绘制图像
			FlushBatchDraw();//统一输出图像
			pau(1225*width/720,20*width/720,1260*width/720,55*width/720);
			if (pausestate==true)
			{
				while (MouseHit())//捕捉鼠标信息
					msg=GetMouseMsg();	
				if (nowtime!=time(NULL)-starttime)
					starttime++;
			}
			if (nowtime==time(NULL)-starttime-1 && mode==3)
				score-=15;
			if (nowtime==time(NULL)-starttime-1 && mode==1)
				score-=20;
			nowtime=time(NULL)-starttime;

			if (pausestate==false)
				if (nowtime/leveluptime>50)
					level(nbullet+41,ndrop+41,nsub+50,nplane+50,nsbomb+50,npbomb+50);
				else
					level(nbullet+(nowtime/(leveluptime+3)),ndrop+(nowtime/(leveluptime+3)),nsub+(nowtime/leveluptime),nplane+(nowtime/leveluptime),nsbomb+(nowtime/leveluptime),npbomb+(nowtime/leveluptime));
		}
		lose();//失败处理
	}
	//排行榜处理
	{
		putimage(0,0,&rankimg);
		FlushBatchDraw();
		writename("ENDLESS",score);
		printname("ENDLESS");
		FlushBatchDraw();
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (!kbhit() || getch()!=27);
		getimage(&timg,0,0,length,width);
		for (k=100;k>=0;k--)
		{
			shade(k,0,0,timg);//处理图片明暗
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",k*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
	}
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
}


void career()//生涯模式
{//Normal   Challenge mode  Infinite mode  Config Information Exit
	//key config  level  quality  time  life
	TCHAR cmd[256]; // 媒体命令
	IMAGE timg;
	loadimage(&story1,_T("IMAGE"), _T("story1"),width*16/9,width);
	loadimage(&story2,_T("IMAGE"), _T("story2"),width*16/9,width);
	loadimage(&dialog,_T("IMAGE"), _T("dialog"),width*16/9,width);
	loadimage(&beginning,_T("IMAGE"), _T("beginning"),804*width/720,width);
	loadimage(&level1,_T("IMAGE"), _T("l1"),885*width/720,width);
	loadimage(&level2,_T("IMAGE"), _T("l2"),891*width/720,width);
	int i,total=0; bool o=false;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	mciSendString(_T("open \"Resource\\music\\longlong.mp3\" alias longlong"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\oneday.mp3\" alias oneday"), NULL, 0, NULL);

//第一关
	mciSendString(_T("play longlong"), NULL, 0, NULL);// 播放音乐
	for (i=1;i<100;i++)
	{
		shade(i, 0, 0, story1);
		if (kbhit()) o=true;
		Sleep(4);
	}
	if (o==false)
	{
		for (i=1;i<1100;i++)
		{
			Sleep(10);
			if (kbhit()) 
			{
				o=true;
				break;
			}
		}
	}
	for (i=100;i>1;i--)
	{
		shade(i, 0, 0, story1);
		if (kbhit()) o=true;
		Sleep(4);
		if (o)	chanvolume("longlong",0,1001,-20);
	}

	if (o==false)
	{
		mciSendString(_T("play oneday"), NULL, 0, NULL);// 播放音乐
		for (i=1;i<100;i++)
		{
			shade(i, 0, 0, story2);
			if (kbhit()) o=true;
			Sleep(4);
		}	
		if (o==false)
		{
			for (i=1;i<1000;i++)
			{
				Sleep(10);
				if (kbhit()) 
				{
					o=true;
					break;
				}
			}
		}
		for (i=100;i>1;i--)
		{
			shade(i, 0, 0, story2);
			if (kbhit()) o=true;
			Sleep(4);
			if (o) chanvolume("longlong",0,1001,-20);
		}

	mciSendString(_T("close longlong"), NULL, 0, NULL);
	}

	if (o==false)                                                                                                                      
	{
		mciSendString(_T("open \"Resource\\music\\narrator1.mp3\" alias narrator1"), NULL, 0, NULL);
		for (i=1;i<100;i++)
		{
			shade(i, 0, 0, dialog);
			if (kbhit()) o=true;
			Sleep(4);
		}	
		mciSendString(_T("play narrator1"), NULL, 0, NULL);
		for (i=1;i<100;i++)
		{
			shade(i,(1280- 804)*width/720, 0, beginning);
			if (kbhit()) o=true;
			Sleep(4);
		}	
		if (o==false)
		{
			for (i=1;i<1900;i++)
			{
				Sleep(10);
				if (kbhit()) 
				{
					o=true;
					break;
				}
			}
		}
		getimage(&timg,0,0,length,width);
		for (i=100;i>1;i--)
		{
			shade(i, 0, 0, timg);
			if (kbhit()) o=true;
			Sleep(4);
			if (o) chanvolume("narrator1",0,1001,-20);
		}
		mciSendString(_T("close narrator1"), NULL, 0, NULL);
	}
	mciSendString(_T("close oneday"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\fightbgm.mp3\" alias fightbgm"), NULL, 0, NULL);
	mciSendString(_T("play fightbgm repeat"), NULL, 0, NULL);// 播放音乐
	turnvolume("fightbgm",0);
	if (!Careergame(4,5,3,3,3,2,600,1)) 
	{
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,losebk);//处理图片明暗
			//chanvolume("fightbgm",0,1001,-10);
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
		return ;
	}
	else total+=score;


//第二关
	o=false;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	if (o==false)
	{
		mciSendString(_T("open \"Resource\\music\\narrator2.mp3\" alias narrator2"), NULL, 0, NULL);
		for (i=1;i<100;i++)
		{
			shade(i, 0, 0, dialog);
			//if (kbhit()) o=true;
			Sleep(4);
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",1000-i*6); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
		}	
		mciSendString(_T("play narrator2"), NULL, 0, NULL);
		turnvolume("narrator2",1000);
		for (i=1;i<100;i++)
		{
			shade(i,(1280-885)*width/720, 0, level1);
			if (kbhit()) o=true;
			Sleep(4);
		}	
		if (o==false)
		{
			for (i=1;i<900;i++)
			{
				Sleep(10);
				if (kbhit()) 
				{
					o=true;
					break;
				}
			}
		}
		getimage(&timg,0,0,length,width);
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,timg);//处理图片明暗
			if (kbhit()) o=true;
			Sleep(4);
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*4); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			//chanvolume("fightbgm",0,1001,-10);
			if (o) chanvolume("narrator2",0,1001,-20);
		}
		mciSendString(_T("close narrator2"), NULL, 0, NULL);
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
	}

	mciSendString(_T("open \"Resource\\music\\mediumbgm.mp3\" alias fightbgm"), NULL, 0, NULL);
	mciSendString(_T("play fightbgm repeat"), NULL, 0, NULL);// 播放音乐
	turnvolume("fightbgm",0);
	if (!Careergame(5,5,6,5,5,4,900,2))
	{
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,losebk);//处理图片明暗
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
		return ;
	} 
	else total+=score;
	
//第三关
	o=false;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	if (o==false)
	{
		mciSendString(_T("open \"Resource\\music\\narrator3.mp3\" alias narrator3"), NULL, 0, NULL);
		for (i=1;i<100;i++)
		{
			shade(i, 0, 0, dialog);
			//if (kbhit()) o=true;
			Sleep(4);
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",1000-i*6); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
		}	
		mciSendString(_T("play narrator3"), NULL, 0, NULL);
		turnvolume("narrator3",1000);
		for (i=1;i<100;i++)
		{
			shade(i,(1280-891)*width/720, 0, level2);
			if (kbhit()) o=true;
			Sleep(4);
		}	
		if (o==false)
		{
			for (i=1;i<1200;i++)
			{
				Sleep(10);
				if (kbhit()) 
				{
					o=true;
					break;
				}
			}
		}
		getimage(&timg,0,0,length,width);
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,timg);//处理图片明暗
			if (kbhit()) o=true;
			Sleep(4);
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*4); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			//chanvolume("fightbgm",0,1001,-10);
			if (o) chanvolume("narrator3",0,1001,-20);
		}
		mciSendString(_T("close narrator3"), NULL, 0, NULL);
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
	}
	mciSendString(_T("open \"Resource\\music\\hardbgm.mp3\" alias fightbgm"), NULL, 0, NULL);
	mciSendString(_T("play fightbgm repeat"), NULL, 0, NULL);// 播放音乐
	turnvolume("fightbgm",0);
	if (!Careergame(6,6,8,8,7,6,1200,3)) 
	{
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,losebk);//处理图片明暗
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			Sleep(1);
		}
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
		return ;
	}
	else total+=score;
	o=false;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	for (i=1;i<50;i++)
	{
		shade(i*2, 0, 0, youwin);
		Sleep(3);
	}
	if (o==false)
	{
		putimage(0,0,&youwin);
		FlushBatchDraw();//统一输出图像
		Sleep(9000);
	}
	for (i=50;i>1;i--)
	{
		shade(i*2, 0, 0, youwin);
		Sleep(3);
	}

	{
		putimage(0,0,&rankimg);
		FlushBatchDraw();
		writename("CAREER",total);
		printname("CAREER");
		FlushBatchDraw();
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (!kbhit() || getch()!=27);
		getimage(&timg,0,0,length,width);
		for (i=100;i>=0;i--)
		{
			shade(i,0,0,timg);//处理图片明暗
			wsprintf(cmd, "status %s volume","fightbgm");
			wsprintf(cmd, "setaudio %s volume to %i","fightbgm",i*10); // 生成媒体命令 
			mciSendString(cmd,"",0,NULL); // 执行媒体命令
			//chanvolume("fightbgm",0,1001,-10);
		}
		mciSendString(_T("close fightbgm"), NULL, 0, NULL);
	}
}


void init()//游戏开始界面
{
	IMAGE loading,imgbtn1,imgbtn2;
	IMAGE background1,rankimgl,rankimgr;//背景图
	int k; float n=1.1; char c=66;
	loadimage(&loading,_T("IMAGE"), _T("loading"),width*16/9,width);
	for (k=0;k<=100;k++)
	{
		shade(k,0,0,loading);//处理图片明暗
		Sleep(4);
	}
	loadimage(&background1,_T("IMAGE"), _T("start"),width*16/9,width);
	loadimage(&oceanbkg,_T("IMAGE"), _T("oceanbkg"),width*16/9,width);
	loadimage(&careerbtn,_T("IMAGE"), _T("career"),width*16/9,width);
	loadimage(&crew,_T("IMAGE"), _T("crew"),width*16/9,width);
	loadimage(&difficult,_T("IMAGE"), _T("difficult"),width*16/9,width);
	loadimage(&easy,_T("IMAGE"), _T("easy"),width*16/9,width);
	loadimage(&endless,_T("IMAGE"), _T("endless"),width*16/9,width);
	loadimage(&exitbtn,_T("IMAGE"), _T("exit"),width*16/9,width);
	loadimage(&guild,_T("IMAGE"), _T("guild"),width*16/9,width);
	loadimage(&medium,_T("IMAGE"), _T("medium"),width*16/9,width);
	loadimage(&menu,_T("IMAGE"), _T("menu"),width*16/9,width);
	loadimage(&normalmenu,_T("IMAGE"), _T("normalmenu"),width*16/9,width);
	loadimage(&normal,_T("IMAGE"), _T("normal"),width*16/9,width);
	loadimage(&optionsmenu,_T("IMAGE"), _T("optionsmenu"),width*16/9,width);
	loadimage(&options,_T("IMAGE"), _T("options"),width*16/9,width);
	loadimage(&rank,_T("IMAGE"), _T("rank"),width*16/9,width);
	loadimage(&retry,_T("IMAGE"), _T("retry"),width*16/9,width);
	loadimage(&help,_T("IMAGE"), _T("help"),width*16/9,width);
	loadimage(&namelist,_T("IMAGE"), _T("namelist"),width*16/9,width);
	loadimage(&youwin,_T("IMAGE"), _T("youwin"),width*16/9,width);
	loadimage(&beginning,_T("IMAGE"), _T("beginning"),width*16/9,width);
	loadimage(&rankimg,_T("IMAGE"), _T("rankimg"),width*16/9,width);
	loadimage(&rankimgl,_T("IMAGE"), _T("rankimgl"),width*16/9,width);
	loadimage(&rankimgr,_T("IMAGE"), _T("rankimgr"),width*16/9,width);

	bool flag=true,smenubgm=1,sfightbgm=0,slonglong=0,soneday=0;int vol;

	mciSendString(_T("open \"Resource\\music\\Ships of Legend.mp3\" alias menubgm"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\shot.mp3\" alias shotbgm"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\planebomb.mp3\" alias planebomb"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\subbomb.mp3\" alias subbomb"), NULL, 0, NULL);
	mciSendString(_T("open \"Resource\\music\\gameoverbgm.mp3\" alias gameoverbgm"), NULL, 0, NULL);

	for (k=100;k>=0;k--)
	{
		shade(k,0,0,loading);//处理图片明暗
		Sleep(4);
	}

	mciSendString(_T("play menubgm repeat"), NULL, 0, NULL);// 播放音乐
	turnvolume("menubgm",0);
	smenubgm=true;
	for (k=0;k<=100;k++)
	{
		shade(k,0,0,background1);//处理图片明暗
		Sleep(2);
		chanvolume("menubgm",0,1000,10);
	}
	putimage(0,0,&background1);
	FlushMouseMsgBuffer();
	FlushBatchDraw();

	while(flag)
	{
		if (smenubgm==false)
		{
			mciSendString(_T("play menubgm repeat"), NULL, 0, NULL);// 播放音乐
			turnvolume("menubgm",0);
			//mciSendString("status movie volume",chVolume,255,0);lVolume=strtol(chVolume,NULL,10);
			vol=1;
			smenubgm=true;
		}
		chanvolume("menubgm",0,1000,8);
		while(MouseHit())
			msg=GetMouseMsg();
		if(click(length*378/1280,width*452/720,length*378/1280+n*162*width/720,width*452/720+n*31*width/720,careerbtn))
		{
			for (k=100;k>=0;k--)
			{
				shade(k,0,0,background1);//处理图片明暗
				chanvolume("menubgm",0,1001,-20);		
				Sleep(2);
			}
			mciSendString(_T("stop menubgm"), NULL, 0, NULL);
			smenubgm=false;
			career();//生涯模式
			for (k=0;k<=100;k++)
				shade(k,0,0,background1);//处理图片明暗
			putimage(0,0,&background1);
			FlushBatchDraw();
		}

		else
		if(click(length*375/1280,width*499/720,length*375/1280+n*172*width/720,width*499/720+n*28*width/720,normal))
		{
			putimage(0,0,&normalmenu);
			FlushBatchDraw();
			bool normalloop=true;
			Sleep(200);
			FlushMouseMsgBuffer();
			msg=GetMouseMsg();
			c=66;
			while(normalloop && c!=27)
			{
				while(MouseHit())
					msg=GetMouseMsg();
				if (kbhit())
					c=getch();	
				if(click(length*378/1280,width*452/720,length*378/1280+n*162*width/720,width*452/720+n*31*width/720,easy))
				{
					for (k=100;k>=0;k--)
					{
						shade(k,0,0,normalmenu);//处理图片明暗
						chanvolume("menubgm",0,1001,-20);	
						Sleep(2);
					}
					mciSendString(_T("stop menubgm"), NULL, 0, NULL);
					smenubgm=false;
					Normalgame(4,5,3,3,2,2,90,1);//普通模式1
					normalloop=false;
				}		
				else
				if(click(length*374/1280,width*549/720,length*374/1280+n*193*width/720,width*549/720+n*25*width/720,medium))
				{
					for (k=100;k>=0;k--)
					{
						shade(k,0,0,normalmenu);//处理图片明暗
						chanvolume("menubgm",0,1001,-20);	
						Sleep(2);
					}
					mciSendString(_T("stop menubgm"), NULL, 0, NULL);
					smenubgm=false;
					Normalgame(5,5,6,5,5,4,60,2);//普通模式2
					normalloop=false;
				}
				else
				if(click(length*377/1280,width*643/720,length*377/1280+n*210*width/720,width*643/720+n*20*width/720,difficult))
				{
					for (k=100;k>=0;k--)
					{
						shade(k,0,0,normalmenu);//处理图片明暗
						chanvolume("menubgm",0,1001,-20);	
						Sleep(2);
					}
					mciSendString(_T("stop menubgm"), NULL, 0, NULL);
					smenubgm=false;
					Normalgame(6,6,10,10,10,10,30,3);//普通模式3
					normalloop=false;
				}
				else
				{
					FlushBatchDraw();
					putimage(0,0,&normalmenu);
				}
			}
			if (c!=27)
				for (k=0;k<=100;k++)
					shade(k,0,0,background1);//处理图片明暗
			putimage(0,0,&background1);
			FlushBatchDraw();
		}

		else
		if(click(length*374/1280,width*549/720,length*374/1280+n*193*width/720,width*549/720+n*25*width/720,endless))
		{
			for (k=100;k>=0;k--)
			{
				shade(k,0,0,background1);//处理图片明暗
				chanvolume("menubgm",0,1001,-20);	
				Sleep(3);
			}
			mciSendString(_T("stop menubgm"), NULL, 0, NULL);
			smenubgm=false;
			Infinitemode(4,5,3,3,3,2);//无限模式
			for (k=0;k<=20;k++)
				shade(k*5,0,0,background1);//处理图片明暗
			putimage(0,0,&background1);
			FlushBatchDraw();
		}

		else
		if(click(length*374/1280,width*597/720,length*374/1280+n*181*width/720,width*597/720+n*21*width/720,options))
		{
			//积分榜
			putimage(0,0,&optionsmenu);
			FlushBatchDraw();
			bool normalloop=true;
			Sleep(200);
			FlushMouseMsgBuffer();
			msg=GetMouseMsg();
			c=66; int x,y;
			while(normalloop && c!=27)
			{
				while(MouseHit())
					msg=GetMouseMsg();
				if (kbhit())
					c=getch();	
				if(click(length*378/1280,width*452/720,length*378/1280+n*162*width/720,width*452/720+n*31*width/720,guild))
				{
					putimage(0,0,&help);
						FlushBatchDraw();
					c=getch();	
					while (c!=27)
						c=getch();	
					c=66;
				}		
				else
				if(click(length*374/1280,width*549/720,length*374/1280+n*193*width/720,width*549/720+n*25*width/720,crew))
				{
					putimage(0,0,&namelist);
						FlushBatchDraw();
					c=getch();
					while (c!=27)
						c=getch();
					c=66;
				}
				else
				if(click(length*377/1280,width*643/720,length*377/1280+n*100*width/720,width*643/720+n*20*width/720,rank))
				{
					int printmode=1;
					FlushMouseMsgBuffer();
					msg=GetMouseMsg();
					while (c!=27)
					{
						while(MouseHit())
							msg=GetMouseMsg();
						x=msg.x; y=msg.y;
						if (kbhit())
							c=getch();	
						putimage(0,0,&rankimg);
						if(printmode>1 && click(length*300/1280,width*282/720,length*300/1280+73*width/720,width*282/720+120*width/720,rankimgl))
							printmode--;
						else
						if(printmode<5 && click(length*955/1280,width*282/720,length*955/1280+73*width/720,width*282/720+120*width/720,rankimgr) && printmode<5)
							printmode++;
						switch (printmode)
						{
							case 1: printname("CAREER");break;
							case 2: printname("ENDLESS");break;
							case 3: printname("EASY");break;
							case 4: printname("MEDIUM");break;
							case 5: printname("DIFFICULT");break;
						}
						//putimage(length*300/1280,width*282/720,&imgbtn1);
						//putimage(length*300/1280+73*width/720,width*282/720+120*width/720,&imgbtn1);
						//putimage(length*955/1280,width*282/720,&imgbtn1);
						//putimage(length*955/1280+73*width/720,width*282/720+120*width/720,&imgbtn1);
						FlushBatchDraw();
						Sleep(120);
						msg.x=x; msg.y=y;
					}
					c=66;
				}
				else
				{
					FlushBatchDraw();
					putimage(0,0,&optionsmenu);
				}
			}
			if (c!=27)
			{
				for (k=0;k<=100;k++)
					shade(k,0,0,background1);//处理图片明暗
			}
			putimage(0,0,&background1);
			FlushBatchDraw();
		}

		else
		if(click(length*377/1280,width*643/720,length*377/1280+n*86*width/720,width*643/720+n*20*width/720,exitbtn))
		{
			cleardevice();
			flag=false;
			break;
		}
		else
		if(click(length*38/1280,width*412/720,length*38/1280+n*250*width/720,width*412/720+n*250*width/720,background1))
		{
			for (k=100;k>=0;k--)
			{
				shade(k,0,0,background1);//处理图片明暗
				chanvolume("menubgm",0,1001,-20);	
				Sleep(3);
			}
			mciSendString(_T("stop menubgm"), NULL, 0, NULL);
			smenubgm=false;
			Infinitemode(15,15,30,30,30,30);//无限模式
			for (k=0;k<=20;k++)
				shade(k*5,0,0,background1);//处理图片明暗
			putimage(0,0,&background1);
			FlushBatchDraw();
		}		
		else
		{
			FlushBatchDraw();
			putimage(0,0,&background1);
		}
	}
}


void main()
{
	initgraph(length, width/*,NOCLOSE*/); 
	HCURSOR hcur = LoadCursor(NULL, IDC_CROSS);		// 加载系统预置的鼠标样式
	HWND hwnd = GetHWnd();
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);	// 设置窗口类的鼠标样式
	SetWindowText(hwnd, "Ocean Crisis");// 设置窗口标题文字
	srand( (unsigned)time( NULL ) );//设置随机种子
	BeginBatchDraw(); 
	init();//初始界面
	EndBatchDraw(); 
	closegraph();// 关闭图形窗口
}
