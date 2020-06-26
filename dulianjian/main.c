/*
这是组合包 test




*/




#include"reg51.h"
#include"intrins.h"
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;


//循迹
u16 timer1,value1,value2,value3,value4,k=0;//定义全局变量
float s,s1,s2,s3;
u8 x3,x4;

//避障
u16 i=1,h=0,t=0,Q=0;//定义全局变量
float s,s1,s2,s3;
u16 pwm=1100,juli=25;//调节pwm周期和检测距离

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
sbit ss1=P2^5;//寻迹使用
sbit ss2=P2^4;//
sbit ss3=P2^3;//
sbit ss4=P2^2;
sbit ss5=P2^1;
sbit near=P2^0;
sbit k1=P3^4;//独立开关


void delayus(u16 x) //us延时函数

{

while(x--);

}

void delay(u16 z) //秒延时函数
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}

void init()//避障定时器初始化函数
{
	TMOD|=0X01; //T0 为方式1, GATE=1;
	TH0=0;
	TL0=0;	
	EA=1; 
	ET0=1;
	TR0=0;
	TRIG1=0;
	TRIG2=0;
	TRIG3=0;

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

//避障程序
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





//delay调速程序


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




//避障判断程序
void bizhang()//对s1 s2 s3的值进行对号入座  然后执行相应的运动情况。
{
	if(s1>juli&&s2>juli&&s3>juli)//s1为正面s2为左面s3为右面
	{
		if(h==0)
		{
	       //直走
	   yido(800);
		 zhizou();
		}
    else
		{
		        //向右转90度
	 	 yido(1000);
		 yz();
		 h=0;
		}			
	}
  else if(s1<=juli&&s2>juli&&s3>juli)  
	{
	         //向右转
	 	yido(1000);
		yz();
	}
	else if(s2<=juli&&s1>juli&&s3>juli)
	{
	      //向右转
	  yido(1000);
		yz();
		h=0;
	}
	else if(s3<=juli&&s1>juli&&s2>juli)
	{
	      //向左转
	 	yido(1000);
		zz();
		h=0;
	}
	else if(s1<=juli&&s2<=juli&&s3>juli)
	{
	        //向右转
	  yido(1000);
		yz();
	}
	else if(s1<=juli&&s3<=juli&&s2>juli)
	{
	     //向左转
	  yido(1000);
		zz();
	}
	else if(s1>juli&&s2<=juli&&s3<=juli)//这个情况有问题
	{
		if(h==0)
		{
	      //减速向前
	 	  yido(1000);
			zhizou();
		}
		else
		{
	    //加速后退
	 	  yido(1000);
			houtui();
		}
		
	}
	else if(s1<=juli&&s2<=juli&&s3<=juli)
	{ 
		//delay_ms(1);
		h=0;
    //加速后退
	  yido(1000);
		houtui();
		h++;
	}
	else
	{
   //停止
	  yido(0);
		zhizou();
	}



}


	
/*
	循迹逻辑程序
*/
void xunji()//寻迹函数
{
		if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//线在中间
	{
	   yido(250);
		 zhizou();
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//线在较左侧
	{
		while(ss3==1&&ss4==1)
		{
		 yido(200);
		 zz();
			
		k++;
		}
		yz();
			yido(20);
	}
	
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //线在较右侧
	{
		while(ss3==1)
		{
		 yido(200);
		yz();
			
		k++;
		}
		zz();
			yido(20);
	}
	

if(k==0)
{
     yido(250);
		 zhizou();
}
	k=0;
}

void bizhang3()
{
				ceju();   		//开始测距
				ceju();
				ceju();
				bizhang();
}


	
//主函数
void main()
{
	init();
	while(1)
	{  
		if(k1==0)		  //检测按键K1是否按下
		{	
			delayus(1000);   //消除抖动 一般大约10ms
			Q++;
//			if(k1==0)	 //再次判断按键是否按下
//			{			
//			}
//			while(!k1);	 //检测按键是否松开
		}	
    if(Q==0) 
	  {	
			xunji();			
		}
		else
		{
			bizhang3(); 
		}
		Q=0;
	}
}


