/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件: GlobalVars.c
	功能：全局变量声明
	作者：flotox@yeah.net
	日期：2014.9.4
	更新：2014.9.4
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "GlobalVars.h"

volatile int32_t		iCount_R = 0;															//在单位时间(20ms)内,右轮光栅脉冲计数,大于0则正转,小于0则反转,定时时间到后需清零该数值
volatile int32_t		iCount_L = 0;															//在单位时间(20ms)内,左轮光栅脉冲计数,大于0则正转,小于0则反转,定时时间到后需清零该数值
volatile uint32_t		uiCount_Led = 0;													//Led闪烁计数器
volatile int32_t		iMotion_Type = MOTION_TYPE_NONE;					//动作类型
volatile int32_t		iMotion_Type_Last = MOTION_TYPE_NONE;			//动作类型暂存
volatile uint32_t		iCount_Motion_Type_Change = 0;							//动作类型改变则为1,否则为0

volatile float			fPitchAngle = 0;													//俯仰角度,小于0则前倾,大于0则后仰
volatile float			fPitchAngle_Last = 0;											//上一次仰角角度,小于0则前倾,大于0则后仰
volatile float			fAngle_Vel = 0;														//俯仰角速度,小于0则角加速度方向为前倾方向,大于0则角加速度方向为后仰方向

volatile float			fpid_angle = 29;													//角度控制系数/34
volatile float			fpid_angle_vel = 170;											//角加速度控制系数/210
volatile float			fpid_speed = 7;														//6.7速度控制系数/8.8
volatile float			fpid_position = 0.085;										//0.095位移控制系数/0.095
volatile float			fLimit_Speed = 1.0;												//速度限制因子

volatile float			fTarget_Speed = 0;												//速度调节量
volatile float			fTarget_Turn = 0;													//旋转调节量

volatile float			fSpeed_L = 0;															//左轮速度
volatile float			fSpeed_R = 0;															//右轮速度
volatile float			fSpeed_Vechile = 0;												//小车整体速度
volatile float			fSpeed_Vechile_F = 0;											//低通滤波后的小车整体速度
volatile float			fPosition = 0;														//位移

volatile float			fPWM = 0;																	//计算得出的PWM输出量
volatile float			fPWM_L = 0;																//左轮PWM输出量
volatile float			fPWM_R = 0;																//右轮PWM输出量
