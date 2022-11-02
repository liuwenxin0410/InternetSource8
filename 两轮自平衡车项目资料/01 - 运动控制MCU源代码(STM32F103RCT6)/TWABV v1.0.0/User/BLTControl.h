/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件：BLTControl.h
	功能：蓝牙控制指令接收
	作者：flotox@yeah.net
	日期：2014.9.5
	更新：2014.9.5
----------------------------------------------------------------*/

#ifndef	_BLTCONTROL_H_
#define	_BLTCONTROL_H_

/*开启蓝牙模块电源*/
#define		BLTCONTROL_POWER_ON					(GPIO_SetBits(GPIOB, GPIO_Pin_0))
/*关闭蓝牙模块电源*/
#define		BLTCONTROL_POWER_OFF				(GPIO_ResetBits(GPIOB, GPIO_Pin_0))

/*置高BLT_CONF*/
#define		BLTCONTROL_BLT_CONF_HIGH		(GPIO_SetBits(GPIOC, GPIO_Pin_5))
/*置低BLT_CONF*/
#define		BLTCONTROL_BLT_CONF_LOW			(GPIO_ResetBits(GPIOC, GPIO_Pin_5))

void	BLTControl_Init(void);										//蓝牙外设初始化

#endif
