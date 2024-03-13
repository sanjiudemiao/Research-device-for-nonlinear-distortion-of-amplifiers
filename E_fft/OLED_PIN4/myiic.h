#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//��ֲӳ���
#define SDA_IN()  IN_CHG()	 //PD5����ģʽ
#define SDA_OUT() OUT_CHG() //PD5���ģʽ
//IO��������	 
#define IIC_SCL    PDout(12) //SCL
#define IIC_SDA    PDout(13) //SDA	 
#define READ_SDA   PDin(13)  //����SDA 



//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
//��ֲʱ��һ����ֲ
void IN_CHG(void);          //D5����ģʽ
void OUT_CHG(void);         //D5���ģʽ
//****************
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif