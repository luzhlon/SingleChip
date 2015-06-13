#include <reg52.h>

#define USE_SERIAL
#include <luzhlon.h>

void main()
{
    //EA = 1;
	TMOD = 0x20;//设置定时器工作模式
    //SM0 = 0; SM1 = 1;//串口工作模式
	//REN = 1;//允许接收串口数据
	TH1 = TL1 = 0xFD;//对应9600波特率,11.0592MHz晶振
	SCON = 0x50;//串口控制
	PCON = 0x00;//速率不倍增
	TR1 = 1;//运行Timer1
	//SerialSend(TH1);
    while(1)
    {
        /*
	    if(RI)
        {
            RI = 0;
            P0 = SBUF;
            SBUF = P0 + 1;
            while(!TI);
            TI = 0;
        } // */
        uchar d = SerialRecv();
        P0 = d;
        SerialSend(d+1);
    }
}
