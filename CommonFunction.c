void Delay(unsigned int i)      //��ʱ����,i����ʽ����,iΪ1ʱ ��ʱԼ1MS
{
unsigned int j;
for(;i>0;i--)             //����i��ʵ�ʲ�������һ��ֵ,���i���ܸ���ֵ
for(j=0;j<125;j++) //j��0������125 ���õ�ʱ���Լ��1MS       
{;}
}
