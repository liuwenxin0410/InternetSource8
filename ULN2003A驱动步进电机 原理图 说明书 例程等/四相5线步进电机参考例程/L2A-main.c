/***************************************************************************
标题：	步进电机实验
效果：	按1键正转，按2键反转，按3键正转，按4键反转（转速不同）
工作于：LY5A-L2A开发板
作者：	林洋电子-单片机
网站：	lydz8.taobao.com
说明：	本例程这里介绍输出两相四拍方式驱动：A-B-C-D
		通过扩展板ULN2003A驱动电机，因ULN2003A是高电平驱动，因此在初始化时，将驱动的四脚拉低
******************************************************************************/
#include <reg51.h>

/********IO引脚定义***********************************************************/
sbit LA=P2^0;		//定义两相，选择P2口的低四位输出
					//LA为A，LB为B，LC为C，LD为D
sbit LB=P2^1;
sbit LC=P2^2;
sbit LD=P2^3;

sbit key1 = P3^7;	//定义按键1
sbit key2 = P3^6;	//定义按键2
sbit key3 = P3^5;	//定义按键3
sbit key4 = P3^4;	//定义按键4
sbit key5 = P3^3;	//定义按键5
sbit key6 = P3^2;	//定义按键6

/********函数声明*************************************************************/
void right(unsigned int Speed,unsigned int road);	//正转
void left(unsigned int Speed,unsigned int road);	//反转
void mDelay(unsigned int DelayTime);						//延时函数

/********变量定义*************************************************************/
int MotorStep=0;		//步进索引

/***********主函数开始********************************************************/	
void main()
{  
	P2&=0XF0;			//因ULN2003A直接驱动，初始时为低电平，这里一定不能少了
	while(1){			//循环
		if(key1==0)		//1键按下正转
			right(20,32);		//两个参数代表转速和步进量，以20的速度进32步
		if(key2==0)		//2键按下反转
			left(20,32);			
		if(key3==0)		//3键接下正转
			right(10,32);		//两个参数代表转速和步进量，以10的速度进32步
		if(key4==0)		//4键接下反转
			left(10,32);
	}
		
}

/***********正转，两个参数代表转速和步进量*************************************/	
void right(unsigned int Speed,unsigned int road)
{
	//步进节拍：A-B-C-D
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
		mDelay(Speed);	//这里的延时即控制转速
		road--;			//完成一步		
	}
	P2&=0XF0;		//将四个脚复位0，停止
}

/***********反转，两个参数代表转速和步进量*************************************/
void left(unsigned int Speed,unsigned int road)
{
	//步进节拍：D-C-B-A
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
		mDelay(Speed);	//这里的延时即控制转速
		road--;			//完成一步		
	}
	P2&=0XF0;		//将四个脚复位0
}

/***********延时函数*******************************************************/
void mDelay(unsigned int DelayTime)
{    
	unsigned char j=0;
     while(DelayTime--){
	 	for(j=0;j<100;j++){}
	 }
}
