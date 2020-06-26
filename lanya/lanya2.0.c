#include"reg51.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
u16 Buffer;
u16 pwm=1000;//调节pwm周期和检测距离
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
void delayus(u16 x) //us延时函数

{

while(x--);

}

void UsartInit()//串口的初始化注意蓝牙模块的波特率也要是4800
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
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
		delayus(pwm-x1);
}

void main()
{
	UsartInit();//初始化
	while(1)
	{
		switch(Buffer)
		{
			case 'a': yido(100);zhizou(); break;//前进
		  case 'b': yido(100);houtui(); break;//后退
			case 'c': yido(200);zz(); break;//左转
			case 'd': yido(200);yz(); break;//右转
			case 'e': yido(100);tingzhi();break;//停止
		}	
	}
}

void UartIsr(void) interrupt 4
{
//EA=0; 
  if(RI==1)//当硬件接收到一个数据时，RI会置位
	{
	  RI=0;
		Buffer=SBUF;//将接收到的字符放入Buffer
	}
//EA=1;
}