//本代码的原网站：https://blog.csdn.net/weixin_59552290/article/details/122124843

/*按键一功能是发送500个脉冲，从而精准控制一厘米。
为什么是发送500个脉冲，是因为驱动器可以通过拨动开关调电流，调细分，
我用的是200个脉冲转一圈的状态。
经过调试200个脉冲刚好转动0.4厘米，
经过换算，500个脉冲为1厘米，所以实现精准控制1厘米。
同理可得，其他距离的脉冲数。
按键二的功能为使电机反转，
通过转换引脚电平，给驱动器DIR口状态取反，DIR控制电机正方转，故按键二控制电机方向。
两个外部中断是响应接近开关，使电机转换方向，防止电机超过限位。*/

#include <stc15f2k60s2.h>
#include <intrins.h>
#define u8 unsigned char
#define u16 unsigned int

unsigned int h;		//无符号整型变量

sbit maichong=P1^1;		//定义脉冲输出脚(PUL)
sbit zhuang=P0^3;		//定义方向引脚(DIR)
sbit K1=P2^0;			//脉冲发送
sbit K2=P2^1;			//转向
sbit K3=P2^2;			//调试按键，调试中断是否能进入，可注释掉
sbit right=P3^2		//右限位
sbit left=P3^3;		//左限位
sbit yuan=P0^2;		//坐标原点



void Delay400us()		//400微秒延时函数	
{
	unsigned char i, j;

	i = 5;
	j = 74;
	do
	{
		while (--j);
	} while (--i);
}

void Delay10ms()		//10毫秒延时函数
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void right_EXINT0() interrupt 0		//外部中断函数0
{
	if(right==1)
	{
		Delay10ms(); 
		if(right==1)
		{
			zhuang=~zhuang;
		}
	}
}
	
void left_EXINT1() interrupt 2		//外部中断函数1
{
	if(left==1)
	{
		Delay10ms(); 
	if(left==1)
		{
			zhuang=~zhuang;
		}
	}
}	

void EXINT_init()			//外部中断初始化
{

	IT0=0;
	IT1=0;
	IE0=0;
	IE1=1;
	EA =1;
	EX0=1;
	EX1=1;
}

void main ()				//主函数
{
	EXINT_init();
	P0M0=0XFF;
	P0M1=0X00;
	while(1)
	 {
	    if (K1==0)			//按键1判断
		{
			 Delay10ms();   //延时
			 if (K1==0)
			{	
				for(h=0;h<500;h++) //for循环500次，发送500个脉冲
					{
					maichong=0;           
					Delay400us();      
					maichong=1;            
					Delay400us();
					}  
			}		
		}	 
		if (K2==0)			//判断按键2
			{
			 Delay10ms();  //延时
			 if (K2==0)
				{
				zhuang=~zhuang;		//引脚状态取反
				while(K2==0);		//判断是否松开按键
					
				}
				
			}
	if(K3==0)
	{
		if(K3==0)
		{
			P1=0X00;
		}
	}
	else{
		P1=0Xff;
	}
	}
}