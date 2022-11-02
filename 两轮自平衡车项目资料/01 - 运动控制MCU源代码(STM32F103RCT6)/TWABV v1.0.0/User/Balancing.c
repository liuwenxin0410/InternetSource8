/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件: Balancing.c
	功能：自平衡计算
	作者：flotox@yeah.net
	日期：2014.9.5
	更新：2014.9.5
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "GlobalVars.h"
#include "MotorDriver.h"

// float		Temp[200] = {0};
// uint32_t	Temp_i = 0;

/**
	*	@brief		自平衡计算外设初始化
	*	@param		none
	*	@retval		none
	*/
void	Balancing_Init(void){
	
	GPIO_InitTypeDef				GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;
	
	/*开启GPIOC外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/*开启TIM4外设时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/*配置PC.15端口为Out_PP模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*TIM4 NVIC设置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);													//2 bits for pre-emption priority 2 bits for subpriority
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;								//指定抢占式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;											//指定响应式优先级别,可取0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_DeInit(TIM4);
	
	/*定时器基本配置*/
	TIM_TimeBaseStructure.TIM_Prescaler = 0;																//时钟预分频数0,TIM4的时钟计数频率为72MHz
	TIM_TimeBaseStructure.TIM_Period = 20 - 1;															//自动重装载寄存器数20,10ms溢出1次
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;														//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							//向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_PrescalerConfig(TIM4, 0x8c9f, TIM_PSCReloadMode_Immediate);					//时钟分频系数36000,定时器时钟为2KHz
	TIM_ARRPreloadConfig(TIM4, DISABLE);																		//禁止ARR预装载缓冲器
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);															//使能Update中断
	
	TIM_Cmd(TIM4, ENABLE);																									//开启时钟
}

/**
	*	@brief		自平衡计算循环子程序(10ms被调用一次)
	*	@param		none
	*	@retval		none
	*/
void	Balancing_Loop(void){

	uiCount_Led++;
	
	/*Led闪烁*/
	if(uiCount_Led >= 40){
		
		uiCount_Led = 0;
		GPIO_WriteBit(GPIOC,GPIO_Pin_15,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15)));
	}
	
	switch(iMotion_Type){

		case	MOTION_TYPE_NONE:
			
			fTarget_Speed = 0;
			fTarget_Turn = 0;
			break;
		
		case	MOTION_TYPE_FORWARDS:
			
			fTarget_Speed = -150;
			fTarget_Turn = 0;
			break;
		
		case	MOTION_TYPE_BACKWARDS:
			
			fTarget_Speed = 150;			//150
			fTarget_Turn = 0;
			break;
		
		case	MOTION_TYPE_TURN_LEFT:
			
			fTarget_Speed = 0;
			fTarget_Turn = -210;
			break;
		
		case	MOTION_TYPE_TURN_RIGHT:
			
			fTarget_Speed = 0;
			fTarget_Turn = 210;
			break;
		
		case	MOTION_TYPE_FORWARDS_TURN_LEFT:
			
			fTarget_Speed = -150;
			fTarget_Turn = -210;
			break;
		
		case	MOTION_TYPE_FORWARDS_TURN_RIGHT:
			
			fTarget_Speed = -150;
			fTarget_Turn = 210;
			break;
		
		case	MOTION_TYPE_BACKWARDS_TURN_LEFT:
			
			fTarget_Speed = 150;
			fTarget_Turn = -210;
			break;
		
		case	MOTION_TYPE_BACKWARDS_TURN_RIGHT:
			
			fTarget_Speed = 150;
			fTarget_Turn = 210;
			break;
	}
	
	/*动作改变时增加中间的过度过程,过度过程为50ms*/
	if((iMotion_Type != iMotion_Type_Last) && (iCount_Motion_Type_Change == 0)){
		
		iCount_Motion_Type_Change = 10;
		iMotion_Type_Last = iMotion_Type;
	}
	
	if(iCount_Motion_Type_Change != 0){
		
		iCount_Motion_Type_Change --;
		fTarget_Speed = 0;
		fTarget_Turn = 0;
	}
	
	/*速度和位移一阶低通滤波*/
	fSpeed_Vechile = (iCount_L + iCount_R) * 0.5;
	fSpeed_Vechile_F = fSpeed_Vechile_F * 0.7 + fSpeed_Vechile * 0.3;
	
	/*累加求位移*/
	fPosition += fSpeed_Vechile_F;

	/*位移调节量*/
	fPosition += fTarget_Speed;

	/*位移限制,上下限待调节*/
	if(fPosition > 5000){																//5000
		
		fPosition = 5000;
	}
	else if(fPosition < -5000){
		
		fPosition = -5000;
	}
	
	/*左轮和右轮光栅脉冲计数器清零*/
	iCount_L = 0;
	iCount_R = 0;
	
	/*角度限制*/
	if( (fPitchAngle < 40.0) && (fPitchAngle > -40.0) ){
		
		
		/*俯仰角速度,单位deg/s,大于0则角速度方向向前,小于0则角速度方向向后*/
		fAngle_Vel = -(fPitchAngle - fPitchAngle_Last);
		
		/*保存此次俯仰角度*/
		fPitchAngle_Last = fPitchAngle;
		
		/*计算PWM输出控制量*/
		fPWM =	(-fpid_angle) * fPitchAngle +
						fpid_angle_vel * fAngle_Vel +
						fpid_speed * fSpeed_Vechile_F +
						fPosition * fpid_position;
		
		/*旋转调节量*/
		fPWM_L = fPWM + fTarget_Turn;
		fPWM_R = fPWM - fTarget_Turn;
		
		/*左轮控制*/
		if(fPWM_L > 0){
			
			MotorDriver_L_Turn_Forward();
		}
		else{
			
			MotorDriver_L_Turn_Reverse();
			fPWM_L = -fPWM_L;
		}
			
		/*右轮控制*/
		if(fPWM_R > 0){
			
			MotorDriver_R_Turn_Forward();
		}
		else{
			
			MotorDriver_R_Turn_Reverse();
			fPWM_R = -fPWM_R;
		}
		
		/*加上死区电压*/
		fPWM_L += 40;
		fPWM_R += 10;
		
		/*限制PWM*/
		if(fPWM_L > 1000){
			
			fPWM_L = 1000;
		}
		
		if(fPWM_R > 1000){
			
			fPWM_R = 1000;
		}
		
		/*输出PWM控制量*/
		TIM_SetCompare2(TIM8, (uint16_t)fPWM_L);					//左轮
		TIM_SetCompare2(TIM1, (uint16_t)fPWM_R);					//右轮
	}
	/*倾角过大时的姿态数据不参与计算*/
	else{
		
		TIM_SetCompare2(TIM8, 0);													//左轮
		TIM_SetCompare2(TIM1, 0);													//右轮
	}
	
}
