
#ifndef __LUZHLON_H_
#define __LUZHLON_H_

#include <at89x52.h>

#define DECLARE_IO( n, io ) sbit n = io
#define LOW(io) io = 0
#define HIGH(io) io = 1
#define MILLI_SECOND_COUNT 120

//类型定义
#define uchar unsigned char
#define uint unsigned int
//关于定时器的
#define TIMER_MS(ms) 0xFFFF - ms*1000
#define TIMER_MS_H(ms) TIMER_MS(ms)/0xFF
#define TIMER_MS_L(ms) TIMER_MS(ms)%0xFF
//2^8 || 2^16
#define BAUD_BIT 256
//根据波特率计算要装载的值 ----晶振频率, 波特率, 是否加倍---有问题
#define INIT_BAUD_RATE(rate, baud, smod) \
	(BAUD_BIT-((smod ? 2:1)*(rate)*1000000)/(32*(baud)*12))
//初始化串口
//设置定时器工作模式
#define INIT_SERIAL() TMOD = 0x20;\
    TH1 = 0xFD; TL1 = 0xFD;\
	SCON = 0x50;\
	PCON = 0x00;\
	TR1 = 1;//运行Timer1

#ifdef USE_BCD
    uchar BCD(uchar byte)
    {
        return (((byte)<<4) | ((byte)&0xf));
    }

    uchar BCD2Int(uchar bcd)
    {
        return ((((bcd)>>4)*10) + ((bcd)&0xf));
    }
#endif

#ifdef  USE_DELAY
    //延时程序,i是形式参数,i为1时 延时约1MS
    void DelayX(uint i)
    {
        while(i--);
    }
    void Delay(uint i)      
    {
        while(i-- > 0)
        {
            uint j = MILLI_SECOND_COUNT;
            while(j-- > 0);
        }
    }
    void Delay100us(uint i)
    {
        while(i-- > 0)
        {
            uint j = 12;
            while(j-- > 0);
        }
    }
#endif

#ifdef  USE_SMG
    #define DUAN_SELECT P0 //段选择口
    #define WEI_SELECT P2 //位选择口 
    //共阳编码表                 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F   
    uchar code SMG_GYBMB[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
    uchar SMG_SHOW_TIME = 10;
    //点亮(熄灭)第i个数码管，不能超过8,c为显示的编码
    void SMG_Light(uint i, uchar c)
    {
        uchar time = SMG_SHOW_TIME,
              ss[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
        if(i> 8) i= 8;
        if(i< 1) i= 1;
        WEI_SELECT = ss[i-1];
        DUAN_SELECT = c;
        while(time--);
        DUAN_SELECT = 0xFF;
    }
    #define SMG_LightNum(Index, Num) SMG_Light(Index, SMG_GYBMB[(Num)])
#endif

#ifdef USE_SERIAL
    void SerialSend(uchar d)//data不能用于标识符,貌似被预定义了或者是关键字
    {
        SBUF = d;
        while(!TI);//等待数据发送
        TI = 0;
    }
    uchar SerialRecv()
    {
        while(1)if(RI==1)//检测是否有数据收到
        {
            RI = 0;
            return SBUF;
        }
    }
#endif

//DS1302时钟存储
#ifdef USE_DS1302
    //几个引脚定义
    sbit DS_SCLK = P1^0;
    sbit DS_IO   = P1^1;
    sbit DS_RST  = P1^2;
#ifndef USE_DELAY
    void DelayX(uint i)
    {
        while(i--);
    }
#endif
    void DS_Write(uchar dat)
    {
        uchar i = 8;
        while(i--)//从低位往高位写入
        {
            DS_SCLK = 0;//拉低SCLK，准备写入数据
            DelayX(2);//稍微等待
            DS_IO = dat & 1;
            DS_SCLK = 1;//上升沿，通知DS1302写入数据
            DelayX(2);//稍微等待
            dat >>= 1;//右移数据位
        }
    }
    void DS_WriteSet(uchar cmd, uchar dat)
    {
        DS_RST = 1;//允许数据传输

        DelayX(2);//稍微等待
        DS_Write(cmd);
        DS_Write(dat);

        DS_RST = 0;//禁止数据传输
    }
    uchar DS_Read()
    {
        uchar i = 8, dat = 0;
        DS_SCLK = 1;//上升沿，准备读数据
        DelayX(2);//稍微等待
        DS_SCLK = 0;//拉低SCLK，读取数据
        DelayX(2);//稍微等待
        while(i--)//从低位往高位读出
        {
            dat >>= 1;//右移数据位
            if(DS_IO) dat |= 0x80;//最高位并入1
            DS_SCLK = 1;//上升沿，准备读数据
            DelayX(2);//稍微等待
            DS_SCLK = 0;//拉低SCLK，读取数据
            DelayX(2);//稍微等待
        }
        return dat;
    }
    uchar DS_ReadSet(uchar cmd)
    {
        uchar dat;
        DS_RST = 1;//允许数据传输

        DelayX(2);//稍微等待
        DS_Write(cmd | 1);//确保是读
        dat = DS_Read();

        DS_RST = 0;//禁止数据传输
        return dat;
    }
    #define DS_READ_SEC 0x81
    #define DS_READ_MIN 0x83
    #define DS_READ_HOUR 0x85
    #define DS_READ_DAY 0x87
    #define DS_READ_MONTH 0x89
    #define DS_READ_WEAK 0x8B
    #define DS_READ_YEAR 0x8D
    #define DS_WRITE_SEC 0x80
    #define DS_WRITE_MIN 0x82
    #define DS_WRITE_HOUR 0x84
    #define DS_WRITE_DAY 0x86
    #define DS_WRITE_MONTH 0x88
    #define DS_WRITE_WEAK 0x8A
    #define DS_WRITE_YEAR 0x8C
#endif
 

#endif
