#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "Data_map.h"

/*
ʹ�� STM32F �ڲ�Flash��EEPROM
������API �ӳ���
*/

void Write_config(void);  //д������
void load_config(void);	  //��ȡ����

#endif /* __EEPROM_H */

//------------------End of File----------------------------
