/***************************************************************************
���⣺	�������ʵ��
Ч����	��1����ת����2����ת����3����ת����4����ת��ת�ٲ�ͬ��
�����ڣ�LY5A-L2A������
���ߣ�	�������-��Ƭ��
��վ��	lydz8.taobao.com
˵����	�����������������������ķ�ʽ������A-B-C-D
		ͨ����չ��ULN2003A�����������ULN2003A�Ǹߵ�ƽ����������ڳ�ʼ��ʱ�����������Ľ�����
******************************************************************************/
#include <reg51.h>

/********IO���Ŷ���***********************************************************/
sbit LA=P2^0;		//�������࣬ѡ��P2�ڵĵ���λ���
					//LAΪA��LBΪB��LCΪC��LDΪD
sbit LB=P2^1;
sbit LC=P2^2;
sbit LD=P2^3;

sbit key1 = P3^7;	//���尴��1
sbit key2 = P3^6;	//���尴��2
sbit key3 = P3^5;	//���尴��3
sbit key4 = P3^4;	//���尴��4
sbit key5 = P3^3;	//���尴��5
sbit key6 = P3^2;	//���尴��6

/********��������*************************************************************/
void right(unsigned int Speed,unsigned int road);	//��ת
void left(unsigned int Speed,unsigned int road);	//��ת
void mDelay(unsigned int DelayTime);						//��ʱ����

/********��������*************************************************************/
int MotorStep=0;		//��������

/***********��������ʼ********************************************************/	
void main()
{  
	P2&=0XF0;			//��ULN2003Aֱ����������ʼʱΪ�͵�ƽ������һ����������
	while(1){			//ѭ��
		if(key1==0)		//1��������ת
			right(20,32);		//������������ת�ٺͲ���������20���ٶȽ�32��
		if(key2==0)		//2�����·�ת
			left(20,32);			
		if(key3==0)		//3��������ת
			right(10,32);		//������������ת�ٺͲ���������10���ٶȽ�32��
		if(key4==0)		//4�����·�ת
			left(10,32);
	}
		
}

/***********��ת��������������ת�ٺͲ�����*************************************/	
void right(unsigned int Speed,unsigned int road)
{
	//�������ģ�A-B-C-D
	while(road){
		switch(MotorStep){
		case 0:
			LB=0;
			LC=0;
			LD=0;
			LA=1;		//A	
			MotorStep=1;	
			break;
		case 1:
			LA=0;
			LC=0;
			LD=0;
			LB=1;		//B
			MotorStep=2;
			break;
		case 2:
			LA=0;
			LB=0;
			LD=0;
			LC=1;		//C
			MotorStep=3;
			break;
		case 3:
			LA=0;
			LB=0;
			LC=0;
			LD=1;		//D
			MotorStep=0;
			break;
		}
		mDelay(Speed);	//�������ʱ������ת��
		road--;			//���һ��		
	}
	P2&=0XF0;		//���ĸ��Ÿ�λ0��ֹͣ
}

/***********��ת��������������ת�ٺͲ�����*************************************/
void left(unsigned int Speed,unsigned int road)
{
	//�������ģ�D-C-B-A
	while(road){
		switch(MotorStep){
		case 3:	
			LA=0;
			LC=0;
			LD=0;
			LB=1;		//B
			MotorStep=2;
			break;
		case 2:
			LB=0;
			LC=0;
			LD=0;
			LA=1;		//A	
			MotorStep=1;	
			break;
		case 1:
			LA=0;
			LB=0;
			LC=0;
			LD=1;		//D
			MotorStep=0;
			break;
		case 0:
			LA=0;
			LB=0;
			LD=0;
			LC=1;		//C
			MotorStep=3;
			break;		
		}
		mDelay(Speed);	//�������ʱ������ת��
		road--;			//���һ��		
	}
	P2&=0XF0;		//���ĸ��Ÿ�λ0
}

/***********��ʱ����*******************************************************/
void mDelay(unsigned int DelayTime)
{    
	unsigned char j=0;
     while(DelayTime--){
	 	for(j=0;j<100;j++){}
	 }
}
