/*************************************************************************************
�������
���ƣ�������·ѭ����ѭ�����򣨴�����
���ܣ�ʵ��С����ѭ��
����ԭ��������·ѭ����⵽�ĺڰ����������ߵ͵�ѹ������ʱ
					������Ӧ�������ݾ���·����ʵ��ѭ����
*************************************************************************************/


#include"reg51.h"
#include"intrins.h"
typedef unsigned char u8;//֮������8 ����Ϊ��������8���ֽ������ͬ��
typedef unsigned int u16;//���¶��� ��u8 u16 ������ԭ���ķ��� 
u16 timer1,value1,value2,value3,value4,i=1,h=0;//����ȫ�ֱ���
float s,s1,s2,s3;


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
sbit TRIG1=P3^3; //����ʹ��
sbit ECHO1=P3^2; //
sbit TRIG2=P0^6; //
sbit ECHO2=P0^7; //
sbit TRIG3=P2^7; //
sbit ECHO3=P2^6;
sbit FMQ=P1^6;

/*void delay_ms(u16 n)//΢����ʱ����
{
  u16 y;   //����ʵ��������Ӧ���ֵõ���Ӧ����ʱ
	for(;n>0;n--)     //��λms
	{
	  for(y=110;y>0;y--);
	}
}
void delay(u16 z) //����ʱ����
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}
*/


//��ʱ����ʼ������
void Timer1Init()
{
	TMOD|=0X11;//ѡ��Ϊ��ʱ��1ģʽ��������ʽ1������TR1��������

	TH1 = 0xFF; 
	TL1 = 0xce;   //1ms
		
	ET1=1;//�򿪶�ʱ��1�ж�����
	EA=1;//�����ж�
	TR1=1;//�򿪶�ʱ��			
}


void init()//���϶�ʱ����ʼ������
{
	TMOD|=0X11; //T0 Ϊ��ʽ1, GATE=1;
	TH0=0;
	TL0=0;	
	EA=1; 
	ET0=1;
	TR0=0;
	TRIG1=0;
	TRIG2=0;
	TRIG3=0;

}


void  StartModule1() 		   //��������1
{
	  TRIG1=1; 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //�������Ϊ��׼����ʱ1us�ĺ���
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //����ͷ�ļ�Ϊ"intrins.h"����
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG1=0;
}

void  StartModule2() 		   //��������2
{
	  TRIG2=1;
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG2=0; 
}

void  StartModule3() 		   //��������3
{
	  TRIG3=1;
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_();
	  TRIG3=0;
}


void Conut(void) //������� ��λcm
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
			switch(i)                       //��ECHOΪ��ʱ�ȴ�
		  {
			  case 1:while(!ECHO1);break; 
			  case 2:while(!ECHO2);break; 
			  case 3:while(!ECHO3);break; 
		  }
			//while(!ECHO1||!ECHO2||!ECHO3); 
			TR0=1; //��ʼ����
			switch(i)                      //��ECHOΪ1ʱ�������ȴ�
		  {
			  case 1:while(ECHO1);break; 
			  case 2:while(ECHO2);break; 
			  case 3:while(ECHO3);break; 
		  }
			//while(ECHO1||ECHO2||ECHO3); 
			TR0=0; //ֹͣ����
		  Conut(); //���
		  TH0=0;	 //����׼����һ�μ���
		  TL0=0; 
	    switch(i)
		  {
			  case 1:s1=s;i++;break; 
			  case 2:s2=s;i++;break; 
			  case 3:s3=s;i=1;break; 
		  }
} 


//�ĸ����ӵ�PWM���ٺ�����������
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

void bizhang()//��s1 s2 s3��ֵ���жԺ�����  Ȼ��ִ����Ӧ���˶������
{
	if(s1>50&&s2>50&&s3>50)//s1Ϊ����s2Ϊ����s3Ϊ����
	{
		if(h==0)
		{
	   FMQ=1;    //ֱ��
	   qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		 qy(500,1);
		 hz(500,1);
		 hy(500,1);
		}
    else
		{
		 FMQ=0;         //����ת90��
	 	 qz(700,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		 qy(700,0);
		 hz(700,1);
		 hy(700,0);
		 h=0;
		}			
	}
  else if(s1<=50&&s2>50&&s3>50)  
	{
	  FMQ=0;         //����ת
	 	qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(500,0);
		hz(500,1);
		hy(500,0);
	}
	else if(s2<=50&&s1>50&&s3>50)
	{
	  FMQ=0;       //����ת
	 	qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(500,0);
		hz(500,1);
		hy(500,0);
		h=0;
	}
	else if(s3<=50&&s1>50&&s2>50)
	{
	  FMQ=0;      //����ת
	 	qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(500,0);
		hz(500,1);
		hy(500,0);
		h=0;
	}
	else if(s1<=50&&s2<=50&&s3>50)
	{
	  FMQ=0;       //����ת
	 	qz(500,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(500,0);
		hz(500,1);
		hy(500,0);
	}
	else if(s1<=50&&s3<=50&&s2>50)
	{
	  FMQ=0;      //����ת
	 	qz(500,0);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(500,1);
		hz(500,0);
		hy(500,1);
	}
	/*else if(s1>50&&s2<=50&&s3<=50)//������������
	{
		if(h==0)
		{
	    FMQ=0;      //������ǰ
	 	  qz(300,0);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		  qy(300,0);
		  hz(300,0);
		  hy(300,0);
		}
		else
		{
		  FMQ=0;       //���ٺ���
	 	  qz(600,0);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		  qy(600,0);
		  hz(600,0);
		  hy(600,0);
		}
		
	}*/
	else if(s1<=50&&s2<=50&&s3<=50)
	{ 
		h=0;
	  FMQ=0;       //���ٺ���
	 	qz(600,0);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(600,0);
		hz(600,0);
		hy(600,0);
		h++;
	}
	else
	{
	  FMQ=0;      //ֹͣ
	 	qz(0,1);//���ĸ�������ֵ����ʵ���ĸ����ӵĵ������ٺ�����ת
		qy(0,1);
		hz(0,1);
		hy(0,1);
		
	}

}


void main()
{
	init();//�����ö�ʱ����ʼ��
	Timer1Init();//�����ö�ʱ����ʼ��
  
	while(1)
	{

		 ceju();   		//��ʼ���
		 ceju();
		 ceju();
	   bizhang();       //���б���
 
	}
}


//�жϺ���
void Time1(void) interrupt 3    //3 Ϊ��ʱ��1���жϺ�  1 ��ʱ��0���жϺ� 0 �ⲿ�ж�1 2 �ⲿ�ж�2  4 �����ж�
{
	TH1 = 0xFF; 
	TL1 = 0xce;   //1ms
	timer1++;   
}