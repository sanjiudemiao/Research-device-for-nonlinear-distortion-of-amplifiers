#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//移植映射表
#define SDA_IN()  IN_CHG()	 //PD5输入模式
#define SDA_OUT() OUT_CHG() //PD5输出模式
//IO操作函数	 
#define IIC_SCL    PDout(12) //SCL
#define IIC_SDA    PDout(13) //SDA	 
#define READ_SDA   PDin(13)  //输入SDA 



//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
//移植时需一并移植
void IN_CHG(void);          //D5输入模式
void OUT_CHG(void);         //D5输出模式
//****************
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif