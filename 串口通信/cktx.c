#include <reg52.h>

#define USE_SERIAL
#include <luzhlon.h>

void main()
{
    //EA = 1;
	TMOD = 0x20;//���ö�ʱ������ģʽ
    //SM0 = 0; SM1 = 1;//���ڹ���ģʽ
	//REN = 1;//������մ�������
	TH1 = TL1 = 0xFD;//��Ӧ9600������,11.0592MHz����
	SCON = 0x50;//���ڿ���
	PCON = 0x00;//���ʲ�����
	TR1 = 1;//����Timer1
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
