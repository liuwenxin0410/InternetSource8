/* main.c file
��д�ߣ�lisn3188
��ַ��www.chiplab7.com
����E-mail��lisn3188@163.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2012-04-25
���ԣ� ���������ڵ���ʵ���ҵ�mini IMU����ɲ���
���ܣ�
��̬���� IMU
�������������ֵ������̬���㡣�õ�Ŀ������ĸ����Ǻͺ���� �ͺ����
------------------------------------
 */

#include "IMU.h"

volatile float exInt, eyInt, ezInt;  // ������
volatile float q0, q1, q2, q3; // ȫ����Ԫ��
volatile float integralFBhand,handdiff;
volatile uint32_t lastUpdate, now; // �������ڼ��� ��λ us


void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_Timer3(void)
*��������:	  ��ʼ��Tim2  Tim3 ��������ʱ���������Բ���һ��32λ�Ķ�ʱ�����ṩϵͳus ���ļ�ʱ	
�����������
���������û��	
*******************************************************************************/
void Initial_Timer3(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE); 
	/* TIM2 configuration*/ 
  /* Time Base configuration �������� ���ö�ʱ����ʱ����Ԫ*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //�Զ���װֵ         
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
  
  TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Update);
  /* Disable the TIM2 Update event */
  TIM_UpdateDisableConfig(TIM2, ENABLE);
  /* ----------------------TIM2 Configuration as slave for the TIM3 ----------*/
  /* Select the TIM2 Input Trigger: TIM3 TRGO used as Input Trigger for TIM2*/
  TIM_SelectInputTrigger(TIM2, TIM_TS_ITR2);
  /* Use the External Clock as TIM2 Slave Mode */
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);
  /* Enable the TIM2 Master Slave Mode */
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
  TIM_ARRPreloadConfig(TIM2, ENABLE);	
	/* ��ʱ������:
	1.���ö�ʱ��������ֵ 50000
	2.����ʱ�ӷ�Ƶϵ����TIM_CKD_DIV1
	3. ����Ԥ��Ƶ��  1Mhz/50000= 1hz 
	4.��ʱ������ģʽ  ���ϼ���ģʽ
	*/		 
  	TIM_TimeBaseStructure.TIM_Period = 0xffff;     
  	TIM_TimeBaseStructure.TIM_Prescaler = 72;	 //1M ��ʱ��  
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//Ӧ�����õ�TIM3 
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// ʹ��TIM3���ؼĴ���ARR
  	TIM_ARRPreloadConfig(TIM3, ENABLE);	

	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	/* ----------------------TIM3 Configuration as Master for the TIM2 -----------*/
  	/* Use the TIM3 Update event  as TIM3 Trigger Output(TRGO) */
  	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  	/* Enable the TIM3 Master Slave Mode */
  	TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);

  	//������ʱ��
	TIM_Cmd(TIM3, ENABLE); 
  	TIM_Cmd(TIM2, ENABLE);                  
}

// Fast inverse square-root
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   float invSqrt(float x)
*��������:	   ���ټ��� 1/Sqrt(x) 	
��������� Ҫ�����ֵ
��������� ���
*******************************************************************************/
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint32_t micros(void)
*��������:	  ��ȡϵͳ���е�ʱ�� �����ص�λΪus ��ʱ������	
�����������
�����������������ǰʱ�䣬���ϵ翪ʼ��ʱ  ��λ us
*******************************************************************************/
uint32_t micros(void)
{
 	uint32_t temp=0 ;
 	temp = TIM2->CNT; //����16λʱ��
 	temp = temp<<16;
 	temp += TIM3->CNT; //����16λʱ��
 	return temp;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_init(void)
*��������:	  ��ʼ��IMU���	
			  ��ʼ������������
			  ��ʼ����Ԫ��
			  ����������
			  ����ϵͳʱ��
�����������
���������û��
*******************************************************************************/
void IMU_init(void)
{	 
	MPU6050_initialize();
	HMC5883L_SetUp();
	delay_ms(50);
	MPU6050_initialize();
	HMC5883L_SetUp();
	Initial_Timer3();
	// initialize quaternion
  	q0 = 1.0f;  //��ʼ����Ԫ��
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
  	exInt = 0.0;
  	eyInt = 0.0;
  	ezInt = 0.0;
  	lastUpdate = micros();//����ʱ��
  	now = micros();
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getValues(float * values)
*��������:	 ��ȡ���ٶ� ������ ������ �ĵ�ǰֵ  
��������� �������ŵ������׵�ַ
���������û��
*******************************************************************************/
void IMU_getValues(float * values) {  
	int16_t accgyroval[6];
	int i;
	//��ȡ���ٶȺ������ǵĵ�ǰADC
    MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);
    for(i = 0; i<6; i++) {
      if(i < 3) {
        values[i] = (float) accgyroval[i];
      }
      else {
        values[i] = ((float) accgyroval[i]) / 32.8f; //ת�ɶ�ÿ��
		//�����Ѿ������̸ĳ��� 1000��ÿ��  32.8 ��Ӧ 1��ÿ��
      }
    }
    HMC58X3_mgetValues(&values[6]);	//��ȡ�����Ƶ�ADCֵ
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_AHRSupdate
*��������:	 ����AHRS ������Ԫ�� 
��������� ��ǰ�Ĳ���ֵ��
���������û��
*******************************************************************************/
#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases

void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez,halfT;
  float tempq0,tempq1,tempq2,tempq3;

  // �Ȱ���Щ�õõ���ֵ���
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;   
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;          
  
  now = micros();  //��ȡʱ��
  if(now<lastUpdate){ //��ʱ��������ˡ�
  halfT =  ((float)(now + (0xffff- lastUpdate)) / 2000000.0f);
  }
  else	{
  halfT =  ((float)(now - lastUpdate) / 2000000.0f);
  }
  lastUpdate = now;	//����ʱ��

  norm = invSqrt(ax*ax + ay*ay + az*az);       
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;
  //�ѼӼƵ���ά����ת�ɵ�λ������

  norm = invSqrt(mx*mx + my*my + mz*mz);          
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;

  //�ӻ�������ϵ�ĵ������̲⵽��ʸ��ת�ɵ�������ϵ�µĴų�ʸ��hxyz������ֵ��
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);
  /*
  �����������ϵ�µĴų�ʸ��bxyz���ο�ֵ����
  ��Ϊ����ش�ˮƽ�нǣ�������֪��0�ȣ���ȥ��ƫ�ǵ����أ��̶��򱱣�������by=0��bx=ĳֵ
  ������ο��ش�ʸ���ڴ�ֱ����Ҳ�з���bz��������ÿ���ط����ǲ�һ���ġ�
  �����޷���֪��Ҳ���޷������ںϣ��и��ʺ�����ֱ���������ںϵļ��ٶȼƣ�������ֱ�ӴӲ���ֵhz�ϸ��ƹ�����bz=hz��
  �ų�ˮƽ�������ο�ֵ�Ͳ���ֵ�Ĵ�СӦ����һ�µ�(bx*bx) + (by*by)) = ((hx*hx) + (hy*hy))��
  ��Ϊby=0�����Ծͼ򻯳�(bx*bx)  = ((hx*hx) + (hy*hy))�������bx��
  */         
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;     
  
  /*
  ���ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
  �������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
  ���������vx\y\z����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������������λ������
  */
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
  /*
  ���ǰѵ�������ϵ�ϵĴų�ʸ��bxyz��ת����������wxyz��
  ��Ϊby=0�����������漰��by�Ĳ��ֶ���ʡ���ˡ�
  ������������vxyz�����㣬��Ϊ����g��gz=1��gx=gy=0�����������漰��gxgy�Ĳ���Ҳ��ʡ����
  */
  wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
  
  //���ڰѼ��ٶȵĲ���ʸ���Ͳο�ʸ����������Ѵų��Ĳ���ʸ���Ͳο�ʸ��Ҳ����������������������ݡ�
//   ex = (ay*vz - az*vy) + (my*wz - mz*wy);
//   ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
//   ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

	ex = ( ay * vz - az * vy ) ;  
	ey = ( az * vx - ax * vz ) ;  
	ez = ( ax * vy - ay * vx ) ;

  /*
  axyz�ǻ����������ϵ�ϣ����ٶȼƲ����������������Ҳ����ʵ�ʲ����������������
  axyz�ǲ����õ�������������vxyz�����ݻ��ֺ����̬����������������������Ƕ��ǻ����������ϵ�ϵ�����������
  ������֮�������������������ݻ��ֺ����̬�ͼӼƲ��������̬֮�����
  ������������������������Ҳ�������������ˣ�����ʾ��exyz�����������������Ĳ����
  �����������Ծ���λ�ڻ�������ϵ�ϵģ������ݻ������Ҳ���ڻ�������ϵ�����Ҳ���Ĵ�С�����ݻ����������ȣ����������������ݡ���������Լ��ö�������һ�£����������ǶԻ���ֱ�ӻ��֣����Զ����ݵľ�������ֱ�������ڶԻ�������ϵ�ľ�����
  */
if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
  exInt = exInt + ex * Ki * halfT;
  eyInt = eyInt + ey * Ki * halfT;	
  ezInt = ezInt + ez * Ki * halfT;

  // �ò���������PI����������ƫ
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;

  }

  // ��Ԫ��΢�ַ���
  tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
  
  // ��Ԫ���淶��
  norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getQ(float * q)
*��������:	 ������Ԫ�� ���ص�ǰ����Ԫ����ֵ
��������� ��Ҫ�����Ԫ���������׵�ַ
���������û��
*******************************************************************************/
float mygetqval[9];	//���ڴ�Ŵ�����ת�����������
void IMU_getQ(float * q) {

  IMU_getValues(mygetqval);	 
  //�������ǵĲ���ֵת�ɻ���ÿ��
  //���ٶȺʹ����Ʊ��� ADCֵ������Ҫת��
IMU_AHRSupdate(mygetqval[3] * M_PI/180, mygetqval[4] * M_PI/180, mygetqval[5] * M_PI/180,
   mygetqval[0], mygetqval[1], mygetqval[2], mygetqval[6], mygetqval[7], mygetqval[8]);

  q[0] = q0; //���ص�ǰֵ
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getYawPitchRoll(float * angles)
*��������:	 ������Ԫ�� ���ص�ǰ��������̬����
��������� ��Ҫ�����̬�ǵ������׵�ַ
���������û��
*******************************************************************************/
void IMU_getYawPitchRoll(float * angles) {
  float q[4]; //����Ԫ��
  volatile float gx=0.0, gy=0.0, gz=0.0; //������������
  IMU_getQ(q); //����ȫ����Ԫ��
  
  angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1)* 180/M_PI; // yaw
  angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch
  angles[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll
  //if(angles[0]<0)angles[0]+=360.0f;  //�� -+180��  ת��0-360��
}

//------------------End of File----------------------------
