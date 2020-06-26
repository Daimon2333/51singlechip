#include"reg51.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;
u16 Buffer;
u16 pwm=1000;//����pwm���ںͼ�����
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
void delayus(u16 x) //us��ʱ����

{

while(x--);

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

void main()
{
	UsartInit();//��ʼ��
	while(1)
	{
		switch(Buffer)
		{
			case 'a': yido(100);zhizou(); break;//ǰ��
		  case 'b': yido(100);houtui(); break;//����
			case 'c': yido(200);zz(); break;//��ת
			case 'd': yido(200);yz(); break;//��ת
			case 'e': yido(100);tingzhi();break;//ֹͣ
		}	
	}
}

void UartIsr(void) interrupt 4
{
//EA=0; 
  if(RI==1)//��Ӳ�����յ�һ������ʱ��RI����λ
	{
	  RI=0;
		Buffer=SBUF;//�����յ����ַ�����Buffer
	}
//EA=1;
}