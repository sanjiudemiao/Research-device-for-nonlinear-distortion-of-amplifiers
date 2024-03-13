#ifndef _OLED_H
#define _OLED_H
#include "sys.h"
#include "stdlib.h"	

#define high 1
#define low 0

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void Write_IIC_Byte(unsigned char IIC_Byte);// -- ͨ��I2C����дһ��byte������
//void OLED_WrDat(unsigned char dat);// -- ��OLED��д����
//void OLED_WrCmd(unsigned char cmd);// -- ��OLED��д����
//void OLED_Set_Pos(unsigned char x, unsigned char y);// -- ������ʾ����
//void OLED_Fill(unsigned char bmp_dat);// -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
//void OLED_CLS(void);// -- ��λ/����
//void OLED_Init(void);// -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
//void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8������������ʾASCII�����С���У���̫����
//void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16������������ʾASCII�룬�ǳ�����
//void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
//void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void DrawPoint(u8 x,u8 y);
//void ClearPoint(u8 x,u8 y);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ShowChinese0(u8 x,u8 y,u8 num,u8 size1);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
void OLED_Init(void);

#endif


