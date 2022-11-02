/**---------------------------------------------------------------
	项目：两轮自平衡小车(Two-Wheels Auto-Balancing Vechile)
	版本：v1.0.0
	文件：main.c
	功能：主函数
	作者：flotox@yeah.net
	日期：2014.9.4
	更新：2014.9.4
----------------------------------------------------------------*/

#include "stm32f10x.h"
#include "QuadratureEncoder.h"
#include "MotorDriver.h"
#include "Balancing.h"
#include "BLTControl.h"
#include "AHRS_Rcv.h"

/**
	*	@brief		主函数
	*	@param		none
	*	@retval		none
	*/
int	main(void){

	QE_Init();											//正交编码器外设初始化
	MotorDriver_Init();							//电机驱动外设初始化
	Balancing_Init();								//自平衡计算外设初始化
	BLTControl_Init();							//蓝牙模块外设初始化
	AHRS_Rcv_Init();								//AHRS姿态数据接收外设初始化

	while(1);
}
