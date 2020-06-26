/*************************************************************************************
程序介绍
名称：基于五路循迹的循迹程序（待定）
功能：实现小车的循迹
核心原理：根据五路循迹检测到的黑白线来反馈高低电压，并及时
					作出反应，来根据具体路线来实现循迹。
*************************************************************************************/


#include"reg51.h"
#include"intrins.h"
typedef unsigned char u8;//之所以用8 是因为他正好有8个字节下面的同理
typedef unsigned int u16;//重新定义 用u8 u16 来代替原来的符号 
u16 timer1,value1,value2,value3,value4,i=1,h=0;//定义全局变量
float s,s1,s2,s3;


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

/*void delay_ms(u16 n)//微妙延时函数
{
  u16 y;   //可以实现输入相应数字得到相应的延时
	for(;n>0;n--)     //单位ms
	{
	  for(y=110;y>0;y--);
	}
}
void delay(u16 z) //秒延时函数
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}
*/


//定时器初始化函数
void Timer1Init()
{
	TMOD|=0X11;//选择为定时器1模式，工作方式1，仅用TR1打开启动。

	TH1 = 0xFF; 
	TL1 = 0xce;   //1ms
		
	ET1=1;//打开定时器1中断允许
	EA=1;//打开总中断
	TR1=1;//打开定时器			
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


//四个轮子的PWM调速函数依次如下
void qz(u16 x1,u16 zx1)//前左轮
{
	   value1=x1;
     if(timer1>1800)  //PWM周期为1000*1us
		{
			timer1=0;
		}
		if(timer1 <value1)	//value为调节占空比用的C值
		{
		  BENA=1;
		}
		else
		{
		  BENA=0;
		}		
		if(zx1==1)//为一则正转
		{
		 	BIN1=1; //左前 正转
		  BIN2=0;
		}
		else
		{
		  BIN1=0; //左前 反转
		  BIN2=1;
		}
}

void qy(u16 x2,u16 zx2)//前右轮
{
	   value2=x2;
     if(timer1>1800)  //PWM周期为1000*1us
		{
			timer1=0;
		}
		if(timer1 <value2)	//value为调节占空比用的C值
		{
		  BENB=1;
		}
		else
		{
		  BENB=0;
		}	
    if(zx2==1)//为一则正转
		{
		 	BIN3=0; // 正转
		  BIN4=1;
		}
		else
		{
		  BIN3=1; // 反转
		  BIN4=0;
		}		
}

void hz(u16 x3,u16 zx3)//后左
{
	   value3=x3;
     if(timer1>1800)  //PWM周期为1000*1us
		{
			timer1=0;
		}
		if(timer1 <value3)	//value为调节占空比用的C值
		{
		  AENB=1;
		}
		else
		{
		  AENB=0;
		}	
    if(zx3==1)//为一则正转
		{
		 	AIN3=1; // 正转
		  AIN4=0;
		}
		else
		{
		  AIN3=0; // 反转
		  AIN4=1;
		}		
}

void hy(u16 x4,u16 zx4)//后右
{
	   value4=x4;
     if(timer1>1800)  //PWM周期为1000*1us
		{
			timer1=0;
		}
		if(timer1 <value4)	//value为调节占空比用的C值
		{
		  AENA=1;
		}
		else
		{
		  AENA=0;
		}
    if(zx4==1)//为一则正转
		{
		 	AIN1=0; // 正转
		  AIN2=1;
		}
		else
		{
		  AIN1=1; // 反转
		  AIN2=0;
		}		
}

void bizhang()//对s1 s2 s3的值进行对号入座  然后执行相应的运动情况。
{
	if(s1>50&&s2>50&&s3>50)//s1为正面s2为左面s3为右面
	{
		if(h==0)
		{
	   FMQ=1;    //直走
	   qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		 qy(500,1);
		 hz(500,1);
		 hy(500,1);
		}
    else
		{
		 FMQ=0;         //向右转90度
	 	 qz(700,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		 qy(700,0);
		 hz(700,1);
		 hy(700,0);
		 h=0;
		}			
	}
  else if(s1<=50&&s2>50&&s3>50)  
	{
	  FMQ=0;         //向右转
	 	qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(500,0);
		hz(500,1);
		hy(500,0);
	}
	else if(s2<=50&&s1>50&&s3>50)
	{
	  FMQ=0;       //向右转
	 	qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(500,0);
		hz(500,1);
		hy(500,0);
		h=0;
	}
	else if(s3<=50&&s1>50&&s2>50)
	{
	  FMQ=0;      //向左转
	 	qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(500,0);
		hz(500,1);
		hy(500,0);
		h=0;
	}
	else if(s1<=50&&s2<=50&&s3>50)
	{
	  FMQ=0;       //向右转
	 	qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(500,0);
		hz(500,1);
		hy(500,0);
	}
	else if(s1<=50&&s3<=50&&s2>50)
	{
	  FMQ=0;      //向左转
	 	qz(500,0);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(500,1);
		hz(500,0);
		hy(500,1);
	}
	/*else if(s1>50&&s2<=50&&s3<=50)//这个情况有问题
	{
		if(h==0)
		{
	    FMQ=0;      //减速向前
	 	  qz(300,0);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		  qy(300,0);
		  hz(300,0);
		  hy(300,0);
		}
		else
		{
		  FMQ=0;       //加速后退
	 	  qz(600,0);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		  qy(600,0);
		  hz(600,0);
		  hy(600,0);
		}
		
	}*/
	else if(s1<=50&&s2<=50&&s3<=50)
	{ 
		h=0;
	  FMQ=0;       //加速后退
	 	qz(600,0);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(600,0);
		hz(600,0);
		hy(600,0);
		h++;
	}
	else
	{
	  FMQ=0;      //停止
	 	qz(0,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
		qy(0,1);
		hz(0,1);
		hy(0,1);
		
	}

}


void main()
{
	init();//避障用定时器初始化
	Timer1Init();//调速用定时器初始化
  
	while(1)
	{

		 ceju();   		//开始测距
		 ceju();
		 ceju();
	   bizhang();       //进行避障
 
	}
}


//中断函数
void Time1(void) interrupt 3    //3 为定时器1的中断号  1 定时器0的中断号 0 外部中断1 2 外部中断2  4 串口中断
{
	TH1 = 0xFF; 
	TL1 = 0xce;   //1ms
	timer1++;   
}