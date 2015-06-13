#include <reg52.h>
#include <stdio.h>

#define uchar unsigned char
#define uint  unsigned int

    void Delay(uint i)      
    {
        while(i-- > 0)
        {
            uint j = 120;
            while(j-- > 0);
        }
    }

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

sbit K1  = P1^4;
sbit K2  = P1^5;
sbit K3  = P1^6;
sbit K4  = P1^7;

#define CHECK_KEY(KEY) (KEY==0 && (Delay(10), KEY==0))

uint num = 0;

void main()
{
    EA = 1;
    //
    SCON = 0x50;
    PCON = 0x00;
    TMOD = 0x20;
    TI = 1;
    TH1 = 0xFD; TL1 = 0xFD;
    TR1 = 1;

    while(1)
    {
        if(CHECK_KEY(K1))
        {
            num ++;
            printf("KEY1 ++ \r\n");
        }
        if(CHECK_KEY(K2))
        {
            num --;
            printf("KEY2 ++ \r\n");
        }
        SMG_LightNum(8,num);
    }
}

