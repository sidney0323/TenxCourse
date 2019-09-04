/*-----------------------------------------------
  名称：双色点阵
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用hc595驱动双色点阵  
------------------------------------------------*/
#include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include <intrins.h> 

//unsigned char  segout[8]={0,1,2,3,4,5,6,7}; //8列
unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8列
unsigned char code tab[]={
                        0x00,0x6C,0x92,0x82,0x44,0x28,0x10,0x00
                        };
/*------------------------------------------------
                硬件端口定义
------------------------------------------------*/

sbit LATCH = P1^0; 
sbit SRCLK= P1^1;
sbit SER  = P1^2;


sbit LATCH_B = P2^2;
sbit SRCLK_B= P2^1;
sbit SER_B= P2^0;

/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
                发送字节程序
------------------------------------------------*/
void SendByte(unsigned char dat)
{    
  unsigned char i; 
        
   for(i=0;i<8;i++)
        {
         SRCLK=0;
         SER=dat&0x80;
         dat<<=1;
         SRCLK=1;
         }
         
}
/*------------------------------------------------
                发送双字节程序
     595级联，n个595，就需要发送n字节后锁存
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2)
{    
   SendByte(dat1);
   SendByte(dat2);      
}
/*------------------------------------------------
                   595锁存程序
		  595级联发送数据后，锁存有效
------------------------------------------------*/
void Out595(void)
{
        LATCH=0;
        _nop_();
        LATCH=1;
}

/*------------------------------------------------
                发送位码字节程序
               使用另外一片单独595
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
unsigned char i; 
        
   for(i=0;i<8;i++)  //发送字节
        {
         SRCLK_B=0;
         SER_B=dat&0x80;
         dat<<=1;
         SRCLK_B=1;
         }
      LATCH_B=0;    //锁存
      _nop_();
      LATCH_B=1;
         
         
}
/*------------------------------------------------
                   主程序
------------------------------------------------*/
void main()
{
unsigned char i,j;
while(1)
{
            for(j=0;j<60;j++)
              for(i=0;i<8;i++) //8列显示
                  {
                     SendSeg(segout[i]);

					 Send2Byte(~tab[i],0xff);
					 Out595();
                     DelayMs(1);
			
					 Send2Byte(0xff,0xff);//delay(10); //防止重影
					 Out595();
                  }
			  //另外一种颜色
		   for(j=0;j<60;j++)
              for(i=0;i<8;i++) //8列显示
                  {
                     SendSeg(segout[7-i]); //反向显示同样图形

					 Send2Byte(0xff,~tab[i]);
					 Out595();
                     DelayMs(1);
			
					 Send2Byte(0xff,0xff);//delay(10); //防止重影
					 Out595();
                  }
          
    }
}