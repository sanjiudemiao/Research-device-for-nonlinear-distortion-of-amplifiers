//头文件
#include "oled.h"
#include "codetab.h"
#include "delay.h"
#include "myiic.h"
#include "oledfont.h"
#include "stdlib.h"

u8 OLED_GRAM[128][8];

//***********************************************
///**********************************************
//// 通过I2C总线写一个字节
//**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
  for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		IIC_SCL=high;
        delay_us(4);  
		IIC_SCL=0;
		IIC_Byte<<=1;
	}
    IIC_SDA = 1; //原程序这里有一个拉高SDA，根据OLED的DATASHEET，此句必须去掉。
		IIC_SCL=1;
    delay_us(4);
		IIC_SCL=0;
}

///*********************OLED写数据************************************/ 
//void OLED_WrDat(unsigned char IIC_Data)
//{
//	IIC_Start();
//   IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
//	IIC_Wait_Ack();	
//   IIC_Send_Byte(0x40);			//write data
//	IIC_Wait_Ack();	
//   IIC_Send_Byte(IIC_Data);
//	IIC_Wait_Ack();
//	IIC_Stop();
//}
///*********************OLED写命令************************************/
//void OLED_WrCmd(unsigned char IIC_Command)
//{
//	IIC_Start();
//   IIC_Send_Byte(0x78);            //Slave address,SA0=0
//	if(IIC_Wait_Ack())
//	{
//		while(1);
//	}
//   IIC_Send_Byte(0x00);			//write command
//	IIC_Wait_Ack();	
//   IIC_Send_Byte(IIC_Command); 
//	IIC_Wait_Ack();
//	IIC_Stop();
//}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	IIC_Start();
	IIC_Send_Byte(0x78);
	IIC_Wait_Ack();
	if(mode){IIC_Send_Byte(0x40);}
    else{IIC_Send_Byte(0x00);}
	IIC_Wait_Ack();
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}

//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

///*********************OLED 设置坐标************************************/
//void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WrCmd(0xb0+y);
//	OLED_WrCmd(((x&0xf0)>>4)|0x10);
//	OLED_WrCmd((x&0x0f)|0x01);
//} 
///*********************OLED全屏************************************/
////更新显示到oled
//void OLED_Fill(unsigned char bmp_dat) 
//{
//	unsigned char y,x;
//	for(y=0;y<8;y++)
//	{
//		OLED_WrCmd(0xb0+y);
//		OLED_WrCmd(0x01);
//		OLED_WrCmd(0x10);
//		for(x=0;x<X_WIDTH;x++)
//		OLED_WrDat(bmp_dat);
//	}
//}
///*********************OLED复位************************************/
////清屏函数
//void OLED_CLS(void)
//{
//	unsigned char y,x;
//	for(y=0;y<8;y++)
//	{
//		OLED_WrCmd(0xb0+y);
//		OLED_WrCmd(0x01);
//		OLED_WrCmd(0x10);
//		for(x=0;x<X_WIDTH;x++)
//		OLED_WrDat(0);
//	}
//}
/*********************OLED初始化************************************/
void OLED_Init(void)
{
//	delay_ms(500);//初始化之前的延时很重要！
//	OLED_WrCmd(0xae);//--turn off oled panel
//	OLED_WrCmd(0x00);//---set low column address
//	OLED_WrCmd(0x10);//---set high column address
//	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WrCmd(0x81);//--set contrast control register
//	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
//	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//	OLED_WrCmd(0xa6);//--set normal display
//	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
//	OLED_WrCmd(0x3f);//--1/64 duty
//	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WrCmd(0x00);//-not offset
//	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
//	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WrCmd(0xd9);//--set pre-charge period
//	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock   
//	OLED_WrCmd(0xda);//--set com pins hardware configuration
//	OLED_WrCmd(0x12);
//	OLED_WrCmd(0xdb);//--set vcomh
//	OLED_WrCmd(0x40);//Set VCOM Deselect Level
//	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WrCmd(0x02);//
//	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
//	OLED_WrCmd(0x14);//--set(0x10) disable
//	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WrCmd(0xaf);//--turn on oled panel
//	OLED_Fill(0x00); //初始清屏
//	OLED_Set_Pos(0,0);
	
	
	delay_ms(500);//初始化之前的延时很重要！
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
} 
///***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
//void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
//{
//	unsigned char c=0,i=0,j=0;
//	while (ch[j]!='\0')
//	{
//		c =ch[j]-32;
//		if(x>126){x=0;y++;}
//		OLED_Set_Pos(x,y);
//		for(i=0;i<6;i++)
//		OLED_WrDat(F6x8[c][i]);
//		x+=6;
//		j++;
//	}
//}
///*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
//void OLED_P8x16Str(unsigned char x,unsigned  char y,unsigned char ch[])
//{
//	unsigned char c=0,i=0,j=0;
//	while (ch[j]!='\0')
//	{
//		c =ch[j]-32;
//		if(x>120){x=0;y++;}
//		OLED_Set_Pos(x,y);
//		for(i=0;i<8;i++)
//		OLED_WrDat(F8X16[c*16+i]);
//		OLED_Set_Pos(x,y+1);
//		for(i=0;i<8;i++)
//		OLED_WrDat(F8X16[c*16+i+8]);
//		x+=8;
//		j++;
//	}
//}
///*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
//void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N)
//{
//	unsigned char wm=0;
//	unsigned int adder=32*N;
//	OLED_Set_Pos(x , y);
//	for(wm = 0;wm < 16;wm++)
//	{
//		OLED_WrDat(F16x16[adder]);
//		adder += 1;
//	}
//	OLED_Set_Pos(x,y + 1);
//	for(wm = 0;wm < 16;wm++)
//	{
//		OLED_WrDat(F16x16[adder]);
//		adder += 1;
//	} 	  	
//}



//画点 
//x:0~127
//y:0~63
void DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小

//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //换行
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
				for(m=0;m<8;m++)           //写入数据
				{
					if(temp&0x80)OLED_DrawPoint(x,y);
					else OLED_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}


//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==12)
						{temp=Chinese_1212[chr1][i];}//调用12*12字体
				else if(size1==16)
						{temp=Chinese_1616[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=Chinese_2424[chr1][i];}//调用24*24字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

void OLED_ShowChinese0(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Chinese_1616[chr1][i];}//调用16*16字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r,s=23;
	while(s>0)
	{
		if(m==0)
		{
	    OLED_ShowChinese0(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
			s--;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=0;i<144;i++)
						{
	   						for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}
///***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
//{
//	unsigned int j=0;
//	unsigned char x,y;

//  if(y1%8==0) y=y1/8;      
//  else y=y1/8+1;
//	for(y=y0;y<y1;y++)
//	{
//		OLED_Set_Pos(x0,y);
//    for(x=x0;x<x1;x++)
//	    {      
//	    	OLED_WrDat(BMP[j++]);
//	    }
//	}
//}



