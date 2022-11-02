/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件：BLTControl.c
	功能：蓝牙控制指令接收
	作者：flotox@yeah.net
	日期：2014.9.5
	更新：2014.9.5
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "BLTControl.h"

/**
	*	@brief		蓝牙外设配置
	*	@param		none
	*	@retval		none
	*/
void	BLTControl_Init(void){
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*开启GPIOB、GPIOC、GPIOD外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	/*初始化PB.00端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*初始化PC.05端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*关闭蓝牙模块电源*/
	BLTCONTROL_POWER_OFF;
	
	/*开启UART5外设时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
	/*UART5的GPIO配置*/
	/*配置USART5 Tx(PC.12)为AF_PP*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*配置UART5 Rx(PD.02)为IN_FLOATING*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*UART5 NVIC配置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);													//2 bits for pre-emption priority 2 bits for subpriority
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;								//指定抢占式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;											//指定响应式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*UART5模式配置*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_Cmd(UART5, ENABLE);
	
	/*接收中断使能*/
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	
	/*开启蓝牙模块电源*/
	BLTCONTROL_POWER_ON;
}
