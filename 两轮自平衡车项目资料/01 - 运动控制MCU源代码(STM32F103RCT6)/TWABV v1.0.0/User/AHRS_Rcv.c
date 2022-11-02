/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件：AHRS_Rcv.c
	功能：AHRS姿态数据接收
	作者：flotox@yeah.net
	日期：2014.9.7
	更新：2014.9.7
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "AHRS_Rcv.h"

/**
	*	@brief		AHRS姿态数据接收外设初始化
	*	@param		none
	*	@retval		none
	*/
void	AHRS_Rcv_Init(void){
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*开启GPIOC外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*开启USART2外设时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/*USART2的GPIO配置*/
	/*配置USART2 Tx(PA.02)为AF_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*配置USART2 Rx(PA.03)为IN_FLOATING*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*USART2 NVIC配置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);													//2 bits for pre-emption priority 2 bits for subpriority
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;								//指定抢占式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;											//指定响应式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*USART2模式配置*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
	
	/*接收中断使能*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}
