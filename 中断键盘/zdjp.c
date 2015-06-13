#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int

uint num = 0;

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

void main()
{
	uint n,i;
    EA = 1;
    EX0 = 1;

    while(1)
    {
        n = num;
        for(i=0; n; i++, n/=10)
        {
            SMG_LightNum(8-i, n%10);
        }
    }
}

void OnINT0() interrupt 0
{
    int i = 120*500;
	num++;
	P0 = 0x7f;
	//while(i--);
}
