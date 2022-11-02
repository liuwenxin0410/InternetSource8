/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件: MotorDriver.c
	功能：电机驱动
	作者：flotox@yeah.net
	日期：2014.9.5
	更新：2014.9.5
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "MotorDriver.h"
#include "GlobalVars.h"

#define	MOTOR_L_IN1_LOW			(GPIO_ResetBits(GPIOC, GPIO_Pin_6))
#define	MOTOR_L_IN1_HIGH		(GPIO_SetBits(GPIOC, GPIO_Pin_6))
#define	MOTOR_L_IN2_LOW			(GPIO_ResetBits(GPIOA, GPIO_Pin_7))
#define	MOTOR_L_IN2_HIGH		(GPIO_SetBits(GPIOA, GPIO_Pin_7))

#define	MOTOR_R_IN1_LOW			(GPIO_ResetBits(GPIOA, GPIO_Pin_8))
#define	MOTOR_R_IN1_HIGH		(GPIO_SetBits(GPIOA, GPIO_Pin_8))
#define	MOTOR_R_IN2_LOW			(GPIO_ResetBits(GPIOB, GPIO_Pin_13))
#define	MOTOR_R_IN2_HIGH		(GPIO_SetBits(GPIOB, GPIO_Pin_13))

/**
	*	@brief		电机驱动左轮配置
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_L_Config(void){
	
	GPIO_InitTypeDef					GPIO_InitStructure ;
	TIM_TimeBaseInitTypeDef		TIM_BaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	
	/*使能GPIOA、GPIOC、AFIO、TIM8外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM8, ENABLE);
	
	/*初始化PA.07端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*初始化PC.06端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*初始化PC.07端口(TIM8_CH2)为AF_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*定时器基本配置*/
	TIM_BaseInitStructure.TIM_Prescaler = 3-1;													//时钟预分频数3,TIM8的计数时钟频率为24MHz
	TIM_BaseInitStructure.TIM_Period = 1000-1;													//自动重装载寄存器数值,PWM2频率为24MHz/1000=24KHz
	TIM_BaseInitStructure.TIM_ClockDivision = 0;												//采样分频
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;					//向上计数
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;										//重复寄存器
	TIM_TimeBaseInit(TIM8, &TIM_BaseInitStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWM1输出模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;				//使能该通道输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;						//输出极性
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);														//按指定参数初始化

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);										//使能TIM8在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);																	//使能TIM8在ARR上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);																							//打开TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);																		//PWM输出使能
}

/**
	*	@brief		电机驱动右轮配置
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_R_Config(void){

	GPIO_InitTypeDef					GPIO_InitStructure ;
	TIM_TimeBaseInitTypeDef		TIM_BaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;

	/*使能GPIOA、GPIOB、AFIO、TIM1外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);

	/*初始化PA.08端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*初始化PB.13端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*初始化PA.09端口(TIM1_CH2)为AF_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*定时器基本配置*/
	TIM_BaseInitStructure.TIM_Prescaler = 3-1;													//时钟预分频数3,TIM1的计数时钟频率为24MHz
	TIM_BaseInitStructure.TIM_Period = 1000-1;													//自动重装载寄存器数值,PWM2频率为24MHz/1000=24KHz
	TIM_BaseInitStructure.TIM_ClockDivision = 0;												//采样分频
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;					//向上计数
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;										//重复寄存器
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;										//PWM1输出模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;				//使能该通道输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;						//输出极性
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);														//按指定参数初始化

	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);										//使能TIM1在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);																	//使能TIM1在ARR上的预装载寄存器

	TIM_Cmd(TIM1, ENABLE);																							//打开TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);																		//PWM输出使能
}

/**
	*	@brief		左轮电机正转
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_L_Turn_Forward(void){
	
	MOTOR_L_IN1_LOW;
	MOTOR_L_IN2_HIGH;
}

/**
	*	@brief		左轮电机反转
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_L_Turn_Reverse(void){
	
	MOTOR_L_IN1_HIGH;
	MOTOR_L_IN2_LOW;
}

/**
	*	@brief		右轮电机正转
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_R_Turn_Forward(void){
	
	MOTOR_R_IN1_HIGH;
	MOTOR_R_IN2_LOW;
}

/**
	*	@brief		右轮电机反转
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_R_Turn_Reverse(void){
	
	MOTOR_R_IN1_LOW;
	MOTOR_R_IN2_HIGH;
}

/**
	*	@brief		电机驱动外设初始化
	*	@param		none
	*	@retval		none
	*/
void	MotorDriver_Init(void){
	
	MotorDriver_L_Config();
	MotorDriver_R_Config();
	
	MotorDriver_L_Turn_Forward();
	MotorDriver_R_Turn_Forward();
}
