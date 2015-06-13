#include <reg52.h>
#include <luzhlon.h>
#define TIME 100
void main()
{
    EA = 1;//开放总中断
    TF0 = 0;//设置溢出标志为0
    ET0 = 1;//允许定时器中断
    TMOD = 1;//定时器工作方式 16位
    TH0 = TIMER_MS_H(TIME);
    TL0 = TIMER_MS_L(TIME);
    TR0 = 1;//启用定时器0
    while(1);
}

void int0() interrupt 1 //定时器0中断处理程序
{
    static int n = 0;
	if(n==10)
	{
        P0 = ~P0;//LED灯
        n = 0;
	}
    TH0 = TIMER_MS_H(TIME);
    TL0 = TIMER_MS_L(TIME);
    n++;
}
