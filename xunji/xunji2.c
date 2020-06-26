/*************************************************************************************
�������
���ƣ�������·ѭ����ѭ�����򣨴�����
���ܣ�ʵ��С����ѭ��
����ԭ��������·ѭ����⵽�ĺڰ����������ߵ͵�ѹ������ʱ
					������Ӧ�������ݾ���·����ʵ��ѭ����
*************************************************************************************/


#include"reg51.h"
#include"intrins.h"
void xunji();
typedef unsigned char u8;//֮������8 ����Ϊ��������8���ֽ������ͬ��
typedef unsigned int u16;//���¶��� ��u8 u16 ������ԭ���ķ��� 
u16 timer1,value1,value2,value3,value4,k=0;//����ȫ�ֱ���
float s,s1,s2,s3;
u8 x3,x4;

sbit AIN1=P1^0;//������������
sbit AIN2=P1^1;
sbit AIN3=P1^2;
sbit AIN4=P1^3;
sbit AENA=P1^4;
sbit AENB=P1^5;
sbit BIN1=P0^0;//ǰ����������
sbit BIN2=P0^1;
sbit BIN3=P0^2;
sbit BIN4=P0^3;
sbit BENA=P0^4;
sbit BENB=P0^5;
sbit ss1=P2^5;//Ѱ��ʹ��
sbit ss2=P2^4;//
sbit ss3=P2^3;//
sbit ss4=P2^2;
sbit ss5=P2^1;
sbit near=P2^0;
//sbit TRIG1=P3^3; //����ʹ��
//sbit ECHO1=P3^2; //
//sbit TRIG2=P0^6; //
//sbit ECHO2=P0^7; //
//sbit TRIG3=P2^7; //
//sbit ECHO3=P2^6;
//sbit FMQ=P1^6;

void delay(u16 z) //����ʱ����
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}


//��ʱ����ʼ������
void Timer1Init()
{
	TMOD|=0X11;//ѡ��Ϊ��ʱ��1ģʽ��������ʽ1������TR1��������

	TH1 = 0xFf; 
	TL1 = 0xf6;   //1ms
		
	ET1=1;//�򿪶�ʱ��1�ж�����
	EA=1;//�����ж�
	TR1=1;//�򿪶�ʱ��
}
void ds(u8 x1,u8 x2)//T1��ʼ��Ѱ��ʹ��
{
	TMOD|=0X10;
	TH1=x1;
	TL1=x2;
	x3=x1;
	x4=x2;
  EA=1;ET1=1;
	TR1=1;	
}

void qz(u16 x1,u16 zx1)//ǰ����
{
	   value1=x1;
     if(timer1>1800)  //PWM����Ϊ1000*1us
		{
			timer1=0;
		}
		if(timer1 <value1)	//valueΪ����ռ�ձ��õ�Cֵ
		{
		  BENA=1;
		}
		else
		{
		  BENA=0;
		}		
		if(zx1==1)//Ϊһ����ת
		{
		 	BIN1=1; //��ǰ ��ת
		  BIN2=0;
		}
		else
		{
		  BIN1=0; //��ǰ ��ת
		  BIN2=1;
		}
}

void qy(u16 x2,u16 zx2)//ǰ����
{
	   value2=x2;
     if(timer1>1800)  //PWM����Ϊ1000*1us
		{
			timer1=0;
		}
		if(timer1 <value2)	//valueΪ����ռ�ձ��õ�Cֵ
		{
		  BENB=1;
		}
		else
		{
		  BENB=0;
		}	
    if(zx2==1)//Ϊһ����ת
		{
		 	BIN3=0; // ��ת
		  BIN4=1;
		}
		else
		{
		  BIN3=1; // ��ת
		  BIN4=0;
		}		
}

void hz(u16 x3,u16 zx3)//����
{
	   value3=x3;
     if(timer1>1800)  //PWM����Ϊ1000*1us
		{
			timer1=0;
		}
		if(timer1 <value3)	//valueΪ����ռ�ձ��õ�Cֵ
		{
		  AENB=1;
		}
		else
		{
		  AENB=0;
		}	
    if(zx3==1)//Ϊһ����ת
		{
		 	AIN3=1; // ��ת
		  AIN4=0;
		}
		else
		{
		  AIN3=0; // ��ת
		  AIN4=1;
		}		
}

void hy(u16 x4,u16 zx4)//����
{
	   value4=x4;
     if(timer1>1800)  //PWM����Ϊ1000*1us
		{
			timer1=0;
		}
		if(timer1 <value4)	//valueΪ����ռ�ձ��õ�Cֵ
		{
		  AENA=1;
		}
		else
		{
		  AENA=0;
		}
    if(zx4==1)//Ϊһ����ת
		{
		 	AIN1=0; // ��ת
		  AIN2=1;
		}
		else
		{
		  AIN1=1; // ��ת
		  AIN2=0;
		}		
}

//void xunji()//Ѱ������
//{
//	if(ss1==0&&ss2==0&&ss3==0&&ss4==0&&ss5==0)//ȫΪ����
//	{
//		 qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
//		 qy(500,1);
//		 hz(500,1);
//		 hy(500,1);
//	}
//	if((ss1==1&&ss2==1&&ss3==1&&ss4==1&&ss5==1)||(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1))//ȫΪ���߻��м�Ϊ����
//	{
//		 qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
//		 qy(500,1);
//		 hz(500,1);
//		 hy(500,1);
//	}
//	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==0)	//�����
//	{
//		 qz(300,1);//������
//		 qy(300,0);
//		 hz(300,1);
//		 hy(300,0);
//	}
//	if(ss1==0&&ss2==0&&ss3==1&&ss4==1&&ss5==1) //�ұ���
//	{
//		 qz(300,0);//������
//		 qy(300,1);
//		 hz(300,0);
//		 hy(300,1);
//	}
//	if(ss1==0&&ss2==0&&ss3==1&&ss4==1&&ss5==1) //�ұ���
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
																								qz(500,1);//ֱ��{11111}
																								qy(500,1);
																								hz(500,1);
																								hy(500,1);
																							}
																							else
																							{
																							  qz(300,1);//ԭ����ת(11110}
																								qy(300,0);
																								hz(300,1);
																								hy(300,0);
																							}
																				}
																	case 0:switch(ss5)
																				{
																					case 0:
																								qz(600,1);//������ת{11100}
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
																								qz(500,1);//ֱ��{11011}
																								qy(500,1);
																								hz(500,1);
																								hy(500,1);
																								xunji();
																								break;
																			};break;
																case 0:switch(ss5)
																			{
																					case 1:
																								qz(400,1);//һ����ת{11001}
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
         																				qz(400,0);//һ����ת{10011}
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
																			qz(300,0);//ԭ����ת{01111}
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
																							qz(600,0);//������ת{00111}
																							qy(600,1);
																							hz(600,0);
																							hy(600,1);

																	};
													};
									};
					};
			default:
				qz(0,0);//����}
				qy(0,0);
				hz(0,0);
				hy(0,0);
				xunji();
				break;
	}
}*/
/*
	s   0 Ϊ���� 1Ϊ����
*/
void xunji()//Ѱ������
{
		if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//�����м�
	{
	   qz(200,1);//ǰ��
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//���ڽ����
	{
		while(ss3==1&&ss4==1)
		{
		 qz(300,0);//һ����ת
		 qy(300,1);
		 hz(300,0);
		 hy(300,1);
		}
		 qz(300,0);//һ����ת
		 qy(300,1);
		 hz(300,0);
		 hy(300,1);
		delay(1);
		k++;
	}
	
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //���ڽ��Ҳ�
	{
		while(ss3==1)
		{
		 qz(300,1);//һ����ת
		 qy(300,0);
		 hz(300,1);
		 hy(300,0);
		}
		 qz(200,1);//������ת
		 qy(200,0);
		 hz(200,1);
		 hy(200,0);
		delay(1);
		k++;
	}
	

if(k==0)
{
     qz(200,1);//ǰ��
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
}
	k=0;
}
/*void xunji()//Ѱ������
{
	if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//�����м�
	{
	   qz(200,1);//ǰ��
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//���ڽ����
	{
		
		 qz(300,0);//һ����ת
		 qy(300,1);
		 hz(300,1);
		 hy(300,1);
		
		

		k++;
	}
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //���ڽ��Ҳ�
	{
		
		
		 qz(300,1);//һ����ת
		 qy(300,0);
		 hz(300,1);
		 hy(300,1);
		
		k++;
	}
		if(k==0)
{
     qz(200,1);//ǰ��
		 qy(200,1);
		 hz(200,1);
		 hy(200,1);
}
	k=0;
	
}*/
void main()//������
{
	ds();
	Timer1Init();
	while(1)
	{
  xunji();
	delay(0.01);
	}
}

	void Time1(void) interrupt 3    //3 Ϊ��ʱ��1���жϺ�  1 ��ʱ��0���жϺ� 0 �ⲿ�ж�1 2 �ⲿ�ж�2  4 �����ж�
{
	TH1 = 0xFf; 
	TL1 = 0xf6;   //1ms
	timer1++;   
}