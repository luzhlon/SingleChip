#include <reg52.h>
#include <luzhlon.h>

DECLARE_IO(FMQ, P3^6);

void main()
{
    while(1)
    {
        LOW(FMQ);//Feng Ming
        Delay100us(3);
        HIGH(FMQ);
        Delay100us(5);
    }
}
