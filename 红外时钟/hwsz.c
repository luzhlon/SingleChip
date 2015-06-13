#include <at89x52.h>
#include <stdio.h>

#define uchar unsigned char
#define uint unsigned int
#define DUAN_SELECT P0 //段选择口
#define WEI_SELECT P2 //位选择口 
//共阳编码表                 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F   
uchar code SMG_GYBMB[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
uchar SMG_SHOW_TIME = 3;
//点亮(熄灭)第i个数码管，不能超过8,c为显示的编码
void SMG_Light(uint i, uchar c)
{
    uchar time = SMG_SHOW_TIME,
          ss[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
    if(i> 8) i= 8;
    if(i< 1) i= 1;
    WEI_SELECT = ss[i-1];
    DUAN_SELECT = c;
}
#define SMG_LightNum(Index, Num) SMG_Light(Index, SMG_GYBMB[(Num)])

//和DS1302相关的几个引脚
#define SCLK   P1_0
#define IO     P1_1
#define RST    P1_2

#define uchar unsigned char
#define uint unsigned int

uchar BCD(uchar byte)
{
    return (((byte)<<4) | ((byte)&0xf));
}

uchar BCD2Int(uchar bcd)
{
    return ((((bcd)>>4)*10) + ((bcd)&0xf));
}

void Delay(unsigned int ms)
{
	while(ms--)
	{
		int j = 120;
		while(j--);
	}
}

void SetInternal(uint count, uchar index)
{
	uchar l = (0xFFFF - count)>>8;
    uchar h = 0xFF & (0xFFFF - count);
    if(count > 0xFFFF) count = 0xFFFF;
    switch(index)
    {
    case 0:
        TL0 = l; TH0 = h;
        break;
    case 1:
        TL1 = l; TH1 = h;
        break;
    }
}

void DelayX(unsigned int x)
{
	while(x--);
}

void WriteDS(uchar dat)
{
    uchar i = 8;
    while(i--)
    {
        SCLK = 0;
        //DelayX(2);//稍微等待
        IO = dat & 1;
        SCLK = 1;//拉高SCLK, 通知DS 要写入数据
        //DelayX(2);//稍微等待
        dat >>= 1;
    }
}
void WriteSet(uchar cmd, uchar dat)
{
    RST = 1;//开启IO传输

    WriteDS(cmd);
    WriteDS(dat);

    RST = 0;//关闭IO传输
}
uchar ReadDS()
{
    uchar dat, i = 8;
    SCLK = 1;
    //DelayX(2);//稍微等待
    SCLK = 0;//拉低SCLK, 通知DS 要读取数据
    //DelayX(2);//稍微等待
    while(i--)
    {
        dat >>= 1;
        if(IO) dat |= 0x80;
        SCLK = 1;
        //DelayX(2);//稍微等待
        SCLK = 0;//拉低SCLK, 通知DS 要读取数据
        //DelayX(2);//稍微等待
    }
    return dat;
}
uchar ReadSet(uchar cmd)
{
    uchar dat;
    RST = 1;//开启IO传输

    WriteDS(cmd);
    dat = ReadDS();

    RST = 0;//关闭IO传输
    return dat;
}

uchar INPUT = 0;

uint  g_50ms_count = 0;

void ModeShow()
{
    uchar sec, min, hour;
    ///*
    sec = ReadSet(0x81);// sec = BCD2Int(sec);
    min = ReadSet(0x83);//  min = BCD2Int(min);
    hour = ReadSet(0x85);// hour = BCD2Int(hour);	
    // */

    if(g_50ms_count<100) //闪烁效果
    {
        SMG_Light(3, ~0x06);
        SMG_Light(6, ~0x06);
    }
    SMG_LightNum(1, hour >> 4);
    SMG_LightNum(2, hour & 0xF);
    SMG_LightNum(4, min >> 4);
    SMG_LightNum(5, min & 0xF);
    SMG_LightNum(7, sec >> 4);
    SMG_LightNum(8, sec & 0xF);

    if(g_50ms_count>200) g_50ms_count = 0;
}

void InitSerial()
{
    //串口通信初始化
	ES = 1;//允许串口中断
    SCON = 0x50;//10位异步收发+允许接收数据 
    PCON = 0x00;//电源控制，波特率不加倍
    TMOD |= 0x20;//定时器1工作方式2
    TI = 1;//初始化传输
    TH1 = 0xFD; TL1 = 0xFD; //对应波特率9600
    TR1 = 1;//启动Timer1
}

void InitTimer0()
{
    ET0=1;       //定时器T0中断允许
    IT0=1;       //外中断的下降沿触发  
    TMOD |= 0x01;   //使用定时器T0的模式1
    SetInternal(50*1000, 0);//间隔太短会引起数码管闪烁
	//printf("T0: %u", (uint)(TH0<<8 | TL0));	
    TR0=1;       //定时器T0启动
}

void main()
{
    EA = 1;//开户总中断
    EX0 = 1;//允许INT0中断

    InitSerial();
    InitTimer0();

    while(1)
    {
		ModeShow();
        ///*
		if(INPUT)
        {
            printf("INPUT : 0x%x \r\n", (uint)INPUT);
			printf("MILLS : %u \r\n", (uint)g_50ms_count);
            INPUT = 0;
			
        } // */
    }
}

///*
void OnTimer0() interrupt 1
{
    SetInternal(50*1000, 0);
    g_50ms_count++;
}// */

void OnSerial() interrupt 4
{
    if(INPUT) return ;
    if(RI)
    {
        RI = 0;
        INPUT = SBUF;
    }
}
