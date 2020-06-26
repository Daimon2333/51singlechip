/*************************************************************************************
程序介绍
名称：基于三个超声波避障器的避障小车程序（待定）
功能：实现小车的避障
核心原理：三个超声波依次检测距离并分别储存在s1,s2,s3三个值中
          通过对三个值的情况与预设情况进行一一比较，符合则执
					行相应的运动，不符合则停止运动并发出警报，可以以此
			    为依据修改程序。
*************************************************************************************/


#include"reg51.h"
#include"intrins.h"
typedef unsigned char u8;//之所以用8 是因为他正好有8个字节下面的同理
typedef unsigned int u16;//重新定义 用u8 u16 来代替原来的符号 
u16 i=1,h=0;//定义全局变量
float s,s1,s2,s3;
u16 pwm=1100,juli=20;//调节pwm周期和检测距离
sbit AIN1=P1^0;//后面两个轮子
sbit AIN2=P1^1;
sbit AIN3=P1^2;
sbit AIN4=P1^3;
sbit AENA=P1^4;
sbit AENB=P1^5;
sbit BIN1=P0^0;//前面两个轮子
sbit BIN2=P0^1;
sbit BIN3=P0^2;
sbit BIN4=P0^3;
sbit BENA=P0^4;
sbit BENB=P0^5;
sbit TRIG1=P3^3; //避障使用
sbit ECHO1=P3^2; //
sbit TRIG2=P0^6; //
sbit ECHO2=P0^7; //
sbit TRIG3=P2^7; //
sbit ECHO3=P2^6;
sbit FMQ=P1^6;



/*
void delay(u16 z) //秒延时函数
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}
*/


void delayus(u16 x) //us延时函数

{

while(x--);

}


void init()//避障定时器初始化函数
{
	TMOD|=0X11; //T0 为方式1, GATE=1;
	TH0=0;
	TL0=0;	
	EA=1; 
	ET0=1;
	TR0=0;
	TRIG1=0;
	TRIG2=0;
	TRIG3=0;

}


void  StartModule1() 		   //启动避障1
{
	  TRIG1=1; 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //这个函数为标准的延时1us的函数
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //他在头文件为"intrins.h"里面
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG1=0;
}

void  StartModule2() 		   //启动避障2
{
	  TRIG2=1;
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG2=0; 
}

void  StartModule3() 		   //启动避障3
{
	  TRIG3=1;
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG3=0;
}


void Conut(void) //计算距离 单位cm
{	
	u16 time;
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	s=(time*1.7)/100;
}


void ceju()
{	   
      switch(i)
		  {
			  case 1:StartModule1();break; 
			  case 2:StartModule2();break; 
			  case 3:StartModule3();break; 
		  }
			switch(i)                       //当ECHO为零时等待
		  {
			  case 1:while(!ECHO1);break; 
			  case 2:while(!ECHO2);break; 
			  case 3:while(!ECHO3);break; 
		  }
			//while(!ECHO1||!ECHO2||!ECHO3); 
			TR0=1; //开始计数
			switch(i)                      //当ECHO为1时计数并等待
		  {
			  case 1:while(ECHO1);break; 
			  case 2:while(ECHO2);break; 
			  case 3:while(ECHO3);break; 
		  }
			//while(ECHO1||ECHO2||ECHO3); 
			TR0=0; //停止计数
		  Conut(); //测距
		  TH0=0;	 //清零准备下一次计数
		  TL0=0; 
	    switch(i)
		  {
			  case 1:s1=s;i++;break; 
			  case 2:s2=s;i++;break; 
			  case 3:s3=s;i=1;break; 
		  }
} 

void zz()
{
    BIN1=0; //1 反转
		BIN2=1;
	  BIN3=0; //2 正
		BIN4=1;
	  AIN3=0; //3 反转
		AIN4=1;
	  AIN1=0; //4 正转
		AIN2=1;
}

void yz()
{
    BIN1=1; 
		BIN2=0;
	  BIN3=1; 
		BIN4=0;
	  AIN3=1; 
		AIN4=0;
	  AIN1=1; 
		AIN2=0;
}

void zhizou()
{
    BIN1=1; 
		BIN2=0;
	  BIN3=0; 
		BIN4=1;
	  AIN3=1; 
		AIN4=0;
	  AIN1=0; 
		AIN2=1;
}

void houtui()
{
    BIN1=0; 
		BIN2=1;
	  BIN3=1; 
		BIN4=0;
	  AIN3=0; 
		AIN4=1;
	  AIN1=1; 
		AIN2=0;
}
//四个轮子的PWM调速函数依次如下
void yido(u16 x1)//前左轮
{
		BENA=1;
	  BENB=1;
	  AENB=1;
	  AENA=1;
		delayus(x1);//置高电平后延时x1us 占空比为x1%100 即  x1/pwm*%100			
	  BENA=0;
	  BENB=0;
	  AENB=0;	
	  AENA=0;
		//delayus(pwm-x1);
}

void tingzhi()
{
    BIN1=1; 
		BIN2=1;
	  BIN3=1; 
		BIN4=1;
	  AIN3=1; 
		AIN4=1;
	  AIN1=1; 
		AIN2=1;
}


void bizhang()//对s1 s2 s3的值进行对号入座  然后执行相应的运动情况。
{
	/*
	if(s1>juli&&s2>juli&&s3>juli)//s1为正面s2为左面s3为右面
	{
		if(h==0)
		{
	   FMQ=1;    //直走
	   yido(1000);
		 zhizou();
		}
    else
		{
		 FMQ=0;         //向右转90度
	 	 yido(1000);
		 yz();
		 h=0;
		}			
	}
  else if(s1<=juli&&s2>juli&&s3>juli)  
	{
	  FMQ=0;         //向右转
	 	yido(1000);
		yz();
	}
	else if(s2<=juli&&s1>juli&&s3>juli)
	{
	  FMQ=0;       //向右转
	  yido(1000);
		yz();
		h=0;
	}
	else if(s3<=juli&&s1>juli&&s2>juli)
	{
	  FMQ=0;      //向左转
	 	yido(1000);
		zz();
		h=0;
	}
	else if(s1<=juli&&s2<=juli&&s3>juli)
	{
	  FMQ=0;       //向右转
	  yido(1000);
		yz();
	}
	else if(s1<=juli&&s3<=juli&&s2>juli)
	{
	  FMQ=0;      //向左转
	  yido(1000);
		zz();
	}
	else if(s1>juli&&s2<=juli&&s3<=juli)//这个情况有问题
	{
		if(h==0)
		{
	    FMQ=0;      //减速向前
	 	  yido(1000);
			zhizou();
		}
		else
		{
		  FMQ=0;       //加速后退
	 	  yido(1000);
			houtui();
		}
		
	}
	else if(s1<=juli&&s2<=juli&&s3<=juli)
	{ 
		//delay_ms(1);
		h=0;
	  FMQ=0;       //加速后退
	  yido(1000);
		houtui();
		h++;
	}
	else
	{
	  FMQ=0;      //停止
	  yido(0);
		zhizou();
	}
*/
	if(s1<=juli&&s2>juli&&s3>juli)//s1为正面s2为左面s3为右面
	{
		
	   yido(1000);
		 zhizou();
		}
  if(s1>juli&&s2<juli&&s3>juli)
		{
			yido(1000);
			zz();
		}	
	if(s1>juli&&s2>juli&&s3<juli)
	{
			yido(1000);
		yz();
	}
}


void main()
{
	init();//避障用定时器初始化 
	while(1)
	{	 
		 ceju();   		//开始测距
		 ceju();
		 ceju();
	   bizhang();       //进行避障	 
	}
}
