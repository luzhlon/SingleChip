
#ifndef __LUZHLON_H_
#define __LUZHLON_H_

#include <at89x52.h>

#define DECLARE_IO( n, io ) sbit n = io
#define LOW(io) io = 0
#define HIGH(io) io = 1
#define MILLI_SECOND_COUNT 120

//���Ͷ���
#define uchar unsigned char
#define uint unsigned int
//���ڶ�ʱ����
#define TIMER_MS(ms) 0xFFFF - ms*1000
#define TIMER_MS_H(ms) TIMER_MS(ms)/0xFF
#define TIMER_MS_L(ms) TIMER_MS(ms)%0xFF
//2^8 || 2^16
#define BAUD_BIT 256
//���ݲ����ʼ���Ҫװ�ص�ֵ ----����Ƶ��, ������, �Ƿ�ӱ�---������
#define INIT_BAUD_RATE(rate, baud, smod) \
	(BAUD_BIT-((smod ? 2:1)*(rate)*1000000)/(32*(baud)*12))
//��ʼ������
//���ö�ʱ������ģʽ
#define INIT_SERIAL() TMOD = 0x20;\
    TH1 = 0xFD; TL1 = 0xFD;\
	SCON = 0x50;\
	PCON = 0x00;\
	TR1 = 1;//����Timer1

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
    //��ʱ����,i����ʽ����,iΪ1ʱ ��ʱԼ1MS
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
    #define DUAN_SELECT P0 //��ѡ���
    #define WEI_SELECT P2 //λѡ��� 
    //���������                 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F   
    uchar code SMG_GYBMB[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
    uchar SMG_SHOW_TIME = 10;
    //����(Ϩ��)��i������ܣ����ܳ���8,cΪ��ʾ�ı���
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
    void SerialSend(uchar d)//data�������ڱ�ʶ��,ò�Ʊ�Ԥ�����˻����ǹؼ���
    {
        SBUF = d;
        while(!TI);//�ȴ����ݷ���
        TI = 0;
    }
    uchar SerialRecv()
    {
        while(1)if(RI==1)//����Ƿ��������յ�
        {
            RI = 0;
            return SBUF;
        }
    }
#endif

//DS1302ʱ�Ӵ洢
#ifdef USE_DS1302
    //�������Ŷ���
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
        while(i--)//�ӵ�λ����λд��
        {
            DS_SCLK = 0;//����SCLK��׼��д������
            DelayX(2);//��΢�ȴ�
            DS_IO = dat & 1;
            DS_SCLK = 1;//�����أ�֪ͨDS1302д������
            DelayX(2);//��΢�ȴ�
            dat >>= 1;//��������λ
        }
    }
    void DS_WriteSet(uchar cmd, uchar dat)
    {
        DS_RST = 1;//�������ݴ���

        DelayX(2);//��΢�ȴ�
        DS_Write(cmd);
        DS_Write(dat);

        DS_RST = 0;//��ֹ���ݴ���
    }
    uchar DS_Read()
    {
        uchar i = 8, dat = 0;
        DS_SCLK = 1;//�����أ�׼��������
        DelayX(2);//��΢�ȴ�
        DS_SCLK = 0;//����SCLK����ȡ����
        DelayX(2);//��΢�ȴ�
        while(i--)//�ӵ�λ����λ����
        {
            dat >>= 1;//��������λ
            if(DS_IO) dat |= 0x80;//���λ����1
            DS_SCLK = 1;//�����أ�׼��������
            DelayX(2);//��΢�ȴ�
            DS_SCLK = 0;//����SCLK����ȡ����
            DelayX(2);//��΢�ȴ�
        }
        return dat;
    }
    uchar DS_ReadSet(uchar cmd)
    {
        uchar dat;
        DS_RST = 1;//�������ݴ���

        DelayX(2);//��΢�ȴ�
        DS_Write(cmd | 1);//ȷ���Ƕ�
        dat = DS_Read();

        DS_RST = 0;//��ֹ���ݴ���
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
