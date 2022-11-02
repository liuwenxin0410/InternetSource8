#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

//LEDs ����LED ������.
//(����͵�ƽ,����;����ߵ�ƽ����)
#define LED_ON()   GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED_OFF()  GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define DRDY_High()  GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define DRDY_Low()   GPIO_ResetBits(GPIOA, GPIO_Pin_1)

void Initial_LED_GPIO(void);
void LED_Reverse(void);
void Initial_PWMLED(void);
void LED_Change(void);

#endif


//------------------End of File----------------------------
