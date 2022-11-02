/* BMP180.c file
��д�ߣ�lisn3188
��ַ��www.chiplab7.com
����E-mail��lisn3188@163.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2012-04-25
���ԣ� ���������ڵ���ʵ���ҵ�mini IMU����ɲ���
���ܣ�
�ṩBMP180 ��ʼ�� ���� ��ȡ�¶� ��ѹ API
------------------------------------
 */
#include "BMP180.h"
#include <math.h>

// ��ѹ��״̬��
#define SCTemperature  0x01
#define CTemperatureing  0x02
#define SCPressure  0x03
#define SCPressureing  0x04
#define Time_Limit 30000L  //ÿ��ת������ʱʱ��
#define Time_tempC 6000L

#define MOVAVG_SIZE   32  //��������� MOVAVG_SIZE ��ֵ 

volatile int16_t ac1,ac2,ac3,b1,b2,mb,mc,md;     // �궨������  
volatile uint16_t ac4,ac5,ac6;                   // �궨������
volatile int32_t b5;                    //�¶�
volatile uint32_t  ConvetTime;  
uint8_t _buff[BUFFER_SIZE];    // ���ݻ�����
int16_t _oss;                 // ����������
  
int32_t _cm_Offset, _Pa_Offset;
int32_t _param_datum, _param_centimeters;
volatile unsigned char BPM085_ST;
int32_t last_Temperature,last_Pressure,last_Alt;

//�Ƚ��ȳ�����������
int32_t  test_buf[20];
int32_t  Temp_buffer[MOVAVG_SIZE],Press_buffer[MOVAVG_SIZE],Alt_buffer[MOVAVG_SIZE];
uint8_t temp_index=0,press_index=0,alt_index=0; //����ָ��

unsigned char BMP180_IS_Finish(void);
void BMP180_writemem(uint8_t _addr, uint8_t _val);
void BMP180_calcTrueTemperature(u8 rw);
void BMP180_getTemperature(int32_t *_Temperature,u8 rw);
void BMP180_calcTruePressure(int32_t *_TruePressure,u8 writeread);
void BMP180_getAltitude(int32_t *_centimeters,u8 rw);

//��ȡ���� ��ƽ��ֵ
int32_t MS561101BA_getAvg(int32_t * buff, int size) {
  float sum = 0.0;
  int i;
  for(i=0; i<size; i++) {
    sum += buff[i];
  }
  return sum / size;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_newTemperature(int32_t T)
*��������:		���һ���µ�ֵ���¶ȹ�����
*******************************************************************************/
void BMP180_newTemperature(int32_t val)
{	
  Temp_buffer[temp_index] = val;
  temp_index = (temp_index + 1) % MOVAVG_SIZE;

  last_Temperature=MS561101BA_getAvg(Temp_buffer,MOVAVG_SIZE);	//ȡƽ��ֵ
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_newPressure(int32_t P)
*��������:		���һ���µ�ֵ����ѹ������
*******************************************************************************/
void BMP180_newPressure(int32_t val)
{	
  Press_buffer[press_index] = val;
  press_index = (press_index + 1) % MOVAVG_SIZE;

  last_Pressure = MS561101BA_getAvg(Press_buffer,MOVAVG_SIZE);	//ȡƽ��ֵ
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_newALT(int32_t A)
*��������:		���һ���µ�ֵ���߶ȹ�����
*******************************************************************************/
void BMP180_newALT(int32_t val)
{	
  Alt_buffer[alt_index] = val;
  alt_index = (alt_index + 1) % MOVAVG_SIZE;

  last_Alt = MS561101BA_getAvg(Alt_buffer,MOVAVG_SIZE);	//ȡƽ��ֵ
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_getAlt(int32_t *_centimeters)
*��������:		��ȡ���µĸ߶�ֵ����λΪ cm 
*******************************************************************************/
void BMP180_getAlt(int32_t *_centimeters)
{
	*_centimeters = last_Alt;	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_getPress(int32_t *_TruePressure)
*��������:		��ȡ���µ���ѹֵ����λΪ pa 
*******************************************************************************/
void BMP180_getPress(int32_t *_TruePressure)
{
	*_TruePressure = last_Pressure;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_getTemperat(int32_t *_Temperature)
*��������:		��ȡ���µ��¶�ֵ����λΪ 0.1C 
*******************************************************************************/
void BMP180_getTemperat(int32_t *_Temperature)
{
	*_Temperature =	last_Temperature;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_Routing(void)
*��������:		BMP180 ����ʱ���õĳ���
				�ó���᲻ͣ�ض�ȡ��ѹֵ���¶�ֵ��
				�û���Ҫ���ڵ�����������Ը��¸߶Ⱥ��¶���Ϣ
*******************************************************************************/
void BMP180_Routing(void)
{
  static int32_t last_Temp;
  switch(BPM085_ST){
  case SCTemperature: //��ʼ�¶�ת��
  				BMP180_writemem(CONTROL, READ_TEMPERATURE); 
				BPM085_ST = CTemperatureing;//״̬�л�
				ConvetTime = micros(); //��ȡ��ʼת����ʱ��
				break;
  case CTemperatureing: //���ڽ����¶�ת��
  			 	if((micros()-ConvetTime)>Time_tempC){ //�Ƿ񵽴�ת��ʱ�䣿
				BMP180_calcTrueTemperature(0);
				BMP180_getTemperature(&last_Temperature,0);
				BMP180_newTemperature(last_Temperature);
				if(last_Temperature>last_Temp){
					if((last_Temperature-last_Temp)>5)
						last_Temp = last_Temperature;
				}else if((last_Temp - last_Temperature)>5)
					  	last_Temp = last_Temperature;
				last_Temp = last_Temperature;
				BPM085_ST = SCPressure;
				}
  				break;
  case SCPressure:  //��ʼ��ѹ��ת��
  				BMP180_writemem(CONTROL, READ_PRESSURE+(_oss << 6));
				BPM085_ST = SCPressureing;
				ConvetTime = micros();  //��ȡ��ʼת����ʱ��
  				break;
  case SCPressureing: //���ڽ�����ѹת�� 
  				if((micros()-ConvetTime)>Time_Limit){  //�Ƿ񵽴�ת��ʱ�䣿
				BMP180_getAltitude(&last_Alt,0);
				BMP180_newALT(last_Alt);
				BPM085_ST = SCTemperature;
				}
  				break;
  default :BPM085_ST=SCTemperature; break;
  }

}

void BMP180_writemem(uint8_t _addr, uint8_t _val) {
  IICwriteByte(BMP180_ADDR,_addr,_val);
}

void BMP180_readmem(uint8_t _addr, uint8_t _nbytes, uint8_t __buff[]) {
  IICreadBytes(BMP180_ADDR,_addr,_nbytes,__buff);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_getCalData(void)
*��������:		���� BMP180 �ڲ��ı궨��Ϣ
*******************************************************************************/
void BMP180_getCalData(void) {
  BMP180_readmem(CAL_AC1, 2, _buff);
  ac1 = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1]));
  BMP180_readmem(CAL_AC2, 2, _buff);
  ac2 = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1]));
  BMP180_readmem(CAL_AC3, 2, _buff);
  ac3 = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1]));
  BMP180_readmem(CAL_AC4, 2, _buff);
  ac4 = ((uint16_t)_buff[0] <<8 | ((uint16_t)_buff[1]));
  BMP180_readmem(CAL_AC5, 2, _buff);
  ac5 = ((uint16_t)_buff[0] <<8 | ((uint16_t)_buff[1]));
  BMP180_readmem(CAL_AC6, 2, _buff);
  ac6 = ((uint16_t)_buff[0] <<8 | ((uint16_t)_buff[1])); 
  BMP180_readmem(CAL_B1, 2, _buff);
  b1 = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1])); 
  BMP180_readmem(CAL_B2, 2, _buff);
  b2 = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1])); 
  BMP180_readmem(CAL_MB, 2, _buff);
  mb = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1]));
  BMP180_readmem(CAL_MC, 2, _buff);
  mc = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1]));
  BMP180_readmem(CAL_MD, 2, _buff);
  md = ((int16_t)_buff[0] <<8 | ((int16_t)_buff[1])); 
}

void BMP180_calcTrueTemperature(u8 rw){
  int32_t ut,x1,x2,mctemp,mdtemp;
  if(rw){
  BMP180_writemem(CONTROL, READ_TEMPERATURE);
  delay_ms(10);                          // min. 4.5ms read Temp delay
  }
  BMP180_readmem(CONTROL_OUTPUT, 2, _buff); 
  ut = ((int32_t)_buff[0] << 8 | ((int32_t)_buff[1]));    // uncompensated temperature value
 
  // calculate temperature
  x1 = (((int32_t)ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
  mctemp= mc;
  mdtemp= md;
  x2 = (mctemp <<11) / (x1 + mdtemp);
  b5 = x1 + x2;
}

void BMP180_setMode(u8 _BMPMode){
  _oss = _BMPMode;
}

void BMP180_calcTruePressure(int32_t *_TruePressure,u8 writeread) {
  volatile int32_t up,x1,x2,x3,b3,b6,p;
  volatile int32_t b4,b7;
  volatile int32_t tmp; 

 //read Raw Pressure
 if(writeread){

  #if AUTO_UPDATE_TEMPERATURE
  BMP180_calcTrueTemperature(writeread);        // b5 update 
  #endif 
  BMP180_writemem(CONTROL, READ_PRESSURE+(_oss << 6));
  delay_ms(30); 
  }
     
  BMP180_readmem(CONTROL_OUTPUT, 3, _buff);  
  up = ((((int32_t)_buff[0] <<16) | ((int32_t)_buff[1] <<8) | ((int32_t)_buff[2])) >> (8-_oss)); // uncompensated pressure value
  
  // calculate true pressure
  b6 = b5 - 4000;             // b5 is updated by calcTrueTemperature().
  x1 = (b2* ((b6 * b6) >> 12)) >> 11;
  x2 = ac2 * b6 >> 11;
  x3 = x1 + x2;
  tmp = ac1;
  tmp = (tmp * 4 + x3) << _oss;
  b3 = (tmp + 2) >> 2;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t)up - b3) * (50000 >> _oss);
  if(b7 < 0x80000000){
   p = ((uint32_t)b7 << 1) / b4;
  }	else{
  p = (b7 / b4) << 1;
  }
  //p = b7 < 0x80000000 ? (b7 << 1) / b4 : (b7 / b4) << 1;
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  *_TruePressure = p + ((x1 + x2 + 3791) >> 4);

}

void BMP180_getPressure(int32_t *_Pa,u8 wr){   
  int32_t TruePressure;
  BMP180_calcTruePressure(&TruePressure,wr); 
  //TruePressure;
  *_Pa = TruePressure / pow((1 - (float)_param_centimeters / 4433000), 5.255) + _Pa_Offset;
  // converting from float to int32_t truncates toward zero, 1010.999985 becomes 1010 resulting in 1 Pa error (max).  
  // Note that BMP180 abs accuracy from 700...1100hPa and 0..+65�C is +-100Pa (typ.)
}

void BMP180_setLocalAbsAlt(int32_t _centimeters){  
  int32_t tmp_Pa;
 
  _param_centimeters = _centimeters;   
  BMP180_getPressure(&tmp_Pa,1);    // calc pressure based on current altitude
  _param_datum = tmp_Pa;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_ResetAlt(int32_t _centimeters)
*��������:		����ǰ����ѹֵ���� �߶Ȳ�0��ʱ����ѹ
*******************************************************************************/
void BMP180_ResetAlt(int32_t _centimeters){  
  int32_t tmp_Pa;
  _param_centimeters = _centimeters;   
  BMP180_getPress(&tmp_Pa);    // calc pressure based on current altitude
  _param_datum = tmp_Pa;
}


void BMP180_getAltitude(int32_t *_centimeters,u8 rw){
  int32_t TruePressure;
  BMP180_calcTruePressure(&TruePressure,rw); 
  BMP180_newPressure(TruePressure);
   TruePressure = last_Pressure;
  *_centimeters =  4433000 * (1 - pow((TruePressure / (float)_param_datum), 0.1903)) + _cm_Offset;  
  // converting from float to int32_t truncates toward zero, 100.999985 becomes 100 resulting in 1 cm error (max).
}

void BMP180_setLocalPressure(int32_t _Pa){   
  int32_t tmp_alt;
 
  _param_datum = _Pa;   
  BMP180_getAltitude(&tmp_alt,1);    // calc altitude based on current pressure   
  _param_centimeters = tmp_alt;
}

void BMP_init(u8 _BMPMode, int32_t _initVal, u8 _Unitmeters){     
  BMP180_getCalData();               // initialize cal data
  BMP180_calcTrueTemperature(1);      // initialize b5
  BMP180_setMode(_BMPMode); //���ù�����
  _Unitmeters>0 ? BMP180_setLocalAbsAlt(_initVal) : BMP180_setLocalPressure(_initVal); 
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_init(void)
*��������:		���ⲿ���õĳ�ʼ������
*******************************************************************************/
void BMP180_init(void) {  
  _cm_Offset = 0;
  _Pa_Offset = 0;               // 1hPa = 100Pa = 1mbar	
   //��ʼ����ѹ��������
   //MODE_ULTRA_HIGHRES  �߾��Ȳ���ģʽ
  BMP_init(MODE_ULTRA_HIGHRES, 0, 1);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void BMP180_getTemperature(int32_t *_Temperature,u8 rw)
*��������:		��ȡ�¶�ֵ��
���� ��
int32_t *_Temperature  �¶Ƚ����ŵ�ָ��
u8 rw	 �Ƿ���Ҫ�ȴ� 0 ����Ҫ�ȴ���
*******************************************************************************/
void BMP180_getTemperature(int32_t *_Temperature,u8 rw) {
  BMP180_calcTrueTemperature(rw);                            // force b5 update
  *_Temperature = ((b5 + (int32_t)8) >> 4);
}

//------------------End of File----------------------------
