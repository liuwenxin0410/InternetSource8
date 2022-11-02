/* eeprom.c file
��д�ߣ�lisn3188
��ַ��www.chiplab7.net
����E-mail��lisn3188@163.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2012-05-05
���ԣ� ���������ڵ���ʵ���ҵ�mini IMU����ɲ���
���ܣ�
��Flash����EEPROM ���ڱ���ƫ�úͱ궨����
------------------------------------
 */			  

#include "eeprom.h"

struct data_map Config;	//������Ϣ

void load_config(void){
	int16_t i;
	int16_t *ptr = &Config.is_good;
	int16_t *temp_addr = (int16_t *)PAGE_Config;
	FLASH_Unlock();
	for(i=0 ; i< sizeof(Config)/2;i++){
		*ptr = *temp_addr;
		temp_addr++;
		ptr++;
	}
	FLASH_Lock();
	if(Config.is_good != (int16_t)0xA55A){ //������Ч ����ʱ��Ҫװ��Ĭ��ֵ��
		Config.is_good = 0xA55A;
		Config.dGx_offset = 0;
		Config.dGy_offset = 0;
		Config.dGz_offset = 0;
	
		Config.dMx_offset = 0;
		Config.dMy_offset = 0;
		Config.dMz_offset = 0;
	
		Config.dMx_scale =1.0f;
		Config.dMy_scale =1.0f;
		Config.dMz_scale =1.0f;
	
		Write_config();	 //��Ĭ��ֵд��flash
	}
}

//����ǰ����д��flash
void Write_config(void){
	int16_t i;
	int16_t *ptr = &Config.is_good;
	uint32_t ptemp_addr = PAGE_Config;
	FLASH_Unlock();
 	FLASH_ErasePage(PAGE_Config); //�� ҳ
	for(i=0;i<sizeof(Config)/2;i++){
	 	FLASH_ProgramHalfWord(ptemp_addr,ptr[i]);
	 	ptemp_addr+=2;
	}
	FLASH_Lock();
}

//------------------End of File----------------------------
