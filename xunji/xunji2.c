/*************************************************************************************
程序介绍
名称：基于五路循迹的循迹程序（待定）
功能：实现小车的循迹
核心原理：根据五路循迹检测到的黑白线来反馈高低电压，并及时
					作出反应，来根据具体路线来实现循迹。
*************************************************************************************/


#include"reg51.h"
#include"intrins.h"
void xunji();
typedef unsigned char u8;//之所以用8 是因为他正好有8个字节下面的同理
typedef unsigned int u16;//重新定义 用u8 u16 来代替原来的符号 
u16 timer1,value1,value2,value3,value4,k=0;//定义全局变量
float s,s1,s2,s3;
u8 x3,x4;

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
sbit ss1=P2^5;//寻迹使用
sbit ss2=P2^4;//
sbit ss3=P2^3;//
sbit ss4=P2^2;
sbit ss5=P2^1;
sbit near=P2^0;
//sbit TRIG1=P3^3; //避障使用
//sbit ECHO1=P3^2; //
//sbit TRIG2=P0^6; //
//sbit ECHO2=P0^7; //
//sbit TRIG3=P2^7; //
//sbit ECHO3=P2^6;
//sbit FMQ=P1^6;

void delay(u16 z) //秒延时函数
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}


//定时器初始化函数
void Timer1Init()
{
	TMOD|=0X11;//选择为定时器1模式，工作方式1，仅用TR1打开启动。

	TH1 = 0xFf; 
	TL1 = 0xf6;   //1ms
		
	ET1=1;//打开定时器1中断允许
	EA=1;//打开总中断
	TR1=1;//打开定时器
}
void ds(u8 x1,u8 x2)//T1初始化寻迹使用
{
	TMOD|=0X10;
	TH1=x1;
	TL1=x2;
	x3=x1;
	x4=x2;
  EA=1;ET1=1;
	TR1=1;	
}

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

//void xunji()//寻迹函数
//{
//	if(ss1==0&&ss2==0&&ss3==0&&ss4==0&&ss5==0)//全为黑线
//	{
//		 qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
//		 qy(500,1);
//		 hz(500,1);
//		 hy(500,1);
//	}
//	if((ss1==1&&ss2==1&&ss3==1&&ss4==1&&ss5==1)||(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1))//全为白线或中间为黑线
//	{
//		 qz(500,1);//对四个函数赋值可以实现四个轮子的单独调速和正反转
//		 qy(500,1);
//		 hz(500,1);
//		 hy(500,1);
//	}
//	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==0)	//左边亮
//	{
//		 qz(300,1);//减速右
//		 qy(300,0);
//		 hz(300,1);
//		 hy(300,0);
//	}
//	if(ss1==0&&ss2==0&&ss3==1&&ss4==1&&ss5==1) //右边亮
//	{
//		 qz(300,0);//减速左
//		 qy(300,1);
//		 hz(300,0);
//		 hy(300,1);
//	}
//	if(ss1==0&&ss2==0&&ss3==1&&ss4==1&&ss5==1) //右边亮
//	{
//	
//	}
//}

/*void xunju()
{
	if(ss1==1)
	{
		 if(ss2==1)
						{
								if(ss3==1)
											{
												 if(ss4==1)
																{
																  	if(ss5==1)
																 			     	  {
																								qz(500,1);//直行{11111}
																								qy(500,1);
																								hz(500,1);
																								hy(500,1);
																							}
																							else
																							{
																							  qz(300,1);//原地右转(11110}
																								qy(300,0);
																								hz(300,1);
																								hy(300,0);
																							}
																				}
																	case 0:switch(ss5)
																				{
																					case 0:
																								qz(600,1);//二级右转{11100}
																								qy(600,0);
																								hz(600,1);
																								hy(600,0);
																								xunji();
																								break;
																				};break;
																};break;
												case 0:switch(ss4)
															{
																case 1:switch(ss5)
																			{
																					case 1:
																								qz(500,1);//直行{11011}
																								qy(500,1);
																								hz(500,1);
																								hy(500,1);
																								xunji();
																								break;
																			};break;
																case 0:switch(ss5)
																			{
																					case 1:
																								qz(400,1);//一级右转{11001}
																								qy(400,0);
																								hz(400,1);
																								hy(400,0);
																								xunji();
																								break;
																			};break;
															};break;

											};break;
							case 0:switch(ss3)
										{	
											case 0:switch(ss4)
														{
															case 1:switch(ss5)
																		{
																			case 1:
         																				qz(400,0);//一级左转{10011}
																								qy(400,1);
																								hz(400,0);
																								hy(400,1);

																		}
														}
										}
						}
		case 0:switch{ss2}
					{
						case 1:switch(ss3)
									{
										case 1:switch(ss4)
													{
														case 1:switch(ss5)
														{
															case 1:
																			qz(300,0);//原地左转{01111}
																			qy(300,1);
																			hz(300,0);
																			hy(300,1);

														};
													};
									};
						case 0:switch(ss3)
									{
										case 1:switch(ss4)
													{
														case 1:switch(ss5)
																	{
																		case 1:
																							qz(600,0);//二级左转{00111}
																							qy(600,1);
																							hz(600,0);
																							hy(600,1);

																	};
													};
									};
					};
			default:
				qz(0,0);//不动}
				qy(0,0);
				hz(0,0);
				hy(0,0);
				xunji();
				break;
	}
}*/
/*
	s   0 为黑线 1为白线
*/
void xunji()//寻迹函数
{
		if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//线在中间
	{
	   qz(200,1);//前进
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//线在较左侧
	{
		while(ss3==1&&ss4==1)
		{
		 qz(300,0);//一级左转
		 qy(300,1);
		 hz(300,0);
		 hy(300,1);
		}
		 qz(300,0);//一级左转
		 qy(300,1);
		 hz(300,0);
		 hy(300,1);
		delay(1);
		k++;
	}
	
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //线在较右侧
	{
		while(ss3==1)
		{
		 qz(300,1);//一级右转
		 qy(300,0);
		 hz(300,1);
		 hy(300,0);
		}
		 qz(200,1);//二级右转
		 qy(200,0);
		 hz(200,1);
		 hy(200,0);
		delay(1);
		k++;
	}
	

if(k==0)
{
     qz(200,1);//前进
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
}
	k=0;
}
/*void xunji()//寻迹函数
{
	if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//线在中间
	{
	   qz(200,1);//前进
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//线在较左侧
	{
		
		 qz(300,0);//一级左转
		 qy(300,1);
		 hz(300,1);
		 hy(300,1);
		
		

		k++;
	}
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //线在较右侧
	{
		
		
		 qz(300,1);//一级右转
		 qy(300,0);
		 hz(300,1);
		 hy(300,1);
		
		k++;
	}
		if(k==0)
{
     qz(200,1);//前进
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
}
	k=0;
	
}*/
void main()//主函数
{
	ds();
	Timer1Init();
	while(1)
	{
  xunji();
	delay(0.01);
	}
}

	void Time1(void) interrupt 3    //3 为定时器1的中断号  1 定时器0的中断号 0 外部中断1 2 外部中断2  4 串口中断
{
	TH1 = 0xFf; 
	TL1 = 0xf6;   //1ms
	timer1++;   
}