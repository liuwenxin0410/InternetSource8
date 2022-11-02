/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件: GlobalVars.h
	功能：全局变量声明
	作者：flotox@yeah.net
	日期：2014.9.4
	更新：2014.9.4
----------------------------------------------------------------*/

#ifndef	_GLOBALVARS_H_
#define	_GLOBALVARS_H_

#define		MOTION_TYPE_NONE									0x00		//不动作
#define		MOTION_TYPE_FORWARDS							0x01		//前进
#define		MOTION_TYPE_BACKWARDS							0x02		//后退
#define		MOTION_TYPE_TURN_LEFT							0x03		//左转
#define		MOTION_TYPE_TURN_RIGHT						0x04		//右转
#define		MOTION_TYPE_FORWARDS_TURN_LEFT		0x05		//前进并左转
#define		MOTION_TYPE_FORWARDS_TURN_RIGHT		0x06		//前进并右转
#define		MOTION_TYPE_BACKWARDS_TURN_LEFT		0x07		//后退并左转
#define		MOTION_TYPE_BACKWARDS_TURN_RIGHT	0x08		//后退并右转

extern	volatile int32_t		iCount_R;													//在单位时间(20ms)内,右轮光栅脉冲计数,大于0则正转,小于0则反转,定时时间到后需清零该数值
extern	volatile int32_t		iCount_L;													//在单位时间(20ms)内,左轮光栅脉冲计数,大于0则正转,小于0则反转,定时时间到后需清零该数值
extern	volatile uint32_t		uiCount_Led;											//Led闪烁计数器
extern	volatile int32_t		iMotion_Type;											//动作类型
extern	volatile int32_t		iMotion_Type_Last;								//动作类型暂存
extern	volatile uint32_t		iCount_Motion_Type_Change;				//动作类型改变则为1,否则为0


extern	volatile float			fPitchAngle;											//俯仰角度,小于0则前倾,大于0则后仰
extern	volatile float			fPitchAngle_Last;									//上一次仰角角度,小于0则前倾,大于0则后仰
extern	volatile float			fAngle_Vel;												//俯仰角速度,小于0则角加速度方向为前倾方向,大于0则角加速度方向为后仰方向

extern	volatile float			fpid_angle;												//角度控制系数
extern	volatile float			fpid_angle_vel;										//角加速度控制系数
extern	volatile float			fpid_speed;												//速度控制系数
extern	volatile float			fpid_position;										//位移控制系数
extern	volatile float			fLimit_Speed;

extern	volatile float			fTarget_Speed;										//速度调节量
extern	volatile float			fTarget_Turn;											//旋转调节量

extern	volatile float			fSpeed_L;													//左轮速度
extern	volatile float			fSpeed_R;													//右轮速度
extern	volatile float			fSpeed_Vechile;										//小车整体速度
extern	volatile float			fSpeed_Vechile_F;									//低通滤波后的小车整体速度
extern	volatile float			fPosition;												//位移

extern	volatile float			fPWM;															//计算得出的PWM输出量
extern	volatile float			fPWM_L;														//左轮PWM输出量
extern	volatile float			fPWM_R;														//右轮PWM输出量

#endif
