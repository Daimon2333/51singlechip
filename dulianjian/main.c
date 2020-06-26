/*
������ϰ� test




*/




#include"reg51.h"
#include"intrins.h"
typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;


//ѭ��
u16 timer1,value1,value2,value3,value4,k=0;//����ȫ�ֱ���
float s,s1,s2,s3;
u8 x3,x4;

//����
u16 i=1,h=0,t=0,Q=0;//����ȫ�ֱ���
float s,s1,s2,s3;
u16 pwm=1100,juli=25;//����pwm���ںͼ�����

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
sbit ss1=P2^5;//Ѱ��ʹ��
sbit ss2=P2^4;//
sbit ss3=P2^3;//
sbit ss4=P2^2;
sbit ss5=P2^1;
sbit near=P2^0;
sbit k1=P3^4;//��������


void delayus(u16 x) //us��ʱ����

{

while(x--);

}

void delay(u16 z) //����ʱ����
{
  u16 j,k;
  for(j=z;j>0;j--)
	   for(k=110;k>0;k--);
}

void init()//���϶�ʱ����ʼ������
{
	TMOD|=0X01; //T0 Ϊ��ʽ1, GATE=1;
	TH0=0;
	TL0=0;	
	EA=1; 
	ET0=1;
	TR0=0;
	TRIG1=0;
	TRIG2=0;
	TRIG3=0;

}

void UsartInit()//���ڵĳ�ʼ��ע������ģ��Ĳ�����ҲҪ��4800
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}

//���ϳ���
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





//delay���ٳ���


void zz()
{
    BIN1=0; //1 ��ת
		BIN2=1;
	  BIN3=0; //2 ��
		BIN4=1;
	  AIN3=0; //3 ��ת
		AIN4=1;
	  AIN1=0; //4 ��ת
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

//�ĸ����ӵ�PWM���ٺ�����������
void yido(u16 x1)//ǰ����
{
		BENA=1;
	  BENB=1;
	  AENB=1;
	  AENA=1;
		delayus(x1);//�øߵ�ƽ����ʱx1us ռ�ձ�Ϊx1%100 ��  x1/pwm*%100			
	  BENA=0;
	  BENB=0;
	  AENB=0;	
	  AENA=0;
		delayus(pwm-x1);
}




//�����жϳ���
void bizhang()//��s1 s2 s3��ֵ���жԺ�����  Ȼ��ִ����Ӧ���˶������
{
	if(s1>juli&&s2>juli&&s3>juli)//s1Ϊ����s2Ϊ����s3Ϊ����
	{
		if(h==0)
		{
	       //ֱ��
	   yido(800);
		 zhizou();
		}
    else
		{
		        //����ת90��
	 	 yido(1000);
		 yz();
		 h=0;
		}			
	}
  else if(s1<=juli&&s2>juli&&s3>juli)  
	{
	         //����ת
	 	yido(1000);
		yz();
	}
	else if(s2<=juli&&s1>juli&&s3>juli)
	{
	      //����ת
	  yido(1000);
		yz();
		h=0;
	}
	else if(s3<=juli&&s1>juli&&s2>juli)
	{
	      //����ת
	 	yido(1000);
		zz();
		h=0;
	}
	else if(s1<=juli&&s2<=juli&&s3>juli)
	{
	        //����ת
	  yido(1000);
		yz();
	}
	else if(s1<=juli&&s3<=juli&&s2>juli)
	{
	     //����ת
	  yido(1000);
		zz();
	}
	else if(s1>juli&&s2<=juli&&s3<=juli)//������������
	{
		if(h==0)
		{
	      //������ǰ
	 	  yido(1000);
			zhizou();
		}
		else
		{
	    //���ٺ���
	 	  yido(1000);
			houtui();
		}
		
	}
	else if(s1<=juli&&s2<=juli&&s3<=juli)
	{ 
		//delay_ms(1);
		h=0;
    //���ٺ���
	  yido(1000);
		houtui();
		h++;
	}
	else
	{
   //ֹͣ
	  yido(0);
		zhizou();
	}



}


	
/*
	ѭ���߼�����
*/
void xunji()//Ѱ������
{
		if(ss1==1&&ss2==1&&ss3==0&&ss4==1&&ss5==1)	//�����м�
	{
	   yido(250);
		 zhizou();
		
		k++;
	}
	if(ss1==1&&ss2==0&&ss3==1&&ss4==1&&ss5==1)	//���ڽ����
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
	
	if(ss1==1&&ss2==1&&ss3==1&&ss4==0&&ss5==1) //���ڽ��Ҳ�
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
				ceju();   		//��ʼ���
				ceju();
				ceju();
				bizhang();
}


	
//������
void main()
{
	init();
	while(1)
	{  
		if(k1==0)		  //��ⰴ��K1�Ƿ���
		{	
			delayus(1000);   //�������� һ���Լ10ms
			Q++;
//			if(k1==0)	 //�ٴ��жϰ����Ƿ���
//			{			
//			}
//			while(!k1);	 //��ⰴ���Ƿ��ɿ�
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


