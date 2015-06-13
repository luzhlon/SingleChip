#include <reg52.h>
#include <luzhlon.h>

#define DECLARE_IO( n, io ) sbit n = io

#define LOW(io) io = 0
#define HIGH(io) io = 1

DECLARE_IO( jdq, P3^7);//sbit类型只能为全局变量

void main()
{
    while(1)
    {
        Delay(1000);
        LOW(jdq);
        Delay(500);
        HIGH(jdq);
    }
}
