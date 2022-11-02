/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件：QuadratureEncoder.c
	功能：通过正交编码器解码得到小车左右两轮转速、转向
	作者：flotox@yeah.net
	日期：2014.9.4
	更新：2014.9.4
----------------------------------------------------------------*/

#include	"stm32f10x.h"
#include	"QuadratureEncoder.h"

/**
	*	@brief		EXTI3配置(左轮)
	*	@param		none
	*	@retval		none
	*/
void	QE_Exti3_Config(void){
	
	EXTI_InitTypeDef	EXTI_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*使能GPIOA、GPIOB、AFIO外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	/*JTAG-DP Disabled and SW-DP Enabled*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*初始化PA.15端口为IN_FLOATING模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*初始化PB.03端口为IN_FLOATING模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*选择PB.03为外部中断*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	
	/*配置Px.03为硬件中断模式、上升沿触发*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*EXTI3 NVIC设置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//2 bits for pre-emption priority 2 bits for subpriority
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//指定抢占式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//指定响应式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		EXTI5配置(右轮)
	*	@param		none
	*	@retval		none
	*/
void	QE_Exti9_5_Config(void){
	
	EXTI_InitTypeDef	EXTI_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*使能GPIOB、AFIO外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	/*JTAG-DP Disabled and SW-DP Enabled*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*初始化PB.04端口和PB.05端口为IN_FLOATING模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*选择PB.05为外部中断*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	
	/*配置Px.05为硬件中断模式、上升沿触发*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*EXTI5 NVIC设置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//2 bits for pre-emption priority 2 bits for subpriority
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//指定抢占式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//指定响应式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		正交编码器外设初始化
	*	@param		none
	*	@retval		none
	*/
void	QE_Init(void){
	
	QE_Exti3_Config();
	QE_Exti9_5_Config();
}
