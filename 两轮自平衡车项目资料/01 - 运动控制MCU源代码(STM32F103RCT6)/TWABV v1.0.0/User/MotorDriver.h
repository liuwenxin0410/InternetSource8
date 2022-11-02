/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件: MotorDriver.h
	功能：电机驱动
	作者：flotox@yeah.net
	日期：2014.9.5
	更新：2014.9.5
----------------------------------------------------------------*/

#ifndef _MOTORDRIVER_H_
#define _MOTORDRIVER_H_

void	MotorDriver_L_Turn_Forward(void);			//左轮电机正转
void	MotorDriver_L_Turn_Reverse(void);			//左轮电机反转
void	MotorDriver_R_Turn_Forward(void);			//右轮电机正转
void	MotorDriver_R_Turn_Reverse(void);			//右轮电机反转
void	MotorDriver_Init(void);								//电机驱动外设初始化

#endif
