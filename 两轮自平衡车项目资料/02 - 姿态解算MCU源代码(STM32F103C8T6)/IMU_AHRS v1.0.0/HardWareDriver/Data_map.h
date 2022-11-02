#ifndef __DATAMAP_H
#define __DATAMAP_H

#define  PAGE_Config    (0x08000000 + 62 * 1024) //��������Ϣ����ڵ�62ҳFlash

struct data_map{
	int16_t is_good;   //�����Ƿ���Ч
	int16_t dGx_offset;
	int16_t dGy_offset;
	int16_t dGz_offset;
	
	int16_t dMx_offset;
	int16_t dMy_offset;
	int16_t dMz_offset;
	float  dMx_scale;
	float  dMy_scale;
	float  dMz_scale;
};

extern struct data_map Config;


#endif

//------------------End of File----------------------------
