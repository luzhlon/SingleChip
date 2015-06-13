#include <reg52.h>
#include <luzhlon.h>

void main()
{
    int i = 0,j;
    unsigned char b[] = GYBMB;
    while(1)
    {	///*
        Light(1,b[1]);
		Light(2,b[2]);
		Light(3,b[3]);
		Light(4,b[4]);
		Light(5,b[5]);
		Light(6,b[6]);
		Light(7,b[7]);
		Light(8,b[8]);
		Update();//*/
		/*
		P2 = 0xFE;
		P0 = b[1];
		Delay(500);
		P2 = 0x7F;
		P0 = b[2];
		Delay(500);//*/
    }
}
