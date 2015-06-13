#include <reg52.h>
#include <luzhlon.h>
#define TIME 100
void main()
{
    EA = 1;//�������ж�
    TF0 = 0;//���������־Ϊ0
    ET0 = 1;//����ʱ���ж�
    TMOD = 1;//��ʱ��������ʽ 16λ
    TH0 = TIMER_MS_H(TIME);
    TL0 = TIMER_MS_L(TIME);
    TR0 = 1;//���ö�ʱ��0
    while(1);
}

void int0() interrupt 1 //��ʱ��0�жϴ������
{
    static int n = 0;
	if(n==10)
	{
        P0 = ~P0;//LED��
        n = 0;
	}
    TH0 = TIMER_MS_H(TIME);
    TL0 = TIMER_MS_L(TIME);
    n++;
}
