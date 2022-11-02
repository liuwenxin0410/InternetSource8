#ifndef __IMU_H
#define __IMU_H

#include "common.h"  //�������е����� ͷ�ļ�

#include <math.h>
#define M_PI  (float)3.1415926535

//Mini IMU AHRS �����API
void IMU_init(void); //��ʼ��
void IMU_getYawPitchRoll(float * ypr); //������̬
uint32_t micros(void);	//��ȡϵͳ�ϵ���ʱ��  ��λ us 

#endif

//------------------End of File----------------------------
